#include <iostream>
#include"inputHandler.h"
#include "SDL.h"

InputHandler* InputHandler::s_pInstance = 0;

InputHandler::InputHandler()
: m_mousePosition(new Vector2D(0, 0)), m_bJoysticksInitialised(false), m_keystates (0)
{
	for (int i = 0; i < 3; i++)
	{
		m_mouseButtonStates.push_back(false);   // inicializa el array con los botones del mouse en false 
	}
}

void InputHandler::initialiseJoysticks()
{
	if (SDL_WasInit(SDL_INIT_JOYSTICK) == 0) // chequeo de subsistema joystick fue inicializado 
	{
		SDL_InitSubSystem(SDL_INIT_JOYSTICK); // si no, aca lo inicializa
	}
	if (SDL_NumJoysticks() > 0) // DL_NumJoysticks no dice si hay joystick 
	{
		for (int i = 0; i < SDL_NumJoysticks(); i++) // loop sobre la cantidad de joystick
		{
			SDL_Joystick* joy = SDL_JoystickOpen(i); // SDL_JoystickOpen va habilitando los joystick en orden

			if (SDL_JoystickGetAttached(joy) == SDL_TRUE) // si hay algun joystick conectado
			{
				m_joysticks.push_back(joy); // array(nos sirve para hacer el cierre de estos) de los joystick abierto
				m_joystickValues.push_back(std::make_pair(new Vector2D(0, 0), new Vector2D(0, 0))); //va creando pares de ejes(vectores en pares)		
				
				std::vector<bool> tempButtons;  // array de bolean
				
				for (int j = 0; j < SDL_JoystickNumButtons(joy); j++) //SDL_JoystickNumButtons para obtener la cantidad de botones por joysticks = (joy)
				{
					tempButtons.push_back(false); // por cada boton suma un elemento (boleano falso, sin apretar) por cada boton, o sea los inicializa todos en 0 o false
				}

				m_buttonStates.push_back(tempButtons); // el array tempButtons es sumado al array de los estados de los botones (m_buttonStates)
			}
			else
			{
				std::cout << SDL_GetError();
			}
		}
		SDL_JoystickEventState(SDL_ENABLE);  // avisa a sdl que comience a escuchar los estados de los joysticks
		m_bJoysticksInitialised = true;       // bolean del la inicializacon pasa a true 
		std::cout << "Initialised " << m_joysticks.size() << "joystick(s)\n";
			
	}
	else
	{
		m_bJoysticksInitialised = false;
	}
}

void InputHandler::clean() // loop del SDL_Joystick* array y llamando a la funcion SDL_JoystickClose con cada iteracion
{
	if (m_bJoysticksInitialised)
	{
		for (unsigned int i = 0; i < SDL_NumJoysticks(); i++)
		{
			SDL_JoystickClose(m_joysticks[i]);
		}
	}
}

