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
#include "ModeWifiScanner.h"

ModeWifiScanner::ModeWifiScanner(EventHandler *const e, U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C* const u8, HardwareSerial *const hws) : BaseMode (e, u8, hws)
{
    u8g2->setFont(u8g2_font_6x10_tf);
    u8g2->firstPage();
    do {

        u8g2->drawUTF8(6, 13, "Loading.");
        u8g2->drawUTF8(6, 24, "Patience, Padawan.");

    } while ( u8g2->nextPage() );

    bool bLoaded = loadAPs();

    if(bLoaded)
    {
        initCurrentAP();
    }
}

void ModeWifiScanner::cleanup()
{
    hs->println("cleaning up, giving back memory");
    vAP.clear();
}

bool ModeWifiScanner::loadAPs()
{
    int n = WiFi.scanNetworks();

    for (int i = 0; i < n; ++i)
    {
        AP ap = AP();

        ap.ssid = WiFi.SSID(i).c_str();
        ap.rssi = WiFi.RSSI(i);
        ap.encryptionType = WiFi.encryptionType(i);
        ap.channel = WiFi.channel(i);

        vAP.push_back(ap);
    }

    WiFi.disconnect();

    return true;
}

void ModeWifiScanner::initCurrentAP()
{

}

void ModeWifiScanner::handleEvents()
{
    if(eh->isLeftJustPressed())
    {
        if(selectedAP > 0)
        {
            selectedAP--;
        }
        else
        {
            selectedAP = 0;
        }

        if(selectedAP - viewPos < 2 && viewPos > 0)
        {
            viewPos--;
        }
    }

    if(eh->isRightJustPressed())
    {
        if(selectedAP < vAP.size() - 1)
        {
            selectedAP++;
        }

        if(selectedAP - viewPos > 1 && viewPos < (vAP.size() - 4))
        {
            viewPos++;
        }

    }
}

void ModeWifiScanner::paintFrameInternal()
{
    u8g2->setFont(u8g2_font_6x10_tf);

    u8g2->firstPage();
    do {

        for(int i = viewPos; i < viewPos + 4; i++)
        {
            AP ap = vAP.at(i);

            u8g2->drawUTF8(6, ((i - viewPos) * 8) + 8, ap.ssid.substr(0, 15).c_str());
            u8g2->drawUTF8(110, ((i - viewPos) * 8) + 8, getEncStr(ap.encryptionType).c_str());

            if(i == selectedAP)
            {
                u8g2->drawUTF8(0, ((i - viewPos) * 8) + 8, ">");
            }
        }

    } while ( u8g2->nextPage() );
}

String ModeWifiScanner::getEncStr(uint8_t encryptionCode)
{
    switch (encryptionCode)
    {
        case ENC_TYPE_NONE:
            return String("  ");
        case ENC_TYPE_WEP:
            return String("WEP");
        case ENC_TYPE_TKIP:
            return String("WPA");
        case ENC_TYPE_CCMP:
            return String("WP2");
        case ENC_TYPE_AUTO:
            return String("AUT");
        default:
            return String("  ?");
    }
}
//EOF