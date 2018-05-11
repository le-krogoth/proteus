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

// Todo decide on using multi or "standard" for update
//ESP8266WiFiMulti WiFiMulti;

U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 5, /* data=*/ 4, /* reset=*/ 16);
//U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 14, /* data=*/ 2, /* reset=*/ 4);

// ------------------------------------------------------------------
// ------------------------------------------------------------------
void setup()
{
    // start serial port
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    delay(10);

    Serial.println("Proteus v2 booting up");

    Serial.println("Setting config");

    WiFi.mode(WIFI_AP_STA);
    WiFi.setAutoConnect(false);
    WiFi.disconnect(true);
    WiFi.softAPdisconnect(true);
    //WiFiMulti.addAP(SSID, PWD);

    Serial.println("Checking filesystem");
    if(SPIFFS.begin())
    {
        Serial.println("Mounting filesystem succeeded");
    }
    else
    {
        Serial.println("Mounting filesystem failed. Formatting...");

        SPIFFS.format();
    }

    Serial.println("Loading config");

    c = new Config(&Serial, false);
    c->loadConfig();

    Serial.println("Initialising objects");

    eh = new EventHandler(&Serial);
    mm = new ModeManager(eh, c, &u8g2, &Serial);

    dm = new DisplayManager(mm, &u8g2, &Serial);
    dm->begin();

    if(c->getShowBootLogo())
    {
        dm->showBootLogo();
    }

    pinMode(13, INPUT_PULLUP);
    pinMode(12, INPUT_PULLUP);
    //pinMode(1, INPUT_PULLUP);

    Serial.println("Let's go");

    delay(10);
}

// ------------------------------------------------------------------
void loop()
{
    // only enforce framerate if the module wants it enforced
    if(mm->moduleWantsEnforcedFramerate() && !dm->nextFrame())
    {
        return;
    }

    eh->poll();

    mm->checkEvents();

    dm->handleFrame();


    /*

    uint16_t buttons1 = ((~PIN_IN) & (bit(1) | bit(12) | bit(13)));

    Serial.println(PIN_IN);
    Serial.println(buttons1);

    //uint16_t buttons2 = ((PIN_IN) & (bit(12) | bit(13)));

    //Serial.print("-");
    //Serial.println(buttons2);

    Serial.print("left: ");
    Serial.print((buttons1 & bit(13)) == bit(13));

    Serial.print(" right: ");
    Serial.print((buttons1 & bit(12)) == bit(12));

    Serial.print(" prog: ");
    Serial.print((buttons1 & bit(1)) == bit(1));

    Serial.print(" both: ");
    Serial.println((buttons1 & (bit(12) | bit(13))) == (bit(12) | bit(13)));
    */

    //Serial.println("Proteus v2 trying to update");
    //update();
    //delay(200);
}

//EOF
