#pragma once

class BaseAllocator
{
public:
	static void*	Alloc(int32 _size);
	static void		Release(void* _ptr);
};

/*
			--------------StompAllocator------------
동적할당 메모리를 delete한다고 해서 해당 메모리를 바로 운영체제에 반납하지 않음
이로 인해 delete를 했지만 해당 메모리에 접근하여 프로그램이 오동작할 가능성이 있음
이를 방지하기 위한 메모리 할당자

단점: 작은 사이즈의 메모리를 할당하려고 해도 PAGE_SIZE의 배수만큼의 엄청 큰 메모리를 할당함
=> 개발단계에서만 사용
*/
class StompAllocator
{
	enum { PAGE_SIZE = 0x1000 };
public:
	static void*	Alloc(int32 _size);
	static void		Release(void* _ptr);
};

/*
	MemoryPool에서 사용하는 Allocator
*/
class PoolAllocator
{
public:
	static void*	Alloc(int32 _size);
	static void		Release(void* _ptr);
};

template<typename T>
class StlAllocator
{
public:
	using value_type = T;

	StlAllocator() {}

	template<typename Other>
	StlAllocator(const StlAllocator<Other>*) {}

	template<typename Other>
	constexpr StlAllocator(StlAllocator<Other> const&) noexcept {}

	T* allocate(size_t _count) {
		const int32 size = static_cast<int32>(_count * sizeof(T));
		return static_cast<T*>(stalloc(size));
	}

	void deallocate(T* ptr, size_t count) {
		strelease(ptr);
	}
};