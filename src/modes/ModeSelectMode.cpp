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
    selectedMode = currentMode;

    if(selectedMode >= M_MODE_COUNT)
    {
        selectedMode = 1;
    }
    else if (selectedMode <= 0)
    {
        selectedMode = 1;
    }


    viewPos = selectedMode - 1;

    if(viewPos <= 0)
    {
        viewPos = 1;
    }
    else if(viewPos >= (M_MODE_COUNT - 3))
    {
        viewPos = (M_MODE_COUNT - 4);
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
        if(selectedMode > 1)
        {
            selectedMode--;
        }
        else
        {
            selectedMode = 1;
        }

        if(selectedMode - viewPos < 2 && viewPos > 1)
        {
            viewPos--;
        }
    }

    if(eh->isRightJustPressed())
    {
        if(selectedMode < M_MODE_COUNT - 1)
        {
            selectedMode++;
        }

        if(selectedMode - viewPos > 1 && viewPos < (M_MODE_COUNT - 4))
        {
            viewPos++;
        }
    }
}

void ModeSelectMode::paintFrameInternal()
{
    u8g2->firstPage();
    do {

        u8g2->setFont(u8g2_font_6x10_tf);

        for(int i = viewPos; i < viewPos + 4; i++)
        {
            u8g2->drawUTF8(6, ((i - viewPos) * 8) + 8, DISPLAY_MODES[i]);

            if(i == selectedMode)
            {
                u8g2->drawUTF8(0, ((i - viewPos) * 8) + 8, ">");
            }
        }

    } while ( u8g2->nextPage() );
}


