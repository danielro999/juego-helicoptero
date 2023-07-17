#pragma once
#include <iostream>
#include "GameObjectFactory.h"
#include "BulletHandler.h"
//#include "SoundManager.h"
#include <math.h>
#include "Enemy.h"
#include "Level1Boss.h"


class Turret : public Enemy
{
public:

    Turret()
    {
        m_dyingTime = 500;
        m_health = 2;
        m_bulletFiringSpeed = 50;
    }

    virtual ~Turret() {}

    virtual void collision()
    {
        m_health -= 1;

        if (m_health == 0)
        {
            if (!m_bPlayedDeathSound)
            {
               // TheSoundManager::Instance()->playSound("explode", 0);

                m_textureID = "largeexplosion";
                m_currentFrame = 0;
                m_numFrames = 9;
                m_width = 60;
                m_height = 60;
                m_bDying = true;
            }

        }
    }

    //void scroll(float scrollSpeed) { m_position.setX(m_position.getX() - scrollSpeed) ;}
    int mov = -1;
    virtual void update()
    {
        if (!m_bDying)
        {
            m_position.setX(m_position.getX() + 2*mov);


            if (m_position.getX() < 600)
            {
                mov = 1;
            }
            if((m_position.getX() > 1100))
            {
                mov = -1;
            }

            // we want to scroll this object with the rest          
            //scroll(TheGame::Instance()->getScrollSpeed());          
            
            if (m_bulletCounter == m_bulletFiringSpeed)
            {
                TheBulletHandler::Instance()->addEnemyBullet(m_position.getX(), m_position.getY(), 16, 16, "bullet2", 1, Vector2D(-3, -3));
                TheBulletHandler::Instance()->addEnemyBullet(m_position.getX() + 20, m_position.getY(), 16, 16, "bullet2", 1, Vector2D(0, -3));
                TheBulletHandler::Instance()->addEnemyBullet(m_position.getX() + 40, m_position.getY(), 16, 16, "bullet2", 1, Vector2D(3, -3));
                m_bulletCounter = 0;
            }

            m_bulletCounter++;
        }
        else
        {
            //scroll(TheGame::Instance()->getScrollSpeed());
            doDyingAnimation();
            
        }

    }

};

class TurretCreator : public BaseCreator
{
    GameObject* createGameObject() const
    {
        return new Turret();
    }
};

