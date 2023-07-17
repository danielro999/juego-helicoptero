#pragma once
#include <iostream>
#include <vector>

#include "tinyxml/tinyxml.h"
//#include "GameObject.h"

class GameObject;

class StateParser
{
public:
	bool parseState(const char* stateFile, std::string stateID, std::vector<GameObject*>* pObjects, std::vector<std::string>* pTextureIDs);
	
	//toma un filename de un archivo xml como parametro, junto con el actual  valor del stateID y un puntero a std::vector of GameObject* para ese estado	
private:

	void parseObjects(TiXmlElement* pStateRoot, std::vector<GameObject*>* pObjects);
	void parseTextures(TiXmlElement* pStateRoot, std::vector<std::string>* pTextureIDs);
};