bool InputHandler::isKeyDown(SDL_Scancode key)
{
	if (m_keystates != 0)
	{
		if (m_keystates[key] == 1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}

void InputHandler::update()
{
	
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		//std::cout << m_mousePosition->getX() << std::endl;
		 switch (event.type)
		 {
		 case SDL_QUIT:
			 TheGame::Instance()->quit();
			 break;
		 case SDL_JOYAXISMOTION:
			 onJoystickAxisMove(event);
			 break;
		 case SDL_JOYBUTTONDOWN:
			 onJoystickButtonDown(event);
			 break;
		 case SDL_JOYBUTTONUP:
			 onJoystickButtonUp(event);
			 break;
		 case SDL_MOUSEMOTION:
			 onMouseMove(event);
			 break;
		 case SDL_MOUSEBUTTONDOWN:
			 onMouseButtonDown(event);
			 break;
		 case SDL_MOUSEBUTTONUP:
			 onMouseButtonUp(event);
			 break;
		 case SDL_KEYDOWN:
			 onKeyDown();
			 break;
		 case SDL_KEYUP:
			 onKeyUp();
			 break;
		 default:
			 break;
		 }
	}
}

void InputHandler::onMouseButtonDown(SDL_Event& event)  // botones del mouse
{
	if (event.button.button == SDL_BUTTON_LEFT)
	{
		m_mouseButtonStates[LEFT] = true;               // pasan a true en el array m_mouseButtonStates cuando apreto un boton del mouse
	}
	if (event.button.button == SDL_BUTTON_MIDDLE)
	{
		m_mouseButtonStates[MIDDLE] = true;
	}
	if (event.button.button == SDL_BUTTON_RIGHT)
	{
		m_mouseButtonStates[RIGHT] = true;
	}
}

void InputHandler::onMouseButtonUp(SDL_Event& event)
{
	if (event.button.button == SDL_BUTTON_LEFT)      // pasan a false el el array m_mouseButtonStates cuando suelto un boton del mouse
	{
		m_mouseButtonStates[LEFT] = false;
	}

	if (event.button.button == SDL_BUTTON_MIDDLE)
	{
		m_mouseButtonStates[MIDDLE] = false;
	}

	if (event.button.button == SDL_BUTTON_RIGHT)
	{
		m_mouseButtonStates[RIGHT] = false;
	}
}

void InputHandler::reset()
{
	for (int i = 0; i < 3; i++)
	{
		m_mouseButtonStates.push_back(false);
	}
}

void InputHandler::onJoystickAxisMove(SDL_Event& event)
{
	int whichOne = event.jaxis.which;  // de donde proviene el event , que joystick lo genero,  whichOne es el identificador de ese joy
	// left stick move left or right
	if (event.jaxis.axis == 0)  // chequeo de ejes
	{
		if (event.jaxis.value > m_joystickDeadZone)
		{
			m_joystickValues[whichOne].first->setX(1);  // m_joystickValues[whichOne] guarda en el array el evento

		}													 //m_joystickValues array de vector2D en pares, para los ejes de los joysticks
		else if (event.jaxis.value < -m_joystickDeadZone)
		{
			m_joystickValues[whichOne].first->setX(-1);  // aca muevo en el eje 1 un numero negativo, o sea muevo el personaje para atras
		}
		else
		{
			m_joystickValues[whichOne].first->setX(0); // cuando el eje no es precionado no ha movimineto
		}
	}
	// left stick move up or down
	if (event.jaxis.axis == 1)
	{
		if (event.jaxis.value > m_joystickDeadZone)
		{
			m_joystickValues[whichOne].first->setY(1);
		}
		else if (event.jaxis.value < -m_joystickDeadZone)
		{
			m_joystickValues[whichOne].first->setY(-1);
		}
		else
		{
			m_joystickValues[whichOne].first->setY(0);
		}
	}
	// right stick move left or right
	if (event.jaxis.axis == 3)
	{
		if (event.jaxis.value > m_joystickDeadZone)
		{
			m_joystickValues[whichOne].second->setX(1);
		}
		else if (event.jaxis.value < -m_joystickDeadZone)
		{
			m_joystickValues[whichOne].second->setX(-1);
		}
		else
		{
			m_joystickValues[whichOne].second->setX(0);
		}
	}
	// right stick move up or down
	if (event.jaxis.axis == 4)
	{
		if (event.jaxis.value > m_joystickDeadZone)
		{
			m_joystickValues[whichOne].second->setY(1);
		}
		else if (event.jaxis.value < -m_joystickDeadZone)
		{
			m_joystickValues[whichOne].second->setY(-1);
		}
		else
		{
			m_joystickValues[whichOne].second->setY(0);
		}
	}
}

void InputHandler::onJoystickButtonDown(SDL_Event& event)  //eventos botones joysticks
{																	
	int whichOne = event.jaxis.which;
	m_buttonStates[whichOne][event.jbutton.button] = true; //[event.jbutton.button] We then use the button number(event.jbutton.button) to set the correct button to true
														   // en el array de estado de los botones, pasa a true el boton precionado
}

void InputHandler::onJoystickButtonUp(SDL_Event & event)
{
	int whichOne = event.jaxis.which;
	m_buttonStates[whichOne][event.jbutton.button] = false; // al soltar el boton en estados de botones vuelve a false el boton soltado
}

void InputHandler::onKeyDown()
{
	m_keystates = SDL_GetKeyboardState(0); // apretar tecla
}

void InputHandler::onKeyUp()            
{
	m_keystates = SDL_GetKeyboardState(0); //soltal tecla
}

void InputHandler::onMouseMove(SDL_Event& event)
{
	m_mousePosition->setX(event.motion.x);
	m_mousePosition->setY(event.motion.y);
}
		
int InputHandler::xvalue(int joy, int stick)
{
	if(m_joystickValues.size() > 0)
	{
		if(stick == 1)
		{
			return m_joystickValues[joy].first->getX();   // joy (joystick id)= whichOne, cual joystick estoy usando
		}
		else if(stick == 2)                               // de ese joystica es el eje 2?
		{
			return m_joystickValues[joy].second->getX();  // si es que estoy tocando el joystick el eje 2 retorna un par de vectores, (vector_eje1, vector_eje2 )  
		}												  // el segundo(second) vector es el que modifico					
	}
	return 0;
}
int InputHandler::yvalue(int joy, int stick)
{
	if (m_joystickValues.size() > 0)
	{
		if (stick == 1)
		{
			return m_joystickValues[joy].first->getY();
		}
		else if (stick == 2)
		{
			return m_joystickValues[joy].second->getY();
		}
	}
	return 0;
}