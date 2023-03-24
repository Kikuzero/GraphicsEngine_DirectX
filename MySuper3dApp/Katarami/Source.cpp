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

	std::vector<LayoutCreationDesc> _inputElements = { {"POSITION", LayoutType::float4}, {"TEXCOORD", LayoutType::float2} };
	ge.CreatePixelShader(L"../Shaders/KatamariShader.hlsl");
	ge.CreateVertexShader(L"../Shaders/KatamariShader.hlsl");
	ge.BindVertexShader(_inputElements);

	InitComponents(engine);
	InitSystems(engine);

	CreatePlane(engine, ge);

	auto p1 = CreatePumpkin(engine, ge,
		DirectX::SimpleMath::Vector3(0.f, 0.f, 0.f),
		DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f),
		DirectX::SimpleMath::Vector3(0.f, 1.f, 0.f),
		1.f);

	CreateCamera(engine, ge, p1);

	engine.SetPlayer(p1);
	ge.GetMouseReady();
	engine.Run();

	return 0;
}