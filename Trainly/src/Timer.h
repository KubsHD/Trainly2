#pragma once

#include "stdafx.h"

class Timer
{
private:
	std::chrono::steady_clock::time_point m_lastTime;
	std::chrono::steady_clock::time_point m_runTime;
	std::chrono::duration<float> m_deltaTime;
public:

	Timer() {
		m_deltaTime = std::chrono::duration<float>(0.0f);
		m_runTime = std::chrono::steady_clock::now();
		m_lastTime = std::chrono::steady_clock::now();
	};
	~Timer() {};

	void Reset();
	void Tick();

	float ElapsedTime()
	{
		return (std::chrono::steady_clock::now() - m_runTime).count();
	}

	float DeltaTime()
	{
		return m_deltaTime.count();
	}
};