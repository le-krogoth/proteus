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
#ifndef mode_wifiscanner_h
#define mode_wifiscanner_h

#include "ESP8266WiFi.h"

#include <U8g2lib.h>
#include "../EventHandler.h"
#include "BaseMode.h"
#include "../pod/SimpleList.h"
#include "../pod/AP.h"

class ModeWifiScanner : public BaseMode
{
public:
    ModeWifiScanner(EventHandler* const e, HardwareSerial* const hws);

    void handleEvents();
    //void paintFrame(U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C* const u8g2);
    void paintFrameInternal();

protected:

private:
    uint8_t selectedAP = 0;
    uint8_t viewPos = 0;
    SimpleList<AP>* sAP = new SimpleList<AP>;

    bool loadAPs();
    void initCurrentAP();
    String getEncStr(uint8_t encryptionCode);
};

#endif // mode_wifiscanner_h
