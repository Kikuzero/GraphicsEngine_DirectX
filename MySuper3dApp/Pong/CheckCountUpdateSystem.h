#pragma once
#include "../ECS/Engine.h"
#include "../ECS/System.h"
#include "CustomVectorMath.h"
#include "PhysicsComponent.h"
#include "TransformComponent.h"
#include "DirectionComponent.h"
#include "ItemTypeComponent.h"
#include "CountUploadedComponent.h"

class CheckCountUpdateSystem : public System
{
public:
	CheckCountUpdateSystem(Engine* _engine) : System(_engine) {}

	void Update(float dt) override
	{
		auto ball = engine->FindEntityByName("Ball");
		const auto entity = *mEntities.begin();

		auto& transformComponent = engine->GetComponent<TransformComponent>(ball);
		auto& directionComponent = engine->GetComponent<DirectionComponent>(ball);
		auto& speed = engine->GetComponent<SpeedComponent>(ball);
		auto& countUploadedComponent = engine->GetComponent<CountUploadedComponent>(entity);
		auto& isGameManagerComponent = engine->GetComponent<IsGameManagerComponent>(entity);

		if (countUploadedComponent.uploaded)
		{
			transformComponent.localPosition = {};
			speed.ToDefault();

			switch (countUploadedComponent.side)
			{
			case SideUploader::Left:
				isGameManagerComponent.count1++;
				directionComponent.direction = { 1.f, 0.f, 0.f };
				break;
			case SideUploader::Right:
				isGameManagerComponent.count2++;
				directionComponent.direction = { -1.f, 0.f, 0.f };
				break;
			default:
				break;
			}

			std::cout << "Count is: " << isGameManagerComponent.count1 << " : " << isGameManagerComponent.count2 << "\n";

			countUploadedComponent = { .side = SideUploader::None, .uploaded = false };
		}
	}
};