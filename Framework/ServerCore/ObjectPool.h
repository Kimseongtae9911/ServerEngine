#pragma once
#include "MemoryPool.h"
#include <tbb/concurrent_queue.h>

/*
	각 클래스별 고유의 오브젝트 풀을 가지며, 각 오브젝트 풀은 메모리풀을 가짐	
	메모리풀 하나를 쓰는 것과 동일하지만 각 객체별 메모리풀을 사용하므로 어디서 문제가 발생했는지 파악하기 쉽다

	오브젝트 자체는 매번 생성과 해제를 반복함
*/
template<typename Type>
class ObjectPool	
{
public:
	static std::shared_ptr<Type> MakeObject() {
		return std::shared_ptr<Type>(PopObject(), PushObject);
	}

private:
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
	 
private:
	static int32 s_allocSize;
	static MemoryPool s_pool;
};

template<typename Type>
int32 ObjectPool<Type>::s_allocSize = sizeof(Type) + sizeof(MemoryHeader);

template<typename Type>
MemoryPool ObjectPool<Type>::s_pool{ s_allocSize };

/*
	객체 생성해제를 하지 않고 메모리풀을 사용하는 오브젝트풀
*/
class ReuseObject
{
public:
	template<typename... Args>
	void InitObject(Args&&... args) { InitObjectImpl(std::forward<Args>(args)...); }

	virtual void ResetObject() = 0;	
private:
	// 가변 인자를 받는 순수 가상 함수
	virtual void InitObjectImpl() = 0;	// 가변인자를 받는 함수는 가상함수로 설정이 안됨... 인자 없는거라도 강제..
};

template<typename Type>
class ReuseObjectPool
{
public:
	template<typename... Args>
	static std::shared_ptr<Type> MakeObject(Args&&... args) {
		return std::shared_ptr<Type>(PopObject(std::forward<Args>(args)), PushObject);
	}

	//오브젝트풀을 초기화, 메모리풀은 유지
	static void ResetObjectPool() {
		while (!s_objectPool.empty()) {
			Type* obj = nullptr;
			s_objectPool.try_pop(obj);
			obj->~Type();
#ifdef _STOMP		
			StompAllocator::Release(MemoryHeader::DetachHeader(obj));
#else
			s_pool.PushMemory(MemoryHeader::DetachHeader(obj));
#endif
		}
	}

private:
	template<typename... Args>
	static Type* PopObject(Args&&... args) {
		Type* obj = nullptr;
		if (s_objectPool.try_pop(obj)) {
			obj->InitObject(std::forward<Args>(args)...);
			return obj;
		}

		//객체가 없다면 생성
#ifdef _STOMP
		MemoryHeader* ptr = reinterpret_cast<MemoryHeader*>(StompAllocator::Alloc(s_allocSize));
		obj = static_cast<Type*>(MemoryHeader::AttachHeader(ptr, s_allocSize));
#else
		obj = static_cast<Type*>(MemoryHeader::AttachHeader(s_pool.PopMemory(), s_allocSize));
#endif
		new(obj)Type();
		obj->InitObject(std::forward<Args>(args)...);

		// 타입 검사 ReuseObject를 상속받는지
		if (typeid(*obj) != typeid(ReuseObject) && !std::is_base_of<ReuseObject, Type>::value) {
			ASSERT_CRASH("Not Valid Type");	//일단 크래시...메모리 해제하고 nullptr리턴?
		}

		return obj;
	}

	static void PushObject(Type* _obj) {
		_obj->ResetObject();
		s_objectPool.push(_obj);
	}

private:
	static int32 s_allocSize;
	static MemoryPool s_pool;
	static tbb::concurrent_queue<Type*> s_objectPool;
};

template<typename Type>
int32 ReuseObjectPool<Type>::s_allocSize = sizeof(Type) + sizeof(MemoryHeader);

template<typename Type>
MemoryPool ReuseObjectPool<Type>::s_pool{ s_allocSize };

template<typename Type>
tbb::concurrent_queue<Type*> ReuseObjectPool<Type>::s_objectPool;