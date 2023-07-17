#pragma once

#include <string>
#include <map>
#include <SDL_image.h>

#include "SDL.h"



class TextureManager
{
public:
	// Carga el archivi de imagen png bmp jpg..
	bool load(std::string fileName, std::string id, SDL_Renderer* pRenderer);
	// dibuja imagen 
	void draw(std::string id, int x, int y, int width, int height, SDL_Renderer* pRenderer, SDL_RendererFlip flip = SDL_FLIP_NONE);
	// dibuna una imagen en movimiento de un sprite sheet(concurren row, concurrent frame) 
	void drawFrame(std::string id, int x, int y, int width, int height, int currentRow, int currentFrame, SDL_Renderer* pRenderer, double angle, int alpha, SDL_RendererFlip flip = SDL_FLIP_NONE);
	
	void drawTile(std::string id, int margin, int spacing, int x, int y, int width, int height, int currentRow, int currentFrame, SDL_Renderer* pRenderer);
	
	std::map<std::string, SDL_Texture*> getTextureMap() { return m_textureMap; }

	static TextureManager* Instance()// definicion de funcion que crea una sola instancia y por la cual tambien se tiene acceso a esta
	{
		if (s_pInstance == 0)    // verifica si ya hay alguna instancia del objeto 
		{
			s_pInstance = new TextureManager(); // si no hay, crea una
			return s_pInstance;
		}
		return s_pInstance; // si ya hay una instancia del objeto pasa de largo el if anterior y retona la instacia estatica
	}
	void clearFromTextureMap(std::string id);

private:
	TextureManager() {} // contructor privado para crear el singleton
	
	std::map<std::string, SDL_Texture*> m_textureMap;
	static TextureManager* s_pInstance;
};

typedef TextureManager TheTextureManager;
