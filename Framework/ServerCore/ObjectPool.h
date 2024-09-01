#pragma once
#include "MemoryPool.h"
#include <tbb/concurrent_queue.h>

/*
	�� Ŭ������ ������ ������Ʈ Ǯ�� ������, �� ������Ʈ Ǯ�� �޸�Ǯ�� ����	
	�޸�Ǯ �ϳ��� ���� �Ͱ� ���������� �� ��ü�� �޸�Ǯ�� ����ϹǷ� ��� ������ �߻��ߴ��� �ľ��ϱ� ����

	������Ʈ ��ü�� �Ź� ������ ������ �ݺ���
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
	��ü ���������� ���� �ʰ� �޸�Ǯ�� ����ϴ� ������ƮǮ
*/
class ReuseObject
{
public:
	template<typename... Args>
	void InitObject(Args&&... args) { InitObjectImpl(std::forward<Args>(args)...); }

	virtual void ResetObject() = 0;	
private:
	// ���� ���ڸ� �޴� ���� ���� �Լ�
	virtual void InitObjectImpl() = 0;	// �������ڸ� �޴� �Լ��� �����Լ��� ������ �ȵ�... ���� ���°Ŷ� ����..
};

template<typename Type>
class ReuseObjectPool
{
public:
	template<typename... Args>
	static std::shared_ptr<Type> MakeObject(Args&&... args) {
		return std::shared_ptr<Type>(PopObject(std::forward<Args>(args)), PushObject);
	}

	//������ƮǮ�� �ʱ�ȭ, �޸�Ǯ�� ����
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

		//��ü�� ���ٸ� ����
#ifdef _STOMP
		MemoryHeader* ptr = reinterpret_cast<MemoryHeader*>(StompAllocator::Alloc(s_allocSize));
		obj = static_cast<Type*>(MemoryHeader::AttachHeader(ptr, s_allocSize));
#else
		obj = static_cast<Type*>(MemoryHeader::AttachHeader(s_pool.PopMemory(), s_allocSize));
#endif
		new(obj)Type();
		obj->InitObject(std::forward<Args>(args)...);

		// Ÿ�� �˻� ReuseObject�� ��ӹ޴���
		if (typeid(*obj) != typeid(ReuseObject) && !std::is_base_of<ReuseObject, Type>::value) {
			ASSERT_CRASH("Not Valid Type");	//�ϴ� ũ����...�޸� �����ϰ� nullptr����?
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