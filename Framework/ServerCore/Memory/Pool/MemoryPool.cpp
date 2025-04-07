#include "../../pch.h"

MemoryPool::MemoryPool(int32 _allocSize) : m_allocSize(_allocSize)
{

}

MemoryPool::~MemoryPool()
{
	//�߰��� �Ҹ��ڰ� ȣ����� �ʴ´ٰ� ����
	while (!m_concurrentMemoryQueue.empty()) {
		MemoryHeader* header = nullptr;
		if (false == m_concurrentMemoryQueue.try_pop(header))
			continue;

		::free(header);	
	}
}

void MemoryPool::PushMemory(MemoryHeader* _ptr)
{
	_ptr->allocSize = 0;
	m_concurrentMemoryQueue.push(_ptr);
	m_useCount.fetch_sub(1);
	m_reserveCount.fetch_add(1);
}

MemoryHeader* MemoryPool::PopMemory()
{
	MemoryHeader* header = nullptr;
	m_concurrentMemoryQueue.try_pop(header);

	// ������ ���� �����(�����ڿ��� ���� ���� �̸� ����� ���� �͵� ���...)
	if (nullptr == header) {
		header = reinterpret_cast<MemoryHeader*>(::malloc(m_allocSize));
	}
	else {
		ASSERT_CRASH(header->allocSize == 0);
		m_reserveCount.fetch_sub(1);
	}
	
	m_useCount.fetch_add(1);

	return header;
}
