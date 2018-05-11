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
#include "ModeKnightRider.h"
#include "grafx/gradient.h"

ModeKnightRider::ModeKnightRider(EventHandler *const e, U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C* const u8, HardwareSerial *const hws) : BaseMode (e, u8, hws)
{

}

void ModeKnightRider::handleEvents()
{

}

void ModeKnightRider::paintFrameInternal()
{
    // TODO try to not make it flicker
    u8g2->setAutoPageClear(0);

    u8g2->firstPage();
    do {

        if(scrollLeft)
        {
            u8g2->drawXBMP(offset, 0, 96, 32, gradient_toleft_bits);
        }
        else
        {
            u8g2->drawXBMP(offset, 0, 96, 32, gradient_toright_bits);
        }

    } while ( u8g2->nextPage() );

    u8g2->setAutoPageClear(1);

    if (scrollLeft)
    {
        offset -= 8;
    }
    else
    {
        offset += 8;
    }

    if (offset >= 100)
    {
        scrollLeft = true;
    }

    if (offset <= -80)
    {
        scrollLeft = false;
    }
}


