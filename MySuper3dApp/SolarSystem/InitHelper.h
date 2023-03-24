#pragma once
#include "../ECS/Engine.h"
#include "../MySuper3dApp/GraphicsEngine.h"
#include "../MySuper3dApp/ModelManager.h"
#include "../Pong/RenderComponent.h"
#include "../Pong/TransformComponent.h"
#include "../Pong/PreRenderSystem.h"
#include "../Pong/AfterRenderSystem.h"
#include "../Pong/DirectionComponent.h"
#include "CameraComponent.h"
#include "RotatorComponent.h"
#include "RotationSpeedComponent.h"
#include "CameraControllerSystem.h"
#include "ProjectionRendererSystem.h"
#include "SolarSystem.h"
#include "IsObjectComponent.h"
#include "RotationComponent.h"

void InitComponents(Engine& _engine)
{
	_engine.RegisterComponent<
		RenderComponent,
		TransformComponent,
		DirectionComponent,
		CameraComponent,
		RotationSpeedComponent,
		SpeedComponent,
		RotatorComponent,
		IsObjectComponent,
		RotationComponent
	>();
}

void InitSystems(Engine& _engine)
{
	_engine.RegisterSystem<CameraControllerSystem>()
		->AddComponentsToSystemNode< CameraControllerSystem,
		TransformComponent,
		DirectionComponent,
		CameraComponent,
		SpeedComponent,
		RotationSpeedComponent,
		RotationComponent
		>();

	_engine.RegisterSystem<SolarSystem>()
		->AddComponentsToSystemNode< SolarSystem,
		TransformComponent,
		RotatorComponent,
		IsObjectComponent
		>();

	_engine.RegisterSystem<PreRenderSystem>(SystemTypes::RuntimeUnfixed)
		->AddComponentsToSystemNode< PreRenderSystem,
		RenderComponent
		>();

	_engine.RegisterSystem<ProjectionRendererSystem>(SystemTypes::RuntimeUnfixed)
		->AddComponentsToSystemNode< ProjectionRendererSystem,
		RenderComponent,
		TransformComponent
		>();

	_engine.RegisterSystem<AfterRenderSystem>(SystemTypes::RuntimeUnfixed)
		->AddComponentsToSystemNode< AfterRenderSystem,
		RenderComponent
		>();
}

// TODO: utils
std::vector<DirectX::XMFLOAT4> ToXMFLOAT4Vec(std::vector<ModelManager::Vertex> _vec, bool isRandomColor = true)
{
	std::vector<DirectX::XMFLOAT4> vec(_vec.size() * 2);

	for (int i = 0; i < _vec.size(); i++)
	{
		vec.at(i * 2) = { _vec.at(i).position.x, _vec.at(i).position.y, _vec.at(i).position.z, 1.0f };

		if (!isRandomColor)
		{
			vec.at(i * 2 + 1) = { 1.f, 1.f, 1.f, 1.f };
			continue;
		}

		vec.at(i * 2 + 1) = {
			i % 3 == 0 ? 1.f : 0.f,
			(i % 2 != 0 && i % 3 && i != 0) ? 1.f : 0.f,
			i % 2 == 0 && i != 0 ? 1.f : 0.f,
			1.f };
	}

	return vec;
}

Entity CreatePumpkin(
	Engine& _engine,
	GraphicsEngine& _ge,
	DirectX::SimpleMath::Vector3 _position = DirectX::SimpleMath::Vector3::Zero,
	DirectX::SimpleMath::Vector3 _scale = DirectX::SimpleMath::Vector3::One,
	DirectX::SimpleMath::Vector3 _rotationVector = DirectX::SimpleMath::Vector3::Zero,
	float _rotationSpeed = 0,
	Entity _parent = -1)
{
	ModelManager mm;
	if (!mm.Load("../Objects/Pumpkin/model.obj")) return -1;

	auto verteces = mm.GetMeshes();

	DirectX::SimpleMath::Vector3 rotVector = DirectX::SimpleMath::Vector3{ 0.f, 1.f, 0.f };
	float rotSpeed = 300.f;

	Entity pumpkin = _engine.CreateEntity(_engine.EntityNameCreater("Pumpkin"));
	_engine.AddComponents(pumpkin,
		TextureComponent{},
		RenderComponent(&_ge, verteces.at(0).vertices, verteces.at(0).indices),
		TransformComponent{
			.localPosition = _position,
			.localRotation = DirectX::SimpleMath::Quaternion(),
			.localScale = _scale,
			.parent = _parent
		},
		RotatorComponent{ .vector = _rotationVector, .rotationSpeed = _rotationSpeed },
		IsObjectComponent{}
	);

	return pumpkin;
}

Entity CreateCamera(Engine& _engine, GraphicsEngine& _ge)
{
	auto cameraMovingSpeed = 10.f;
	auto cameraRotationSpeed = 10.f;

	auto sizes = _ge.GetWindowSizes();

	Entity camera = _engine.CreateEntity(_engine.EntityNameCreater("Camera"));
	_engine.AddComponents(camera,
		SpeedComponent(cameraMovingSpeed),
		RotationSpeedComponent{ .currentSpeed = cameraRotationSpeed, .defaultSpeed = cameraRotationSpeed },
		DirectionComponent{ .direction = DirectX::SimpleMath::Vector3{ 0.f,0.f,0.f } },
		CameraComponent{
			.viewMatrix = DirectX::SimpleMath::Matrix::Identity,
			.projMatrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
				(float)DirectX::XM_PI * 0.35f,
				(float)sizes.width / sizes.height,
				0.1f,
				10000.f)
		},
		RotationComponent{ .yaw = 0, .pitch = 0 },
		TransformComponent{
			.localPosition = DirectX::SimpleMath::Vector3{ 0.f,0.f,0.f },
			.localRotation = DirectX::SimpleMath::Quaternion(),
			.localScale = DirectX::SimpleMath::Vector3{ 0.f,0.f,0.f } }
	);

	return camera;
}

Entity CreatePlane(Engine& _engine, GraphicsEngine& _ge)
{
	ModelManager mm;
	if (!mm.Load("../Objects/Plane/model.obj")) return -1;

	auto verteces = mm.GetMeshes();

	Entity plane = _engine.CreateEntity(_engine.EntityNameCreater("Plane"));
	_engine.AddComponents(plane,
		RenderComponent(&_ge, ToXMFLOAT4Vec(verteces.at(0).vertices, false), verteces.at(0).indices, Topology::Lines),
		TransformComponent{
			.localPosition = DirectX::SimpleMath::Vector3{ 0.f, 0.f, 0.f },
			.localRotation = DirectX::SimpleMath::Quaternion(),
			.localScale = DirectX::SimpleMath::Vector3{ 1.f, 1.f, 1.f }
		},
		IsObjectComponent{}
	);

	return plane;
}