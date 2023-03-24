#pragma once
#include "../ECS/Engine.h"
#include "../ECS/System.h"
#include "DirectionComponent.h"
#include "ItemTypeComponent.h"

class RacketDirectionCleanerSystem : public System
{
public:
	RacketDirectionCleanerSystem(Engine* _engine) : System(_engine) {}

	void Update(float dt) override
	{
		for (auto const& entity : mEntities)
		{
			auto& direction = engine->GetComponent<DirectionComponent>(entity).direction;
			const auto type = engine->GetComponent<ItemTypeComponent>(entity).type;

			if (type != ItemType::Player) continue;

			direction = {};
		}
	}
};