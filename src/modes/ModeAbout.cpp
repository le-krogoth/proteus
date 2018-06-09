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
#include "ModeAbout.h"

ModeAbout::ModeAbout(EventHandler *const e, U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C* const u8, HardwareSerial *const hws) : BaseMode (e, u8, hws)
{

}

void ModeAbout::handleEvents()
{
    frameCount++;

    if(frameCount < everyXFrame)
    {
        return;
    }

    // run this only if its time to increase
    position++;

    if(position >= scrolltext.length())
    {
        position = 0;
    }
}

void ModeAbout::paintFrameInternal()
{
    // slow down the display
    // yeah, this is an ugly hack, but you know, a few days before deadline...
    if(frameCount < everyXFrame)
    {
        return;
    }
    else
    {
        frameCount = 0;
    }

    u8g2->setFont(u8g2_font_6x10_tf);

    u8g2->firstPage();
    do {

        // wobbly text
        for (int8_t j = 0; j < 13; j++)
        {
            if((position + j) < scrolltext.length())
            {
                u8g2->drawUTF8((j * 10), 14 - sin(d + (j * 0.4)) * 5, scrolltext.substr(position + j, 1).c_str());

                /*
                hs->print("x: ");
                hs->print((j * 10));
                hs->print("y: ");
                hs->print(14 - sin(d + (j * 0.4)) * 5);
                hs->print("c: ");
                hs->println(scrolltext.substr(position + j, 1).c_str());
                 */
            }
        }


    } while ( u8g2->nextPage() );

    // float d goes 1 step further on the sine evolution
    d = d + 0.10;
}




