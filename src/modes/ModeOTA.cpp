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
#include "ModeOTA.h"

ModeOTA::ModeOTA(EventHandler *const e, Config* const c, U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C* const u8, HardwareSerial *const hws) : BaseMode (e, u8, hws)
{
    conf = c;

    u8g2->setFont(u8g2_font_6x10_tf);
    u8g2->firstPage();
    do {

        u8g2->drawUTF8(6, 13, "Blue pill or");
        u8g2->drawUTF8(6, 24, "Red pill?");

    } while ( u8g2->nextPage() );

    WiFi.begin(conf->getUpdateSSID().c_str(), conf->getUpdatePSK().c_str());
}

void ModeOTA::cleanup()
{
    hs->println("disconnecting Wifi and stopping update");
    WiFi.disconnect(true);
}

void ModeOTA::handleEvents()
{
    if(!isInitialised)
    {
        isInitialised = true;
        return;
    }

    if(state == STATE_ASK)
    {
        if(eh->isLeftJustPressed())
        {
            state = STATE_CHICKEN;
        }
        else if(eh->isRightJustPressed())
        {
            state = STATE_UPDATE;
        }
    }
    else if(state == STATE_CHICKEN)
    {
        if(eh->isRightJustPressed())
        {
            state = STATE_UPDATE;
        }
    }
}

void ModeOTA::paintFrameInternal()
{
    if(state == STATE_UPDATE)
    {
        // updates will be done by the update function
        return;
    }

    u8g2->setFont(u8g2_font_6x10_tf);

    u8g2->firstPage();
    do
    {
        switch (state)
        {
            case STATE_UPDATE:
                // should never happen, see first line to this function
                u8g2->drawUTF8(55, 10, "OTA");
                u8g2->drawUTF8(0, 21, "Updating...");
                break;
            case STATE_CHICKEN:
                u8g2->drawUTF8(55, 10, "OTA");
                u8g2->drawUTF8(0, 21, "CHICKEN CHICKEN");
                u8g2->drawUTF8(0, 32, "< Cancel");
                u8g2->drawUTF8(104, 32, "OK >");
                break;
            default:
            case STATE_ASK:
                u8g2->drawUTF8(55, 10, "OTA");
                u8g2->drawUTF8(0, 21, "Want to upgrade?");
                u8g2->drawUTF8(0, 32, "< Cancel");
                u8g2->drawUTF8(104, 32, "OK >");
                break;
        }

    } while (u8g2->nextPage());

    if (state == STATE_UPDATE)
    {
        update();
    }
}

void ModeOTA::printScreen(std::string message)
{
    u8g2->setFont(u8g2_font_6x10_tf);
    u8g2->firstPage();
    do {

        u8g2->drawUTF8(55, 10, "OTA");

        u8g2->drawUTF8(0,30,message.c_str());

    } while ( u8g2->nextPage() );
}

void ModeOTA::update()
{
    // wait for WiFi connection
    if((WiFi.status() == WL_CONNECTED))
    {
        if(!bUpdatedSPIFFS)
        {
            printScreen("Checking SPIFFS");

            Serial.println("Update SPIFFS...");

            String sSpiffsFile = conf->getUpdateServer().c_str();
            sSpiffsFile += "spiffs/file";

            t_httpUpdate_return retS = ESPhttpUpdate.updateSpiffs(sSpiffsFile);
            switch(retS) {
                case HTTP_UPDATE_FAILED:
                    printScreen("SPIFFS update failed");
                    Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
                    break;

                case HTTP_UPDATE_NO_UPDATES:
                    printScreen("SPIFFS, no update");
                    Serial.println("HTTP_UPDATE_NO_UPDATES");
                    break;

                case HTTP_UPDATE_OK:
                    printScreen("SPIFFS updated");
                    Serial.println("HTTP_UPDATE_OK");
                    bUpdatedSPIFFS = true;
                    break;
            }

            // give the user time to read
            delay(3000);
        }

        if(!bUpdatedFW)
        {
            Serial.println("Update sketch...");
            printScreen("Checking FW");

            String sUpdateFile = conf->getUpdateServer().c_str();
            sUpdateFile += "fw/v2";

            t_httpUpdate_return retFW = ESPhttpUpdate.update(sUpdateFile);
            switch (retFW) {
                case HTTP_UPDATE_FAILED:
                    printScreen("FW update failed");
                    Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s", ESPhttpUpdate.getLastError(),
                                  ESPhttpUpdate.getLastErrorString().c_str());
                    break;

                case HTTP_UPDATE_NO_UPDATES:
                    printScreen("FW, no update");
                    Serial.println("HTTP_UPDATE_NO_UPDATES");
                    break;

                case HTTP_UPDATE_OK:
                    printScreen("FW updated, reboot me now");
                    Serial.println("HTTP_UPDATE_OK");
                    bUpdatedFW = true;
                    break;
            }

            // give the user time to read
            delay(3000);
        }
    }
}
