#include "pch.h"
#include "DeadLockProfiler.h"

void DeadLockProfiler::PushLock(const std::string _name)
{
	LockGuard guard(m_lock);

	//아이디 발급
	int32 lockId = 0;

	auto it = m_nameToId.find(_name);
	if (it == m_nameToId.end()) {
		lockId = static_cast<int32>(m_nameToId.size());
		m_nameToId[_name] = lockId;
		m_idToName[lockId] = _name;
	}
	else {
		lockId = it->second;
	}

	if (LLockStack.empty() == false) {
		// 기존에 없던 락이라면 데드락 여부 확인
		const int32 prevId = LLockStack.top();
		// 같은 스레드가 여러 락을 할 수 있다
		if (lockId != prevId) {
			auto& history = m_lockHistory[prevId];
			if (history.find(lockId) == history.end()) {
				history.insert(lockId);
				CheckCycle();
			}
		}
	}

	LLockStack.push(lockId);
}

void DeadLockProfiler::PopLock(const std::string _name)
{
	LockGuard guard(m_lock);

	if (LLockStack.empty())
		CRASH("Multiple_Unlock");

	auto lockId = m_nameToId[_name];
	if (LLockStack.top() != lockId)
		CRASH("Invalid_Unlock");

	LLockStack.pop();
}

void DeadLockProfiler::CheckCycle()
{
	const auto lockCount = static_cast<int32>(m_nameToId.size());
	m_discoverOrder = std::vector<int32>(lockCount, -1);
	m_discoverdCount = 0;
	m_finished = std::vector<bool>(lockCount, false);
	m_parent = std::vector<int32>(lockCount, -1);

	for (int32 lockId = 0; lockId < lockCount; ++lockId)
		Dfs(lockId);

	m_discoverOrder.clear();
	m_finished.clear();
	m_parent.clear();
}

void DeadLockProfiler::Dfs(int32 _target)
{
	if (m_discoverOrder[_target] != -1)
		return;

	m_discoverOrder[_target] = m_discoverdCount++;

	auto it = m_lockHistory.find(_target);
	if (it == m_lockHistory.end()) {
		m_finished[_target] = true;
		return;
	}

	auto& nextSet = it->second;
	for (auto next : nextSet) {
		if (m_discoverOrder[next] == -1) {
			m_parent[next] = _target;
			Dfs(next);
			continue;
		}

		//target이 next보다 작으면 순방향
		if (m_discoverOrder[_target] < m_discoverOrder[next])
			continue;

		//역방향(데드락 발생)
		if (m_finished[_target] == false) {
			std::cout << m_idToName[_target] << " -> " << m_idToName[next] << std::endl;

			auto now = _target;
			while (now != next) {
				std::cout << m_idToName[m_parent[now]] << " -> " << m_idToName[now] << std::endl;
				now = m_parent[now];
			}
			CRASH("Deadlock_Detected");
		}
	}

	m_finished[_target] = true;
}
