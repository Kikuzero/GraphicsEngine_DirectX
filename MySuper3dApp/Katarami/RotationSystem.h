#pragma once
#include "../ECS/Engine.h"
#include "../ECS/System.h"
#include "../Pong/TransformComponent.h"
#include "../Pong/DirectionComponent.h"

class RotationSystem : public System
{
public:
	RotationSystem(Engine* _engine) : System(_engine) {}

	void Update(float dt) override
	{
		for (auto const& entity : mEntities)
		{

		}
	}
};