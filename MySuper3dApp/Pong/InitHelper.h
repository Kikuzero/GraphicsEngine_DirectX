#pragma once
#include "../ECS/Engine.h"
#include "../MySuper3dApp/GraphicsEngine.h"
#include "../MySuper3dApp/ObjectGenerator.h"
#include "RenderComponent.h"
#include "TransformComponent.h"
#include "RenderSystem.h"
#include "AfterRenderSystem.h"
#include "PreRenderSystem.h"
#include "PhysicsSystem.h"
#include "KeyComponent.h"
#include "PhysicsComponent.h"
#include "DirectionComponent.h"
#include "Player1ControllerSystem.h"
#include "Player2ControllerSystem.h"
#include "RacketDirectionCleanerSystem.h"
#include "SpeedComponent.h"
#include "IsBallComponent.h"
#include "IsFirstRacketComponent.h"
#include "IsSecondRacketComponent.h"
#include "ItemTypeComponent.h"
#include "IsGameManagerComponent.h"
#include "CountUploadedComponent.h"
#include "MovementSystem.h"
#include "CheckCountUpdateSystem.h"
#include "../MySuper3dApp/TransitionConstBuffer.h"

void InitComponents(Engine& _engine)
{
	_engine.RegisterComponent<
		RenderComponent,
		TransformComponent,
		KeyComponent,
		SpeedComponent,
		IsBallComponent,
		IsFirstRacketComponent,
		IsSecondRacketComponent,
		DirectionComponent,
		PhysicsComponent,
		ItemTypeComponent,
		IsGameManagerComponent,
		CountUploadedComponent
	>();
}

void InitSystems(Engine& _engine)
{
	_engine.RegisterSystem<Player1ControllerSystem>()
		->AddComponentsToSystemNode< Player1ControllerSystem,
		IsFirstRacketComponent,
		SpeedComponent,
		DirectionComponent
		>();

	_engine.RegisterSystem<Player2ControllerSystem>()
		->AddComponentsToSystemNode< Player2ControllerSystem,
		IsSecondRacketComponent,
		SpeedComponent,
		DirectionComponent
		>();

	_engine.RegisterSystem<PhysicsSystem>()
		->AddComponentsToSystemNode< PhysicsSystem,
		DirectionComponent,
		TransformComponent,
		PhysicsComponent,
		ItemTypeComponent
		>();

	_engine.RegisterSystem<MovementSystem>()
		->AddComponentsToSystemNode< MovementSystem,
		SpeedComponent,
		DirectionComponent,
		TransformComponent,
		ItemTypeComponent
		>();

	_engine.RegisterSystem<PreRenderSystem>(SystemTypes::RuntimeUnfixed)
		->AddComponentsToSystemNode< PreRenderSystem,
		RenderComponent
		>();

	_engine.RegisterSystem<RenderSystem>(SystemTypes::RuntimeUnfixed)
		->AddComponentsToSystemNode< RenderSystem,
		RenderComponent,
		TransformComponent
		>();

	_engine.RegisterSystem<AfterRenderSystem>(SystemTypes::RuntimeUnfixed)
		->AddComponentsToSystemNode< AfterRenderSystem,
		RenderComponent
		>();

	_engine.RegisterSystem<CheckCountUpdateSystem>(SystemTypes::RuntimeUnfixed)
		->AddComponentsToSystemNode< CheckCountUpdateSystem,
		IsGameManagerComponent,
		CountUploadedComponent
		>();
}

