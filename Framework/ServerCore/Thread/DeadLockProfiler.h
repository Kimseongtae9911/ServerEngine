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

	std::vector<int32> m_discoverOrder;	//��尡 �߰ߵ� ������ ����ϴ� �迭
	int32 m_discoverdCount = 0; // ��尡 �߰ߵ� ����
	std::vector<bool> m_finished;	// Dfs(i)�� ����Ǿ����� ����
	std::vector<int32> m_parent;
};

