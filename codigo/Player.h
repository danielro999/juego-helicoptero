#pragma once
#include "ShooterObject.h"
#include "SDL.h"
#include "inputHandler.h"
#include "GameObjectFactory.h"

class Player : public ShooterObject  // la clase player hereda de la clase sdlgameobjet y este de la clase GameObject
{
public:
	Player();
	virtual void draw();
	virtual void update();
	virtual void clean();
	virtual void load(std::unique_ptr<LoaderParams> const& pParams);
	
	virtual void collision();

	virtual std::string type() { return "Player"; }

	void LimitePlayer();

private:
	// bring the player back if there are lives left
	void ressurect();
	// handle any input from the keyboard, mouse, or joystick
	void handleInput();
	// handle any animation for the player
	void handleAnimation();
	// player can be invulnerable for a time
	int m_invulnerable;
	int m_invulnerableTime;
	int m_invulnerableCounter;
};


class PlayerCreator : public BaseCreator	   //creador de clases
{
	GameObject* createGameObject() const
	{
		return new Player();
	}
};