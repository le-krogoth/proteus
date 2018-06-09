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

ModeTimeTable::ModeTimeTable(EventHandler *const e, U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C* const u8, HardwareSerial *const hws) : BaseMode (e, u8, hws)
{
    sTalks = new SimpleList<Talk>;

    hs->println("Loading talks");

    u8g2->setFont(u8g2_font_6x10_tf);
    u8g2->firstPage();
    do {

        u8g2->drawUTF8(6, 13, "Patience you");
        u8g2->drawUTF8(6, 24, "must have");

    } while ( u8g2->nextPage() );

    bool bLoaded = loadTimeTable();

    hs->println("Talks loaded");

    if(bLoaded)
    {
        initCurrentTalk();
    }
}

void ModeTimeTable::cleanup()
{
    hs->println("cleaning up, giving back memory");
    sTalks->clear();

    // crash
    delete(sTalks);
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

    isDirty = true;
    doesScroll = false;

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
    if(!isDirty && !doesScroll)
    {
        //hs->println("no screen update");
        return;
    }

    hs->print("Talk no: ");
    Talk talk = sTalks->get(currentTalk);
    hs->println(currentTalk);

    u8g2->setFont(u8g2_font_6x10_tf);

    hs->print("Current talk: ");
    hs->print(talk.title.c_str());
    hs->print(talk.title.length());
    hs->print(", ");
    hs->println(talk.speaker.c_str());

    if(titleWidth == 0)
    {
        titleWidth = talk.title.length() * 6;
    }

    if(speakerWidth == 0)
    {
        speakerWidth = talk.speaker.length() * 6;
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
        u8g2->drawUTF8(45, 8, talk.room.c_str());
        u8g2->drawUTF8(65, 8, talk.date.c_str());

        if(currentTalk < sTalks->size() -1)
        {
            u8g2->drawUTF8(116, 8, ">");
        }

        u8g2->drawUTF8(6, 16, talk.start.c_str());
        u8g2->drawUTF8(46, 16, "-");
        u8g2->drawUTF8(53, 16, talk.end.c_str());

        u8g2->drawUTF8(titleOffset, 24, talk.title.c_str());

        u8g2->drawUTF8(speakerOffset, 32, talk.speaker.c_str());

    } while ( u8g2->nextPage() );

    // do only scroll if wider than display
    if(titleWidth > 128)
    {
        doesScroll = true;

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
        doesScroll = true;

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

    isDirty = false;
}

bool ModeTimeTable::loadTimeTable()
{
    // open file for reading
    File f = SPIFFS.open("/www/ttbl.json", "r");
    if (!f)
    {
        hs->println("file open failed in modetimetable");
        return false;
    }
    else
    {
        //hs->println("file opened");

        // Allocate the memory pool on the stack.
        DynamicJsonBuffer jsonBuffer(3000);
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

                //hs->println(talk["title"].as<String>());

                t->title = talk["title"].asString();
                t->speaker = talk["speaker"].asString();
                t->room = talk["room"].asString();
                t->date = talk["date"].asString();
                t->start = talk["start"].asString();
                t->end = talk["end"].asString();

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


