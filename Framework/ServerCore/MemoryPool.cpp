#include "pch.h"
#include "MemoryPool.h"

MemoryPool::MemoryPool(int32 _allocSize) : m_allocSize(_allocSize)
{

}

MemoryPool::~MemoryPool()
{
	while (!m_memoryQueue.empty()) {
		MemoryHeader* header = m_memoryQueue.front();
		m_memoryQueue.pop();
		::free(header);
	}
}

void MemoryPool::PushMemory(MemoryHeader* _ptr)
{
	{
		WRITE_LOCK;
		_ptr->allocSize = 0;
		m_memoryQueue.push(_ptr);
	}
	m_allocCount.fetch_sub(1);
}

MemoryHeader* MemoryPool::PopMemory()
{
	MemoryHeader* header = nullptr;
	{
		WRITE_LOCK;
		if (!m_memoryQueue.empty()) {
			header = m_memoryQueue.front();
			m_memoryQueue.pop();
		}
	}

	// 없으면 새로 만든다(생성자에서 일정 개수 미리 만들어 놓는 것도 고려...)
	if (nullptr == header) {
		header = reinterpret_cast<MemoryHeader*>(::malloc(m_allocSize));
	}
	else {
		ASSERT_CRASH(header->allocSize == 0);
	}
	
	m_allocCount.fetch_add(1);

	return header;
}
