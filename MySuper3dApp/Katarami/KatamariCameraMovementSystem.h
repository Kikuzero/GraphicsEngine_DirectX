#pragma once
#include "../ECS/Engine.h"
#include "../ECS/System.h"
#include "../Pong/TransformComponent.h"
#include "../Pong/DirectionComponent.h"
#include "../Pong/SpeedComponent.h"
#include "../SolarSystem/CameraComponent.h"
#include "../SolarSystem/RotationSpeedComponent.h"

class KatamariCameraMovementSystem : public System
{
public:
	KatamariCameraMovementSystem(Engine* _engine) : System(_engine) {}

	void Update(float dt) override
	{
		for (auto const& entity : mEntities)
		{
			auto& transform = engine->GetComponent<TransformComponent>(entity);
			auto& camera = engine->GetComponent<CameraComponent>(entity);

			const auto playerTransform = engine->GetComponent<TransformComponent>(engine->GetPlayer());

			camera.viewMatrix = DirectX::SimpleMath::Matrix::CreateLookAt(
				transform.localPosition,
				playerTransform.localPosition,
				DirectX::SimpleMath::Vector3::Up);

			camera.projMatrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
				DirectX::XM_PIDIV2,
				engine->GetGraphicsEngine()->GetWindow().GetAspectRatio(),
				0.01f,
				1000.0f);
		}
	}
};