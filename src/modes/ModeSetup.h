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
#ifndef mode_setup_h
#define mode_setup_h

#include <U8g2lib.h>

//#include "ESPAsyncTCP.h"
//#include "ESPAsyncWebServer.h"

#include <Arduino.h>
#include <ArduinoJson.h>

#include <FS.h>
#include <ESP8266WebServer.h>

#include "../EventHandler.h"
#include "BaseMode.h"

#include "global.h"
#include "../Config.h"


class ModeSetup : public BaseMode
{
public:
    ModeSetup(EventHandler* const e, Config* const c, U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C* const u8, HardwareSerial* const hws);
    ~ModeSetup();

    void handleEvents();
    //void paintFrame(U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C* const u8g2);
    void paintFrameInternal();
    void cleanup();

protected:

private:

    ESP8266WebServer* server = new ESP8266WebServer(80);
    Config* conf = NULL;

    String getMAC();

    String AP_SSID = "";
    String AP_PWD = "";

    // Configuration Handler
    void handleGetConfig();
    void handleSetConfig();

    // HW Info Handler
    void handleGetHWInfo();

    // File Handler
    void handleFileRequest();

    // Helpers
    void handleNotFound();
    bool sendFromSPIFFS(String path);
    void returnOK();

};

#endif // mode_setup_h
