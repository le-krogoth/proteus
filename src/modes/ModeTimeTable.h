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
#ifndef mode_timetable_h
#define mode_timetable_h

#include <FS.h>
#include <ArduinoJson.h>

#include <U8g2lib.h>
#include "../EventHandler.h"
#include "BaseMode.h"
#include "../pod/Talk.h"
#include "../pod/SimpleList.h"

class ModeTimeTable : public BaseMode
{
public:
    ModeTimeTable(EventHandler* const e, U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C* const u8, HardwareSerial* const hws);

    void handleEvents();
    //void paintFrame(U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C* const u8g2);
    void paintFrameInternal();
    bool loadTimeTable();

protected:

private:
    uint8_t currentTalk = 0;
    //LinkedList<Talk*> lTalks = LinkedList<Talk*>();
    SimpleList<Talk>* sTalks = new SimpleList<Talk>;

    int16_t titleOffset = 0;
    int16_t speakerOffset = 0;

    bool titleScrollLeft = true;
    bool speakerScrollLeft = true;

    uint16_t titleWidth = 0;
    uint16_t speakerWidth = 0;

    void initCurrentTalk();
};

#endif
