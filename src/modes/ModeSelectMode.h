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
#ifndef mode_selectmode_h
#define mode_selectmode_h

#include <U8g2lib.h>
#include <pod/SimpleList.h>
#include "../EventHandler.h"
#include "BaseMode.h"

class ModeSelectMode : public BaseMode
{
public:
    ModeSelectMode(uint8_t currentMode, EventHandler* const e, U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C* const u8, HardwareSerial* const hws);

    void handleEvents();
    //void paintFrame(U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C* const u8g2);
    void paintFrameInternal();

    uint8_t getSelectedMode();

    void loadApps();
    void loadApp(uint8_t id, const char* name);

    // this is a bit of fake, this is the amount of standard modes, not including easter eggs...
    static const uint8_t M_MODE_COUNT   = 10;

    static const uint8_t M_MODE_DEFAULT = 0;

    static const uint8_t M_SELECT_MODE  = 0;
    static const uint8_t M_LOGO         = 1;
    static const uint8_t M_SNAKE        = 2;
    static const uint8_t M_KNIGHT_RIDER = 3;
    static const uint8_t M_NICKNAME     = 4;
    static const uint8_t M_TIMETABLE    = 5;
    static const uint8_t M_UNICORN_GAME = 6;
    static const uint8_t M_AFTER_DARK   = 7;
    static const uint8_t M_WIFISCANNER  = 8;
    static const uint8_t M_SETUP_MODE   = 9;
    // WARNING, if you add a Module, add +1 on the M_MODE_COUNT

    static const uint8_t M_EASTEREGG_1  = 20;
    static const uint8_t M_PLAY_TRACK   = 21;
    static const uint8_t M_TELL_TALE    = 22;
    static const uint8_t M_OTA          = 23;
    static const uint8_t M_ABOUT        = 24;

protected:

private:
    uint8_t selectedMode = M_SETUP_MODE;
    uint8_t viewPos = 1;

    const char* DISPLAY_MODES[M_MODE_COUNT] = {
            "Select Mode",
            "Show Logo",
            "Snake (on a badge)",
            "Cyclope",
            "Show Nickname",
            "Timetable",
            "Unicorn",
            "After Dark",
            "Wifi Scanner",
            "Configure your badge"
    };
};

#endif
