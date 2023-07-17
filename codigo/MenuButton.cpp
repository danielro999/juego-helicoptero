#include "MenuButton.h"

MenuButton::MenuButton() : ShooterObject()
{ 
	m_currentFrame = MOUSE_OUT; // start at frame 0
}

void MenuButton::draw()
{
	ShooterObject::draw(); // use the base class drawing
}
void MenuButton::update()
{
	Vector2D* pMousePos = TheInputHandler::Instance()->getMousePosition();  // pMousePos array con la posision del mouse
	
	//chequeo si el mouse esta sobre el boton	//la posicion en pantalla cuenta desde iz a der de arriba hacia abajo
	if (pMousePos->getX() < (m_position.getX() + m_width) // si el mouse esta en una posicion menor que la pos derecha mas en ancho del boton (x position + width)
		&& pMousePos->getX() > m_position.getX()           // y si el mouse esta en una posision mayor que la pos izq del boton (x position )
		&& pMousePos->getY() < (m_position.getY() + m_height)// lo mismo pero con la pos Y (arriba - abajo)
		&& pMousePos->getY() > m_position.getY())            // 
	// si es true el mouse esta sobre el boton
	{
		if (TheInputHandler::Instance()->getMouseButtonState(LEFT)&& m_bReleased)		
		{
			m_currentFrame = CLICKED;                    // si fue cliqueado se setea  CLICKED(2) 
			m_callback(); // call our callback function
			m_bReleased = false;                          // m_bReleased falso que el boton esta "soltado, liberado"
		}
		else if (!TheInputHandler::Instance()->getMouseButtonState(LEFT))	
		{
			m_bReleased = true;                       // m_bReleased el boton "se ha soltado, liberado"
			m_currentFrame = MOUSE_OVER;             // MOUSE_OVER (1) el mouse sobre el frame boton, enum button_state
		}
	}
	else
	{
		m_currentFrame = MOUSE_OUT;     // si no el mouse no esta sobre el boton y se setea a MOUSE_OUT(0)
	}
}

void MenuButton::clean()
{
	ShooterObject::clean();
}

void MenuButton::load(std::unique_ptr<LoaderParams> const& pParams)
{
	ShooterObject::load(pParams);
	m_callbackID = pParams->getCallbackID();
	m_currentFrame = MOUSE_OUT;
}
