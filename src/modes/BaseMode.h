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
#ifndef basemode_h
#define basemode_h

#include <U8g2lib.h>
#include "../EventHandler.h"

class BaseMode
{
public:
    BaseMode(EventHandler* const e, U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C* const u8, HardwareSerial* const hws);
    virtual ~BaseMode();

    virtual void handleEvents();
    virtual void paintFrame();
    virtual void paintFrameInternal();
    virtual void cleanup();
    virtual bool getEnforceFramerate();

    /// Clears display.
    void clear();

    /// Sets a single pixel on the screen buffer to white or black.
    void drawPixel(int x, int y, uint8_t color);

    //uint8_t getPixel(uint8_t x, uint8_t y);

    /// Draw a circle of a defined radius.
    void drawCircle(int16_t x0, int16_t y0, uint8_t r, uint8_t color);

    /// Draws a filled-in circle.
    void fillCircle(int16_t x0, int16_t y0, uint8_t r, uint8_t color);

    /// Draws a line between two points.
    void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color);

    /// Draws a rectangle of a width and height.
    void drawRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t color);

    /// Draws a filled-in rectangle.
    void fillRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t color);

    /// Fills the screen with white or black.
    void fillScreen(uint8_t color);

    /// Draws a rectangle with rounded edges.
    void drawRoundRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t r, uint8_t color);

    /// Draws a filled-in rectangle with rounded edges.
    void fillRoundRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t r, uint8_t color);

    /// Draws the outline of a triangle.
    void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color);

    /// Draws a filled-in triangle.
    void fillTriangle (int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color);

    /// Draws a bitmap from program memory to a specific X/Y
    void drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t w, uint8_t h, uint8_t color);

    /// Draws an ASCII character at a point.
    void drawChar(int16_t x, int16_t y, unsigned char c, uint8_t color, uint8_t bg, uint8_t size);

protected:

    EventHandler* eh = NULL;
    HardwareSerial* hs = NULL;

    U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C* u8g2 = NULL;

private:
    // sinus
    int i = 0;
    float d = 0.0;
    int c = 129;
};

#endif
