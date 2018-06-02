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
#include "ModeSnake.h"

ModeSnake::ModeSnake(EventHandler *const e, U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C* const u8, HardwareSerial *const hws) : BaseMode (e, u8, hws)
{
    resetSnake();
    resetApple();
    currentSpeed = slowDelay;
    addUnit();
}

void ModeSnake::handleEvents()
{
    if(!gameover)
    {
        updateHead();
        checkCollision();
    }
    else
    {
        // any button restarts the game when the game is over
        if(eh->isRightJustPressed() || eh->isLeftJustPressed())
        {
            resetSnake();
            resetApple();
            currentSpeed = slowDelay;
            addUnit();
            gameover = false;
        }
    }
}

void ModeSnake::paintFrameInternal()
{
    u8g2->firstPage();
    do {
        // check if not gameover. if gameover show "game over" until next click
        if(gameover)
        {
            u8g2->setFont(u8g2_font_logisoso16_tf);
            u8g2->drawUTF8(20,20,"GAME OVER");
        }
        else
        {
            // score
            u8g2->setFont(u8g2_font_6x10_tf);
            u8g2->drawUTF8(50,10, "Score:");
            u8g2->drawUTF8(90,10, String(score).c_str());

            // snake head
            fillRect(x, y, UNIT_SIZE, UNIT_SIZE, 2);

            // snake body
            if (unitCount > 0)
            {
                for (byte i = 0; i < unitCount - 1; i++)
                {
                    fillRect(body[i].x, body[i].y, UNIT_SIZE, UNIT_SIZE, 2);
                }
            }

            // apple
            fillRect(apple.x, apple.y, UNIT_SIZE, UNIT_SIZE, 2);
            drawPixel(apple.x + 1, apple.y - 1, 2);
            drawPixel(apple.x, apple.y - 2, 2);
        }

    } while ( u8g2->nextPage() );
}


void ModeSnake::updateUnits()
{
    int oldPosX, oldPosY, newPosX, newPosY;

    newPosX = x;
    newPosY = y;

    if (unitCount > 0)
    {
        for (byte i = 0; i < MAX_UNITS; i++)
        {
            oldPosX = body[i].x;
            oldPosY = body[i].y;

            body[i].x = newPosX;
            body[i].y = newPosY;
            newPosX = oldPosX;
            newPosY = oldPosY;
        }
    }
}

void ModeSnake::checkCollision()
{
    if (x < 0 | x > SCREEN_WIDTH - UNIT_SIZE | y > SCREEN_HEIGHT - UNIT_SIZE | y < 0)
    {
        hs->println("Left playfield");

        delay(500);
        gameover = true;
    }

    if (unitCount > 0)
    {
        for (byte i = 0; i < unitCount - 1; i++)
        {

            if (collidePointRect(x, y, body[i].x, body[i].y, UNIT_SIZE, UNIT_SIZE))
            {
                hs->println("Collision");

                delay(500);
                gameover = true;

            }

        }
    }

    if (collideRectRect(apple.x, apple.y, UNIT_SIZE, UNIT_SIZE, x, y, UNIT_SIZE, UNIT_SIZE))
    {
        hs->println("Apple hit, adding points, resetting apple");

        switch(currentSpeed)
        {
            case slowDelay:
                score += 5;
                break;
            case medDelay:
                score += 10;
                break;
            case fastDelay:
                score += 20;
                break;
            default:
                score += 5;
                break;
        }

        resetApple();
        addUnit();
    }
}

void ModeSnake::addUnit()
{
    unitCount++;

    if (unitCount > MAX_UNITS)
    {
        unitCount = MAX_UNITS;
    }
    else if(unitCount == 3)
    {
        currentSpeed = medDelay;
    }
    else if(unitCount == 6)
    {
        currentSpeed == fastDelay;
    }

    hs->println("Unit added");
}

void ModeSnake::resetApple()
{
    apple.x = random(UNIT_SIZE + 1, (SCREEN_WIDTH / UNIT_SIZE) - UNIT_SIZE);
    apple.x *= UNIT_SIZE;
    apple.y = random(UNIT_SIZE + 1, (SCREEN_HEIGHT / UNIT_SIZE) - UNIT_SIZE);
    apple.y *= UNIT_SIZE;

    hs->println("Apple reset");
}

void ModeSnake::updateHead()
{
    if(eh->isRightJustPressed())
    {
        //one right
        direction++;
    }

    if(eh->isLeftJustPressed())
    {
        //one left
        direction--;
    }

    if(direction < 1)
    {
        direction = 4;
    }
    else if(direction > 4)
    {
        direction = 1;
    }

    // TODO:
    // this is wrong, fix me
    moveDelay--;

    if (moveDelay <= 0)
    {
        moveDelay = currentSpeed;

        updateUnits();

        switch (direction) {
            case RIGHT:
                x += moveAmount;
                break;
            case LEFT:
                x -= moveAmount;
                break;
            case UP:
                y -= moveAmount;
                break;
            case DOWN:
                y += moveAmount;
                break;
        }

        /*
        hs->print("Player x: ");
        hs->print(x);
        hs->print(" y: ");
        hs->println(y);

        hs->print("Apple x: ");
        hs->print(apple.x);
        hs->print(" y: ");
        hs->println(apple.y);

        hs->print("Direction: ");
        hs->print(direction);
        hs->print(" moveDelay: ");
        hs->print(moveDelay);
        hs->print(" currentSpeed: ");
        hs->print(currentSpeed);
        hs->print(" moveAmount: ");
        hs->println(moveAmount);
         */
    }
}

void ModeSnake::resetSnake()
{
    hs->println("Snake reset");

    unitCount = 0;

    x = START_X;
    y = START_Y;

    direction = RIGHT;
}

bool ModeSnake::collidePointRect(uint8_t x1, uint8_t y1 ,uint8_t x2 ,uint8_t y2, uint8_t w, uint8_t h)
{
    if((x1 >= x2) && (x1 < x2 + w))
    {
        if((y1 >= y2) && (y1 < y2 + h))
        {
            /*
            hs->print("Collision at: ");
            hs->print(x1);
            hs->print(" ");
            hs->println(x2);
             */

            return true;
        }
    }

    return false;
}

bool ModeSnake::collideRectRect(uint8_t x1, uint8_t y1, uint8_t w1, uint8_t h1, uint8_t x2, uint8_t y2, uint8_t w2, uint8_t h2)
{
    return !( x2     >=  x1 + w1  ||
              x2 + w2  <=  x1     ||
              y2     >=  y1 + h1  ||
              y2 + h2  <=  y1     );
}

