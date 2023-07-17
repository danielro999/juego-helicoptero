#include <iostream>
#include "Player.h"
#include "Vector2D.h"
#include "BulletHandler.h"
#include "PauseState.h"

using namespace std;

Player::Player() : ShooterObject(),
m_invulnerable(false),
m_invulnerableTime(200),
m_invulnerableCounter(0)
{
}

void Player::collision()
{
	//std::cout << "hay colision" << std::endl;
	 //if the player is not invulnerable then set to dying and change values for death animation tile sheet
	if (!m_invulnerable && !TheGame::Instance()->getLevelComplete())
	{
		m_textureID = "largeexplosion";
		m_currentFrame = 0;
		m_numFrames = 9;
		m_width = 60;
		m_height = 60;
		m_bDying = true;
	
	}
}

void Player::load(std::unique_ptr<LoaderParams> const& pParams)
{
	// inherited load function
	ShooterObject::load(std::move(pParams));

	// can set up the players inherited values here

	// set up bullets
	m_bulletFiringSpeed = 13;
	m_moveSpeed = 100;

	// we want to be able to fire instantly
	m_bulletCounter = m_bulletFiringSpeed;

	m_bombCounter = m_bulletFiringSpeed*4;

	// time it takes for death explosion
	m_dyingTime = 100;
}

void Player::draw()
{
	//TextureManager::Instance()->drawFrame(m_textureID, (Uint32)m_position.getX(), (Uint32)m_position.getY(), m_width, m_height, m_currentRow, m_currentFrame, TheGame::Instance()->getRenderer(), m_angle, m_alpha, SDL_FLIP_HORIZONTAL);
	ShooterObject::draw(); // we now use SDLGameObject
}
void Player::update()
{
	//m_currentFrame = int(((SDL_GetTicks() / (100)) % m_numFrames));
	// if the level is complete then fly off the screen
	if (TheGame::Instance()->getLevelComplete())
	{
		if (m_position.getX() >= TheGame::Instance()->getGameWidth()) //si salis de la pantalla al final, termina el juego
		{
			TheGame::Instance()->getStateMachine()->pushState(new PauseState());

			//TheGame::Instance()->setCurrentLevel(TheGame::Instance()->getCurrentLevel() + 1);		
		}
		else
		{
			m_velocity.setY(0);
			m_velocity.setX(3);
			ShooterObject::update();
			handleAnimation();
		}
	}
	else
	{
		// if the player is not doing its death animation then update it normally
		if (!m_bDying)
		{
			LimitePlayer();
			// reset velocity
			m_velocity.setX(0);
			m_velocity.setY(0);
			// get input
			handleInput();
			// do normal position += velocity update
			ShooterObject::update();
			// update the animation
			handleAnimation();
		}
		else // if the player is doing the death animation
		{
			m_currentFrame = int(((SDL_GetTicks() / (100)) % m_numFrames));

			// if the death animation has completed
			if (m_dyingCounter == m_dyingTime)
			{
				// ressurect the player
				ressurect();
			}
			m_dyingCounter++;
		}
	}
}
void Player::LimitePlayer()
{
	if (m_position.getX() < 1)
	{
		m_position.setX(0);
	}
	if (m_position.getY() > TheGame::Instance()->getGameHeight()-98)
	{
		m_position.setY(TheGame::Instance()->getGameHeight() - 98);
	}

	if (m_position.getX() > TheGame::Instance()->getGameWidth()-101)
	{
		m_position.setX(TheGame::Instance()->getGameWidth()-101);
	}
	if (m_position.getY() < 0)
	{
		m_position.setY(0);
	}

}

void Player::ressurect()
{
	TheGame::Instance()->setPlayerLives(TheGame::Instance()->getPlayerLives() - 1);

	m_position.setX(50);
	m_position.setY(665);
	m_bDying = false;
	m_textureID = "helicopter";
	m_currentFrame = 0;
	m_numFrames = 5;
	m_width = 101;
	m_height = 46;
	m_dyingCounter = 0;
	m_invulnerable = true;
}

void Player::handleAnimation()
{
	// if the player is invulnerable we can flash its alpha to let´people know

	if (m_invulnerable)
	{
		// invulnerability is finished, set values back
		if (m_invulnerableCounter == m_invulnerableTime)
		{
			m_invulnerable = false;
			m_invulnerableCounter = 0;
			m_alpha = 255;
		}
		else// otherwise, flash the alpha on and off
		{
			if (m_alpha == 255)
			{
				m_alpha = 0;
			}
			else
			{
				m_alpha = 255;
			}
		}
		// increment our counter
		m_invulnerableCounter++;
	}


	// if the player is not dead then we can change the angle with
	//the velocity to give the impression of a moving helicopter
	if (!m_bDead)
	{
		if (m_velocity.getX() < 0)
		{
			m_angle = -10.0;
		}
		else if (m_velocity.getX() > 0)
		{
			m_angle = 10.0;
		}
		else
		{
			m_angle = 0.0;
		}
	}
	// our standard animation code - for helicopter propellors
	m_currentFrame = int(((SDL_GetTicks() / (100)) % m_numFrames));
}


