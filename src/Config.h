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

    String getConfigAsJSON();

    bool getShowBootLogo();

    String getNickname();
    bool setNickname(String n);

    String getSoftAPSSID();
    void setSoftAPSSID(String ssid);
    String getSoftAPPSK();
    void setSoftAPPSK(String psk);

    String getUpdateSSID();
    void setUpdateSSID(String ssid);
    String getUpdatePSK();
    void setUpdatePSK(String psk);

    String getUpdateServer();

    uint8_t getCurrentMode();
    bool setCurrentMode(uint8_t mode);

    const String configFile = "/config.json";

protected:


private:

    // writes default config, chooses sensible values for SoftAP SSID, PSK and such.
    String getMAC();

    bool writeDefaultConfig();

    HardwareSerial* hs;
    bool bReadOnly = true;
    bool bShowBootLogo = true;

    String softAPSSID = "auto";
    String softAPPSK = "auto";
    String nickname = "31337 h4x0r";

    String updateSSID = "proteus";
    String updatePSK = "proteus.1234";
    String updateServer = "http://192.168.4.18/";

    uint8_t selectedMode = ModeSelectMode::M_MODE_DEFAULT;
};

#endif
