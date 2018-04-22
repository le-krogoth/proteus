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
#include "ModeSetup.h"


ModeSetup::ModeSetup(EventHandler *const e, HardwareSerial *const hws) : BaseMode (e, hws)
{
    //if(server)
    //{
    //    hs->println("Freeing old WWWSERVER");
    //    delete server;
    //}

    AP_SSID = ("area41_" + getMAC()).c_str();
    AP_PWD = "abcd1234";

    WiFi.disconnect(true);
    WiFi.softAPdisconnect(true);
//    bool bStarted = WiFi.softAP(AP_SSID.c_str(), AP_PWD.c_str());
    bool bStarted = WiFi.softAP(AP_SSID.c_str(), "");
    WiFi.setSleepMode(WIFI_NONE_SLEEP);

    hs->print("SoftAP started: ");
    hs->println(bStarted);

    //WWWSERVER.on("/api/config", HTTP_GET, getConfig);
    //WWWSERVER.on("/api/config", HTTP_POST, setConfig);
    //WSConfigHandler* confHandler = new WSConfigHandler(hws);
    //WWWSERVER.addHandler(confHandler);
    server->on("/api/config", HTTP_GET, std::bind(&ModeSetup::handleGetConfig, this));
    server->on("/api/config", HTTP_POST, std::bind(&ModeSetup::handleSetConfig, this));

    //WSHWInfoHandler* hwHandler = new WSHWInfoHandler(hws);
    //WWWSERVER.addHandler(hwHandler);
    server->on("/api/hwinfo", HTTP_GET, std::bind(&ModeSetup::handleGetHWInfo, this));

    // files
    server->on("/index.html", HTTP_GET, std::bind(&ModeSetup::sendFromSPIFFS, this, "/www/index.html"));
    server->on("/siimple.css", HTTP_GET, std::bind(&ModeSetup::sendFromSPIFFS, this, "/www/siimple.css"));
    server->on("/siimple-colors.css", HTTP_GET, std::bind(&ModeSetup::sendFromSPIFFS, this, "/www/siimple-colors.css"));
    server->on("/zepto.min.js", HTTP_GET, std::bind(&ModeSetup::sendFromSPIFFS, this, "/www/zepto.min.js"));
    server->on("/mntsrt.css", HTTP_GET, std::bind(&ModeSetup::sendFromSPIFFS, this, "/www/mntsrt.css"));
    server->on("/mntsrt-b.woff2", HTTP_GET, std::bind(&ModeSetup::sendFromSPIFFS, this, "/www/mntsrt-b.woff2"));
    server->on("/mntsrt-r.woff2", HTTP_GET, std::bind(&ModeSetup::sendFromSPIFFS, this, "/www/mntsrt-r.woff2"));

    server->on("/ttbl.json", HTTP_GET, std::bind(&ModeSetup::sendFromSPIFFS, this, "/www/ttbl.json"));

    //server->serveStatic("/", SPIFFS, "/www/");

    server->on("/heap", [&]() {
        server->send(200, "text/plain", String(ESP.getFreeHeap()));
    });

    server->onNotFound(std::bind(&ModeSetup::handleNotFound, this));

    //WWWSERVER.on("/heap", HTTP_GET, [](AsyncWebServerRequest *request){
    //    request->send(200, "text/plain", String(ESP.getFreeHeap()));
    //});

    /*WWWSERVER.on("/iii", HTTP_GET, [](AsyncWebServerRequest *request){


        File f = SPIFFS.open("/www/ttbl.json", "r");
        f.readString()

        request->send(SPIFFS, "/www/index.html");
    });
     */

    //WWWSERVER.serveStatic("/", SPIFFS, "/www/");

    server->begin();
}

ModeSetup::~ModeSetup()
{
    hs->println("destructor called");
    cleanup();
}

void ModeSetup::cleanup()
{
    hs->println("disconnecting Wifi Soft AP and shutting down Server");
    WiFi.softAPdisconnect(true);
    server->stop();
    //WWWserver->reset();
}

void ModeSetup::handleEvents()
{
    //hs->println();
    server->handleClient();
}

void ModeSetup::paintFrameInternal()
{
    u8g2->setFont(u8g2_font_6x10_tf);

    u8g2->firstPage();
    do {

        //u8g2->drawUTF8(0,10,"SSID: ");
        u8g2->drawUTF8(0,10, AP_SSID.c_str());
        //8g2->drawUTF8(0,20,"PSK: ");
        u8g2->drawUTF8(0,20, AP_PWD.c_str());
        u8g2->drawUTF8(96,20, "#");
        u8g2->drawUTF8(102,20, String(WiFi.softAPgetStationNum()).c_str());
        //u8g2->drawUTF8(0,30,"URL: ");
        u8g2->drawUTF8(0,30, "http://");
        u8g2->drawUTF8(42,30, WiFi.softAPIP().toString().c_str());

    } while ( u8g2->nextPage() );
}

String ModeSetup::getMAC()
{
    uint8_t mac[6];

    WiFi.macAddress(mac);

    char result[14];

    snprintf( result, sizeof( result ), "%02x%02x%02x%02x%02x%02x", mac[ 0 ], mac[ 1 ], mac[ 2 ], mac[ 3 ], mac[ 4
    ], mac[ 5 ] );

    hs->print("MAC: ");
    hs->println(result);

    return String( result );
}

void ModeSetup::handleNotFound()
{
    hs->println("handle not found");

    String url = server->uri();

    hs->println(url);

    if(url.length() == 0 || url == "/")
    {
        url = "/www/index.html";
    }

    hs->println(url);

    if (sendFromSPIFFS(url)) {
        return;
    }

    server->send(200, "text/plain", "file not found");
}

void ModeSetup::handleGetConfig()
{
    String json = "{\n\"nickname\": \"Uberh4x0r\"\n}";

    server->send(200, "application/json; charset=utf-8", json);
}

void ModeSetup::handleSetConfig()
{
    hs->println(server->arg("plain"));

    DynamicJsonBuffer jsonBuffer(1000);
    JsonObject& root = jsonBuffer.parseObject(server->arg("plain"));

    const char* nickname = root["nickname"];
    //long time          = root["time"];
    //double latitude    = root["data"][0];

    hs->println(nickname);

    server->send(200, "application/json; charset=utf-8", "{success:true}");
}

void ModeSetup::handleGetHWInfo() {

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

    //hs->println(json);

    server->send(200, "application/json; charset=utf-8", json);
}

void ModeSetup::handleFileRequest()
{

}


bool ModeSetup::sendFromSPIFFS(String path)
{
    hs->print("file requested: ");
    hs->println(path);

    String dataType = "text/plain";
    if (path.endsWith("/")) {
        path += "index.html";
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

    if(!SPIFFS.exists(path))
    {
        hs->print("File does not exist in SPIFFS: ");
        hs->println(path);
        return false;
    }

    File dataFile = SPIFFS.open(path.c_str(), "r");
    if (!dataFile) {
        hs->print("Something is wrong with file in SPIFFS: ");
        hs->println(path);
        return false;
    }

    //if (dataFile.isDirectory()) {
    //    return false;
    //}

    if (server->hasArg("download")) {
        dataType = "application/octet-stream";
    }

    if (server->streamFile(dataFile, dataType) != dataFile.size()) {
        Serial.println("Sent less data than expected. Probably stopped by client.");
    }

    dataFile.close();

    hs->print("file sent: ");
    hs->println(path);

    return true;
}

void ModeSetup::returnOK() {
    server->send(200, "text/plain", "ok");
}