void Player::clean()
{
	ShooterObject::clean();
}


void Player::handleInput()
{

//evento boton mouse
	if (TheInputHandler::Instance()->getMouseButtonState(LEFT))
	{
		m_velocity.setX(1);
	}

// teclas teclado

	if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_RIGHT))
	{
		m_velocity.setX(3);
	}
	if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_LEFT))
	{
		m_velocity.setX(-3);
	}
	if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_UP))
	{
		m_velocity.setY(-3);
	}
	if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_DOWN))
	{
		m_velocity.setY(3);
	}

	if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_SPACE))
	{
		if (m_bulletCounter == m_bulletFiringSpeed)
		{
			//TheSoundManager::Instance()->playSound("shoot", 0);
			TheBulletHandler::Instance()->addPlayerBullet(m_position.getX() + 90, m_position.getY() + 12, 11, 11, "bullet1", 1, Vector2D(10, 0));
			m_bulletCounter = 0;
		}
		m_bulletCounter++;
	}
	else
	{
		m_bulletCounter = m_bulletFiringSpeed;
	}

	if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_LCTRL))
	{
		if (m_bombCounter == m_bulletFiringSpeed *4)
		{
			TheBulletHandler::Instance()->addPlayerBullet(m_position.getX() + 90, m_position.getY() + 12, 33, 33, "bomb", 1, Vector2D(m_velocity.getX()/2, 3));
			m_bombCounter = 0;
		}
		m_bombCounter++;
	}
	else
	{
		m_bombCounter = m_bulletFiringSpeed*4;
	}
//joysticks

	if (TheInputHandler::Instance()->joysticksInitialised())
	{    // eje 1, X
		if (TheInputHandler::Instance()->xvalue(0, 1) > 0 || TheInputHandler::Instance()->xvalue(0, 1) < 0)	
		{
			m_velocity.setX(3 * TheInputHandler::Instance()->xvalue(0,1));	
		}
		// eje1, Y
		if (TheInputHandler::Instance()->yvalue(0, 1) > 0 || TheInputHandler::Instance()->yvalue(0, 1) < 0)	
		{
			m_velocity.setY(3 * TheInputHandler::Instance()->yvalue(0,1));		
		}
		//// eje 2, X     
		//if (TheInputHandler::Instance()->xvalue(0, 2) > 0 || TheInputHandler::Instance()->xvalue(0, 2) < 0)	
		//{
		//	m_velocity.setX(3 * TheInputHandler::Instance()->xvalue(0,2));		                                    ////   triggers del joystick
		//}
		//// eje 2, Y
		//if (TheInputHandler::Instance()->yvalue(0, 2) > 0 || TheInputHandler::Instance()->yvalue(0, 2) < 0)	
		//{
		//	m_velocity.setY(3 * TheInputHandler::Instance()->yvalue(0,2));		
		//}

// evento boton joystick

		/*if(TheInputHandler::Instance()->getButtonState(0, 8))     // pausa en el joystick, no hay forma de volver al juego desdel el menu con el joystick
		{
		    TheGame::Instance()->getStateMachine()->pushState(new PauseState());
		}*/

		if (TheInputHandler::Instance()->getButtonState(0, 0))  // (joy id, nr boton) xbox 0 = A,   1 = B,    2 = X,    3 = Y ...
		{
			if (m_bulletCounter == m_bulletFiringSpeed)
			{
				//TheSoundManager::Instance()->playSound("shoot", 0);
				TheBulletHandler::Instance()->addPlayerBullet(m_position.getX() + 90, m_position.getY() + 12, 11, 11, "bullet1", 1, Vector2D(10, 0));
				m_bulletCounter = 0;
			}
			m_bulletCounter++;
		}
		/*else
		{
			m_bulletCounter = m_bulletFiringSpeed;
		}*/


		if (TheInputHandler::Instance()->getButtonState(0, 2))  // (joy id, nr boton) xbox 0 = A,    1 = B,    2 = X,    3 = Y , 4 =LB
		{
			if (m_bombCounter == m_bulletFiringSpeed * 4)
			{
				TheBulletHandler::Instance()->addPlayerBullet(m_position.getX() + 90, m_position.getY() + 12, 33, 33, "bomb", 1, Vector2D(m_velocity.getX() / 2, 3));
				m_bombCounter = 0;
			}
			m_bombCounter++;
		}
		/*else
		{
			m_bombCounter = m_bulletFiringSpeed*4;
		}*/
	}
}


