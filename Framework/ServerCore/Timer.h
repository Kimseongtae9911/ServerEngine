#pragma once

class Service;

class Timer
{
public:
	Timer() = default;
	~Timer() = default;

	void SetService(Service* _service) { m_service = _service; }
	void StartTimer(int32 _timePeriodMS) {
		if (m_isRunning)
			return;

		m_isRunning = true;
		m_nextCheckTimeMS = _timePeriodMS;
		RunTimer();
	}
	void StopTimer() { m_isRunning = false; }	

protected:
	void RunTimer();
	virtual void OnTimer() {}

protected:
	int32 m_nextCheckTimeMS = 0;
	std::atomic_bool m_isRunning = false;
	Service* m_service = nullptr;
};