#include "../../pch.h"

void* BaseAllocator::Alloc(int32 _size)
{
	return ::malloc(_size);
}

void BaseAllocator::Release(void* _ptr)
{
	::free(_ptr);
}

/*---------------------
	StompAllocator
---------------------*/

void* StompAllocator::Alloc(int32 _size)
{
	// _size가 이미 PAGE_SIZE의 배수인 경우 버림 처리
	const int64 pageCount = (_size + PAGE_SIZE - 1) / PAGE_SIZE;

	//오버플로 방지를 위해 메모리의 뒷부분에 할당(언더플로우보다 오버플로우가 일어나는 경우가 많다..)
	const int64 dataOffset = pageCount * PAGE_SIZE - _size;
#ifdef _WINDOW
	void* baseAddress = ::VirtualAlloc(NULL, pageCount * PAGE_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	return static_cast<void*>(static_cast<int8*>(baseAddress) + dataOffset);
#else
	void* baseAddress = mmap(NULL, pageCount * PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	return static_cast<void*>(static_cast<int8_t*>(baseAddress) + dataOffset);
#endif
}

void StompAllocator::Release(void* _ptr)
{
	const int64 address = reinterpret_cast<int64>(_ptr);
	const int64 baseAddress = address - (address % PAGE_SIZE);
	
#ifdef _WINDOW
	::VirtualFree(reinterpret_cast<void*>(baseAddress), 0, MEM_RELEASE);
#else
	munmap(reinterpret_cast<void*>(baseAddress), PAGE_SIZE);
#endif
}

/*---------------------
	 PoolAllocator
---------------------*/

void* PoolAllocator::Alloc(int32 _size)
{
	return GMemoryManager->Allocate(_size);
}

void PoolAllocator::Release(void* _ptr)
{
	GMemoryManager->Release(_ptr);
}
