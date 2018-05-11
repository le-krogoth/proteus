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
#include "ModeUnicorn.h"

ModeUnicorn::ModeUnicorn(EventHandler *const e, U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C* const u8, HardwareSerial *const hws) : BaseMode (e, u8, hws)
{

}

void ModeUnicorn::handleEvents()
{

}

void ModeUnicorn::paintFrameInternal()
{
    frameCounter++;

    if(frameCounter > 1)
    {
        frameCounter = 0;

        currentSprite++;

        if(currentSprite > 2)
        {
            currentSprite = 0;
        }
    }

    u8g2->firstPage();
    do {

//u8g2.setFont(u8g2_font_logisoso32_tf);
//u8g2->setFont(u8g2_font_m2icon_7_tf);
//u8g2.drawUTF8(0,31,"Proteus");
//u8g2.drawXBM( 1, 2, 125,32, area41logo);



        u8g2->drawXBMP(10, 10, 22, 21, unicorn_1_bits);

        switch(currentSprite)
        {
            case 0:
                u8g2->drawXBMP(50, 10, 22, 21, afUnicorn[currentSprite]);
                break;
            case 2:
                u8g2->drawXBMP(50, 6, 22, 21, afUnicorn[currentSprite]);
                break;
            default:
                u8g2->drawXBMP(50, 10, 26, 20, afUnicorn[currentSprite]);
                break;
        }

    u8g2->drawXBMP(90, 0, 32, 32, toaster_0_bits);

//arduboy.drawBitmap(unicorn.pos.x, unicorn.pos.y - 15, animationFramesUnicornMask[unicorn.frame], 16, 16, BLACK);
//arduboy.drawBitmap(unicorn.pos.x, unicorn.pos.y - 15, animationFramesUnicorn[unicorn.frame], 16, 16, WHITE);

    } while ( u8g2->nextPage() );
}


