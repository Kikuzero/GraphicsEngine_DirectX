#pragma once
#include <unordered_set>
#include <Windows.h>

enum class WheelState
{
	Up,
	Down,
	Neutral
};

class MouseState {
	friend class Window;

public:
	struct PositionInfo
	{
		int x;
		int y;
		int dx;
		int dy;

		void Set(int _x, int _y)
		{
			dx = _x - x;
			dy = _y - y;

			x = _x;
			y = _y;
		}

		PositionInfo Pop()
		{
			auto retInfo = *this;

			dx = 0;
			dy = 0;

			return retInfo;
		}

		void ClearDelta()
		{
			dx = 0;
			dy = 0;
		}
	};

	struct ButtonsInfo
	{
		std::unordered_set<unsigned int> keys;
	};

	// TODO: вынести из структуры
	struct MouseInfo {
		PositionInfo position;

		bool isLeftClicked;
		bool isRightClicked;

		WheelState wheel;
	};

	MouseState(long x, long y)
	{
		mouseInfo = { 
			.position = { x, y, 0, 0 },
			.isLeftClicked = false, 
			.isRightClicked = false 
		};
	}

	PositionInfo GetMousePositionInfo()
	{
		return mouseInfo.position;
	}

	PositionInfo PopMousePositionInfo()
	{
		return mouseInfo.position.Pop();
	}

	WheelState GetWheelState()
	{
		return mouseInfo.wheel;
	}

private:
	MouseInfo mouseInfo;

	void MouseMove(float _x, float _y)
	{
		mouseInfo.position.Set(_x, _y);
	}

	void LeftButtonDown() { mouseInfo.isLeftClicked = true; }
	void LeftButtonUp() { mouseInfo.isLeftClicked = false; }
	void RightButtonDown(){ mouseInfo.isRightClicked = true; }
	void RightButtonUp(){ mouseInfo.isRightClicked = false; }
	void WheelUp() { mouseInfo.wheel = WheelState::Up; }
	void WheelDown(){ mouseInfo.wheel = WheelState::Down; }
	void WheelNeutral(){ mouseInfo.wheel = WheelState::Neutral; }
};