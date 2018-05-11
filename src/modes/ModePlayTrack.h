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
#ifndef mode_playtrack_h
#define mode_playtrack_h

#include <U8g2lib.h>
#include "../EventHandler.h"
#include "BaseMode.h"

#include <AudioFileSourceSPIFFS.h>
#include "AudioGeneratorMOD.h"
#include "AudioOutputI2SNoDAC.h"
#include "user_interface.h"

class ModePlayTrack : public BaseMode
{
public:
    ModePlayTrack(EventHandler* const e, U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C* const u8, HardwareSerial* const hws);

    void handleEvents() override;
    //void paintFrame(U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C* const u8g2);
    void paintFrameInternal() override;

    void cleanup() override;

    bool getEnforceFramerate() override;

protected:

private:
    AudioGeneratorMOD *mod;
    AudioFileSourceSPIFFS *file;
    AudioOutputI2SNoDAC *out;

};

#endif // mode_playtrack_h
