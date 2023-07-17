#pragma once
#include <string>
#include "ShooterObject.h"
#include "GameObjectFactory.h"

class Enemy : public ShooterObject
{	
public:

	virtual std::string type() { return"Enemy"; }

protected:

	int m_health;

	Enemy() : ShooterObject() {}

	virtual ~Enemy() {} // for polymorphism
};


//
//class EnemyCreator : public BaseCreator
//{
//	GameObject * createGameObject() const
//	{
//		return new Enemy();
//	}
//};