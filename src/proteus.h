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

#include <Esp.h> // deep sleep and system info

void update();

DisplayManager* dm = NULL;
EventHandler* eh = NULL;
ModeManager* mm = NULL;
Config* c = NULL;

uint32 lastFreeHeap = 0;

#endif //PROTEUS_PROTEUS_H
