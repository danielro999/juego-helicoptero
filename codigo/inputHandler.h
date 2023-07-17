#pragma once

#include <vector>
#include "Vector2D.h"
#include "SDL.h"
#include "Game.h"

enum mouse_buttons
{
	LEFT = 0,
	MIDDLE = 1,
	RIGHT = 2
};


class InputHandler
{
public:

	static InputHandler* Instance()
	{
		if (s_pInstance == nullptr)
		{
			s_pInstance = new InputHandler();
		}

		return s_pInstance;
	}

	bool getMouseButtonState(int buttonNumber) { return m_mouseButtonStates[buttonNumber]; }
	bool getButtonState(int joy, int buttonNumber)  {return m_buttonStates[joy][buttonNumber];}
	bool joysticksInitialised() { return m_bJoysticksInitialised; }
	
	Vector2D* getMousePosition() { return m_mousePosition; }
	void reset();

	bool isKeyDown(SDL_Scancode key);

	void initialiseJoysticks();
	void update();
	void clean();


	int xvalue(int joy, int stick); // The 'joy' parameter is the identifier (ID) of the joystick we want to use,
	int yvalue(int joy, int stick); //         and the 'stick' is 1 for the left stick and 2 for the right stick

	
private:

	InputHandler();
	~InputHandler() {}

	// handle keyboard events
	void onKeyDown();
	void onKeyUp();

	// handle mouse events
	void onMouseMove(SDL_Event& event);
	void onMouseButtonDown(SDL_Event& event);
	void onMouseButtonUp(SDL_Event& event);

	// handle joysticks events
	void onJoystickAxisMove(SDL_Event& event);
	void onJoystickButtonDown(SDL_Event& event);
	void onJoystickButtonUp(SDL_Event& event);

	static InputHandler* s_pInstance;

	std::vector<std::pair<Vector2D*, Vector2D*>> m_joystickValues; // array de vector2D en pares, para los ejes de los joysticks
	std::vector<SDL_Joystick*> m_joysticks;          // array con los joysticks conectados
	std::vector<std::vector<bool>> m_buttonStates;   // array con boleanos para los botones
	std::vector<bool> m_mouseButtonStates;      // array con los estados del mouse
	
	Vector2D* m_mousePosition;
	
	
	const Uint8* m_keystates;

	const int m_joystickDeadZone = 10000;
	bool m_bJoysticksInitialised;
};
typedef InputHandler TheInputHandler;
