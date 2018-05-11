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
#include "ModeNickname.h"

ModeNickname::ModeNickname(EventHandler *const e, Config* const c, U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C* const u8, HardwareSerial *const hws) : BaseMode (e, u8, hws)
{
    conf = c;

    hs->println("before getting nick");
}

void ModeNickname::handleEvents()
{

}

void ModeNickname::paintFrameInternal()
{
    // only do this if not initialised yet
    if(nickname.length() == 0)
    {
        nickname = conf->getNickname();

        hs->println("after getting nick");

        if (nickname.length() <= 4) {
            u8g2->setFont(u8g2_font_logisoso32_tf);
        } else if (nickname.length() <= 8) {
            u8g2->setFont(u8g2_font_logisoso24_tf);
        } else {
            u8g2->setFont(u8g2_font_logisoso16_tf);
        }
    }

    u8g2->firstPage();
    do {

        u8g2->drawUTF8(0,31, nickname.c_str());

    } while ( u8g2->nextPage() );
}


