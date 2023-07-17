#include "PlayState.h"
#include "TextureManager.h"
#include "Game.h"
#include "PauseState.h"
#include "GameOverState.h"
#include "StateParser.h"
#include "LevelParser.h"
#include "Level.h"
#include "GameObject.h"
#include "BulletHandler.h"
#include "LevelParser.h"

const std::string PlayState::s_playID = "PLAY";

void PlayState::update()
{
	if (m_loadingComplete && !m_exiting)
	{
		if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_ESCAPE))
		{
			TheGame::Instance()->getStateMachine()->pushState(new PauseState());
		}

		TheBulletHandler::Instance()->updateBullets();

		if (TheGame::Instance()->getPlayerLives() == 0)
		{
			TheGame::Instance()->getStateMachine()->changeState(new GameOverState());
		}

		if (pLevel != 0)
		{
			pLevel->update();
		}
	}
}

void PlayState::render()
{
	if (m_loadingComplete)
	{
		if (pLevel != 0)
		{
			pLevel->render();
		}

		for (int i = 0; i < TheGame::Instance()->getPlayerLives(); i++)
		{
			TheTextureManager::Instance()->drawFrame("lives", i * 30, 0, 32, 30, 0, 0, TheGame::Instance()->getRenderer(), 0.0, 255);
		}
		
		TheBulletHandler::Instance()->drawBullets();
	}
}

bool PlayState::onEnter()
{	
	TheGame::Instance()->setPlayerLives(3);
	 /*parse the state*/
	TheGame::Instance()->setLevelComplete(false);
	//StateParser stateParser;
	//stateParser.parseState("test.xml", s_playID, &m_gameObjects, &m_textureIDList);
	LevelParser levelParser;
	//pLevel = levelParser.parseLevel("map1.tmx");
	//pLevel = levelParser.parseLevel(TheGame::Instance()->getCurrentLevel());

	pLevel = levelParser.parseLevel(TheGame::Instance()->getLevelFiles()[TheGame::Instance()->getCurrentLevel()-1].c_str());
	
	//LevelParser levelParser;
	//pLevel = levelParser.parseLevel("map1.tmx");

	TheTextureManager::Instance()->load("assets/bullet1.png", "bullet1", TheGame::Instance()->getRenderer());
	TheTextureManager::Instance()->load("assets/bullet2.png", "bullet2", TheGame::Instance()->getRenderer());
	TheTextureManager::Instance()->load("assets/bullet3.png", "bullet3", TheGame::Instance()->getRenderer());
	TheTextureManager::Instance()->load("assets/lives.png", "lives", TheGame::Instance()->getRenderer());

	if (pLevel != 0)
	{
		m_loadingComplete = true;
	}

	std::cout << "level complete?: " << TheGame::Instance()->getLevelComplete() << std::endl;
	std::cout << "player live: " << TheGame::Instance()->getPlayerLives() << std::endl;
	std::cout << "current level: " << TheGame::Instance()->getCurrentLevel() << std::endl;
	std::cout << "mapa: " << TheGame::Instance()->getLevelFiles()[TheGame::Instance()->getCurrentLevel() - 1].c_str() << std::endl;
	


	std::cout << "entering PlayState\n";
	return true;
}

bool PlayState::onExit()
{/*
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects[i]->clean();
	}
	
	m_gameObjects.clear();
	
	for (int i = 0; i < m_textureIDList.size(); i++)
	{
		TheTextureManager::Instance()->clearFromTextureMap(m_textureIDList[i]);
	}*/

	m_exiting = true;

	TheInputHandler::Instance()->reset();
	TheBulletHandler::Instance()->clearBullets();
		
	std::cout << "exiting PlayState\n";
	
	return true;
}

//bool PlayState::checkCollision(ShooterObject* p1, ShooterObject* p2)
//{
//	int leftA, leftB;
//	int rightA, rightB;
//	int topA, topB;
//	int bottomA, bottomB;
//
//	leftA = p1->getPosition().getX();
//	rightA = p1->getPosition().getX() + p1->getWidth();
//	topA = p1->getPosition().getY();
//	bottomA = p1->getPosition().getY() + p1->getHeight();
//
//	//Calculate the sides of rect B
//	leftB = p2->getPosition().getX();
//	rightB = p2->getPosition().getX() + p2->getWidth();
//	topB = p2->getPosition().getY();
//	bottomB = p2->getPosition().getY() + p2->getHeight();
//
//	//If any of the sides from A are outside of B
//	if (bottomA <= topB) { return false; }
//	if (topA >= bottomB) { return false; }
//	if (rightA <= leftB) { return false; }
//	if (leftA >= rightB) { return false; }
//	return true;
//}