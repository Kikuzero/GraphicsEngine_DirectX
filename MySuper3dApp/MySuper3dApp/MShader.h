#pragma once
#include <wrl.h>
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <iostream>

// TODO: разделить
class MShader
{
private:
	ID3DBlob* shaderBC;

public:
	MShader(const wchar_t* _filePath,
		const char* _entryPoint,
		const char* _shaderTarget,
		D3D_SHADER_MACRO* _macros = nullptr)
	{
		ID3DBlob* errorVertexCode;
		auto res = D3DCompileFromFile(_filePath,
			_macros,
			nullptr /*include*/,
			_entryPoint,
			_shaderTarget,
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
			0,
			&this->shaderBC,
			&errorVertexCode);

		if (FAILED(res) && errorVertexCode) {
			char* compileErrors = (char*)(errorVertexCode->GetBufferPointer());
			std::cout << compileErrors << std::endl;
		}
	}

	ID3D11VertexShader* GetVertexShader(Microsoft::WRL::ComPtr<ID3D11Device>& _device)
	{
		ID3D11VertexShader* vertexShader;
		_device->CreateVertexShader(
			this->GetShaderBufferPointer(),
			this->GetShaderBufferSize(),
			nullptr, &vertexShader);

		return vertexShader;
	}

	ID3D11PixelShader* GetPixelShader(Microsoft::WRL::ComPtr<ID3D11Device>& _device)
	{
		ID3D11PixelShader* pixelShader;

		_device->CreatePixelShader(
			this->GetShaderBufferPointer(),
			this->GetShaderBufferSize(),
			nullptr, &pixelShader);

		return pixelShader;
	}

	void* GetShaderBufferPointer()
	{
		return shaderBC->GetBufferPointer();
	}

	size_t GetShaderBufferSize()
	{
		return shaderBC->GetBufferSize();
	}

	ID3DBlob* GetShaderBC()
	{
		return shaderBC;
	}
};