Entity CreatePlayer1(Engine& _engine, GraphicsEngine& _ge, float _width, float _height, float _speed)
{
	auto quad = GenerateColoredPtsRectangle(0, 0, _width, _height, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

	auto device = _ge.GetDevice();

	auto vertexBuffer = new VertexBuffer();
	vertexBuffer->Init<DirectX::XMFLOAT4>(quad.points, quad.pointsSize);
	vertexBuffer->Bind(device);

	auto indexBuffer = new IndexBuffer();
	indexBuffer->Init(quad.indeces, quad.indecesSize);
	indexBuffer->Bind(device);

	auto constBuffer = new ConstBuffer();
	constBuffer->Init<TransitionConstBuffer>();
	constBuffer->Bind(device);

	Entity racket = _engine.CreateEntity("Racket1");
	_engine.AddComponents(racket,
		IsFirstRacketComponent{},
		KeyComponent{ .pressedKeys = {} },
		SpeedComponent(_speed),
		TransformComponent{ .localPosition = DirectX::SimpleMath::Vector3(0.8f, 0.f, 0.f),
							.localRotation = DirectX::SimpleMath::Quaternion(),
							.localScale = DirectX::SimpleMath::Vector3(1.f,1.f,1.f) },
		RenderComponent(& _ge, vertexBuffer, indexBuffer, constBuffer),
		DirectionComponent{},
		PhysicsComponent{ .type = FormType::Quadrangle, .width = _width, .height = _height, .radius = -1, .normalVector = {-1.f, 0.f, 0.f} },
		ItemTypeComponent{ .type = ItemType::Player }
	);

	return racket;
}

Entity CreatePlayer2(Engine& _engine, GraphicsEngine& _ge, float _width, float _height, float _speed)
{
	auto quad = GenerateColoredPtsRectangle(0, 0, _width, _height, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

	auto device = _ge.GetDevice();

	auto vertexBuffer = new VertexBuffer();
	vertexBuffer->Init<DirectX::XMFLOAT4>(quad.points, quad.pointsSize);
	vertexBuffer->Bind(device);

	auto indexBuffer = new IndexBuffer();
	indexBuffer->Init(quad.indeces, quad.indecesSize);
	indexBuffer->Bind(device);

	auto constBuffer = new ConstBuffer();
	constBuffer->Init<TransitionConstBuffer>();
	constBuffer->Bind(device);

	Entity racket = _engine.CreateEntity("Racket2");
	_engine.AddComponents(racket,
		IsSecondRacketComponent{},
		KeyComponent{ .pressedKeys = {} },
		SpeedComponent(_speed),
		TransformComponent{ .localPosition = DirectX::SimpleMath::Vector3(-0.8f, 0.f, 0.f),
							.localRotation = DirectX::SimpleMath::Quaternion(),
							.localScale = DirectX::SimpleMath::Vector3(1.f,1.f,1.f) },
		RenderComponent(&_ge, vertexBuffer, indexBuffer, constBuffer),
		DirectionComponent{},
		PhysicsComponent{ .type = FormType::Quadrangle, .width = _width, .height = _height, .radius = -1, .normalVector = {1.f, 0.f, 0.f} },
		ItemTypeComponent{ .type = ItemType::Player }
	);

	return racket;
}

Entity CreateBall(Engine& _engine, GraphicsEngine& _ge, float _radius = 0.03f, float _speed = 10.f)
{
	auto circle = GenerateColoredPtsCircle(0, 0, _radius, 32, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

	auto device = _ge.GetDevice();

	auto vertexBuffer = new VertexBuffer();
	vertexBuffer->Init<DirectX::XMFLOAT4>(circle.points, circle.pointsSize);
	vertexBuffer->Bind(device);

	auto indexBuffer = new IndexBuffer();
	indexBuffer->Init(circle.indeces, circle.indecesSize);
	indexBuffer->Bind(device);

	auto constBuffer = new ConstBuffer();
	constBuffer->Init<TransitionConstBuffer>();
	constBuffer->Bind(device);

	Entity ball = _engine.CreateEntity(_engine.EntityNameCreater("Ball"));
	_engine.AddComponents(ball,
		IsBallComponent{},
		SpeedComponent(_speed),
		TransformComponent{ .localPosition = DirectX::SimpleMath::Vector3(0.f, 0.f,0.f),
							.localRotation = DirectX::SimpleMath::Quaternion(),
							.localScale = DirectX::SimpleMath::Vector3(1.f,1.f,1.f) },
		RenderComponent(&_ge, vertexBuffer, indexBuffer, constBuffer),
		DirectionComponent{ .direction = { 1.f, 0.f, 0.f} },
		PhysicsComponent{ .type = FormType::Circle, .width = -1, .height = -1, .radius = _radius, .normalVector = {} },
		ItemTypeComponent{ .type = ItemType::Ball }
	);

	return ball;
}

void CreateWalls(Engine& _engine, GraphicsEngine& _ge)
{
	const auto filedWidth = 0.825f * 2.f;
	const auto filedHeight = 0.2f;

	auto quadField = GenerateColoredPtsRectangle(0, 0, filedWidth, filedHeight, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

	auto device = _ge.GetDevice();

	auto vertexBuffer = new VertexBuffer();
	vertexBuffer->Init<DirectX::XMFLOAT4>(quadField.points, quadField.pointsSize);
	vertexBuffer->Bind(device);

	auto indexBuffer = new IndexBuffer();
	indexBuffer->Init(quadField.indeces, quadField.indecesSize);
	indexBuffer->Bind(device);

	auto constBuffer_1 = new ConstBuffer();
	constBuffer_1->Init<TransitionConstBuffer>();
	constBuffer_1->Bind(device);
	auto constBuffer_2 = new ConstBuffer();
	constBuffer_2->Init<TransitionConstBuffer>();
	constBuffer_2->Bind(device);

	Entity filedUpper = _engine.CreateEntity("WallUp");
	_engine.AddComponents(filedUpper,
		TransformComponent{ .localPosition = DirectX::SimpleMath::Vector3(0.f, 1.f, 0.f),
							.localRotation = DirectX::SimpleMath::Quaternion(),
							.localScale = DirectX::SimpleMath::Vector3(1.f,1.f,1.f) },
		RenderComponent(&_ge, vertexBuffer, indexBuffer, constBuffer_1),
		DirectionComponent{},
		PhysicsComponent{ .type = FormType::Quadrangle, .width = filedWidth, .height = filedHeight, .radius = -1, .normalVector = {0.f, -1.f, 0.f} },
		ItemTypeComponent{ .type = ItemType::Wall }
	);

	Entity filedLower = _engine.CreateEntity("WallDown");
	_engine.AddComponents(filedLower,
		TransformComponent{ .localPosition = DirectX::SimpleMath::Vector3(0.f, -.9f, 0.f),
							.localRotation = DirectX::SimpleMath::Quaternion(),
							.localScale = DirectX::SimpleMath::Vector3(1.f,1.f,1.f) },
		RenderComponent(&_ge, vertexBuffer, indexBuffer, constBuffer_2),
		DirectionComponent{},
		PhysicsComponent{ .type = FormType::Quadrangle, .width = filedWidth, .height = filedHeight, .radius = -1, .normalVector = {0.f, 1.f, 0.f} },
		ItemTypeComponent{ .type = ItemType::Wall }
	);
}

void CreateGoals(Engine& _engine, GraphicsEngine& _ge)
{
	const auto filedHeight = 0.9f * 2.f;
	const auto filedWidth = 0.2f;

	const auto sideOffset = 0.91f;

	auto quadGoal = GenerateColoredPtsRectangle(0, 0, filedWidth, filedHeight, DirectX::XMFLOAT4(0.0f, .0f, .0f, 0.0f));

	auto device = _ge.GetDevice();

	auto vertexBuffer = new VertexBuffer();
	vertexBuffer->Init<DirectX::XMFLOAT4>(quadGoal.points, quadGoal.pointsSize);
	vertexBuffer->Bind(device);

	auto indexBuffer = new IndexBuffer();
	indexBuffer->Init(quadGoal.indeces, quadGoal.indecesSize);
	indexBuffer->Bind(device);

	auto constBuffer_1 = new ConstBuffer();
	constBuffer_1->Init<TransitionConstBuffer>();
	constBuffer_1->Bind(device);
	auto constBuffer_2 = new ConstBuffer();
	constBuffer_2->Init<TransitionConstBuffer>();
	constBuffer_2->Bind(device);

	Entity goalLeft = _engine.CreateEntity("GoalLeft");
	_engine.AddComponents(goalLeft,
		TransformComponent{ .localPosition = DirectX::SimpleMath::Vector3(-sideOffset, 0.f, 0.f),
							.localRotation = DirectX::SimpleMath::Quaternion(),
							.localScale = DirectX::SimpleMath::Vector3(1.f,1.f,1.f) },
		RenderComponent(&_ge, vertexBuffer, indexBuffer, constBuffer_1),
		DirectionComponent{},
		PhysicsComponent{ .type = FormType::Quadrangle, .width = filedWidth, .height = filedHeight, .radius = -1, .normalVector = {} },
		ItemTypeComponent{ .type = ItemType::Goal }
	);

	Entity goalRight = _engine.CreateEntity("GoalRight");
	_engine.AddComponents(goalRight,
		TransformComponent{ .localPosition = DirectX::SimpleMath::Vector3(sideOffset, 0.f, 0.f),
							.localRotation = DirectX::SimpleMath::Quaternion(),
							.localScale = DirectX::SimpleMath::Vector3(1.f,1.f,1.f) },
		RenderComponent(&_ge, vertexBuffer, indexBuffer, constBuffer_2),
		DirectionComponent{},
		PhysicsComponent{ .type = FormType::Quadrangle, .width = filedWidth, .height = filedHeight, .radius = -1, .normalVector = {} },
		ItemTypeComponent{ .type = ItemType::Goal }
	);

	Entity gameManager = _engine.CreateEntity("GameManager");
	_engine.AddComponents(gameManager,
		IsGameManagerComponent{ .count1 = 0, .count2 = 0 },
		CountUploadedComponent{ .side = SideUploader::None, .uploaded = false}
	);
}

void CreateField(Engine& _engine, GraphicsEngine& _ge)
{
	CreateGoals(_engine, _ge);
	CreateWalls(_engine, _ge);
}