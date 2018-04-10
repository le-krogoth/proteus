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
#include "ModeTimeTable.h"

ModeTimeTable::ModeTimeTable(EventHandler *const e, HardwareSerial *const hws) : BaseMode (e, hws)
{
    bool bLoaded = loadTimeTable();

    if(bLoaded)
    {
        // TODO change me...
        Talk* talk = sTalks.get(0);
        title = talk->title;
        room = talk->room;
        speaker = talk->speaker;
        date = talk->date;
        start = talk->start;
        end = talk->end;
    }
}

void ModeTimeTable::handleEvents()
{
    if(eh->isLeftJustPressed())
    {
        if(currentTalk == 0)
        {
            currentTalk = sTalks.size() - 1;
        }
        else
        {
            currentTalk--;
        }

        Talk* talk = sTalks.get(currentTalk);
        title = talk->title;
        room = talk->room;
        speaker = talk->speaker;
        date = talk->date;
        start = talk->start;
        end = talk->end;
    }

    if(eh->isRightJustPressed())
    {
        currentTalk++;
        if(currentTalk >= sTalks.size())
        {
            currentTalk = 0;
        }

        Talk* talk = sTalks.get(currentTalk);
        title = talk->title;
        room = talk->room;
        speaker = talk->speaker;
        date = talk->date;
        start = talk->start;
        end = talk->end;
    }
}

void ModeTimeTable::paintFrameInternal()
{
    u8g2->firstPage();
    do {

        u8g2->setFont(u8g2_font_6x10_tf);

        /*
        hs->print("Current talk: ");
        hs->print(currentTalk);
        hs->print(" Total count: ");
        hs->println(sTalks.size());
         */

        Talk* talk = sTalks.get(currentTalk);
        if(currentTalk > 0)
        {
            u8g2->drawUTF8(0, 8, "<");
        }

        u8g2->drawUTF8(15, 8, room.c_str());
        u8g2->drawUTF8(25, 8, date.c_str());
        u8g2->drawUTF8(50, 8, start.c_str());
        u8g2->drawUTF8(85, 8, end.c_str());

        if(currentTalk < sTalks.size() -1)
        {
            u8g2->drawUTF8(116, 8, ">");
        }

        u8g2->drawUTF8(0, 16, title.substring(0, 20).c_str());
        if(title.length() > 20)
        {
            // if title <= 20 chars, only show one line, otherwise
            u8g2->drawUTF8(0, 24, title.substring(20).c_str());
        }
        u8g2->drawUTF8(0, 32, speaker.c_str());

    } while ( u8g2->nextPage() );
}

bool ModeTimeTable::loadTimeTable()
{
    // open file for reading
    File f = SPIFFS.open("/ttbl.json", "r");
    if (!f)
    {
        //hs->println("file open failed in modetimetable");
        return false;
    }
    else
    {
        //hs->println("file opened");

        // Allocate the memory pool on the stack.
        DynamicJsonBuffer jsonBuffer(1500);
        JsonObject &root = jsonBuffer.parseObject(f);

        f.close();

        if (!root.success())
        {
            //hs->println(F("Failed to parse file"));
            return false;
        }
        else
        {
            //hs->println(F("File parsed"));

            JsonArray& talks =  root["talks"];
            for (auto& talk : talks) {

                //hs->println(F("before Talk t = {}"));

                Talk* t = new Talk();

                t->title = talk["title"];
                t->speaker = talk["speaker"];
                t->room = talk["room"];
                t->date = talk["date"];
                t->start = talk["start"];
                t->end = talk["end"];

                //hs->println(F("after, before adding to list"));
                //Serial.print(speaker);
                //Serial.print(": ");
                //Serial.println(String(title));
                sTalks.add(t);

                //hs->println(F("added to list"));
            }
        }
    }

    return true;
}

/*
    // open file for reading
    File f = SPIFFS.open("/ttbl.json", "r");
    if (!f)
    {
        Serial.println("file open failed");
    }
    else
    {
        Serial.println("====== Reading from SPIFFS file =======");

        // Allocate the memory pool on the stack.
        DynamicJsonBuffer jsonBuffer(4000);
        JsonObject& root = jsonBuffer.parseObject(f);

        f.close();

        if (!root.success())
        {
            Serial.println(F("Failed to read file"));
        }
        else
        {
            JsonArray& talks =  root["talks"];
            for (auto& talk : talks) {
                const char* title = talk["title"];
                const char* speaker = talk["speaker"];
                Serial.print(speaker);
                Serial.print(": ");
                Serial.println(String(title));
            }
        }
    }
    */

