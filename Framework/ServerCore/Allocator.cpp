#include "pch.h"
#include "Allocator.h"

void* BaseAllocator::Alloc(int32 _size)
{
	return ::malloc(_size);
}

void BaseAllocator::Release(void* _ptr)
{
	::free(_ptr);
}
