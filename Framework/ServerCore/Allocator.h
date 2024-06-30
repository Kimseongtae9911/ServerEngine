#pragma once

class BaseAllocator
{
public:
	static void*	Alloc(int32 _size);
	static void		Release(void* _ptr);
};

/*
			--------------StompAllocator------------
�����Ҵ� �޸𸮸� delete�Ѵٰ� �ؼ� �ش� �޸𸮸� �ٷ� �ü���� �ݳ����� ����
�̷� ���� delete�� ������ �ش� �޸𸮿� �����Ͽ� ���α׷��� �������� ���ɼ��� ����
�̸� �����ϱ� ���� �޸� �Ҵ���

����: ���� �������� �޸𸮸� �Ҵ��Ϸ��� �ص� PAGE_SIZE�� �����ŭ�� ��û ū �޸𸮸� �Ҵ���
=> ���ߴܰ迡���� ���
*/
class StompAllocator
{
	enum { PAGE_SIZE = 0x1000 };
public:
	static void*	Alloc(int32 _size);
	static void		Release(void* _ptr);
};

/*
	MemoryPool���� ����ϴ� Allocator
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