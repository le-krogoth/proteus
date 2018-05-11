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

#ifndef PROTEUS_PROTEUS_H
#define PROTEUS_PROTEUS_H

#include <Arduino.h>
#include "global.h"

#include <U8g2lib.h>
#include "DisplayManager.h"
#include "EventHandler.h"
#include "ModeManager.h"
#include "Config.h"

#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <Esp.h> // deep sleep and system info

// change these to your setup
//static const char* SSID  = "proteus";
//static const char* PWD   = "abcd.1234";

//static const char* SERVER_URL = "http://192.168.1.15:80/";

void update();

DisplayManager* dm = NULL;
EventHandler* eh = NULL;
ModeManager* mm = NULL;
Config* c = NULL;

#endif //PROTEUS_PROTEUS_H
