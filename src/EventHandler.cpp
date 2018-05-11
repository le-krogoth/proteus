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
    // store history to find out if user has a both button down event happening
    last4Bitmask = last3Bitmask;
    last3Bitmask = last2Bitmask;
    last2Bitmask = lastBitmask;
    lastBitmask = currentBitmask;

    // get current button state
    currentBitmask = ((~PIN_IN) & (bit(0) | bit(12) | bit(13)));

    // store button sequence for easter eggs
    if(isLeftAndRightJustPressed())
    {
        sKeyStream += "B";
    }
    else if(isLeftJustPressed())
    {
        sKeyStream += "L";
    }
    else if(isRightJustPressed())
    {
        sKeyStream += "R";
    }

    //hs->print("keystream after button: ");
    //hs->println(sKeyStream.c_str());

    // make sure that the easter egg keystream does not grow beyond 10
    if(sKeyStream.length() > LENGTH_OF_EASTEREGG)
    {
        sKeyStream = sKeyStream.substr(sKeyStream.length() - LENGTH_OF_EASTEREGG);
        //sKeyStream.erase(sKeyStream.begin(), sKeyStream.end() - ));

        //hs->print("keystream after cutting: ");
        //hs->println(sKeyStream.c_str());
    }
}

std::string EventHandler::getKeyStream()
{
    return sKeyStream;
}

// usually called when easter egg was triggered
void EventHandler::clearKeyStream()
{
    sKeyStream = "";
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
boolean EventHandler::isLeftAndRightPressed()
{
    return pressed(LEFT_BUTTON_BIT) && pressed(RIGHT_BUTTON_BIT);
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
boolean EventHandler::isLeftAndRightJustPressed()
{
    return justPressed(LEFT_BUTTON_BIT) && justPressed(RIGHT_BUTTON_BIT);
}
