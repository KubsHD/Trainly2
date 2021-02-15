#pragma once

#include "stdafx.h"
#include <map>

class InputManager
{
public:
	InputManager() { }

	void ResetMouseDelta()
	{
		m_deltaMousePos = { 0.0f, 0.0f };
	}

	void SetKeyState(int key, bool pressed)
	{
		m_keyboardMap[key] = pressed;
	}

	void SetMouseButtonState(int key, bool pressed)
	{
		m_mouseMap[key] = pressed;
	}


	bool IsMouseMoving() { return m_isMoving; }

	void SetMouseState(bool state)
	{
		m_isMoving = state;
	}

	void SetMousePosition(DirectX::SimpleMath::Vector2 pos)
	{
		m_mousePos = pos;
	}

	void SetDeltaMouseState(DirectX::SimpleMath::Vector2 pos)
	{
		m_deltaMousePos = pos;
	}

	DirectX::SimpleMath::Vector2 GetDeltaMousePos()
	{
		return m_deltaMousePos;
	}

	DirectX::SimpleMath::Vector2 GetMousePos()
	{
		return m_mousePos;
	}

	bool IsKeyPressed(int key)
	{
		return m_keyboardMap[key];
	}

	bool IsMouseButtonPressed(int key)
	{
		return m_mouseMap[key];
	}
	
private:
	std::map<int, bool> m_keyboardMap;
	std::map<int, bool> m_mouseMap;

	DirectX::SimpleMath::Vector2 m_mousePos = { 0.0f, 0.0f };
	DirectX::SimpleMath::Vector2 m_deltaMousePos = { 0.0f, 0.0f };
	bool m_isMoving;
};

