#include "../ECS/Engine.h"
#include "../MySuper3dApp/GraphicsEngine.h"
#include "InitHelper.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

int main()
{
	GraphicsEngine ge(800, 800);
	Engine engine;
	engine.Init(ge);

	std::vector<const char*> inputElements = { "POSITION", "COLOR" };
	ge.CreatePixelShader(L"../Shaders/MyVeryFirstShader.hlsl");
	ge.CreateVertexShader(L"../Shaders/MyVeryFirstShader.hlsl");
	ge.BindVertexShader(inputElements);

	const auto circleRadius = 0.03f;
	const auto quadWidth = 0.05f;
	const auto quadHeight = 0.2f;
	const auto rocketSpeed = 10.f;
	const auto ballSpeed = 7.f;

	InitComponents(engine);
	InitSystems(engine);
	CreateField(engine, ge);

	Entity racket1 = CreatePlayer1(engine, ge, quadWidth, quadHeight, rocketSpeed);
	Entity racket2 = CreatePlayer2(engine, ge, quadWidth, quadHeight, rocketSpeed);
	Entity ball = CreateBall(engine, ge, circleRadius, ballSpeed);

	engine.Run();
}