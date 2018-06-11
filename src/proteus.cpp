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

    // TODO:
    // randomize seed
    // randomSeed(analogRead(0));

    // Erase config for some AP settings are stored otherwise
    // TODO: Could only be done if requested by user
    ESP.eraseConfig();

    Serial.println("Setting standard Wifi Configuration");

    // We deactivate the WIFI to save battery power
    WiFi.mode(WIFI_AP_STA);
    WiFi.setAutoConnect(false);
    WiFi.disconnect(true);
    WiFi.softAPdisconnect(true);

    // activate buttons
    Serial.println("Activating buttons");
    pinMode(13, INPUT_PULLUP);
    pinMode(12, INPUT_PULLUP);

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

    // initialising event handler, making sure to poll for the first time to allow to reset during boot
    eh = new EventHandler(&Serial);
    eh->poll();

    // initialise the mode manager and implicitly the first mode to be run
    mm = new ModeManager(eh, c, &u8g2, &Serial);

    // initialise the display
    dm = new DisplayManager(mm, &u8g2, &Serial);
    dm->begin();

    // only show boot logo if not deactivated by the user
    if(c->getShowBootLogo())
    {
        dm->showBootLogo();
    }

    Serial.println("Let's go. Welcome to Proteus.");
}

// ------------------------------------------------------------------
void loop()
{
    // only enforce framerate if the module wants it enforced
    if(mm->moduleWantsEnforcedFramerate() && !dm->nextFrame())
    {
        return;
    }

    // for debugging purposes, print when heap changes
    uint32 freeHeap = ESP.getFreeHeap();
    if(freeHeap != lastFreeHeap)
    {
        Serial.print("Free Heap: ");
        Serial.println(freeHeap);

        lastFreeHeap = freeHeap;
    }

    // poll fills the bitmask with the current button state. no poll, no updates.
    eh->poll();

    // have the module check its events
    mm->checkEvents();

    // have the module paint its UI
    dm->handleFrame();
}

//EOF
