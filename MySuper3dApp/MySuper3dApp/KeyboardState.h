#pragma once
#include <unordered_set>
#include <Windows.h>

enum class Keys {
	kA = 0x41,
	kD = 0x44,
	kS = 0x53,
	kW = 0x57,
	kShift = VK_SHIFT,
	kControl = VK_CONTROL,
	kSpace = VK_SPACE,
	kLeft = VK_LEFT,
	kUp = VK_UP,
	kRight = VK_RIGHT,
	kDown = VK_DOWN,
};

class KeyboardState {
	friend class Window;

private:
	std::unordered_set<unsigned int> keys;

	void AddPressedKey(unsigned int _key)
	{
		keys.insert(_key);
	}

	void RemovePressedKey(unsigned int _key)
	{
		keys.erase(_key);
	}

public:
	bool IsKeyDown(unsigned int _key) 
	{
		return keys.count(_key);
	}

	bool IsKeyDown(Keys _key) 
	{
		return IsKeyDown((unsigned int)_key);
	}
};