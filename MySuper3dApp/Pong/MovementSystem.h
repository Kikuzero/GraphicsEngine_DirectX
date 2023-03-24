#pragma once
#include <cmath>
#include "../ECS/Engine.h"
#include "../ECS/System.h"
#include "TransformComponent.h"
#include "DirectionComponent.h"
#include "SpeedComponent.h"
#include "ItemTypeComponent.h"

class MovementSystem : public System
{
public:
	MovementSystem(Engine* _engine) : System(_engine) {}

	void Update(float dt) override
	{
		for (auto const& entity : mEntities)
		{
			auto& transform = engine->GetComponent<TransformComponent>(entity);
			auto& direction = engine->GetComponent<DirectionComponent>(entity).direction;
			const auto speed = engine->GetComponent<SpeedComponent>(entity).Get();
			const auto type = engine->GetComponent<ItemTypeComponent>(entity).type;

			transform.localPosition.x += direction.x * speed * dt;
			transform.localPosition.y += direction.y * speed * dt;
			transform.localPosition.z += direction.z * speed * dt;

			if (type != ItemType::Player) continue;
			direction = {};
		}
	}
};