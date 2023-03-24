#pragma once
#include "../ECS/Engine.h"
#include "../ECS/System.h"
#include "../Pong/TransformComponent.h"
#include "../Pong/DirectionComponent.h"
#include "../Pong/SpeedComponent.h"
#include "../SolarSystem/CameraComponent.h"
#include "../SolarSystem/RotationSpeedComponent.h"
#include "../SolarSystem/RotationComponent.h"

class KatamariControllerSystem : public System
{
private:
	bool isFirst = true;

public:
	KatamariControllerSystem(Engine* _engine) : System(_engine) {}

	void Update(float dt) override
	{
		for (auto const& entity : mEntities)
		{
			auto& direction = engine->GetComponent<DirectionComponent>(entity).direction;
			auto& rotation = engine->GetComponent<RotationComponent>(entity);
			auto& speed = engine->GetComponent<SpeedComponent>(entity);
			const auto rotSpeed = engine->GetComponent<RotationSpeedComponent>(entity).currentSpeed;
			auto& transform = engine->GetComponent<TransformComponent>(entity);

			auto ks = engine->GetGraphicsEngine()->GetKeyboardState();

			if (ks.IsKeyDown(Keys::kW)) direction.x = 1;
			if (ks.IsKeyDown(Keys::kS))	direction.x = -1;
			if (ks.IsKeyDown(Keys::kA))	direction.z = -1;
			if (ks.IsKeyDown(Keys::kD))	direction.z = 1;
			if (ks.IsKeyDown(Keys::kSpace))	direction.y = 1;
			if (ks.IsKeyDown(Keys::kControl)) direction.y = -1;

			auto mousePosition = engine->GetGraphicsEngine()->GetMouseState()->PopMousePositionInfo();

			if (isFirst)
			{
				mousePosition.dx = 0;
				mousePosition.dy = 0;
				isFirst = false;
			}

			if (!ks.IsKeyDown(Keys::kShift))
			{
				rotation.yaw -= mousePosition.dx * 0.003f * rotSpeed;
			}

			auto rotationMatrix = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(rotation.yaw, rotation.pitch, 0);
			transform.SetRotation(engine, DirectX::SimpleMath::Quaternion::CreateFromRotationMatrix(rotationMatrix));
		}
	}
};