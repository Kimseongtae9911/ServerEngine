#include "../pch.h"

MemoryManager::MemoryManager()
{
	int32 size = 0;
	int32 tableIndex = 0;

	for (size = 32; size <= 1024; size += 32) {
		MemoryPool* pool = new MemoryPool(size);
		m_pools.push_back(pool);

		while (tableIndex <= size) {
			m_poolTable[tableIndex] = pool;
			++tableIndex;
		}
	}
	size -= 32;

	for (; size <= 2048; size += 128) {
		MemoryPool* pool = new MemoryPool(size);
		m_pools.push_back(pool);

		while (tableIndex <= size) {
			m_poolTable[tableIndex] = pool;
			++tableIndex;
		}
	}
	size -= 128;

	for (; size <= 4096; size += 256) {
		MemoryPool* pool = new MemoryPool(size);
		m_pools.push_back(pool);

		while (tableIndex <= size) {
			m_poolTable[tableIndex] = pool;
			++tableIndex;
		}
	}
}

MemoryManager::~MemoryManager()
{
	for (auto pool : m_pools)
		delete pool;

	m_pools.clear();
}

void* MemoryManager::Allocate(int32 _size)
{
	MemoryHeader* header = nullptr;
	const int32 allocSize = _size + sizeof(MemoryHeader);

#ifdef _STOMP
	header = reinterpret_cast<MemoryHeader*>(StompAllocator::Alloc(allocSize));
#else
	if (allocSize > MAX_ALLOC_SIZE) {
		header = reinterpret_cast<MemoryHeader*>(::malloc(allocSize));	// 너무 크다면 풀에서 사용하지 않고 할당
	}
	else {
		header = m_poolTable[allocSize]->PopMemory();
	}
#endif

	return MemoryHeader::AttachHeader(header, allocSize);
}

void MemoryManager::Release(void* _ptr)
{
	MemoryHeader* header = MemoryHeader::DetachHeader(_ptr);
	const int32 allocSize = header->allocSize;
	ASSERT_CRASH(allocSize > 0);

#ifdef _STOMP
	StompAllocator::Release(header);
#else
	if (allocSize > MAX_ALLOC_SIZE) {
		::free(header);	// 너무 크다면 풀에 넣지 않고 해제
	}
	else {
		m_poolTable[allocSize]->PushMemory(header);
	}
#endif
}
