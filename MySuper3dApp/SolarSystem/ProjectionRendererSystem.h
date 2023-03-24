#pragma once
#include "../ECS/Engine.h"
#include "../ECS/System.h"
#include "../Pong/RenderComponent.h"
#include "../Pong/TransformComponent.h"
#include "CameraComponent.h"
#include "../Pong/Material.h"
#include "../Fong/DirectionLightData.h"
#include "../Fong/DirectionalLightComponent.h"

class ProjectionRendererSystem : public System
{
public:
	ProjectionRendererSystem(Engine* _engine) : System(_engine) {}

	void Update(float dt) override
	{
		for (auto const& entity : mEntities)
		{
			const auto ge = engine->GetComponent<RenderComponent>(entity);
			auto transform = engine->GetComponent<TransformComponent>(entity);

			auto& graphicsEngine = ge.graphicsEngine;

			const auto camera = engine->FindEntityByName("Camera");
			const auto cameraComponent = engine->GetComponent<CameraComponent>(camera);
			const auto cameraTransforment = engine->GetComponent<TransformComponent>(camera);

			auto model = transform.GetModelMatrix(engine);
			auto mvp = model * cameraComponent.viewMatrix * cameraComponent.projMatrix;

			const auto light = engine->FindEntityByName("DirectionalLight");
			auto lightComponent = engine->GetComponent<DirectionalLightComponent>(light).directionLightData;

			graphicsEngine->SetupDrawTopology(ge.topology);

			graphicsEngine->SetupIndexBuffer(ge.ib);
			graphicsEngine->SetupVertexBuffer(ge.vb);

			auto test1 = ConstMatrixBuffer{
				model.Transpose(),
				mvp.Transpose(),
				model.Transpose().Invert().Transpose()
			};

			auto test2 = Material{0.1, 32., 0.1, 0};

			auto test3 = DirectionLightData{
				DirectX::SimpleMath::Vector4(cameraTransforment.localPosition.x, cameraTransforment.localPosition.y, cameraTransforment.localPosition.z, 1.f),
				lightComponent.lightDirection,
				0,
				lightComponent.lightColor,
				0
			};

			graphicsEngine->SetupConstBuffer(ge.cb, 0);
			graphicsEngine->SetupConstBuffer(ge.mcb, 1);
			graphicsEngine->SetupConstBuffer(ge.lcb, 2);

			graphicsEngine->GetContext()->UpdateSubresource(ge.cb->GetBuffer().Get(), 0, nullptr, &test1, 0, 0);
			graphicsEngine->GetContext()->UpdateSubresource(ge.mcb->GetBuffer().Get(), 0, nullptr, &test2, 0, 0);
			graphicsEngine->GetContext()->UpdateSubresource(ge.lcb->GetBuffer().Get(), 0, nullptr, &test3, 0, 0);

			graphicsEngine->Draw(ge.ib->GetSize());
		}
	}
};