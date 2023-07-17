#ifndef __Game__
#define __Game__

#include <vector>

#include "SDL.h"
#include "TextureManager.h"
#include "GameObject.h"
#include "Player.h"
#include "Enemy.h"
#include "GameStateMachine.h"
#include "MenuState.h"
#include "PlayState.h"

class Game
{
public:

	bool init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
	void render();
	void update();
	void handleEvents();
	void clean();

	bool running() { return m_bRunning; }
	void quit() { m_bRunning = false; }

	SDL_Renderer* getRenderer() const { return m_pRenderer; } // return our SDL_Renderer object
	SDL_Window* getWindow() const { return m_pWindow; }
	GameStateMachine* getStateMachine() { return m_pGameStateMachine; }
	
	void setPlayerLives(int lives) { m_playerLives = lives; }
	int getPlayerLives() { return m_playerLives; }

	void setCurrentLevel(int currentLevel);
	const int getCurrentLevel() { return m_currentLevel; }

	void setNextLevel(int nextLevel) { m_nextLevel = nextLevel; }
	const int getNextLevel() { return m_nextLevel; }

	void setLevelComplete(bool levelComplete) { m_bLevelComplete = levelComplete; }
	const bool getLevelComplete() { return m_bLevelComplete; }


	int getGameWidth() const { return m_gameWidth; }
	int getGameHeight() const { return m_gameHeight; }
	float getScrollSpeed() { return m_scrollSpeed; }

	bool changingState() { return m_bChangingState; }
	void changingState(bool cs) { m_bChangingState = cs; }

	std::vector<std::string> getLevelFiles() { return m_levelFiles; }


	static Game* Instance() // create the public instance function
	{
		if (s_pInstance == 0)
		{
			s_pInstance = new Game();
			return s_pInstance;
		}
		return s_pInstance;
	}

private:
	//Game() {} // make the constructor private

	static Game* s_pInstance; // create the s_pInstance member variable

	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;
	SDL_Texture* m_pTexture;

	GameStateMachine* m_pGameStateMachine;

	std::vector<GameObject*> m_gameObjects;
	
	std::vector<std::string> m_levelFiles;

	bool m_bChangingState;
	bool m_bRunning;
	int m_currentFrame;

	int m_gameWidth;
	int m_gameHeight;

	float m_scrollSpeed;

	int m_playerLives;

	int m_currentLevel;
	int m_nextLevel;
	bool m_bLevelComplete;
	
	 Game();
    ~Game();
    
    Game(const Game&);
	Game& operator=(const Game&);
	
};

typedef Game TheGame;// create the typedef

#endif /* defined(__Game__) */