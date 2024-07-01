#include "pch.h"
#include "MemoryPool.h"

MemoryPool::MemoryPool(int32 _allocSize) : m_allocSize(_allocSize)
{

}

MemoryPool::~MemoryPool()
{
	//중간에 소멸자가 호출되지 않는다고 가정
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
	m_allocCount.fetch_sub(1);
}

MemoryHeader* MemoryPool::PopMemory()
{
	MemoryHeader* header = nullptr;
	m_concurrentMemoryQueue.try_pop(header);

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
