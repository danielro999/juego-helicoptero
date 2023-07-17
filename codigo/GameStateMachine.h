#pragma once
#include <vector>
#include "GameState.h"

class GameStateMachine                           // manejo de los estados del juego
{
private:
	std::vector<GameState*> m_gameStates;  // array de tipo GameState
public:
	void pushState(GameState* pState);   // agrega otro estado sin remover el anterior
	void changeState(GameState* pState); // remueve el estado previo 
	void popState();                     // remueve el stado actual sin agregar otro

	void update();	
	void render();
	void clean();

	std::vector<GameState*>& getGameStates() { return m_gameStates; }
	
};