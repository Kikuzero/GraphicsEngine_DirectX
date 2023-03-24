#pragma once
#include <vector>
#include <DirectXMath.h>
#include <cmath> 

#define PI 3.14159265 

struct ObjectInfo
{
	std::vector<DirectX::XMFLOAT4> points;
	std::vector<int> indeces;
};

struct ObjectInfoPtrs
{
	DirectX::XMFLOAT4* points;
	size_t pointsSize;
	int* indeces;
	size_t indecesSize;
};

ObjectInfo GenerateCircle(float _x, float _y, float _r, int _res)
{
	if (_res % 2 != 0) _res += 1;

	std::vector<DirectX::XMFLOAT4> circleCoords(_res + 1);
	std::vector<int> circleIndeces;
	
	circleCoords.at(0) = {_x, _y, 0.f, 1.f};

	float t = 0;
	float step = 360 / _res;

	circleIndeces.push_back(0);
	for (int j = 0; j < 2; j++)
	{
		auto x = _r * (float)cos(t * PI / 180.f) + _x;
		auto y = _r * (float)sin(t * PI / 180.f) + _y;
		circleCoords.at(j + 1) = { abs(x) < 0.001 ? 0.f : x, abs(y) < 0.001? 0.f : y, 0.f, 1.f};

		circleIndeces.push_back(j + 1);
		t += step;
	}

	int i = 3;
	for (; i < (_res + 1); i++)
	{
		circleIndeces.push_back(0);
		circleIndeces.push_back(i - 1);

		auto x = _r * (float)cos(t * PI / 180.f) + _x;
		auto y = _r * (float)sin(t * PI / 180.f) + _y;
		circleCoords.at(i) = { abs(x) < 0.001 ? 0.f : x, abs(y) < 0.001 ? 0.f : y, 0.f, 1.f };

		circleIndeces.push_back(i);
		t += step;
	}

	circleIndeces.push_back(0);
	circleIndeces.push_back(i - 1);
	circleIndeces.push_back(1);

	return ObjectInfo{ .points = circleCoords, .indeces = circleIndeces };
}

ObjectInfo GenerateRectangle(float _x, float _y, float _width, float _height)
{
	if (_width <= 0 && _height <= 0) return ObjectInfo{};

	return ObjectInfo{
		.points = {
			DirectX::XMFLOAT4(_x + _width / 2, _y + _height / 2, 0.f, 1.0f),
			DirectX::XMFLOAT4(_x - _width / 2, _y - _height / 2, 0.f, 1.0f),
			DirectX::XMFLOAT4(_x - _width / 2, _y + _height / 2, 0.f, 1.0f),
			DirectX::XMFLOAT4(_x + _width / 2, _y - _height / 2, 0.f, 1.0f)
		},
		.indeces = { 0,1,2, 1,0,3 }
	};
}

ObjectInfoPtrs CastToPts(ObjectInfo _info)
{
	auto ipsize = _info.points.size();
	auto iisize = _info.indeces.size();

	auto iPoints = new DirectX::XMFLOAT4[ipsize];
	auto iIndeces = new int[iisize];

	for (int i = 0; i < ipsize; i++)
	{
		iPoints[i] = _info.points.at(i);
	}
	for (int i = 0; i < iisize; i++)
	{
		iIndeces[i] = _info.indeces.at(i);
	}

	return ObjectInfoPtrs{ .points = iPoints, .pointsSize = ipsize, .indeces = iIndeces, .indecesSize = iisize };
}

ObjectInfoPtrs GeneratePtsCircle(float _x, float _y, float _r, int _res)
{
	return CastToPts(GenerateCircle(_x, _y, _r, _res));
}

ObjectInfoPtrs GeneratePtsRectangle(float _x, float _y, float _width, float _height)
{
	return CastToPts(GenerateRectangle(_x, _y, _width, _height));
}

ObjectInfoPtrs CastColoredToPts(ObjectInfo _info, DirectX::XMFLOAT4 _color)
{
	auto ipsize = _info.points.size();
	auto iisize = _info.indeces.size();

	auto iPoints = new DirectX::XMFLOAT4[ipsize * 2];
	auto iIndeces = new int[iisize];

	for (int i = 0; i < ipsize; i++)
	{
		iPoints[2 * i] = _info.points.at(i);
		iPoints[2 * i + 1] = _color;
	}
	for (int i = 0; i < iisize; i++)
	{
		iIndeces[i] = _info.indeces.at(i);
	}

	return ObjectInfoPtrs{ .points = iPoints, .pointsSize = ipsize * 2, .indeces = iIndeces, .indecesSize = iisize };
}

ObjectInfoPtrs GenerateColoredPtsCircle(float _x, float _y, float _r, int _res, DirectX::XMFLOAT4 _color)
{
	return CastColoredToPts(GenerateCircle(_x, _y, _r, _res), _color);
}

ObjectInfoPtrs GenerateColoredPtsRectangle(float _x, float _y, float _width, float _height, DirectX::XMFLOAT4 _color)
{
	return CastColoredToPts(GenerateRectangle(_x, _y, _width, _height), _color);
}
