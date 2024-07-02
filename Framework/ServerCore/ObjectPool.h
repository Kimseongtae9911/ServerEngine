#pragma once
#include "Types.h"
#include "MemoryPool.h"

/*
	각 클래스별 고유의 오브젝트 풀을 가지며, 각 오브젝트 풀은 메모리풀을 가짐	
	메모리풀 하나를 쓰는 것과 동일하지만 각 객체별 메모리풀을 사용하므로 어디서 문제가 발생했는지 파악하기 쉽다

	오브젝트 자체는 매번 생성과 해제를 반복함
*/
template<typename Type>
class ObjectPool
{
public:
	template<typename... Args>
	static Type* PopObject(Args&&... args) {
#ifdef _STOMP
		MemoryHeader* ptr = reinterpret_cast<MemoryHeader*>(StompAllocator::Alloc(s_allocSize));
		Type* memory = static_cast<Type*>(MemoryHeader::AttachHeader(ptr, s_allocSize));
#else
		Type* memory = static_cast<Type*>(MemoryHeader::AttachHeader(s_pool.PopMemory(), s_allocSize));
#endif
		new(memory)Type(std::forward<Args>(args)...);
		return memory;
	}

	static void PushObject(Type* _obj) {
		_obj->~Type();
#ifdef _STOMP		
		StompAllocator::Release(MemoryHeader::DetachHeader(_obj));
#else
		s_pool.PushMemory(MemoryHeader::DetachHeader(_obj));
#endif
	}

	static std::shared_ptr<Type> MakeShared() {
		return std::shared_ptr<Type>(PopObject(), PushObject);
	}
	 
private:
	static int32 s_allocSize;
	static MemoryPool s_pool;
};

template<typename Type>
int32 ObjectPool<Type>::s_allocSize = sizeof(Type) + sizeof(MemoryHeader);

template<typename Type>
MemoryPool ObjectPool<Type>::s_pool{ s_allocSize };

/*
	todo: 객체 생성해제를 하지 않고 메모리풀을 사용하는 오브젝트풀
*/
template<typename Type>
class ReuseObjectPool
{

};