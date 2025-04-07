#include "../../pch.h"

void Timer::RunTimer()
{
	if (!m_isRunning)
		return;

	OnTimer();

	std::this_thread::sleep_for(std::chrono::milliseconds(m_nextCheckTimeMS));
	RunTimer();
}
