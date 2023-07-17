#pragma once

#include <iostream>
#include <string>
#include <map>

#include "GameObject.h"

class BaseCreator
{
public:
	virtual GameObject* createGameObject() const = 0;
	virtual ~BaseCreator() {}
};

class GameObjectFactory

{
public:

	static GameObjectFactory* Instance() // check de instacia para singleton
	{
		if (pInstance == 0)
		{
			pInstance = new GameObjectFactory();
		}

		return pInstance;
	}

	//registerType -> busca agregar types al mapa
	bool registerType(std::string typeID, BaseCreator* pCreator) // Esta función toma la ID con la que queremos asociar con el tipo de objeto (como un string), y
																// el objeto creador de esa clase.
	{
		std::map<std::string, BaseCreator*>::iterator it = m_creators.find(typeID);
		// if the type is already registered, do nothing
		if (it != m_creators.end())
		{                           // si el type es encontrado este ya estaba registrado. entonces la funcion borra el puntero pasado
			delete pCreator;         //  y devuelve falso 
			return false;
		}
		m_creators[typeID] = pCreator; // Si el tipo aún no está registrado,este puede ser asignado al mapa y luego retorna true
		return true;                 
	}

	GameObject* create(std::string typeID) // busca el type similar a lo que lo hace registertype(), pero cheque si el type no fue encontrado, retorna 0, y 
	{								       // y si encontro al type retorna una nueva instancia de este			
		std::map<std::string, BaseCreator*>::iterator it = m_creators.find(typeID);
			
		if (it == m_creators.end())
		{
			std::cout << "could not find type: " << typeID << "\n";
			return NULL;                                            // si no encontro el type retorna 0
		}
		BaseCreator* pCreator = (*it).second; // si encontro el type usa el creator object para el type y retornar como 
		return pCreator->createGameObject();  // nueva instancia de esta un puntero GameObjenct
	}

private:
	GameObjectFactory() {}
	~GameObjectFactory() {}

	static GameObjectFactory* pInstance;

	std::map<std::string, BaseCreator*> m_creators;  // Este mapa contiene los elementos importantes de nuestra fábrica, las funcion de la clase
                                                  // esencialmente agrega o quita de este mapa.
};

typedef GameObjectFactory TheGameObjectFactory;