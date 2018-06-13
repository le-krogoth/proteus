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
#include "ModeSetup.h"


ModeSetup::ModeSetup(EventHandler *const e, Config* const c, U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C* const u8, HardwareSerial *const hws) : BaseMode (e, u8, hws)
{
    conf = c;

    u8g2->setFont(u8g2_font_6x10_tf);
    u8g2->firstPage();
    do {

        // thanks, A.A. Milne
        u8g2->drawUTF8(6, 13, "Rivers know this:");
        u8g2->drawUTF8(6, 24, "there is no hurry.");

    } while ( u8g2->nextPage() );

    WiFi.disconnect(true);
    WiFi.softAPdisconnect(true);

    bool bStarted = WiFi.softAP(conf->getSoftAPSSID().c_str(), conf->getSoftAPPSK().c_str());
    //bool bStarted = WiFi.softAP(conf->getSoftAPSSID().c_str(), "");
    WiFi.setSleepMode(WIFI_NONE_SLEEP);

    delay(500);

    hs->print("SoftAP started: ");
    hs->println(bStarted);

    server->on("/api/config", HTTP_GET, std::bind(&ModeSetup::handleGetConfig, this));
    server->on("/api/config", HTTP_POST, std::bind(&ModeSetup::handleSetConfig, this));

    server->on("/api/hwinfo", HTTP_GET, std::bind(&ModeSetup::handleGetHWInfo, this));

    // files
    // TODO: favicon
    // server->on("/favicon.ico
    server->on("/index.html", HTTP_GET, std::bind(&ModeSetup::sendFromSPIFFS, this, "/www/index.html"));
    server->on("/siimple.css", HTTP_GET, std::bind(&ModeSetup::sendFromSPIFFS, this, "/www/siimple.css"));
    server->on("/siimple-colors.css", HTTP_GET, std::bind(&ModeSetup::sendFromSPIFFS, this, "/www/siimple-colors.css"));
    server->on("/zepto.min.js", HTTP_GET, std::bind(&ModeSetup::sendFromSPIFFS, this, "/www/zepto.min.js"));
    server->on("/mntsrt.css", HTTP_GET, std::bind(&ModeSetup::sendFromSPIFFS, this, "/www/mntsrt.css"));
    server->on("/mntsrt-b.woff2", HTTP_GET, std::bind(&ModeSetup::sendFromSPIFFS, this, "/www/mntsrt-b.woff2"));
    server->on("/mntsrt-r.woff2", HTTP_GET, std::bind(&ModeSetup::sendFromSPIFFS, this, "/www/mntsrt-r.woff2"));

    server->on("/ttbl.json", HTTP_GET, std::bind(&ModeSetup::sendFromSPIFFS, this, "/www/ttbl.json"));

    //server->serveStatic("/", SPIFFS, "/www/");

    server->on("/heap", [&]()
    {
        server->sendHeader("Cache-Control", "no-cache");
        server->send(200, "text/plain", String(ESP.getFreeHeap()));
    });

    server->onNotFound(std::bind(&ModeSetup::handleNotFound, this));

    server->begin();
}

void ModeSetup::cleanup()
{
    hs->println("disconnecting Wifi Soft AP and shutting down Server");
    WiFi.softAPdisconnect(true);
    server->stop();
    delete(server);
}

void ModeSetup::handleEvents()
{
    //hs->println();
    server->handleClient();
}

void ModeSetup::paintFrameInternal()
{
    u8g2->setFont(u8g2_font_6x10_tf);

    u8g2->firstPage();
    do {

        //u8g2->drawUTF8(0,10,"SSID: ");
        u8g2->drawUTF8(0,10, conf->getSoftAPSSID().c_str());
        //8g2->drawUTF8(0,20,"PSK: ");
        u8g2->drawUTF8(0,20, conf->getSoftAPPSK().c_str());
        u8g2->drawUTF8(96,20, "#");
        u8g2->drawUTF8(102,20, String(WiFi.softAPgetStationNum()).c_str());
        //u8g2->drawUTF8(0,30,"URL: ");
        u8g2->drawUTF8(0,30, "http://");
        u8g2->drawUTF8(42,30, WiFi.softAPIP().toString().c_str());

    } while ( u8g2->nextPage() );
}

void ModeSetup::handleNotFound()
{
    hs->println("handle not found");

    String url = server->uri();

    hs->println(url);

    if(url.length() == 0 || url == "/")
    {
        url = "/www/index.html";
    }

    hs->println(url);

    if (sendFromSPIFFS(url)) {
        return;
    }

    server->sendHeader("Cache-Control", "no-cache");
    server->send(200, "text/plain", "file not found");
}

void ModeSetup::handleGetConfig()
{
    String json = conf->getConfigAsJSON();

    server->sendHeader("Cache-Control", "no-cache");
    server->send(200, "application/json; charset=utf-8", json);
}

