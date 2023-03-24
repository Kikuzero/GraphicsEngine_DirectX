#pragma once
#include <windows.h>
#include <WindowsX.h>
#include <WinUser.h>
#include <string>
#include <set>
#include <iostream>
#include "KeyboardState.h"
#include "MouseState.h"

class Window
{
public:
	Window(int xSize, int ySize)
	{
		applicationName = L"App";

		hInstance = GetModuleHandle(nullptr);

		screenWidth = xSize;
		screenHeight = ySize;
	}

	int CustomRegisterClass()
	{
		WNDCLASSEX wc;

		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
		wc.hIconSm = wc.hIcon;
		wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = applicationName;
		wc.cbSize = sizeof(WNDCLASSEX);

		if (!RegisterClassEx(&wc)) return 0;
	}

	HWND Init()
	{
		if (!CustomRegisterClass()) return 0;

		RECT windowRect = { 0, 0, static_cast<LONG>(screenWidth), static_cast<LONG>(screenHeight) };
		AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

		auto dwStyle = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_THICKFRAME;

		auto posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		auto posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;

		HWND hWnd = CreateWindowEx(
			WS_EX_APPWINDOW,
			applicationName,
			applicationName,
			dwStyle,
			posX,
			posY,
			windowRect.right - windowRect.left,
			windowRect.bottom - windowRect.top,
			nullptr,
			nullptr,
			hInstance,
			this);

		ShowWindow(hWnd, SW_SHOW);
		SetForegroundWindow(hWnd);
		SetFocus(hWnd);

		POINT pt = {screenWidth / 2, screenHeight / 2};

		ClientToScreen(hWnd, &pt);
		SetCursorPos(pt.x, pt.y);

		mouseState = new MouseState(0, 0);
		mouseState->mouseInfo.position.ClearDelta();
		ShowCursor(true);

		return hWnd;
	}

	float GetAspectRatio()
	{
		return screenWidth /screenHeight;
	}

	KeyboardState GetKeyboardState()
	{
		return keyboardState;
	}	
	
	MouseState* GetMouseState()
	{
		return mouseState;
	}

	void GetMouseReady()
	{
		mouseState->mouseInfo.position.ClearDelta();
	}

private:
	LPCWSTR applicationName;
	HINSTANCE hInstance;
	KeyboardState keyboardState;
	MouseState* mouseState;

	int screenWidth;
	int screenHeight;

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		Window* window;

		if (msg == WM_CREATE) {
			const auto createStruct = reinterpret_cast<CREATESTRUCT*>(lparam);
			window = static_cast<Window*>(createStruct->lpCreateParams);
			SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
		}
		else {
			window = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		}

		switch (msg)
		{
		case WM_KEYDOWN: {
			const auto key = static_cast<unsigned int>(wparam);
			window->keyboardState.AddPressedKey(key);
			break;
		}
		case WM_KEYUP: {
			const auto key = static_cast<unsigned int>(wparam);
			window->keyboardState.RemovePressedKey(key);
			break;
		}
		case WM_LBUTTONDOWN:
			window->mouseState->LeftButtonDown();
			break;
		case WM_LBUTTONUP:
			window->mouseState->LeftButtonUp();
			break;
		case WM_RBUTTONDOWN:
			window->mouseState->RightButtonDown();
			break;
		case WM_RBUTTONUP:
			window->mouseState->RightButtonUp();
			break;
		case WM_MOUSEMOVE:
			window->mouseState->MouseMove(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
			break;
		case WM_MOUSEWHEEL:		
			if (GET_WHEEL_DELTA_WPARAM(wparam) > 0)	window->mouseState->WheelUp();
			else window->mouseState->WheelDown();
			break;
		default:
			if (window != nullptr)
			{
				if(window->mouseState != nullptr) window->mouseState->WheelNeutral();
			}
			return DefWindowProc(hwnd, msg, wparam, lparam);
		}

	}
};

