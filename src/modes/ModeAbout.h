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
#ifndef mode_about_h
#define mode_about_h

#include <U8g2lib.h>
#include "../EventHandler.h"
#include "BaseMode.h"

class ModeAbout : public BaseMode
{
public:
    ModeAbout(EventHandler* const e, U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C* const u8, HardwareSerial* const hws);

    void handleEvents();
    //void paintFrame(U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C* const u8g2);
    void paintFrameInternal();

protected:
    uint8_t frameCount = 0;
    static const uint8_t everyXFrame = 7;

private:

    float_t d = 0.0;
    uint16_t position = 0; // position where in the string we are (left position of screen)
    std::string scrolltext = "              "
                             "AREA41 SECURITY CONFERENCE 2018 - PRESENTED BY - DEFCON SWITZERLAND - ZURICH SWITZERLAND "
                             "              "
                             " * LEGENDS NEVER DIE * "
                             "              "
                             "nice, you found the secret screen. the one where silly text scrolls by. "
                             "              "
                             "this badge was made with <3 by @mylaocoon and @krogoth@mastodon.social. "
                             "have you found all the easter eggs yet? "
                             "              "
                             "did we mention that there is a hidden feature to flash this badge OTA? "
                             "did you know that no unicorns were harmed by writing this scrolling text? "
                             "              "
                             "and now a quick public service announcement: "
                             "    * mess with the best, die like the rest *   "
                             "thanks for watching to the end... ";

};

#endif // mode_about_h
