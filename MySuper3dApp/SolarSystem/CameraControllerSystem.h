#pragma once
#include "../ECS/Engine.h"
#include "../ECS/System.h"
#include "../Pong/TransformComponent.h"
#include "../Pong/DirectionComponent.h"
#include "../Pong/SpeedComponent.h"
#include "CameraComponent.h"
#include "RotationSpeedComponent.h"
#include "RotationComponent.h"

class CameraControllerSystem : public System
{
private:
	bool isFirst = true;

public:
	CameraControllerSystem(Engine* _engine) : System(_engine) {}

	// TODO рефактор...
	void Update(float dt) override
	{
		for (auto const& entity : mEntities)
		{
			auto& direction = engine->GetComponent<DirectionComponent>(entity).direction;
			auto& camera = engine->GetComponent<CameraComponent>(entity);
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
			auto wheelInfo = engine->GetGraphicsEngine()->GetMouseState()->GetWheelState();

			if (wheelInfo == WheelState::Down) speed.Down(1);
			if (wheelInfo == WheelState::Up) speed.Up(1);

			// Необходимое зло... :(
			if (isFirst)
			{
				mousePosition.dx = 0;
				mousePosition.dy = 0;
				isFirst = false;
			}

			if (!ks.IsKeyDown(Keys::kShift))
			{
				rotation.yaw -= mousePosition.dx * 0.003f * rotSpeed;
				rotation.pitch -= mousePosition.dy * 0.003f * rotSpeed;
			}

			auto rotationMatrix = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(rotation.yaw, rotation.pitch, 0);
			auto velDir = rotationMatrix.Forward() * direction.x +
				DirectX::SimpleMath::Vector3::Up * direction.y +
				rotationMatrix.Right() * direction.z;

			if (velDir.Length() != 0) velDir.Normalize();

			transform.localPosition += velDir * speed.Get() * dt;

			camera.viewMatrix = DirectX::SimpleMath::Matrix::CreateLookAt(
				transform.localPosition,
				transform.localPosition + rotationMatrix.Forward(),
				rotationMatrix.Up());

			camera.projMatrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
				DirectX::XM_PIDIV2,
				engine->GetGraphicsEngine()->GetWindow().GetAspectRatio(),
				0.01f,
				1000.0f);

			direction = {};
		}
	}
};