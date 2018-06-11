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
#include "ModePlayTrack.h"

ModePlayTrack::ModePlayTrack(EventHandler *const e, U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C* const u8, HardwareSerial *const hws) : BaseMode (e, u8, hws)
{
    u8g2->setFont(u8g2_font_6x10_tf);
    u8g2->firstPage();
    do {

        u8g2->drawUTF8(0, 8, "Astronauts Requiem");
        u8g2->drawUTF8(48, 16, "by");
        u8g2->drawUTF8(42, 24, "u4ia");

    } while ( u8g2->nextPage() );

    // Astronauts Requiem by u4ia
    const char* modFile = "/astro2.mod";

    hs->println("Increasing CPU frequency");

    system_update_cpu_freq(SYS_CPU_160MHZ);

    file = new AudioFileSourceSPIFFS(modFile);
    out = new AudioOutputI2SNoDAC();

    mod = new AudioGeneratorMOD();
    mod->SetBufferSize(3 * 1024);
    mod->SetSampleRate(44100);
    mod->SetStereoSeparation(32);
    mod->begin(file, out);
}

// let's not have pauses in the track, run as fast as possible
bool ModePlayTrack::getEnforceFramerate()
{
    return false;
}

void ModePlayTrack::cleanup()
{
    system_update_cpu_freq(SYS_CPU_80MHZ);
    delete(mod);
    delete(file);
    delete(out);
}

void ModePlayTrack::handleEvents()
{
    if (mod->isRunning()) {
        if (!mod->loop()) mod->stop();
    } else {
        Serial.printf("MOD done\n");
    }
}

void ModePlayTrack::paintFrameInternal()
{
    // do nothing
}


