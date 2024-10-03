#pragma once

//todo ΩÃ±€≈Ê ≈¨∑°Ω∫?
class GameSessionManager
{
public:
	static GameSessionManager* GetInstance() { if (!s_instance) s_instance = stnew<GameSessionManager>(); return s_instance; }

	void AddSession(GameSessionRef _session);
	void RemoveSession(GameSessionRef _session);
	void Broadcast(SendBufRef _sendBuffer);


	static GameSessionManager* s_instance;

private:
	USE_LOCK;
	StSet<GameSessionRef> m_sessions;
};

#define GameSessionMgr GameSessionManager::GetInstance()