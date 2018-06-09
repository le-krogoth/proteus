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
#include "DisplayManager.h"


DisplayManager::DisplayManager(ModeManager* const m, U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C* const u8, HardwareSerial* const hws)
{
    // store pointer to lib for later use
    mm = m;
    u8g2 = u8;
    hs = hws;

    // frame management
    setFrameRate(60);
    frameCount = 0;
    nextFrameStart = 0;
    post_render = false;
    // init not necessary, will be reset after first use
    // lastFrameStart
    // lastFrameDurationMs
}

void DisplayManager::begin()
{
    u8g2->begin();
    u8g2->clearDisplay();
    u8g2->setFont(u8g2_font_logisoso32_tf);
    u8g2->setFontMode(0);    // enable transparent mode, which is faster
}

void DisplayManager::showBootLogo()
{
    u8g2->clearDisplay();

    for(int8_t y = -32; y <= 10; y++) {

        u8g2->firstPage();
        do {
            //u8g2.setFont(u8g2_font_logisoso32_tf);
            //u8g2->setFont(u8g2_font_m2icon_7_tf);
            //u8g2.drawUTF8(0,31,"Proteus");
            //u8g2.drawXBM( 1, 2, 125,32, area41logo);
            u8g2->drawXBMP(1, y, 125, 32, area41logo);

            //u8g2->drawXBMP(50, 20, 16, 16, spriteUnicorn_1);

            //arduboy.drawBitmap(unicorn.pos.x, unicorn.pos.y - 15, animationFramesUnicornMask[unicorn.frame], 16, 16, BLACK);
            //arduboy.drawBitmap(unicorn.pos.x, unicorn.pos.y - 15, animationFramesUnicorn[unicorn.frame], 16, 16, WHITE);

            if(y >= 3) {
                u8g2->setFont(u8g2_font_micro_tr);
                u8g2->drawUTF8(48, 6, "welcome to");
            }

        } while ( u8g2->nextPage() );

        delay(10);
        // longer delay post boot, we put it inside the loop to
        // save the flash calling clear/delay again outside the loop
        if (y==-30) {
            delay(250);
        }
    }

    delay(2000);
}

//Frame management

void DisplayManager::setFrameRate(uint8_t rate)
{
    frameRate = rate;
    eachFrameMillis = 1000 / rate;
}

bool DisplayManager::everyXFrames(uint8_t frames)
{
    return frameCount % frames == 0;
}

bool DisplayManager::nextFrame()
{
    long now = millis();
    //uint8_t remaining;

    // post render
    if (post_render) {
        lastFrameDurationMs = now - lastFrameStart;
        frameCount++;
        post_render = false;
    }

    // if it's not time for the next frame yet
    if (now < nextFrameStart) {
        // remaining = nextFrameStart - now;
        // if we have more than 1ms to spare, lets sleep
        // we should be woken up by timer0 every 1ms, so this should be ok
        //if (remaining > 1)
        //    idle();
        return false;
    }

    // pre-render

    // technically next frame should be last frame + each frame but if we're
    // running a slow render we would constantly be behind the clock
    // keep an eye on this and see how it works.  If it works well the
    // lastFrameStart variable could be eliminated completely
    nextFrameStart = now + eachFrameMillis;
    lastFrameStart = now;
    post_render = true;

    return post_render;
}

void DisplayManager::handleFrame()
{
    //hs->println("handle Frame");

    BaseMode* m = mm->getCurrentModeObject();

    //hs->println("handle events");
    m->handleEvents();

    //hs->println("paint frame");
    m->paintFrame();
}
