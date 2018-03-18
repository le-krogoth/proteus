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
#include <Arduino.h>

#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

//#include <ESP8266HTTPClient.h>
//#include <ESP8266httpUpdate.h>

#include "Esp.h" // deep sleep and system info

ESP8266WiFiMulti WiFiMulti;


// ------------------------------------------------------------------
// ------------------------------------------------------------------
void setup()
{
    // start serial port
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    delay(1000);

    Serial.println("Proteus v2 booting up");

    WiFi.mode(WIFI_STA);
    WiFiMulti.addAP("AP", "PWD");
}

// ------------------------------------------------------------------
// ------------------------------------------------------------------
// not needed
void loop()
{
    Serial.println("Proteus v2 trying to update");
    update();
    delay(5000);
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
        //t_httpUpdate_return ret = ESPhttpUpdate.updateSpiffs("http://192.168.1.5/spiffs/file");
        //if(ret == HTTP_UPDATE_OK) {
            Serial.println("Update sketch...");
            t_httpUpdate_return ret = ESPhttpUpdate.update("http://192.168.1.5:8081/fw/v2");

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
