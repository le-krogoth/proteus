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
        initCurrentTalk();
    }
}

void ModeTimeTable::initCurrentTalk()
{
    hs->print("Init current talk: ");
    hs->println(currentTalk);

    titleOffset = 0;
    speakerOffset = 0;

    titleScrollLeft = true;
    speakerScrollLeft = true;

    titleWidth = 0;
    speakerWidth = 0;

    hs->println("End of initcurrenttalk");
}

void ModeTimeTable::handleEvents()
{
    if(eh->isLeftJustPressed())
    {
        if(currentTalk == 0)
        {
            currentTalk = sTalks->size() - 1;
        }
        else
        {
            currentTalk--;
        }

        initCurrentTalk();
    }

    if(eh->isRightJustPressed())
    {
        currentTalk++;
        if(currentTalk >= sTalks->size())
        {
            currentTalk = 0;
        }

        initCurrentTalk();
    }
}

void ModeTimeTable::paintFrameInternal()
{
    Talk talk = sTalks->get(currentTalk);

    u8g2->setFont(u8g2_font_6x10_tf);

    // lazy load since u8g2 is only set after the first round...
    if(titleWidth == 0)
    {
        //titleWidth = talk.title.length() * 10;
        titleWidth = u8g2->getUTF8Width(talk.title.c_str());
    }

    // lazy load since u8g2 is only set after the first round...
    if(speakerWidth == 0)
    {
        //speakerWidth = talk.speaker.length() * 10;
        speakerWidth = u8g2->getUTF8Width(talk.speaker.c_str());
    }

    // now do the drawing...
    u8g2->firstPage();
    do {
        /*
        hs->print("Current talk: ");
        hs->print(currentTalk);
        hs->print(" Total count: ");
        hs->println(sTalks.size());
         */

        if(currentTalk > 0)
        {
            u8g2->drawUTF8(0, 8, "<");
        }

        u8g2->drawUTF8(10, 8, "Room: ");
        u8g2->drawUTF8(30, 8, talk.room.c_str());
        u8g2->drawUTF8(65, 8, talk.date.c_str());

        if(currentTalk < sTalks->size() -1)
        {
            u8g2->drawUTF8(116, 8, ">");
        }

        u8g2->drawUTF8(20, 16, talk.start.c_str());
        u8g2->drawUTF8(50, 16, "-");
        u8g2->drawUTF8(85, 16, talk.end.c_str());

        u8g2->drawUTF8(titleOffset, 24, talk.title.c_str());

        u8g2->drawUTF8(speakerOffset, 32, talk.speaker.c_str());

    } while ( u8g2->nextPage() );

    // do only scroll if wider than display
    if(titleWidth > 128)
    {
        if (titleScrollLeft)
        {
            titleOffset -= 2;
        }
        else
        {
            titleOffset += 2;
        }

        if (titleWidth - (titleOffset * -1) <= 126)
        {
            titleScrollLeft = false;
        }

        if (titleOffset >= 2)
        {
            titleScrollLeft = true;
        }
    }

    if(speakerWidth > 128)
    {
        if(speakerScrollLeft)
        {
            speakerOffset -= 2;
        }
        else
        {
            speakerOffset += 2;
        }

        if(speakerWidth - (speakerOffset * -1) <= 126)
        {
            speakerScrollLeft = false;
        }

        if(speakerOffset >= 2)
        {
            speakerScrollLeft = true;
        }
    }

    /*
    hs->print("titleWidth: ");
    hs->print(titleWidth);
    hs->print(" titleOffset: ");
    hs->print(titleOffset);
    hs->print(" titleScrollLeft: ");
    hs->print(titleScrollLeft);
    hs->print(" titleWidth - (titleOffset * -1) <= 128: ");
    hs->println(titleWidth - (titleOffset * -1) <= 128);
     */
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

                t->title = talk["title"].as<String>();
                t->speaker = talk["speaker"].as<String>();
                t->room = talk["room"].as<String>();
                t->date = talk["date"].as<String>();
                t->start = talk["start"].as<String>();
                t->end = talk["end"].as<String>();

                //hs->println(F("after, before adding to list"));
                //Serial.print(speaker);
                //Serial.print(": ");
                //Serial.println(String(title));
                sTalks->add(*t);

                //hs->println(F("added to list"));
            }
        }
    }

    return true;
}


