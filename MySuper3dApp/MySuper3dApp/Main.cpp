#include <windows.h>
#include <WinUser.h>
#include <wrl.h>
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <chrono>
#include <iostream>
#include <memory>
#include "Window.h";
#include "MShader.h"
#include "GraphicsEngine.h"
#include "Layout.h"

#pragma region DirectX_Comments

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

#pragma endregion

#pragma region Variables

int width = 800;
int height = 600;

#pragma endregion

int main()
{
	std::vector<const char*> _inputElements = { "POSITION", "COLOR" };
	DirectX::XMFLOAT4 points[8] = {
		DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(-0.5f, -0.5f, 0.5f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(0.5f, -0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(-0.5f, 0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
	};
	int indeces[] = { 0,1,2, 1,0,3 };


	GraphicsEngine ge(width, height);

	ge.CreatePixelShader(L"../../Shaders/MyVeryFirstShader.hlsl");
	ge.CreateVertexShader(L"../../Shaders/MyVeryFirstShader.hlsl");

	ge.BindVertexShader(_inputElements);
	Microsoft::WRL::ComPtr<ID3D11Buffer> _vb = ge.CreateVertexBuffer<DirectX::XMFLOAT4>(points, 8);
	Microsoft::WRL::ComPtr<ID3D11Buffer> _ib = ge.CreateIndexBuffer(indeces, 6);

	//MSG msg = {};
	//bool isExitRequested = false;
	//while (!isExitRequested) {
	//	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
	//		TranslateMessage(&msg);
	//		DispatchMessage(&msg);
	//	}

	//	if (msg.message == WM_QUIT) {
	//		isExitRequested = true;
	//	}
	//	ge.Render(_vb, _ib);
	//}
}
