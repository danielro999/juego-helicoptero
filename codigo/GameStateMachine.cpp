#include"GameStateMachine.h"

void GameStateMachine::pushState(GameState* pState) 
{
	m_gameStates.push_back(pState); //  agrega el alemento a la ultima posicion del vector
	m_gameStates.back()->onEnter(); // back() retorna(muestra) el ultimo elemento de la lista
}

void GameStateMachine::popState()  // chequeo si hay algun estado por remover, si es asi lo remueve
{
	if (!m_gameStates.empty())
	{
		m_gameStates.back()->onExit();
		m_gameStates.pop_back();
	}

	m_gameStates.back()->resume();

	//if (!m_gameStates.empty())  // si el array no esta vacio
	//{
	//	if (m_gameStates.back()->onExit()) // si hay algun estado por remover
	//	{
	//		delete m_gameStates.back(); // destructor del objeto, en la ultima posicion de la lista
	//		m_gameStates.pop_back();      //remueve el ultimo estado de  la lista
	//	}
	//}
}

void GameStateMachine::changeState(GameState* pState)
{


	if (!m_gameStates.empty())
	{
		if (m_gameStates.back()->getStateID() == pState->getStateID())
		{
			return; // do nothing
		}

		m_gameStates.back()->onExit();
		m_gameStates.pop_back();
	}

	// initialise it
	pState->onEnter();

	// push back our new state
	m_gameStates.push_back(pState);


	//if (!m_gameStates.empty())                // si el array no esta vacio
	//{
	//	if (m_gameStates.back()->getStateID() == pState->getStateID())  // hay algun estado actual concurrente? coincidencia del id
	//	{
	//		return; // do nothing
	//	}
	//	if (m_gameStates.back()->onExit())  // If the state IDs do not match,then we remove the current state,	
	//	{
	//		delete m_gameStates.back();
	//		m_gameStates.pop_back();
	//	}
	//}
	//// push back our new state
	//m_gameStates.push_back(pState);   // agregar nuevo estado al final del array
	//// initialise it
	//m_gameStates.back()->onEnter();       // ->onEnter() activarlo
}

void GameStateMachine::update()
{
	if (!m_gameStates.empty()) // Si el array de los estados del juego no esta vacio, o sea hay algo
	{
		m_gameStates.back()->update();  // update el ultimo elemento del array
	}
}
void GameStateMachine::render()
{
	if (!m_gameStates.empty())
	{
		m_gameStates.back()->render();
	}
}

void GameStateMachine::clean()
{
	if (!m_gameStates.empty())
	{
		m_gameStates.back()->onExit();

		delete m_gameStates.back();

		m_gameStates.clear();
	}
}