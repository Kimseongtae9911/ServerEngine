#pragma once
class BaseAllocator
{
public:
	static void*	Alloc(int32 _size);
	static void		Release(void* _ptr);
};

