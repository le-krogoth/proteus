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
    for(uint8_t i=0; i<4; i++) {  // Randomize initial flyer states
        flyer[i].x     = (-32 + random(160)) * 16;
        flyer[i].y     = (-32 + random( 96)) * 16;
        flyer[i].frame = random(3) ? random(4) : 255; // 66% toaster, else toast
        flyer[i].depth = 10 + random(16);             // Speed / stacking order
    }
    //qsort(flyer, 4, sizeof(struct Flyer), compare); // Sort depths

}

// Flyer depth comparison function for qsort()
//static int ModeAfterDark::compare(const void *a, const void *b) {
//    return ((struct Flyer *)a)->depth - ((struct Flyer *)b)->depth;
//}

void ModeAfterDark::handleEvents()
{

}

void ModeAfterDark::paintFrameInternal()
{
    uint8_t i, f;
    int16_t x, y;
    boolean resort = false;     // By default, don't re-sort depths

    u8g2->firstPage();
    do {

        //u8g2->setFont(u8g2_font_logisoso32_tf);
        //u8g2->drawUTF8(0,31,"After Dark");

        /*
        switch(counter)
        {
            case 0:
                u8g2->drawXBMP(0, 0, 32, 32, toaster0_bits);
                u8g2->drawXBMP(32, 0, 32, 32, toaster1_bits);
                u8g2->drawXBMP(64, 0, 32, 32, toaster2_bits);
                break;
            case 1:
                u8g2->drawXBMP(0, 0, 32, 32, toaster1_bits);
                u8g2->drawXBMP(32, 0, 32, 32, toaster0_bits);
                u8g2->drawXBMP(64, 0, 32, 32, toaster1_bits);
                break;
            case 2:
                u8g2->drawXBMP(0, 0, 32, 32, toaster2_bits);
                u8g2->drawXBMP(32, 0, 32, 32, toaster1_bits);
                u8g2->drawXBMP(64, 0, 32, 32, toaster0_bits);
                break;
            case 3:
                u8g2->drawXBMP(0, 0, 32, 32, toaster1_bits);
                u8g2->drawXBMP(32, 0, 32, 32, toaster2_bits);
                u8g2->drawXBMP(64, 0, 32, 32, toaster1_bits);
                break;
        }
         */

        for(i=0; i<4; i++) { // For each flyer...

            // First draw each item...
            f = (flyer[i].frame == 255) ? 4 : (flyer[i].frame++ & 3); // Frame #
            x = flyer[i].x / 16;
            y = flyer[i].y / 16;
            u8g2->drawXBMP(x, y, 32, 32, arrToaster[f]);
        }

        //u8g2->drawXBMP(96, 0, 32, 32, toast_bits);

        counter++;

        if(counter > 3)
        {
            counter = 0;
        }

    } while ( u8g2->nextPage() );

    for(i=0; i<4; i++)
    { // For each flyer...
        // Then update position, checking if item moved off screen...
        flyer[i].x -= flyer[i].depth * 2; // Update position based on depth,
        flyer[i].y += flyer[i].depth;     // for a sort of pseudo-parallax effect.
        if((flyer[i].y >= (64*16)) || (flyer[i].x <= (-32*16))) { // Off screen?
            if(random(7) < 5) {         // Pick random edge; 0-4 = top
                flyer[i].x = random(160) * 16;
                flyer[i].y = -32         * 16;
            } else {                    // 5-6 = right
                flyer[i].x = 128         * 16;
                flyer[i].y = random(64)  * 16;
            }
            flyer[i].frame = random(3) ? random(4) : 255; // 66% toaster, else toast
            flyer[i].depth = 10 + random(16);
            resort = true;
        }
    }
    // If any items were 'rebooted' to new position, re-sort all depths
    //if(resort) qsort(flyer, 4, sizeof(struct Flyer), compare);
}


