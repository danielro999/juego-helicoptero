#pragma once
#include "LoaderParams.h"
#include "ShooterObject.h"
#include "inputHandler.h"
#include "GameObjectFactory.h"

class MenuButton : public ShooterObject
{
public:
	MenuButton();

	virtual void draw();
	virtual void update();
	virtual void clean();
	virtual void load(std::unique_ptr<LoaderParams> const& pParams);

	void setCallback(void(*callback)()) { m_callback = callback; } 
	int getCallbackID() { return m_callbackID; }

private:
	enum button_state
	{
		MOUSE_OUT = 0,
		MOUSE_OVER = 1,
		CLICKED = 2
	};

	void (*m_callback)();  // declaro la funcion puntero
	bool m_bReleased; 
	int m_callbackID;
};

class MenuButtonCreator : public BaseCreator
{
	GameObject* createGameObject() const
	{
		return new MenuButton();
	}
};