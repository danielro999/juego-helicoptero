#pragma once

class Level;
class Layer
{
public:
	virtual void render() = 0;
	//virtual void update() = 0;
	virtual void update(Level* pLevel) = 0;
protected:
	virtual ~Layer() {}
};