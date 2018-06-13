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
#ifndef config_h
#define config_h

#include <Arduino.h>
#include <FS.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <modes/ModeSelectMode.h>

#include "user_interface.h"

class Config
{
public:
    Config(HardwareSerial* const hws, bool readOnly = true);

    bool configExist();

    // returns false if config could not be loaded
    // if config does not exist, automatically writes default config to disc
    bool loadConfig();

    // TODO: clear question -> only write config down when this function is called. no auto store?
    bool storeConfig();

    std::string getConfigAsJSON();

    bool getShowBootLogo();

    std::string getNickname();
    void setNickname(std::string n);

    std::string getSoftAPSSID();
    void setSoftAPSSID(std::string ssid);
    std::string getSoftAPPSK();
    void setSoftAPPSK(std::string psk);

    std::string getUpdateSSID();
    void setUpdateSSID(std::string ssid);
    std::string getUpdatePSK();
    void setUpdatePSK(std::string psk);

    std::string getUpdateServer();

    uint8_t getCurrentMode();
    void setCurrentMode(uint8_t mode);

    const std::string configFile = "/config.json";

protected:

    static const uint8_t pwdLength = 10;


private:

    // writes default config, chooses sensible values for SoftAP SSID, PSK and such.
    std::string getMAC();

    void writeDefaultConfig();

    HardwareSerial* hs;
    bool bReadOnly = true;
    bool bShowBootLogo = true;

    std::string softAPSSID = "auto";
    std::string softAPPSK = "auto";
    std::string nickname = "31337 h4x0r";

    std::string updateSSID = "Proteus_OTA";
    std::string updatePSK = "932/&jhGF*)1";
    std::string updateServer = "http://192.168.41.41:8081/";

    uint8_t selectedMode = ModeSelectMode::M_MODE_DEFAULT;

    std::string generatePassword();
};

#endif
