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


DisplayManager::DisplayManager(U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C* const lib, HardwareSerial* const hs)
{
    // frame management
    setFrameRate(60);
    frameCount = 0;
    nextFrameStart = 0;
    post_render = false;
    // init not necessary, will be reset after first use
    // lastFrameStart
    // lastFrameDurationMs

    // store pointer to lib for later use
    u8g2 = lib;
    s = hs;
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
    eachFrameMillis = 1000/rate;
}

bool DisplayManager::everyXFrames(uint8_t frames)
{
    return frameCount % frames == 0;
}

bool DisplayManager::nextFrame()
{
    long now = millis();
    uint8_t remaining;

    // post render
    if (post_render) {
        lastFrameDurationMs = now - lastFrameStart;
        frameCount++;
        post_render = false;
    }

    // if it's not time for the next frame yet
    if (now < nextFrameStart) {
        remaining = nextFrameStart - now;
        // if we have more than 1ms to spare, lets sleep
        // we should be woken up by timer0 every 1ms, so this should be ok
        //if (remaining > 1)
        //    idle();
        return false;
    }

    // pre-render

    // technically next frame should be last frame + each frame but if we're
    // running a slow render we would constnatly be behind the clock
    // keep an eye on this and see how it works.  If it works well the
    // lastFrameStart variable could be eliminated completely
    nextFrameStart = now + eachFrameMillis;
    lastFrameStart = now;
    post_render = true;
    return post_render;
}

void DisplayManager::handleScreen()
{
    //s->print("Checking next frame ");

    if(this->nextFrame()) {

        //s->println("yes");

        u8g2->firstPage();
        do {

        float p = 2 * sin(d);
        float q = 2 * sin(-d);

        for (byte k = 1; k < 16; k++) {
            // some calculations are used multiple times
            // so better caclutate them once and reuse
            // the results.
            float r = sin(d + (k * 0.4));
            float t = sin(d + 3 + (k * 0.4));

            // First lets draw the top 12 balls
            fillCircle(k * 8 + p, 12 + (r * 10), 2, 1);
            // and now the bottom 12 balls
            fillCircle(k * 8 + q, 12 + (t * 10), 2, 1);

            // now we draw a line from the top ball to the bottom ball
            drawLine(k * 8 + p, 10 + (r * 10), k * 8 + q, (10 + t * 10), 1);

            // and finally we need a line from ball to ball on the top part
            if(k < 15) {
                drawLine((k * 8 + p), 12 + (r * 10), 6 + (k * 8 + p), (12 + sin(d + ((k + 1) * 0.4)) * 10), 1);
            }

            // and the bottom part
            if(k < 15) {
                drawLine((k * 8 + q), 12 + (t * 10), 6 + (k * 8 + q), (12 + sin(d + 3 + ((k + 1) * 0.4)) * 10), 1);
            }
        }

        } while ( u8g2->nextPage() );

    }
    else {
        s->println("no");
    }

    // float d goes 1 step further on the sine evolution
    d = d + 0.10;
}

