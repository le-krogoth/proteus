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

void ModeOTA::handleEvents()
{

    // if user clicks left, update
    //update();

    // right means cancel -> leave mode
}

void ModeOTA::paintFrameInternal()
{
    u8g2->firstPage();
    do {

        switch(state)
        {}
        // TODO
        // Do you want to update?
        //
        //
        // Yes  -> left click twice runs update
        //      -> Right click cansels

        u8g2->setFont(u8g2_font_logisoso32_tf);
        u8g2->drawUTF8(0,31,"ModeOTA");

    } while ( u8g2->nextPage() );
}

void ModeOTA::cleanup()
{
    hs->println("disconnecting Wifi and stopping update");
    WiFi.disconnect(true);
}


void ModeOTA::update()
{
    // wait for WiFi connection
    if((WiFi.status() == WL_CONNECTED)) {

        Serial.println("Update SPIFFS...");

        String sSpiffsFile = conf->getUpdateServer();
        sSpiffsFile += "spiffs/file";

        t_httpUpdate_return retS = ESPhttpUpdate.updateSpiffs(sSpiffsFile);
        switch(retS) {
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

        Serial.println("Update sketch...");

        String sUpdateFile = conf->getUpdateServer();
        sUpdateFile += "fw/v2";

        t_httpUpdate_return retFW = ESPhttpUpdate.update(sUpdateFile);
        switch(retFW) {
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
    }
}

