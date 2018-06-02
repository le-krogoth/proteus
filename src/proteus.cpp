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

U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 5, /* data=*/ 4, /* reset=*/ 16);
// There are some Heltec 8 Boards, which need the config below.
// We haven't found one yet, but we heard stories of.
// U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 14, /* data=*/ 2, /* reset=*/ 4);

// ------------------------------------------------------------------
// ------------------------------------------------------------------
void setup()
{
    // start serial port
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    delay(10);

    Serial.println("Proteus booting up");

    Serial.println("Setting standard Wifi Configuration");

    // We deactivate the WIFI to save battery power
    WiFi.mode(WIFI_AP_STA);
    WiFi.setAutoConnect(false);
    WiFi.disconnect(true);
    WiFi.softAPdisconnect(true);

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

    // only show boot logo if not deactivated by the user
    if(c->getShowBootLogo())
    {
        dm->showBootLogo();
    }

    // activate buttons
    pinMode(13, INPUT_PULLUP);
    pinMode(12, INPUT_PULLUP);

    Serial.println("Let's go. Welcome to Proteus.");

    delay(10);
}

// ------------------------------------------------------------------
void loop()
{
    Serial.println(ESP.getFreeHeap());

    // only enforce framerate if the module wants it enforced
    if(mm->moduleWantsEnforcedFramerate() && !dm->nextFrame())
    {
        return;
    }

    eh->poll();

    mm->checkEvents();

    dm->handleFrame();
}

//EOF
