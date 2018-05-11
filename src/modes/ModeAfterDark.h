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
#ifndef mode_afterdark_h
#define mode_afterdark_h

#include <U8g2lib.h>
#include "../EventHandler.h"
#include "BaseMode.h"

class ModeAfterDark : public BaseMode
{
public:
    ModeAfterDark(EventHandler* const e, U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C* const u8, HardwareSerial* const hws);

    void handleEvents();
    //void paintFrame(U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C* const u8g2);
    void paintFrameInternal();

    //int compare(const void *a, const void *b);

protected:

private:
    uint8_t counter = 0;

    struct Flyer {       // Array of flying things
        int16_t x, y;      // Top-left position * 16 (for subpixel pos updates)
        int8_t  depth;     // Stacking order is also speed, 12-24 subpixels/frame
        uint8_t frame;     // Animation frame; Toasters cycle 0-3, Toast=255
    } flyer[4];

};

#endif // mode_afterdark_h
