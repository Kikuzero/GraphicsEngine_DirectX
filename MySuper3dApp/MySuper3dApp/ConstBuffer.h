#pragma once
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
#include <vector>

class ConstBuffer
{
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;

	D3D11_BUFFER_DESC constBufDesc;

	size_t arraySize;

public:
	template<typename T>
	void Init()
	{
		constBufDesc = {};
		constBufDesc.Usage = D3D11_USAGE_DEFAULT;

		constBufDesc.CPUAccessFlags = 0;
		constBufDesc.MiscFlags = 0;
		constBufDesc.StructureByteStride = 0;
		constBufDesc.ByteWidth = sizeof(T);
		constBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	}

	Microsoft::WRL::ComPtr<ID3D11Buffer> Bind(Microsoft::WRL::ComPtr<ID3D11Device> _device)
	{
		_device->CreateBuffer(&constBufDesc, nullptr, buffer.GetAddressOf());

		return buffer;
	}

	Microsoft::WRL::ComPtr<ID3D11Buffer> GetBuffer()
	{
		return buffer;
	}
};