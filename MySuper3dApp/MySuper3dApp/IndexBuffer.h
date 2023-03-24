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

class IndexBuffer
{
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;

	D3D11_BUFFER_DESC indexBufDesc;
	D3D11_SUBRESOURCE_DATA indexData;

	UINT offset;
	DXGI_FORMAT format;

	size_t arraySize;

public:
	void Init(const void* _array, const size_t _arrSize)
	{
		indexBufDesc = {};
		indexBufDesc.Usage = D3D11_USAGE_DEFAULT;

		indexBufDesc.CPUAccessFlags = 0;
		indexBufDesc.MiscFlags = 0;
		indexBufDesc.StructureByteStride = 0;
		indexBufDesc.ByteWidth = sizeof(int) * _arrSize;
		indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		indexData = {};
		indexData.pSysMem = _array;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		arraySize = _arrSize;

		offset = 0;
		format = DXGI_FORMAT_R32_UINT;
	}

	Microsoft::WRL::ComPtr<ID3D11Buffer> Bind(Microsoft::WRL::ComPtr<ID3D11Device> _device)
	{
		_device->CreateBuffer(&indexBufDesc, &indexData, buffer.GetAddressOf());

		return buffer;
	}

	Microsoft::WRL::ComPtr<ID3D11Buffer> GetBuffer()
	{
		return buffer;
	}

	size_t GetSize()
	{
		return arraySize;
	}

	void SetOffset(UINT _offset)
	{
		offset = _offset;
	}

	UINT GetOffset()
	{
		return offset;
	}

	void SetFormat(DXGI_FORMAT _format)
	{
		format = _format;
	}

	DXGI_FORMAT GetFormat()
	{
		return format;
	}
};