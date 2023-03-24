#pragma once
#include <d3d11.h>
#include <DirectXTex.h>
#include <wrl/client.h>
#include "GraphicsEngine.h"

class Sampler 
{
private:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> mSamplerState;
public:
	Sampler():mSamplerState(nullptr)
	{}

	void Init(
		GraphicsEngine* _ge,
		D3D11_TEXTURE_ADDRESS_MODE _addressMode,
		D3D11_FILTER _filter,
		D3D11_COMPARISON_FUNC _comparisonFunc,
		UINT _maxAnisotropy)
	{
		D3D11_SAMPLER_DESC sampler_desc = {};
		sampler_desc.AddressU = _addressMode;
		sampler_desc.AddressV = _addressMode;
		sampler_desc.AddressW = _addressMode;
		sampler_desc.Filter = _filter;
		sampler_desc.ComparisonFunc = _comparisonFunc;
		sampler_desc.BorderColor[0] = 1.0f;
		sampler_desc.BorderColor[1] = 0.0f;
		sampler_desc.BorderColor[2] = 0.0f;
		sampler_desc.BorderColor[3] = 1.0f;
		sampler_desc.MaxLOD = INT_MAX;
		sampler_desc.MaxAnisotropy = _maxAnisotropy;

		_ge->GetDevice()->CreateSamplerState(&sampler_desc, &mSamplerState);
	}

	void SetSampler(GraphicsEngine* _ge, unsigned int _slot = 0) {
		_ge->GetContext()->PSSetSamplers(_slot, 1, mSamplerState.GetAddressOf());
	}
};