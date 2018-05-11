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
#include "Config.h"

Config::Config(HardwareSerial* const hws, bool readOnly)
{
    hs = hws;
    bReadOnly = readOnly;
}

bool Config::configExist()
{
    return SPIFFS.exists(configFile);
}

bool Config::loadConfig()
{
    File f = SPIFFS.open(configFile, "r");
    if (!f)
    {
        hs->println("file open failed in config");
        return false;
    }
    else
    {
        // hs->println("config file opened, content: ");

        // String configuration = f.readString();

        // hs->println(configuration);

        // Allocate the memory pool on the stack.
        DynamicJsonBuffer jsonBuffer(1000);
        JsonObject &root = jsonBuffer.parseObject(f);

        if (!root.success())
        {
            hs->println(F("Failed to parse config file"));

            f.close();
            return false;
        }
        else
        {
            hs->println(F("Config file parsed, reading it now"));

            softAPSSID = root["softAPSSID"].as<String>();
            softAPPSK = root["softAPPSK"].as<String>();
            nickname = root["nickname"].as<String>();
            selectedMode = root["currentMode"];
            bShowBootLogo = root["showBootLogo"];
            updateServer = root["updateServer"].as<String>();

            hs->print(F("Show boot logo: "));
            hs->println(bShowBootLogo);

        }

        f.close();
    }

    return true;
}

String Config::getConfigAsJSON()
{
    String json = "";

    // "nickname": "31337 h4x0r",
    //  "softAPSSID": "auto",
    //  "softAPPSK": "auto",
    //  "currentMode": 1,
    //  "updateServer": "http://192.168.4.18/",
    //  "showBootLogo": true

    // TODO, clean it up
    json += "{";
    json += "\"nickname\": \"" + nickname + "\",";
    json += "\"SoftAPSSID\": \"" + softAPSSID+ "\",";
    json += "\"SoftAPPSK\": \"" + softAPPSK + "\",";
    json += "\"currentMode\": " + String(selectedMode) + ",";
    json += "\"updateServer\": \"" + updateServer + "\",";
//    json += "\"showBootLogo\": \"" + bShowBootLogo?"true":"false" + "\"";
    json += "\"showBootLogo\": false";
    json += "}";

    return json;
}


// TODO: clear question -> only write config down when this function is called. no auto store?
bool Config::storeConfig()
{
    if(bReadOnly)
    {
        return false;
    }

    File f = SPIFFS.open(configFile, "w");

    if (!f)
    {
        hs->println("Opening config file failed. You might want to re-init your badge.");
        return false;
    }
    else
    {
        String configuration = getConfigAsJSON();

        // hs->println("Writing config file: ");
        // hs->println(configuration);

        f.print(configuration);
        f.close();  //Close file
        f.flush();

        return true;
    }
}

bool Config::getShowBootLogo()
{
    return bShowBootLogo;
}

String Config::getNickname()
{
    return nickname;
}

bool Config::setNickname(String n)
{
    nickname = n;
}

String Config::getSoftAPSSID()
{
    return softAPSSID;
}

void Config::setSoftAPSSID(String ssid)
{
    softAPSSID = ssid;
}

String Config::getSoftAPPSK()
{
    return softAPPSK;
}

void Config::setSoftAPPSK(String psk)
{
    softAPPSK = psk;
}

String Config::getUpdateServer()
{
    return updateServer;
}

uint8_t Config::getCurrentMode()
{
    return selectedMode;
}

bool Config::setCurrentMode(uint8_t mode)
{
    selectedMode = mode;
}

// TODO
// writes default config, chooses sensible values for SoftAP SSID, PSK and such.
bool Config::writeDefaultConfig()
{
    storeConfig();
}

