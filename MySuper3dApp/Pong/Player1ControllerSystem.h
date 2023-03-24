#pragma once
#include <windows.h>
#include <iostream>
#include "../ECS/Engine.h"
#include "../ECS/System.h"
#include "DirectionComponent.h"
#include <iostream>

class Player1ControllerSystem : public System
{
public:
	Player1ControllerSystem(Engine* _engine) : System(_engine) {}

	void Update(float dt) override
	{
		for (auto const& entity : mEntities)
		{
			auto& direction = engine->GetComponent<DirectionComponent>(entity).direction;

			auto ks = engine->GetGraphicsEngine()->GetKeyboardState();
			if (ks.IsKeyDown(Keys::kUp)) direction.y = 1;
			if (ks.IsKeyDown(Keys::kDown)) direction.y = -1;
		}
	}
};