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
#include "ModeSelectMode.h"

ModeSelectMode::ModeSelectMode(uint8_t currentMode, EventHandler *const e, HardwareSerial *const hws) : BaseMode (e, hws)
{
    // this is a bit of a hack, but the selected mode in here == the modes outside - 1
    // but we want to make sure that we do not lower the uint below 0...
    selectedMode = (currentMode > 0) ? currentMode - 1 : currentMode;

    if(selectedMode >= 8)
    {
        selectedMode = 0;
    }
}

uint8_t ModeSelectMode::getSelectedMode()
{
    return selectedMode;
}

void ModeSelectMode::handleEvents()
{
    if(eh->isLeftJustPressed())
    {
        if(selectedMode == 0)
        {
            selectedMode = 8 - 1;
        }
        else
        {
            selectedMode--;
        }
    }

    if(eh->isRightJustPressed())
    {
        selectedMode++;
        if(selectedMode >= 8)
        {
            selectedMode = 0;
        }
    }
}

void ModeSelectMode::paintFrameInternal()
{
    u8g2->firstPage();
    do {

//u8g2.setFont(u8g2_font_logisoso32_tf);
//u8g2->setFont(u8g2_font_m2icon_7_tf);
//u8g2.drawUTF8(0,31,"Proteus");

        u8g2->setFont(u8g2_font_6x10_tf);
        u8g2->drawUTF8(6, 8, "Setup");
        u8g2->drawUTF8(6, 16, "Show Logo");
        u8g2->drawUTF8(6, 24, "Timetable");
        u8g2->drawUTF8(6, 32, "Show Nick");

        u8g2->drawUTF8(70, 8, "Yatsee");
        u8g2->drawUTF8(70, 16, "Fireplace");
        u8g2->drawUTF8(70, 24, "Bomber Pac");
        u8g2->drawUTF8(70, 32, "Unicorn");

        if(selectedMode < 4)
        {
            u8g2->drawUTF8(0, (selectedMode * 8) + 8, ">");
        }
        else
        {
            u8g2->drawUTF8(64, (selectedMode * 8) - 24, ">");
        }

    } while ( u8g2->nextPage() );
}


