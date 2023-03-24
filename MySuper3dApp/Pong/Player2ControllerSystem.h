#pragma once
#include <windows.h>
#include <iostream>
#include "../ECS/Engine.h"
#include "../ECS/System.h"
#include "DirectionComponent.h"
#include <iostream>

class Player2ControllerSystem : public System
{
public:
	Player2ControllerSystem(Engine* _engine) : System(_engine) {}

	void Update(float dt) override
	{
		for (auto const& entity : mEntities)
		{
			auto& direction = engine->GetComponent<DirectionComponent>(entity).direction;

			auto ks = engine->GetGraphicsEngine()->GetKeyboardState();
			if (ks.IsKeyDown(Keys::kW)) direction.y = 1;
			if (ks.IsKeyDown(Keys::kS)) direction.y = -1;
		}
	}
};