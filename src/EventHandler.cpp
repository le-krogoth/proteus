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
#include "EventHandler.h"

EventHandler::EventHandler(HardwareSerial* const hws)
{
    hs = hws;
}

void EventHandler::poll()
{
    lastBitmask = currentBitmask;
    currentBitmask = ((~PIN_IN) & (bit(0) | bit(12) | bit(13)));
}

boolean EventHandler::pressed(uint16_t buttons)
{
    return (currentBitmask & buttons) == buttons;
}

boolean EventHandler::notPressed(uint16_t buttons)
{
    return (currentBitmask & buttons) == 0;
}

// You should not pass it multiple buttons as you can with some of the other button
// functions.
boolean EventHandler::justReleased(uint16_t button)
{
    return ((lastBitmask & button) && !(currentBitmask & button));
}

boolean EventHandler::justPressed(uint16_t button)
{
    return (!(lastBitmask & button) && (currentBitmask & button));
}

boolean EventHandler::isPrgPressed()
{
    return pressed(PRG_BUTTON_BIT);
}
boolean EventHandler::isRightPressed()
{
    return pressed(RIGHT_BUTTON_BIT);
}
boolean EventHandler::isLeftPressed()
{
    return pressed(LEFT_BUTTON_BIT);
}

boolean EventHandler::isPrgJustPressed()
{
    return justPressed(PRG_BUTTON_BIT);
}
boolean EventHandler::isRightJustPressed()
{
    return justPressed(RIGHT_BUTTON_BIT);
}
boolean EventHandler::isLeftJustPressed()
{
    return justPressed(LEFT_BUTTON_BIT);
}
