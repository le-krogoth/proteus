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
#include "ModeManager.h"

ModeManager::ModeManager(EventHandler* const e, HardwareSerial* const hws)
{
    eh = e;
    hs = hws;

    setMode(ModeSelectMode::M_MODE_DEFAULT, ModeSelectMode::M_MODE_DEFAULT);
}

void ModeManager::checkEvents()
{
    //hs->println("checkEvents");

    //hs->println("if is Prg");

    if(eh->isPrgJustPressed())
    {
        // TODO
        // check if just left MODE_SELECTMODE
        // if so, set new mode to selected mode
        // if not change to select mode

        if(currentMode == ModeSelectMode::M_SELECT_MODE)
        {
            ModeSelectMode* m = (ModeSelectMode*)getCurrentModeObject();
            hs->print("********************** ");
            hs->print("Selected Mode is: ");
            hs->println(m->getSelectedMode());

            currentMode = m->getSelectedMode();
            setMode(currentMode, 0);
        }
        else
        {
            uint8_t oldMode = currentMode;
            currentMode = 0;
            setMode(currentMode, oldMode);
        }
    }
}

uint8_t ModeManager::getCurrentMode()
{
    return currentMode;
}

BaseMode* ModeManager::getCurrentModeObject()
{
    return currentModeObject;
}

void ModeManager::setMode(uint8_t newMode, uint8_t oldMode)
{
    if(newMode < 0 || newMode >= ModeSelectMode::M_MODE_COUNT)
    {
        currentMode = ModeSelectMode::M_MODE_DEFAULT;
    }
    else
    {
        currentMode = newMode;
    }

    if(currentModeObject)
    {
        hs->println("Freeing old object");
        delete currentModeObject;
    }

    hs->print("New mode is: ");
    hs->println(currentMode);

    switch(currentMode)
    {
        case ModeSelectMode::M_SELECT_MODE:
            currentModeObject = new ModeSelectMode(oldMode, eh, hs);
            break;
        case ModeSelectMode::M_SETUP_MODE:
            currentModeObject = new ModeSetup(eh, hs);
            break;
        case ModeSelectMode::M_KNIGHT_RIDER:
            currentModeObject = new ModeKnightRider(eh, hs);
            break;
        case ModeSelectMode::M_NICKNAME:
            currentModeObject = new ModeNickname(eh, hs);
            break;
        case ModeSelectMode::M_LOGO:
            currentModeObject = new ModeLogo(eh, hs);
            break;
        case ModeSelectMode::M_TIMETABLE:
            currentModeObject = new ModeTimeTable(eh, hs);
            break;
        case ModeSelectMode::M_UNICORN_GAME:
            currentModeObject = new ModeUnicorn(eh, hs);
            break;
        case ModeSelectMode::M_AFTER_DARK:
            currentModeObject = new BaseMode(eh, hs);
            break;
        case ModeSelectMode::M_WIFISCANNER:
            currentModeObject = new BaseMode(eh, hs);
            break;
        case ModeSelectMode::M_GAME2:
            currentModeObject = new BaseMode(eh, hs);
            break;
        default:
            currentModeObject = new BaseMode(eh, hs);
            break;
    }

    hs->println("Object instantiated");
}
