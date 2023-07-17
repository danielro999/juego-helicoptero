#pragma once
#include <vector>

#include "ShooterObject.h"
#include "GameState.h"
#include "CollisionManager.h"

class GameObject;
class SDLGameObject;
class Level;

class PlayState : public GameState
{
public:
	virtual void update();
	virtual void render();

	virtual bool onEnter();
	virtual bool onExit();

	virtual std::string getStateID() const { return s_playID; }

private:

	//bool checkCollision(ShooterObject* p1, ShooterObject* p2);
	
	CollisionManager m_collisionManager;

	static const std::string s_playID;
	
	std::vector<GameObject*> m_gameObjects;

	Level* pLevel;
};