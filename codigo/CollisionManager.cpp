
#include <vector>
#include <iostream>

#include "CollisionManager.h"
#include "Collision.h"
#include "Enemy.h"
#include "BulletHandler.h"
#include "TileLayer.h"
#include "Level.h"


void CollisionManager::checkEnemyPlayerBulletCollision(const std::vector<GameObject*>& objects)
{
    for (int i = 0; i < objects.size(); i++)  // for iterando sobre el array de objetos
    {
        GameObject* pObject = objects[i];    // puntero al objeto de ese momento [i]

        for (int j = 0; j < TheBulletHandler::Instance()->getPlayerBullets().size(); j++)  // for iterando sobre el array de las balas del player 
        {
            if (pObject->type() != std::string("Enemy") || !pObject->updating())   // si el tipo de objeto NO es "enemy" o No se esta actualizando ->
            {
                continue;                                                        //  CONTINUAR (no hacer nada, salir de bucle) saltar la ejecucion del if y seguir con la siguiente iteracion(bala del jugador)
            }

            SDL_Rect* pRect1 = new SDL_Rect();                      // creo un rectangulo "pRect1" con el tamaño, posicion del objeto
            pRect1->x = pObject->getPosition().getX();
            pRect1->y = pObject->getPosition().getY();
            pRect1->w = pObject->getWidth();
            pRect1->h = pObject->getHeight();

            PlayerBullet* pPlayerBullet = TheBulletHandler::Instance()->getPlayerBullets()[j]; // creo un in puntero sobre la bala del jugador con indece [j]

            SDL_Rect* pRect2 = new SDL_Rect();                     // creo un rectangulo "pRect2" con el tamaño, posicion  de la bala
            pRect2->x = pPlayerBullet->getPosition().getX();
            pRect2->y = pPlayerBullet->getPosition().getY();
            pRect2->w = pPlayerBullet->getWidth();
            pRect2->h = pPlayerBullet->getHeight();

            if (RectRect(pRect1, pRect2))                //RectRect () es un boleano, hace el buffer de achicar el tamaño del "Rect" y verificar si colicionan
            {
                if (!pObject->dying() && !pPlayerBullet->dying())     // si el objeto no se esta muriendo y la bala no se esta muriendo
                {
                    pPlayerBullet->collision();                       // combierte a """true""" la variable m_bDead
                    pObject->collision();
                }

            }

            delete pRect1;                                     // borrado de los rectangulos temporales
            delete pRect2;
        }
    }
}

void CollisionManager::checkPlayerEnemyBulletCollision(Player* pPlayer)
{
    SDL_Rect* pRect1 = new SDL_Rect();
    pRect1->x = pPlayer->getPosition().getX();
    pRect1->y = pPlayer->getPosition().getY();
    pRect1->w = pPlayer->getWidth();
    pRect1->h = pPlayer->getHeight();

    

    for (int i = 0; i < TheBulletHandler::Instance()->getEnemyBullets().size(); i++)
    {
        EnemyBullet* pEnemyBullet = TheBulletHandler::Instance()->getEnemyBullets()[i];

        SDL_Rect* pRect2 = new SDL_Rect();
        pRect2->x = pEnemyBullet->getPosition().getX();
        pRect2->y = pEnemyBullet->getPosition().getY();

        pRect2->w = pEnemyBullet->getWidth();
        pRect2->h = pEnemyBullet->getHeight();

        if (RectRect(pRect1, pRect2))
        {
           // std::cout << "hay colision" << std::endl;
            if (!pPlayer->dying() && !pEnemyBullet->dying())
            {
                pEnemyBullet->collision();
                pPlayer->collision();
            }
        }

        delete pRect2;
    }

    delete pRect1;
}



void CollisionManager::checkPlayerEnemyCollision(Player* pPlayer, const std::vector<GameObject*>& objects)
{
    SDL_Rect* pRect1 = new SDL_Rect();
    pRect1->x = pPlayer->getPosition().getX();
    pRect1->y = pPlayer->getPosition().getY();
    pRect1->w = pPlayer->getWidth();
    pRect1->h = pPlayer->getHeight();

    for (int i = 0; i < objects.size(); i++)
    {
        if (objects[i]->type() != std::string("Enemy") || !objects[i]->updating())
        {
            continue;
        }

        SDL_Rect* pRect2 = new SDL_Rect();
        pRect2->x = objects[i]->getPosition().getX();
        pRect2->y = objects[i]->getPosition().getY();
        pRect2->w = objects[i]->getWidth();
        pRect2->h = objects[i]->getHeight();

        if (RectRect(pRect1, pRect2))
        {
            //std::cout << "hay colision" << std::endl;
            if (!objects[i]->dead() && !objects[i]->dying())
            {
                pPlayer->collision();
            }
        }

        delete pRect2;
    }

    delete pRect1;
}

void CollisionManager::checkPlayerTileCollision(Player* pPlayer, const std::vector<TileLayer*>& collisionLayers)
{
	// iterate through collision layers
	for (std::vector<TileLayer*>::const_iterator it = collisionLayers.begin(); it != collisionLayers.end(); ++it)
	{
		TileLayer* pTileLayer = (*it);
		std::vector<std::vector<int>> tiles = pTileLayer -> getTileIDs();

		// get this layers position
		Vector2D layerPos = pTileLayer->getPosition();
		int x, y, tileColumn, tileRow, tileid = 0;
		// calculate position on tile map
		x = layerPos.getX() / pTileLayer->getTileSize();
		y = layerPos.getY() / pTileLayer->getTileSize();
		// if moving forward or upwards
		if (pPlayer->getVelocity().getX() >= 0 || pPlayer -> getVelocity().getY() >= 0)

		{
			tileColumn = ((pPlayer->getPosition().getX() + pPlayer -> getWidth()) / pTileLayer->getTileSize());

			tileRow = ((pPlayer->getPosition().getY() + pPlayer -> getHeight())/pTileLayer->getTileSize());

			tileid = tiles[tileRow + y][tileColumn + x];
		}
		else if (pPlayer->getVelocity().getX() < 0 || pPlayer -> getVelocity().getY() < 0) // if moving backwards or downwards
		
		{
			tileColumn = pPlayer->getPosition().getX() / pTileLayer -> getTileSize();
		
			tileRow = pPlayer->getPosition().getY() / pTileLayer -> getTileSize();

			tileid = tiles[tileRow + y][tileColumn + x];
		}
		if (tileid != 0) // if the tile id not blank then collide
		{
			pPlayer->collision();
		}
	}
}