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
#ifndef displaymanager_h
#define displaymanager_h

#include <SPI.h>
#include <Print.h>
#include <limits.h>
#include <U8g2lib.h>
#include <Arduino.h>
#include "ModeManager.h"
#include "grafx/logos.h"

#define INVERT 2 //< lit/unlit pixel
#define WHITE 1 //< lit pixel
#define BLACK 0 //< unlit pixel

class DisplayManager
{
public:
    DisplayManager(ModeManager* const m, U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C* const u8, HardwareSerial* const hws);

    /// Initializes the hardware
    void begin();

    // Scrolls in the logo
    void showBootLogo();

    void setFrameRate(uint8_t rate);
    bool nextFrame();
    bool everyXFrames(uint8_t frames);

    void handleFrame();

    uint8_t frameRate;
    uint16_t frameCount;
    uint8_t eachFrameMillis;
    long lastFrameStart;
    long nextFrameStart;
    bool post_render;
    uint8_t lastFrameDurationMs;

protected:

private:
    ModeManager* mm;

    U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C* u8g2 = NULL;
    HardwareSerial* hs = NULL;
};

#endif
