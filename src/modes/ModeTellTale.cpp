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
#include "ModeTellTale.h"

ModeTellTale::ModeTellTale(EventHandler *const e, U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C* const u8, HardwareSerial *const hws) : BaseMode (e, u8, hws)
{
    u8g2->setFont(u8g2_font_6x10_tf);
    u8g2->firstPage();
    do {

        u8g2->drawUTF8(0, 16, "Listen to this...");

    } while ( u8g2->nextPage() );

    out = new AudioOutputI2SNoDAC();

    out->begin();
}

// let's not have pauses in the track, run as fast as possible
bool ModeTellTale::getEnforceFramerate()
{
    return false;
}

void ModeTellTale::cleanup()
{

}

void ModeTellTale::handleEvents()
{
    ESP8266SAM *sam = new ESP8266SAM;
    sam->Say(out, "Can you hear me now?");
    delay(500);
    sam->Say(out, "I can't hear you!");
    delete sam;
}

void ModeTellTale::paintFrameInternal()
{
    // do nothing
}


