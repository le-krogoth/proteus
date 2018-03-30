/*-----------------------------------------------------------------------------
 **
 ** - Proteus -
 **
 ** Copyright 2018 by Krogoth and the contributing authors
 **
 ** This program is free software; you can redistribute it and/or modify it
 ** under the terms of the GNU Affero General Public License as published by the
 ** Free Software Foundation, either version 3 of the License, or (at your option)
 ** any later version.
 **
 ** This program is distributed in the hope that it will be useful, but WITHOUT
 ** ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 ** FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License
 ** for more details.
 **
 ** You should have received a copy of the GNU Affero General Public License
 ** along with this program. If not, see <http://www.gnu.org/licenses/>.
 **
 ** -----------------------------------------------------------------------------*/

#include "proteus.h"


#include <FS.h>
#include <ArduinoJson.h>


#include <U8g2lib.h>
#include "DisplayManager.h"

#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

#include <ESP8266WebServer.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <Esp.h> // deep sleep and system info


ESP8266WiFiMulti WiFiMulti;

U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 5, /* data=*/ 4, /* reset=*/ 16);
//U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 14, /* data=*/ 2, /* reset=*/ 4);

ESP8266WebServer server(1234);

#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

char m[16][8]; // 16 chars across, 8 chars high
int ySpeeds[16];

DisplayManager* dm = NULL;

// ------------------------------------------------------------------
// ------------------------------------------------------------------
void setup()
{
    // start serial port
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    delay(1000);

    Serial.println("Proteus v2 booting up");

    SPIFFS.begin();

    // open file for reading
    File f = SPIFFS.open("/ttbl.json", "r");
    if (!f) {
        Serial.println("file open failed");
    }

    Serial.println("====== Reading from SPIFFS file =======");

    // Allocate the memory pool on the stack.
    // Don't forget to change the capacity to match your JSON document.
    // Use arduinojson.org/assistant to compute the capacity.
    StaticJsonBuffer<512> jsonBuffer;

    // Parse the root object
    JsonObject &root = jsonBuffer.parseObject(f);

    if (!root.success())
    {
        Serial.println(F("Failed to read file"));
    }
    else
    {
        const char* name = root["name"];
        const char* version = root["version"];

        // Print values.
        Serial.println(name);
        Serial.println(version);
    }

    f.close();

    dm = new DisplayManager(&u8g2, &Serial);
    dm->begin();
    dm->showBootLogo();

    WiFi.mode(WIFI_STA);
    WiFiMulti.addAP(SSID, PWD);

    delay(1000);

    server.on("/", HTTP_GET, getRoot);
    server.on("/api/config", HTTP_GET, getConfig);
    server.on("/api/config", HTTP_POST, setConfig);
    server.on("/api/hwinfo", HTTP_GET, getHWInfo);
    server.onNotFound(handleNotFound);

    server.begin();
}

void getConfig() {

    String json = "{\n\"nickname\": \"Uberh4x0r\"\n}";

    server.send(200, "application/json; charset=utf-8", json);
}

void setConfig() {

    Serial.println(server.arg("plain"));

    DynamicJsonBuffer jsonBuffer(4000);
    JsonObject& root = jsonBuffer.parseObject(server.arg("plain"));

    const char* nickname = root["nickname"];
    //long time          = root["time"];
    //double latitude    = root["data"][0];

    Serial.println(nickname);

    server.send(200, "application/json; charset=utf-8", "{success:true}");
}

void reply404() {

    String message = "Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for (uint8_t i = 0; i < server.args(); i++) {
        message += " NAME:" + server.argName(i) + "\n VALUE:" + server.arg(i) + "\n";
    }
    server.send(404, "text/plain", message);

    Serial.println(message);
}

void getRoot() {

    if (loadFromSPIFFS("index.html")) {
        return;
    }

    reply404();
}

void getHWInfo() {

    String json = "{\n\"hwinfo\": {\n";
    json += "\"reset_reason\": \"" + ESP.getResetReason() + "\",\n";
    json += "\"free_heap\": \"" + String(ESP.getFreeHeap()) + "\",\n";
    json += "\"chip_id\": \"" + String(ESP.getChipId()) + "\",\n";
    json += "\"core_version\": \"" + String(ESP.getCoreVersion()) + "\",\n";
    json += "\"sdk_version\": \"" + String(ESP.getSdkVersion()) + "\",\n";
    json += "\"cpu_freq_mhz\": \"" + String(ESP.getCpuFreqMHz()) + "\",\n";
    json += "\"sketch_size\": \"" + String(ESP.getSketchSize()) + "\",\n";
    json += "\"free_sketch_space\": \"" + String(ESP.getFreeSketchSpace()) + "\",\n";
    json += "\"sketch_md5\": \"" + String(ESP.getSketchMD5()) + "\",\n";
    json += "\"flash_chip_id\": \"" + String(ESP.getFlashChipId()) + "\",\n";
    json += "\"flash_chip_size\": \"" + String(ESP.getFlashChipSize()) + "\",\n";
    json += "\"flash_chip_real_size\": \"" + String(ESP.getFlashChipRealSize()) + "\",\n";
    json += "\"flash_chip_speed\": \"" + String(ESP.getFlashChipSpeed()) + "\",\n";
    json += "\"cycle_count\": \"" + String(ESP.getCycleCount()) + "\"\n";

    json += "}\n}";

    server.send(200, "application/json; charset=utf-8", json);
}

