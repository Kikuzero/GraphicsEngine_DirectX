#pragma once
#include "../ECS/Engine.h"
#include "../ECS/System.h"
#include "../Pong/RenderComponent.h"
#include "../Pong/TransformComponent.h"
#include "../Pong/Material.h"
#include "../Fong/DirectionLightData.h"
#include "../Fong/DirectionalLightComponent.h"
#include "../SolarSystem/CameraComponent.h"

class LinesRenderer : public System
{
public:
	LinesRenderer(Engine* _engine) : System(_engine) {}

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

			graphicsEngine->SetupDrawTopology(ge.topology);

			graphicsEngine->SetupIndexBuffer(ge.ib);
			graphicsEngine->SetupVertexBuffer(ge.vb);

			auto test1 = ConstMatrixBuffer{
				model.Transpose(),
				mvp.Transpose(),
				model.Transpose().Invert().Transpose()
			};

			graphicsEngine->SetupConstBuffer(ge.cb, 0);

			//graphicsEngine->UpdateConstantBuffer<ConstMatrixBuffer>(ge.cb->GetBuffer(), test1);
			//graphicsEngine->UpdateConstantBuffer<Material>(ge.mcb->GetBuffer(), test2);
			//graphicsEngine->UpdateConstantBuffer<DirectionLightData>(ge.lcb->GetBuffer(), test3);

			graphicsEngine->GetContext()->UpdateSubresource(ge.cb->GetBuffer().Get(), 0, nullptr, &test1, 0, 0);

			graphicsEngine->Draw(ge.ib->GetSize());
		}
	}
};