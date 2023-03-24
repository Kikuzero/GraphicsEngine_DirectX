#pragma once
#include <set>
#include "Entity.h"

class Engine;

class System
{
protected:
	Engine* engine;

public:
	System(Engine* _engine) : engine(_engine) {}

	std::set<Entity> mEntities;

	virtual void Update(float dt) = 0;
};