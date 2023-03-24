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

class VertexBuffer
{
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;

	D3D11_BUFFER_DESC vertexBufDesc;
	D3D11_SUBRESOURCE_DATA vertexData;

	std::vector<UINT> strides;
	std::vector<UINT> offsets;

	size_t size;

public:
	template<typename T>
	void Init(const void* _array, const size_t _size)
	{
		vertexBufDesc = {};
		vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;

		vertexBufDesc.CPUAccessFlags = 0;
		vertexBufDesc.MiscFlags = 0;
		vertexBufDesc.StructureByteStride = 0;
		vertexBufDesc.ByteWidth = sizeof(T) * _size;
		vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		vertexData = {};
		vertexData.pSysMem = _array;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		size = _size;
	}

	Microsoft::WRL::ComPtr<ID3D11Buffer> Bind(Microsoft::WRL::ComPtr<ID3D11Device> _device)
	{
		_device->CreateBuffer(&vertexBufDesc, &vertexData, buffer.GetAddressOf());

		return buffer;
	}

	Microsoft::WRL::ComPtr<ID3D11Buffer> GetBuffer()
	{
		return buffer;
	}

	std::vector<UINT> GetStrides()
	{
		return strides;
	}

	std::vector<UINT> GetOffsets()
	{
		return offsets;
	}

	size_t GetSize()
	{
		return size;
	}

	void AppendStrides(UINT _stride)
	{
		strides.push_back(_stride);
	}

	void AppendOffsets(UINT _offsets)
	{
		offsets.push_back(_offsets);
	}

	void SetStrides(std::vector<UINT> _strides)
	{
		strides = _strides;
	}

	void SetOffsets(std::vector<UINT> _offsets)
	{
		offsets = _offsets;
	}
};