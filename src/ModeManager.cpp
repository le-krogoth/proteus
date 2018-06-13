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
#include "ModeManager.h"

ModeManager::ModeManager(EventHandler* const e, Config* const c, U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C* const u8, HardwareSerial* const hws)
{
    eh = e;
    hs = hws;
    conf = c;
    u8g2 = u8;

    // check if user presses both buttons during boot
    // if so, start in ModeSelectMode::M_MODE_DEFAULT
    // if not, init the mode to the last one from the config
    // TODO: think about resetting the config as well
    if(eh->isLeftAndRightPressed())
    {
        hs->println("reset into mode default");
        setMode(ModeSelectMode::M_MODE_DEFAULT, ModeSelectMode::M_MODE_DEFAULT, false);
    }
    else if(eh->isPrgPressed())
    {
        hs->println("you found the hidden secret. or so.");
        setMode(ModeSelectMode::M_ABOUT, ModeSelectMode::M_MODE_DEFAULT, false);
    }
    else
    {
        hs->println("booting into last selected mode");
        setMode(c->getCurrentMode(), ModeSelectMode::M_MODE_DEFAULT, false);
    }
}

bool ModeManager::moduleWantsEnforcedFramerate()
{
    BaseMode* m = getCurrentModeObject();

    if(m != NULL)
    {
        return m->getEnforceFramerate();
    }
    else
    {
        return true;
    }
}

void ModeManager::checkEvents()
{
    //hs->println("checkEvents");

    // only allow cheat codes in specific modes
    if(currentMode == ModeSelectMode::M_LOGO || currentMode == ModeSelectMode::M_NICKNAME) {

        // check for easter eggs first
        std::string sKeyStream = eh->getKeyStream();
        if (sKeyStream == "LRLRLLRR") {
            // make sure to not just trigger again
            eh->clearKeyStream();

            uint8_t oldMode = currentMode;
            currentMode = ModeSelectMode::M_HELIX;
            setMode(currentMode, oldMode);
            return;
        } else if (sKeyStream == "RLRLRRLL") {
            // make sure to not just trigger again
            eh->clearKeyStream();

            uint8_t oldMode = currentMode;
            currentMode = ModeSelectMode::M_HELIX;
            setMode(currentMode, oldMode);
            return;
        } else if (sKeyStream == "RLRRLRLL") {
            // make sure to not just trigger again
            eh->clearKeyStream();

            uint8_t oldMode = currentMode;
            currentMode = ModeSelectMode::M_PLAY_TRACK;
            setMode(currentMode, oldMode);
            return;
        } else if (sKeyStream == "LRLLRLLR") {
            // make sure to not just trigger again
            eh->clearKeyStream();

            uint8_t oldMode = currentMode;
            currentMode = ModeSelectMode::M_TELL_TALE;
            setMode(currentMode, oldMode);
            return;
        } else if (sKeyStream == "LLLLLLLR") {
            // make sure to not just trigger again
            eh->clearKeyStream();

            uint8_t oldMode = currentMode;
            currentMode = ModeSelectMode::M_OTA;
            setMode(currentMode, oldMode);
            return;
        }
    }

    if(eh->isPrgJustPressed())
    {
        // check if just left MODE_SELECTMODE
        // if so, set new mode to selected mode
        // if not change to select mode

        if(currentMode == ModeSelectMode::M_SELECT_MODE)
        {
            ModeSelectMode* m = (ModeSelectMode*)getCurrentModeObject();

            hs->print("Selected Mode is: ");
            hs->println(m->getSelectedMode());

            currentMode = m->getSelectedMode();
            setMode(currentMode, 0);
        }
        else if(currentMode == ModeSelectMode::M_TIMETABLE || currentMode == ModeSelectMode::M_WIFISCANNER)
        {
            // there is a memory leak somewhere in the linked list
            // time ran out and the leak was still there
            // so we added this hack to not have any show stoppers.
            conf->setCurrentMode(ModeSelectMode::M_SELECT_MODE);
            conf->storeConfig();

            // reset the environment
            ESP.reset();
            delay(500);
        }
        else
        {
            uint8_t oldMode = currentMode;
            currentMode = 0;
            setMode(currentMode, oldMode);
        }
    }
}

