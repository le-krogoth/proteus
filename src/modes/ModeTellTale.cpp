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
#include "ModeTellTale.h"

ModeTellTale::ModeTellTale(EventHandler *const e, U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C* const u8, HardwareSerial *const hws) : BaseMode (e, u8, hws)
{
    u8g2->setFont(u8g2_font_6x10_tf);
    u8g2->firstPage();
    do {

        u8g2->drawUTF8(0, 16, "Listen to this...");

    } while ( u8g2->nextPage() );

    hs->println("Increasing CPU frequency");

    system_update_cpu_freq(SYS_CPU_160MHZ);

    hs->println("Initialise audio");
    out = new AudioOutputI2SNoDAC();
    out->begin();

    hs->println("Initialise SAM");
    sam = new ESP8266SAM();

    // from ==Phrack Inc.==
    // Volume One, Issue 7, Phile 3 of 10
    // \/\The Conscience of a Hacker/\/
    // by
    // +++The Mentor+++
    // Written on January 8, 1986
    story[0] = "Another one got caught today, it's all over the papers.";
    story[1] = "Teenager arrested in Computer Crime Scandal.";
    story[2] = "Hacker Arrested after Bank Tampering.";
    story[3] = "Damn kids.";
    story[4] = "They're all alike.";
    story[5] = "But did you, in your three-piece psychology and 1950's technobrain, ever take a look behind the eyes of the hacker?";
    story[6] = "Did you ever wonder what made him tick, what forces shaped him, what may have molded him?";
    story[7] = "I am a hacker, enter my world...";
    story[8] = "Damn underachiever.";
    story[9] = "They're all alike.";

    hs->println("Now let me begin");
}

// let's not have pauses in the track, run as fast as possible
bool ModeTellTale::getEnforceFramerate()
{
    return false;
}

void ModeTellTale::cleanup()
{
    hs->println("Decreasing CPU frequency");
    system_update_cpu_freq(SYS_CPU_80MHZ);

    delete(sam);
    delete(out);
}

void ModeTellTale::handleEvents()
{
// do nothing
}

void ModeTellTale::paintFrameInternal()
{
    if(linesSpoken >= 0 && linesSpoken < 10)
    {
            sam->Say(out, story[linesSpoken].c_str());
            delay(500);
            linesSpoken++;
    }

    // do nothing
    u8g2->setFont(u8g2_font_6x10_tf);
    u8g2->firstPage();
    do {

        if(linesSpoken >= 10)
        {
            u8g2->drawUTF8(0, 11, "Nothing to see here,");
            u8g2->drawUTF8(0, 22, "move along");
        }
        else
        {
            u8g2->drawUTF8(0, 16, story[linesSpoken].c_str());
        }

    } while ( u8g2->nextPage() );
}


