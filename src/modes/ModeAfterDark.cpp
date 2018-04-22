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
#include "ModeAfterDark.h"

ModeAfterDark::ModeAfterDark(EventHandler *const e, HardwareSerial *const hws) : BaseMode (e, hws)
{

}

void ModeAfterDark::handleEvents()
{

}

void ModeAfterDark::paintFrameInternal()
{
    u8g2->firstPage();
    do {

        u8g2->setFont(u8g2_font_logisoso32_tf);
        u8g2->drawUTF8(0,31,"After Dark");

    } while ( u8g2->nextPage() );
}