/*

   // We could use font, but it's to small, so I made
  // little bitmaps for each letter. Next to that, if
  // we would use font, we need each letter to separated
  // in an array.
  // Now let's put each letter on the sine wave
  // int c makes the letters move right to left
  // and sine calculation makes them go up an down
  arduboy.drawBitmap(c, 32 - sin(d + (0 * 0.4)) * 5, LetterH, 8, 8, 1);
  arduboy.drawBitmap(c + 10, 32 - sin(d + (1 * 0.4)) * 5, LetterE, 8, 8, 1);
  arduboy.drawBitmap(c + 20, 32 - sin(d + (2 * 0.4)) * 5, LetterL, 8, 8, 1);
  arduboy.drawBitmap(c + 30, 32 - sin(d + (3 * 0.4)) * 5, LetterL, 8, 8, 1);
  arduboy.drawBitmap(c + 40, 32 - sin(d + (4 * 0.4)) * 5, LetterO, 8, 8, 1);
  arduboy.drawBitmap(c + 50, 32 - sin(d + (5 * 0.4)) * 5, Letter, 8, 8, 1);
  arduboy.drawBitmap(c + 60, 32 - sin(d + (6 * 0.4)) * 5, LetterA, 8, 8, 1);
  arduboy.drawBitmap(c + 70, 32 - sin(d + (7 * 0.4)) * 5, LetterR, 8, 8, 1);
  arduboy.drawBitmap(c + 80, 32 - sin(d + (8 * 0.4)) * 5, LetterD, 8, 8, 1);
  arduboy.drawBitmap(c + 90, 32 - sin(d + (9 * 0.4)) * 5, LetterU, 8, 8, 1);
  arduboy.drawBitmap(c + 100, 32 - sin(d + (10 * 0.4)) * 5, LetterB, 8, 8, 1);
  arduboy.drawBitmap(c + 110, 32 - sin(d + (11 * 0.4)) * 5, LetterO, 8, 8, 1);
  arduboy.drawBitmap(c + 120, 32 - sin(d + (12 * 0.4)) * 5, LetterY, 8, 8, 1);
// end wobbling text
 * */


// Graphics

void DisplayManager::clear()
{
    fillScreen(BLACK);
}

void DisplayManager::drawPixel(int x, int y, uint8_t color)
{
    u8g2->drawPixel(x, y);
}

/*uint8_t DisplayManager::getPixel(uint8_t x, uint8_t y)
{
    uint8_t row = y / 8;
    uint8_t bit_position = y % 8;
    return (sBuffer[(row*WIDTH) + x] & _BV(bit_position)) >> bit_position;
}*/

void DisplayManager::drawCircle(int16_t x0, int16_t y0, uint8_t r, uint8_t color)
{
    u8g2->drawCircle(x0, y0, r);
}

void DisplayManager::fillCircle(int16_t x0, int16_t y0, uint8_t r, uint8_t color)
{
    u8g2->drawDisc(x0, y0, r);
}

void DisplayManager::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color)
{
    u8g2->drawLine(x0, y0, x1, y1);
}

void DisplayManager::drawRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t color)
{
    u8g2->drawFrame(x, y, w, h);
}

void DisplayManager::fillRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t color)
{
    u8g2->drawBox(x, y, w, h);
}

void DisplayManager::fillScreen(uint8_t color)
{

}

void DisplayManager::drawRoundRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t r, uint8_t color)
{
    u8g2->drawRFrame(x, y, w, h, r);
}

void DisplayManager::fillRoundRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t r, uint8_t color)
{
    u8g2->drawRBox(x, y, w, h, r);
}

void DisplayManager::drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color)
{

}

void DisplayManager::fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color)
{

}

void DisplayManager::drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t w, uint8_t h, uint8_t color)
{

}

void DisplayManager::drawChar(int16_t x, int16_t y, unsigned char c, uint8_t color, uint8_t bg, uint8_t size)
{

}

/*
void DisplayManager::setTextSize(uint8_t s)
{
    // textsize must always be 1 or higher
    textsize = max(1,s);
}

void DisplayManager::setTextWrap(boolean w)
{
    wrap = w;
}

size_t DisplayManager::write(uint8_t c)
{
    if (c == '\n')
    {
        cursor_y += textsize*8;
        cursor_x = 0;
    }
    else if (c == '\r')
    {
        // skip em
    }
    else
    {
        drawChar(cursor_x, cursor_y, c, 1, 0, textsize);
        cursor_x += textsize*6;
        if (wrap && (cursor_x > (WIDTH - textsize*6)))
        {
            // calling ourselves recursively for 'newline' is
            // 12 bytes smaller than doing the same math here
            write('\n');
        }
    }
}

void DisplayManager::display()
{
    this->paintScreen(sBuffer);
}
 */