void handleNotFound() {

    if (loadFromSPIFFS(server.uri())) {
        return;
    }

    reply404();
}

bool loadFromSPIFFS(String path) {

    String dataType = "text/plain";
    if (path.endsWith("/")) {
        path += "web_index.html";
    }

    if (path.endsWith(".src")) {
        path = path.substring(0, path.lastIndexOf("."));
    } else if (path.endsWith(".html")) {
        dataType = "text/html";
    } else if (path.endsWith(".css")) {
        dataType = "text/css";
    } else if (path.endsWith(".js")) {
        dataType = "application/javascript";
    } else if (path.endsWith(".png")) {
        dataType = "image/png";
    } else if (path.endsWith(".gif")) {
        dataType = "image/gif";
    } else if (path.endsWith(".jpg")) {
        dataType = "image/jpeg";
    } else if (path.endsWith(".ico")) {
        dataType = "image/x-icon";
    } else if (path.endsWith(".xml")) {
        dataType = "text/xml";
    } else if (path.endsWith(".pdf")) {
        dataType = "application/pdf";
    } else if (path.endsWith(".zip")) {
        dataType = "application/zip";
    }

    File dataFile = SPIFFS.open(path.c_str(), "r");
    if (!dataFile) {
        return false;
    }

    //if (dataFile.isDirectory()) {
    //    return false;
    //}

    if (server.hasArg("download")) {
        dataType = "application/octet-stream";
    }

    if (server.streamFile(dataFile, dataType) != dataFile.size()) {
        Serial.println("Sent less data than expected!");
    }

    dataFile.close();
    return true;
}

// ------------------------------------------------------------------
// ------------------------------------------------------------------
// not needed
void loop()
{
    dm->handleScreen();

    server.handleClient();

    /*
    u8g2.firstPage();
    do {

        u8g2.clearDisplay();
        //u8g2.setFont(u8g2_font_logisoso32_tf);
        u8g2.setFont(u8g2_font_m2icon_7_tf);
        //u8g2.drawUTF8(0,31,"Proteus");
        //u8g2.drawXBM( 1, 2, 125,32, area41logo);
        //u8g2.drawXBMP( 42, 0, ironman_width, ironman_height, ironman_bits);

        for(int i=0; i<COUNT_OF(m); i++){
            for(int j=0; j<COUNT_OF(m[i]); j++){

                // move the cursor to the right position with good spacing between chars
                // draw the char partially off the screen for a smooth effect
                //arduboy.setCursor(i*8, (j*8+64+millis()/ySpeeds[i])%(64+ySpeeds[i]%48)-8);

                if(random(1000) > 985){
                    m[i][j] = random(255);  // randomly change the character once in a while
                }

                const char* c = &m[i][j];

                u8g2.drawStr( i*8, (j*8+64+millis()/ySpeeds[i])%(64+ySpeeds[i]%48)-8, c); // print character in 2D array
            }
        }

    } while ( u8g2.nextPage() );
     */

    //Serial.println("Proteus v2 trying to update");
    //update();
    //delay(200);
}


String getMAC()
{
  uint8_t mac[6];
  char result[14];

 snprintf( result, sizeof( result ), "%02x%02x%02x%02x%02x%02x", mac[ 0 ], mac[ 1 ], mac[ 2 ], mac[ 3 ], mac[ 4
 ], mac[ 5 ] );

  return String( result );
}


void update() {
    // wait for WiFi connection
    if((WiFiMulti.run() == WL_CONNECTED)) {

        //Serial.println("Update SPIFFS...");
        //t_httpUpdate_return ret = ESPhttpUpdate.updateSpiffs("http://192.168.100.18/spiffs/file");
        //if(ret == HTTP_UPDATE_OK) {
            Serial.println("Update sketch...");
            t_httpUpdate_return ret = ESPhttpUpdate.update("http://192.168.100.18:8081/fw/v2");

            switch(ret) {
                case HTTP_UPDATE_FAILED:
                    Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
                    break;

                case HTTP_UPDATE_NO_UPDATES:
                    Serial.println("HTTP_UPDATE_NO_UPDATES");
                    break;

                case HTTP_UPDATE_OK:
                    Serial.println("HTTP_UPDATE_OK");
                    break;
            }
        //}
    }
}

//EOF
