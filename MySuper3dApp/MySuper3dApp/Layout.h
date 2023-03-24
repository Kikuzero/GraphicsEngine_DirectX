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

enum class LayoutType
{
	float4,
	float2
};

struct LayoutCreationDesc
{
	const char* name;
	LayoutType type;
};

class MLayout
{
private:
	ID3D11InputLayout* layout;

public:
	ID3D11InputLayout* Init(
		Microsoft::WRL::ComPtr<ID3D11Device>& device,
		D3D11_INPUT_ELEMENT_DESC* elements,
		int elementsCount,
		void* vertexBufferPointer,
		int vertexBufferSize)
	{
		device->CreateInputLayout(
			elements,
			2,
			vertexBufferPointer,
			vertexBufferSize,
			&layout);

		return layout;
	}

	D3D11_INPUT_ELEMENT_DESC CreateLayoutDescElement(const char* _name, bool _isAligned, LayoutType _type)
	{
		DXGI_FORMAT format = DXGI_FORMAT_R32G32_FLOAT;
		switch (_type)
		{
		case LayoutType::float4:
			format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			break;
		case LayoutType::float2:
			format = DXGI_FORMAT_R32G32_FLOAT;
			break;
		default:
			break;
		}

		return D3D11_INPUT_ELEMENT_DESC{
			_name,
			0,
			format,
			0,
			_isAligned? D3D11_APPEND_ALIGNED_ELEMENT : 0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0 
		};
	}

	D3D11_INPUT_ELEMENT_DESC* CreateLayoutInput(std::vector<LayoutCreationDesc> names)
	{
		auto size = names.size();
		auto descs = new D3D11_INPUT_ELEMENT_DESC[size];
		for (auto i = 0; i < size; i++)
			descs[i] = CreateLayoutDescElement(names[i].name, i != 0, names[i].type);

		return descs;
	}

	ID3D11InputLayout* GetLayout()
	{
		return layout;
	}
};