uint8_t ModeManager::getCurrentMode()
{
    return currentMode;
}

BaseMode* ModeManager::getCurrentModeObject()
{
    return currentModeObject;
}

void ModeManager::setMode(uint8_t newMode, uint8_t oldMode, bool storeMode)
{
    // mode changes, reset easter egg buffer
    eh->clearKeyStream();

    // remove the check for >= since easter egg modes are between 20 and XX
    // if(newMode < 0 || newMode >= ModeSelectMode::M_MODE_COUNT)
    if(newMode < 0)
    {
        currentMode = ModeSelectMode::M_MODE_DEFAULT;
    }
    else
    {
        currentMode = newMode;
    }

    if(currentModeObject)
    {
        //hs->println("Freeing old object");

        currentModeObject->cleanup();

        delete currentModeObject;
    }

    hs->print("New mode is: ");
    hs->println(currentMode);

    // make sure that the config file contains the current mode
    // stores only standard modes, no easter eggs...
    if(storeMode && currentMode < ModeSelectMode::M_MODE_COUNT)
    {
        conf->setCurrentMode(currentMode);
        conf->storeConfig();
    }

    switch(currentMode)
    {
        case ModeSelectMode::M_SELECT_MODE:
            currentModeObject = new ModeSelectMode(oldMode, eh, u8g2, hs);
            break;
        case ModeSelectMode::M_SETUP_MODE:
            currentModeObject = new ModeSetup(eh, conf, u8g2, hs);
            break;
        case ModeSelectMode::M_KNIGHT_RIDER:
            currentModeObject = new ModeKnightRider(eh, u8g2, hs);
            break;
        case ModeSelectMode::M_NICKNAME:
            currentModeObject = new ModeNickname(eh, conf, u8g2, hs);
            break;
        case ModeSelectMode::M_LOGO:
            currentModeObject = new ModeLogo(eh, u8g2, hs);
            break;
        case ModeSelectMode::M_TIMETABLE:
            currentModeObject = new ModeTimeTable(eh, u8g2, hs);
            break;
        case ModeSelectMode::M_EYES:
            currentModeObject = new ModeEyes(eh, u8g2, hs);
            break;
        case ModeSelectMode::M_ROBOTEYES:
            currentModeObject = new ModeRobotEyes(eh, u8g2, hs);
            break;
        case ModeSelectMode::M_LOADING:
            currentModeObject = new ModeLoading(eh, u8g2, hs);
            break;
        case ModeSelectMode::M_UNICORN_GAME:
            currentModeObject = new ModeUnicorn(eh, u8g2, hs);
            break;
        case ModeSelectMode::M_AFTER_DARK:
            currentModeObject = new ModeAfterDark(eh, u8g2, hs);
            break;
        case ModeSelectMode::M_WIFISCANNER:
            currentModeObject = new ModeWifiScanner(eh, u8g2, hs);
            break;
        case ModeSelectMode::M_SNAKE:
            currentModeObject = new ModeSnake(eh, u8g2, hs);
            break;
        case ModeSelectMode::M_HELIX:
            currentModeObject = new BaseMode(eh, u8g2, hs);
            break;
        case ModeSelectMode::M_PLAY_TRACK:
            currentModeObject = new ModePlayTrack(eh, u8g2, hs);
            break;
        case ModeSelectMode::M_TELL_TALE:
            currentModeObject = new ModeTellTale(eh, u8g2, hs);
            break;
        case ModeSelectMode::M_OTA:
            currentModeObject = new ModeOTA(eh, conf, u8g2, hs);
            break;
        case ModeSelectMode::M_ABOUT:
            currentModeObject = new ModeAbout(eh, u8g2, hs);
            break;
        default:
            currentModeObject = new BaseMode(eh, u8g2, hs);
            break;
    }

    //hs->println("Object instantiated");
}
