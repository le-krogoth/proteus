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
#ifndef mode_ota_h
#define mode_ota_h

#include <U8g2lib.h>
#include "../EventHandler.h"
#include "BaseMode.h"
#include <ESP8266WiFi.h>
#include <ESP8266httpUpdate.h>
#include "../Config.h"

class ModeOTA : public BaseMode
{
public:
    ModeOTA(EventHandler* const e, Config* const c, U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C* const u8, HardwareSerial* const hws);

    void handleEvents();
    //void paintFrame(U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C* const u8g2);
    void paintFrameInternal();
    void cleanup();

protected:

private:
    void update();
    void printScreen(std::string message);

    // ask user if she wants to check for and install updates OTA
    static const uint8_t STATE_ASK = 0;
    static const uint8_t STATE_CHICKEN = 1;
    static const uint8_t STATE_UPDATE = 2;
    static const uint8_t STATE_NO_UPDATE_AVAIL = 3;

    // TODO define what happens afterwards. reboot? asking for reboot?

    Config* conf = NULL;
    uint8_t state = STATE_ASK; // which state are we in (display, update, etc)

    bool bUpdatedSPIFFS = false;
    bool bUpdatedFW = false;
    bool isInitialised = false;
};

#endif // mode_ota_h
