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
            if(softAPSSID == "auto")
            {
                softAPSSID = ("area41_" + getMAC()).c_str();;
            }
            softAPPSK = root["softAPPSK"].as<String>();
            if(softAPPSK == "auto")
            {
                hs->println("generating password");
                // generate a somewhat strong password
                softAPPSK = generatePassword();
            }

            nickname = root["nickname"].as<String>();
            selectedMode = root["currentMode"];
            bShowBootLogo = root["showBootLogo"];
            updateSSID = root["updateSSID"].as<String>();
            updatePSK = root["updatePSK"].as<String>();
            updateServer = root["updateServer"].as<String>();

            /*
            hs->print(F("Show boot logo: "));
            hs->println(bShowBootLogo);
             */
        }

        f.close();
    }

    return true;
}

String Config::generatePassword()
{
    std::string keyspace = "0aAbB1cCdD1eEf2F2gG3hHiIjJk3K45lLmM9n5N6o4OpPqQ76rRsStT78uUvVwW98xXyY0zZ";
    std::string pwd = "";

    for(int i=0; i < 12; i++)
    {
        pwd += keyspace.at(random(0,72));
    }

    hs->print("New password: ");
    hs->println(pwd.c_str());
    return String(pwd.c_str());
}

String Config::getConfigAsJSON()
{
    String json = "";

    // TODO, clean it up
    json += "{";
    json += "\"nickname\": \"" + nickname + "\",";
    json += "\"softAPSSID\": \"" + softAPSSID+ "\",";
    json += "\"softAPPSK\": \"" + softAPPSK + "\",";
    json += "\"currentMode\": " + String(selectedMode) + ",";
    json += "\"updateSSID\": \"" + updateSSID+ "\",";
    json += "\"updatePSK\": \"" + updatePSK + "\",";
    json += "\"updateServer\": \"" + updateServer + "\",";
//    json += "\"showBootLogo\": \"" + bShowBootLogo?"true":"false" + "\"";
    json += "\"showBootLogo\": false";
    json += "}";

    return json;
}


// TODO: answer question -> only write config down when this function is called. no auto store?
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

void Config::setNickname(String n)
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

String Config::getUpdateSSID()
{
    return updateSSID;
}

void Config::setUpdateSSID(String ssid)
{
    updateSSID = ssid;
}

String Config::getUpdatePSK()
{
    return updatePSK;
}

void Config::setUpdatePSK(String psk)
{
    updatePSK = psk;
}

String Config::getUpdateServer()
{
    return updateServer;
}

uint8_t Config::getCurrentMode()
{
    return selectedMode;
}

void Config::setCurrentMode(uint8_t mode)
{
    selectedMode = mode;
}

void Config::writeDefaultConfig()
{
    storeConfig();
}

String Config::getMAC()
{
    uint8_t mac[6];

    WiFi.macAddress(mac);

    char result[14];

    snprintf( result, sizeof( result ), "%02x%02x%02x%02x%02x%02x", mac[ 0 ], mac[ 1 ], mac[ 2 ], mac[ 3 ], mac[ 4
    ], mac[ 5 ] );

    //hs->print("MAC: ");
    //hs->println(result);

    return String( result );
}

