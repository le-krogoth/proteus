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
#ifndef modemanager_h
#define modemanager_h

#include <Arduino.h>
#include "EventHandler.h"
#include "modes/BaseMode.h"
#include "modes/ModeTimeTable.h"
#include "modes/ModeUnicorn.h"
#include "modes/ModeSelectMode.h"

#define MODE_SELECTMODE 0
#define MODE_LOGO 1
#define MODE_NICK 2
#define MODE_TIMETABLE 3
#define MODE_GAME_A 4
#define MODE_WIFISERVER 5

#define MODE_COUNT 5
#define MODE_DEFAULT 0

class ModeManager
{
public:
    ModeManager(EventHandler* const e, HardwareSerial* const hws);

    void checkEvents();

    uint8_t getCurrentMode();
    BaseMode* getCurrentModeObject();
    //uint8_t nextMode();
    void setMode(uint8_t newMode, uint8_t oldMode);


protected:


private:
    uint8_t currentMode;
    BaseMode* currentModeObject = NULL;

    HardwareSerial* hs = NULL;
    EventHandler* eh = NULL;
};

#endif
