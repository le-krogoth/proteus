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
#include "modes/ModeSetup.h"
#include "modes/ModeKnightRider.h"
#include "modes/ModeNickname.h"
#include "modes/ModeLogo.h"
#include "modes/ModeAfterDark.h"
#include "modes/ModeWifiScanner.h"
#include "modes/ModePlayTrack.h"
#include "modes/ModeTellTale.h"
#include "modes/ModeOTA.h"
#include "modes/ModeSnake.h"
#include "modes/ModeAbout.h"
#include "modes/ModeEyes.h"
#include "modes/ModeRobotEyes.h"
#include "modes/ModeLoading.h"

class ModeManager
{
public:
    ModeManager(EventHandler* const e, Config* const c, U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C* const u8, HardwareSerial* const hws);

    void checkEvents();

    uint8_t getCurrentMode();
    BaseMode* getCurrentModeObject();

    bool moduleWantsEnforcedFramerate();

    void setMode(uint8_t newMode, uint8_t oldMode, bool storeMode = true);

protected:


private:
    uint8_t currentMode = 0;
    BaseMode* currentModeObject = NULL;

    HardwareSerial* hs = NULL;
    EventHandler* eh = NULL;
    Config* conf = NULL;
    U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C* u8g2 = NULL;
};

#endif
