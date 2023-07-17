#include <iostream>

#include "InputHandler.h"
#include "Game.h"
#include "MainMenuState.h"
#include "MenuButton.h"
#include "AnimatedGraphic.h"
#include "GameOverState.h"
#include "Turret.h"
#include "Level1Boss.h"


Game* Game::s_pInstance = 0;

Game::Game() :
	m_pWindow(0),
	m_pRenderer(0),
	m_bRunning(false),
	m_pGameStateMachine(0),
	m_playerLives(3),
	m_scrollSpeed(0.8),
	m_bLevelComplete(false),
	m_bChangingState(false)
{
	// add some level files to an array
	m_levelFiles.push_back("map1.tmx");
	m_levelFiles.push_back("map2.tmx");

	// start at this level
	m_currentLevel = 1;
}

Game::~Game()
{
	// we must clean up after ourselves to prevent memory leaks
	m_pRenderer = 0;
	m_pWindow = 0;
}

bool Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	// attempt to initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{   
		int flags = 0;

		m_gameWidth = width;
		m_gameHeight = height;

		if (fullscreen)
		{
			flags = SDL_WINDOW_FULLSCREEN;
		}

		std::cout << "SDL init success\n";
		// init the window
		m_pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
			
		if (m_pWindow != 0) // window init success
		{
			std::cout << "window creation success\n";
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);
			if (m_pRenderer != 0) // renderer init success
			{
				std::cout << "renderer creation success\n";
				SDL_SetRenderDrawColor(m_pRenderer, 0, 255, 255, 255);
			}
			else
			{
				std::cout << "renderer init fail\n";
				return false; // renderer init fail
			}
		}
		else
		{
			std::cout << "window init fail\n";
			return false; // window init fail
		}
	}
	else
	{
		std::cout << "SDL init fail\n";
		return false; // SDL init fail
	}

	std::cout << "init success\n";

	TheInputHandler::Instance()->initialiseJoysticks();

	m_bRunning = true; // everything inited successfully, start the main loop

	
	TheGameObjectFactory::Instance()->registerType("MenuButton", new MenuButtonCreator()); // restro 
	TheGameObjectFactory::Instance()->registerType("Player", new PlayerCreator());	
	
	TheGameObjectFactory::Instance()->registerType("AnimatedGraphic", new AnimatedGraphicCreator());

	TheGameObjectFactory::Instance()->registerType("Turret", new TurretCreator());
	TheGameObjectFactory::Instance()->registerType("Level1Boss", new Level1BossCreator());

	
		
	m_pGameStateMachine = new GameStateMachine();
	m_pGameStateMachine->changeState(new MainMenuState());

	return true;
}

void Game::setCurrentLevel(int currentLevel)
{
	m_currentLevel = currentLevel;
	m_pGameStateMachine->changeState(new GameOverState());
	m_bLevelComplete = false;
}


void Game::render()
{
	SDL_RenderClear(m_pRenderer);

	m_pGameStateMachine->render();

	SDL_RenderPresent(m_pRenderer);
}
void Game::update()
{
	m_pGameStateMachine->update();
}

void Game::handleEvents()
{
	TheInputHandler::Instance()->update();

	if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_RETURN))
	{
		m_pGameStateMachine->changeState(new PlayState());
	}
}

void Game::clean()
{
	std::cout << "cleaning game\n";
	SDL_DestroyWindow(m_pWindow);
	SDL_DestroyRenderer(m_pRenderer);
	TheInputHandler::Instance()->clean();
	SDL_Quit();
}