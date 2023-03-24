#pragma once
#include <SimpleMath.h>

#pragma pack(push, 4)
struct DirectionLightData {
	DirectX::SimpleMath::Vector4 viewPosition;
	DirectX::SimpleMath::Vector3 lightPosition;
	float _1;
	DirectX::SimpleMath::Vector3 lightColor;
	float _2;
};
#pragma pack(pop)