#pragma once
#include "../ECS/Engine.h"
#include "../ECS/System.h"
#include "../Pong/TransformComponent.h"
#include "../Pong/DirectionComponent.h"

class PhysicsSystem : public System
{
public:
	PhysicsSystem(Engine* _engine) : System(_engine) {}

	void Update(float dt) override
	{
		for (auto const& entity : mEntities)
		{

		}
	}
};