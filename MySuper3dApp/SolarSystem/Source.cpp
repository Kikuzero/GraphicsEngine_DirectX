#include<memory>
#include "../ECS/Engine.h"
#include "../MySuper3dApp/GraphicsEngine.h"
#include "../MySuper3dApp/ModelManager.h"
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
	ge.SetUpDepthBuffer();

	std::vector<LayoutCreationDesc> _inputElements = { {"POSITION", LayoutType::float4}, {"COLOR", LayoutType::float4} };
	ge.CreatePixelShader(L"../Shaders/FongShader.hlsl");
	ge.CreateVertexShader(L"../Shaders/FongShader.hlsl");
	ge.BindVertexShader(_inputElements);

	InitComponents(engine);
	InitSystems(engine);

	CreatePlane(engine, ge);

	auto p1 = CreatePumpkin(engine, ge,
		DirectX::SimpleMath::Vector3(0, 0, 0),
		DirectX::SimpleMath::Vector3(1, 1, 1),
		DirectX::SimpleMath::Vector3(1.f, 1.f, 0.f),
		20.f);

	CreateCamera(engine, ge);

	engine.Run();

	return 0;
}