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
#include "grafx/toaster.h"

ModeAfterDark::ModeAfterDark(EventHandler *const e, U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C* const u8, HardwareSerial *const hws) : BaseMode (e, u8, hws)
{
    for(uint8_t i=0; i<4; i++)
    {
        // Randomize initial flyThing states
        flyThing[i].x     = (-32 + random(160)) * 16;
        flyThing[i].y     = (-32 + random( 96)) * 16;
        flyThing[i].frame = random(3) ? random(4) : 255; // 66% toaster, else toast
        flyThing[i].depth = 10 + random(16);             // Speed / stacking order
    }
}

void ModeAfterDark::handleEvents()
{

}

void ModeAfterDark::paintFrameInternal()
{
    uint8_t i, f;
    int16_t x, y;

    u8g2->firstPage();
    do {

        for(i=0; i<4; i++)
        {
            // For each flyThing...
            // First draw each item...
            f = (flyThing[i].frame == 255) ? 4 : (flyThing[i].frame++ & 3); // Frame #
            x = flyThing[i].x / 16;
            y = flyThing[i].y / 16;

            // draw mask

            //draw image transparent
            u8g2->setBitmapMode(1);
            u8g2->drawXBMP(x, y, 32, 32, arrToaster[f]);
        }

        counter++;

        if(counter > 3)
        {
            counter = 0;
        }

    } while ( u8g2->nextPage() );

    for(i=0; i<4; i++)
    {
        // For each flyThing...
        // Then update position, checking if item moved off screen...
        flyThing[i].x -= flyThing[i].depth * 2; // Update position based on depth,
        flyThing[i].y += flyThing[i].depth;     // for a sort of pseudo-parallax effect.

        if((flyThing[i].y >= (64*16)) || (flyThing[i].x <= (-32*16)))
        {
            // Off screen?
            if(random(7) < 5)
            {
                // Pick random edge; 0-4 = top
                flyThing[i].x = random(160) * 16;
                flyThing[i].y = -32         * 16;
            }
            else
            {
                // 5-6 = right
                flyThing[i].x = 128         * 16;
                flyThing[i].y = random(64)  * 16;
            }

            flyThing[i].frame = random(3) ? random(4) : 255; // 66% toaster, else toast
            flyThing[i].depth = 10 + random(16);
        }
    }
}

//eof