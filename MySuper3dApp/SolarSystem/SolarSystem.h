#pragma once
#include "../ECS/Engine.h"
#include "../ECS/System.h"
#include "../Pong/TransformComponent.h"
#include "RotatorComponent.h"

class SolarSystem : public System
{
public:
	SolarSystem(Engine* _engine) : System(_engine) {}

	void Update(float dt) override
	{
		for (auto const& entity : mEntities)
		{
			auto& transform = engine->GetComponent<TransformComponent>(entity);
			const auto rotator = engine->GetComponent<RotatorComponent>(entity);
			
			auto rotateTo = rotator.vector * rotator.rotationSpeed * dt;		
			transform.localRotation *= DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(rotateTo);
		}
	}
};