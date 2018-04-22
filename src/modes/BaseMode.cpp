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
#include "BaseMode.h"

BaseMode::BaseMode(EventHandler* const e, HardwareSerial* const hws)
{
    eh = e;
    hs = hws;
}

BaseMode::~BaseMode()
{
    cleanup();
}

// TODO
// better have that given in the constructor (via modemanager) than doing it
// in every event. I think we can spare memory for that additional pointer...
void BaseMode::lazySetU8g2(U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C* const u8)
{
    // lazy setting
    if(u8g2 == NULL)
    {
        u8g2 = u8;
    }
}

void BaseMode::handleEvents()
{
    // do nothing
}

void BaseMode::cleanup()
{
    // do nothing
}

void BaseMode::paintFrame(U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C* const u8)
{
    lazySetU8g2(u8);

    paintFrameInternal();
}

void BaseMode::paintFrameInternal()
{
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

    // float d goes 1 step further on the sine evolution
    d = d + 0.10;
}

// Graphics

void BaseMode::clear()
{
    fillScreen(1);
}

void BaseMode::drawPixel(int x, int y, uint8_t color)
{
    u8g2->drawPixel(x, y);
}

/*uint8_t BaseMode::getPixel(uint8_t x, uint8_t y)
{
    uint8_t row = y / 8;
    uint8_t bit_position = y % 8;
    return (sBuffer[(row*WIDTH) + x] & _BV(bit_position)) >> bit_position;
}*/

void BaseMode::drawCircle(int16_t x0, int16_t y0, uint8_t r, uint8_t color)
{
    u8g2->drawCircle(x0, y0, r);
}

void BaseMode::fillCircle(int16_t x0, int16_t y0, uint8_t r, uint8_t color)
{
    u8g2->drawDisc(x0, y0, r);
}

void BaseMode::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color)
{
    u8g2->drawLine(x0, y0, x1, y1);
}

void BaseMode::drawRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t color)
{
    u8g2->drawFrame(x, y, w, h);
}

void BaseMode::fillRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t color)
{
    u8g2->drawBox(x, y, w, h);
}

void BaseMode::fillScreen(uint8_t color)
{

}

void BaseMode::drawRoundRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t r, uint8_t color)
{
    u8g2->drawRFrame(x, y, w, h, r);
}

void BaseMode::fillRoundRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t r, uint8_t color)
{
    u8g2->drawRBox(x, y, w, h, r);
}

void BaseMode::drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color)
{

}

void BaseMode::fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color)
{

}

void BaseMode::drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t w, uint8_t h, uint8_t color)
{

}

void BaseMode::drawChar(int16_t x, int16_t y, unsigned char c, uint8_t color, uint8_t bg, uint8_t size)
{

}