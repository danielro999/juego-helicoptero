#include <string>
#include <iostream>

#include "StateParser.h"
#include "GameObjectFactory.h"
#include "Game.h"

//bool StateParser::parseState(const char* stateFile, string stateID, vector<GameObject*>* pObjects, std::vector<std::string>* pTextureIDs)
bool StateParser::parseState(const char* stateFile, std::string stateID, std::vector<GameObject*>* pObjects, std::vector<std::string>* pTextureIDs)
	
{
	// create the XML document
	TiXmlDocument xmlDoc;
	// load the state file
	if (!xmlDoc.LoadFile(stateFile))
	{
		std::cerr << xmlDoc.ErrorDesc() << "\n";     
		return false;                                 //intenta leer el xml file que le pasamos a la funcion
	}                                                 // se muestra un error si la carga del xml file falla
	// get the root element
	TiXmlElement* pRoot = xmlDoc.RootElement();       // <STATES> el resto de nodos en el archivo son todos hijos(children) de este nodo root
	// pre declare the states root node
	TiXmlElement* pStateRoot = 0;
	// get this states root node and assign it to pStateRoot
	for (TiXmlElement* e = pRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement())
		
	{
		if (e->Value() == stateID)
		{
			pStateRoot = e;
		}                                        //hasta aca va por cada hijo del nodo y chequea si los nombres coinciden con del de stateID
	}														// ejem: <MENU> // the states root node		
	// pre declare the texture root
	TiXmlElement* pTextureRoot = 0;                     ///una vez que tenga el el puntero al nodo root de nuestro estado, comenzamos a sacar los valores de ahi 
	// get the root of the texture elements             
																	
	for (TiXmlElement* e = pStateRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement())	
	{														//First we want to load the textures from the file so we look for the <TEXTURE>
		if (e->Value() == std::string("TEXTURES"))         //node using the children of the pStateRoot object we found before
		{
			pTextureRoot = e;
		}
	}
	// now parse the textures
	parseTextures(pTextureRoot, pTextureIDs);//	Once the <TEXTURE> node is found, we can pass it into the private parseTextures
											 // function(which we will cover a little later)
	// pre declare the object root node
	TiXmlElement* pObjectRoot = 0;           // The function then moves onto searching for the <OBJECT> node and, once found
											// it passes it into the private parseObjects function. We also pass in the pObjects parameter:

// get the root node and assign it to pObjectRoot
	for (TiXmlElement* e = pStateRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement())
	{
		if (e->Value() == std::string("OBJECTS"))
		{
			pObjectRoot = e;
		}
	}
	// now parse the objects
	parseObjects(pObjectRoot, pObjects);
	return true;
}

//This function gets the filename and ID attributes from each of the texture values in this part of the XML :
/*
<TEXTURES>
<texture filename = "button.png" ID = "playbutton" / >
<texture filename = "exit.png" ID = "exitbutton" / >
< / TEXTURES>
*/
//It then adds them to TextureManager.TheTextureManager::Instance()->load(filenameAttribute,idAttribute, TheGame::Instance()->getRenderer());

void StateParser::parseTextures(TiXmlElement* pStateRoot, std::vector<std::string>* pTextureIDs)										
{  //This function gets the filename and ID attributes from each of the texture values in this part of the XML
	for (TiXmlElement* e = pStateRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement())		
	{
		std::string filenameAttribute = e->Attribute("filename");
		std::string idAttribute = e->Attribute("ID");
		pTextureIDs->push_back(idAttribute); // push into list
		TheTextureManager::Instance()->load(filenameAttribute, idAttribute, TheGame::Instance()->getRenderer());
			
	}
}

/*
The parseObjects function is quite a bit more complicated. It creates objects using
our GameObjectFactory function and reads from this part of the XML file:

<OBJECTS>
<object type="MenuButton" x="100" y="100" width="400" height="100" textureID="playbutton" numFrames="0" callbackID="1"/>
<object type="MenuButton" x="100" y="300" width="400" height="100" textureID="exitbutton" numFrames="0" callbackID="2"/>
</OBJECTS>
*/

void StateParser::parseObjects(TiXmlElement* pStateRoot, std::vector<GameObject*>* pObjects)	
{
	for (TiXmlElement* e = pStateRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement())
		
	{ //First we get any values we need from the current node
		int x, y, width, height, numFrames, callbackID, animSpeed; 
		std::string textureID;
		
		e->Attribute("x", &x);   // pasar cada texto puro del archivo xml a un valor que se quiera setear y su atributo
		e->Attribute("y", &y);   // This sets the variable y to the value contained within attribute "y"
		e->Attribute("width", &width);
		e->Attribute("height", &height);
		e->Attribute("numFrames", &numFrames);
		e->Attribute("callbackID", &callbackID);
		e->Attribute("animSpeed", &animSpeed);

		textureID = e->Attribute("textureID");

		//craeacion de la  clase GameObject* usando factory.
		GameObject* pGameObject = TheGameObjectFactory::Instance()->create(e->Attribute("type"));
		// ya le pasamos los valores del tipo de atributo y lo usamos para crear el objeto correcto de la factory
		//ahora usamos load funcion de Gameobject para setear nuestro valores deseados cargados del xmk file
		//pGameObject->load(new LoaderParams(x, y, width, height, textureID, numFrames, callbackID, animSpeed));
		pGameObject->load(std::unique_ptr<LoaderParams>(new LoaderParams(x, y, width, height, textureID, numFrames, callbackID, animSpeed)));
		//al final hacemos un push del pGameObjects en el pObjects array, cual es actualmente un puntero del actual vectos estado de objetos 
		pObjects->push_back(pGameObject);
	}
}