#pragma once
#include <stack>
#include <map>
#include <vector>

class DeadLockProfiler
{
public:
	void PushLock(std::string _name);
	void PopLock(std::string _name);
	void CheckCycle();

private:
	void Dfs(int32 _target);

private:
	std::unordered_map<std::string, int32> m_nameToId;
	std::unordered_map<int32, std::string> m_idToName;
	std::map<int32, std::set<int32>> m_lockHistory;

	Mutex m_lock;

	std::vector<int32> m_discoverOrder;	//노드가 발견된 순서를 기록하는 배열
	int32 m_discoverdCount = 0; // 노드가 발견된 순서
	std::vector<bool> m_finished;	// Dfs(i)가 종료되었는지 여부
	std::vector<int32> m_parent;
};

