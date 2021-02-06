#include "stdafx.h"


#include "timer.h"

using namespace std::chrono;

void Timer::Reset()
{
	//m_lastTime = std::chrono::system_clock::now();
}

void Timer::Tick()
{
	const auto old = m_lastTime;
	m_lastTime = steady_clock::now();
	const duration<float> frameTime = m_lastTime - old;
	m_deltaTime = frameTime;
}
