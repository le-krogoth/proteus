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

    setMode(MODE_DEFAULT, MODE_DEFAULT);
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

        if(currentMode == MODE_SELECTMODE)
        {
            ModeSelectMode* m = (ModeSelectMode*)getCurrentModeObject();
            hs->print("********************** ");
            hs->print("Selected Mode is: ");
            hs->println(m->getSelectedMode());

            currentMode = m->getSelectedMode() + 1;
            setMode(currentMode, 0);
        }
        else
        {
            uint8_t oldMode = currentMode;
            currentMode = 0;
            setMode(currentMode, oldMode);
        }

/*

        currentMode += 1;

        if(currentMode > MODE_COUNT)
        {
            currentMode = 1;
        }

        //hs->print("PrgJustPressed. New Mode: ");
        hs->println(currentMode);

        setMode(currentMode);
        */
        /*
        // if we are in the selectmode mode, user wants to leave
        // otherwise user wants to enter the selectmode mode.
        if(currentMode == MODE_SELECTMODE)
        {
            // TODO
            // find out which one was selected, ask the current module for that...
            setMode(MODE_DEFAULT);
        }
        else
        {
            setMode(MODE_SELECTMODE);
        }
         */
    }

    //hs->println("if is Right");
/*
    if(eh->isRightJustPressed())
    {
        currentMode += 1;

        if(currentMode > MODE_COUNT)
        {
            currentMode = 1;
        }

        //hs->print("RightJustPressed. New Mode: ");
        hs->println(currentMode);

        setMode(currentMode);
    }
*/
    /*
    if(ev->isLeftJustPressed() && ev->isRightPressed())
    {
        mm->nextMode();
    }

    if(ev->isRightJustPressed() && ev->isLeftPressed())
    {
        mm->nextMode();
    }

    if(currentMode != mm->getCurrentMode())
    {
        Serial.print("Switching Mode to: ");
        Serial.println(mm->getCurrentMode());
    }
     */
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
    if(newMode < 0 || newMode > MODE_COUNT)
    {
        currentMode = MODE_DEFAULT;
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
        case MODE_SELECTMODE:
            hs->println("SELECTMODE");
            currentModeObject = new ModeSelectMode(oldMode, eh, hs);
            break;
        case MODE_LOGO:
            hs->println("LOGO");
            currentModeObject = new ModeUnicorn(eh, hs);
            break;
        case MODE_GAME_A:
            hs->println("UNICORN");
            currentModeObject = new ModeUnicorn(eh, hs);
            break;
        case MODE_TIMETABLE:
            hs->println("TIMETABLE");
            //currentModeObject = new ModeUnicorn(eh, hs);
            currentModeObject = new ModeTimeTable(eh, hs);
            break;
        default:
            hs->println("DEFAULT");
            currentModeObject = new BaseMode(eh, hs);
            break;
    }

    hs->println("Object instantiated");
}

/*
uint8_t ModeManager::nextMode()
{
    currentMode += 1;

    if(currentMode > MODE_COUNT)
    {
        currentMode = 1;
    }

    return currentMode;
}
 */