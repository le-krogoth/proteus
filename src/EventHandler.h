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
#ifndef eventhandler_h
#define eventhandler_h

#include <Arduino.h>

/*
 * Search for a file called "*eagle.app.v6.*.ld"
 * It usually is at:
 * /home/user/.platformio/packages/framework-arduinoespressif8266/tools/sdk/ld/eagle.app.v6.common.ld
 * Then add this line:
 * PROVIDE(PIN_IN = 0x60000318);
 * accessing PIN_IN will give you access to the register
 * with all the pins current states
 * */
extern volatile uint32_t PIN_IN;

#define PRG_BUTTON 0
#define RIGHT_BUTTON 12
#define LEFT_BUTTON 13

#define PRG_BUTTON_BIT bit(PRG_BUTTON)
#define RIGHT_BUTTON_BIT bit(RIGHT_BUTTON)
#define LEFT_BUTTON_BIT bit(LEFT_BUTTON)

class EventHandler
{
public:
    EventHandler(HardwareSerial* const hws);

    // run once every frame to update current bitmask state
    void poll();

    boolean pressed(uint16_t buttons);
    boolean notPressed(uint16_t buttons);
    boolean justReleased(uint16_t button);
    boolean justPressed(uint16_t button);

    // returns true if button is current depressed
    boolean isPrgPressed();
    boolean isRightPressed();
    boolean isLeftPressed();

    // returns true if button was just pressed (previously unpressed)
    boolean isPrgJustPressed();
    boolean isRightJustPressed();
    boolean isLeftJustPressed();

protected:


private:
    uint16_t lastBitmask = 0;
    uint16_t currentBitmask = 0;

    HardwareSerial* hs;
};

#endif