void ModeSetup::handleSetConfig()
{
    hs->println(server->arg("plain"));

    DynamicJsonBuffer jsonBuffer(1000);
    JsonObject& root = jsonBuffer.parseObject(server->arg("plain"));

    if(!root.success())
    {
        hs->println("Parsing of json on SetConfig failed");

        server->sendHeader("Cache-Control", "no-cache");
        server->send(500, "application/json; charset=utf-8", "{success:false}");

        return;
    }

    String nickname = root["nickname"].as<String>();
    String softAPSSID = root["softAPSSID"].as<String>();
    String softAPPSK = root["softAPPSK"].as<String>();

    if(nickname)
    {
        conf->setNickname(nickname);
    }

    if(softAPSSID)
    {
        conf->setSoftAPSSID(softAPSSID);
    }

    if(softAPPSK && softAPPSK.length() >= 8)
    {
        conf->setSoftAPPSK(softAPPSK);
    }

    // TODO check that the correct content is written down
    conf->storeConfig();

    //hs->println(nickname);

    server->sendHeader("Cache-Control", "no-cache");
    server->send(200, "application/json; charset=utf-8", "{success:true}");
}

void ModeSetup::handleGetHWInfo() {

    String json = "{\n\"hwinfo\": {\n";
    json += "\"reset_reason\": \"" + ESP.getResetReason() + "\",\n";
    json += "\"free_heap\": \"" + String(ESP.getFreeHeap()) + "\",\n";
    json += "\"chip_id\": \"" + String(ESP.getChipId()) + "\",\n";
    json += "\"core_version\": \"" + String(ESP.getCoreVersion()) + "\",\n";
    json += "\"sdk_version\": \"" + String(ESP.getSdkVersion()) + "\",\n";
    json += "\"cpu_freq_mhz\": \"" + String(ESP.getCpuFreqMHz()) + "\",\n";
    json += "\"sketch_size\": \"" + String(ESP.getSketchSize()) + "\",\n";
    json += "\"free_sketch_space\": \"" + String(ESP.getFreeSketchSpace()) + "\",\n";
    json += "\"sketch_md5\": \"" + String(ESP.getSketchMD5()) + "\",\n";
    json += "\"flash_chip_id\": \"" + String(ESP.getFlashChipId()) + "\",\n";
    json += "\"flash_chip_size\": \"" + String(ESP.getFlashChipSize()) + "\",\n";
    json += "\"flash_chip_real_size\": \"" + String(ESP.getFlashChipRealSize()) + "\",\n";
    json += "\"flash_chip_speed\": \"" + String(ESP.getFlashChipSpeed()) + "\",\n";
    json += "\"cycle_count\": \"" + String(ESP.getCycleCount()) + "\"\n";

    json += "}\n}";

    //hs->println(json);

    server->sendHeader("Cache-Control", "no-cache");
    server->send(200, "application/json; charset=utf-8", json);
}

void ModeSetup::handleFileRequest()
{

}


bool ModeSetup::sendFromSPIFFS(String path)
{
    hs->print("file requested: ");
    hs->println(path);

    String dataType = "text/plain";
    if (path.endsWith("/")) {
        path += "index.html";
    }

    if (path.endsWith(".src")) {
        path = path.substring(0, path.lastIndexOf("."));
    } else if (path.endsWith(".html")) {
        dataType = "text/html";
    } else if (path.endsWith(".css")) {
        dataType = "text/css";
    } else if (path.endsWith(".js")) {
        dataType = "application/javascript";
    } else if (path.endsWith(".png")) {
        dataType = "image/png";
    } else if (path.endsWith(".gif")) {
        dataType = "image/gif";
    } else if (path.endsWith(".jpg")) {
        dataType = "image/jpeg";
    } else if (path.endsWith(".ico")) {
        dataType = "image/x-icon";
    } else if (path.endsWith(".xml")) {
        dataType = "text/xml";
    } else if (path.endsWith(".pdf")) {
        dataType = "application/pdf";
    } else if (path.endsWith(".zip")) {
        dataType = "application/zip";
    }

    if(!SPIFFS.exists(path))
    {
        hs->print("File does not exist in SPIFFS: ");
        hs->println(path);
        return false;
    }

    File dataFile = SPIFFS.open(path.c_str(), "r");
    if (!dataFile) {
        hs->print("Something is wrong with file in SPIFFS: ");
        hs->println(path);
        return false;
    }

    //if (dataFile.isDirectory()) {
    //    return false;
    //}

    if (server->hasArg("download")) {
        dataType = "application/octet-stream";
    }

    if (server->streamFile(dataFile, dataType) != dataFile.size()) {
        Serial.println("Sent less data than expected. Probably stopped by client.");
    }

    dataFile.close();

    hs->print("file sent: ");
    hs->println(path);

    return true;
}

void ModeSetup::returnOK()
{
    server->sendHeader("Cache-Control", "no-cache");
    server->send(200, "text/plain", "ok");
}