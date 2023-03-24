#pragma once
#include <DirectXMath.h>

float VectorLength(DirectX::XMFLOAT3 _vec)
{
	return (float)sqrt(pow(_vec.x, 2) + pow(_vec.y, 2) + pow(_vec.z, 2));
}

DirectX::XMFLOAT3 NormalizeVector(DirectX::XMFLOAT3 _vec)
{
	auto length = VectorLength(_vec);

	return { _vec.x / length, _vec.y / length, _vec.z / length };
}

DirectX::XMFLOAT3 SumVectors(DirectX::XMFLOAT3 _vec1, DirectX::XMFLOAT3 _vec2)
{
	return { _vec1.x + _vec2.x, _vec1.y + _vec2.y, _vec1.z + _vec2.z };
}

DirectX::XMFLOAT3 VectorMulToScalar(DirectX::XMFLOAT3 _vec, float _scalar)
{
	return { _vec.x * _scalar, _vec.y * _scalar, _vec.z * _scalar };
}

bool IsVectorsEqual(DirectX::XMFLOAT3 _vec1, DirectX::XMFLOAT3 _vec2)
{
	return _vec1.x == _vec2.x && _vec1.y == _vec2.y && _vec1.z == _vec2.z;
}