#pragma once
#include "Types.h"
#include "MemoryPool.h"

/*
	�� Ŭ������ ������ ������Ʈ Ǯ�� ������, �� ������Ʈ Ǯ�� �޸�Ǯ�� ����	
	�޸�Ǯ �ϳ��� ���� �Ͱ� ���������� �� ��ü�� �޸�Ǯ�� ����ϹǷ� ��� ������ �߻��ߴ��� �ľ��ϱ� ����

	������Ʈ ��ü�� �Ź� ������ ������ �ݺ���
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
	todo: ��ü ���������� ���� �ʰ� �޸�Ǯ�� ����ϴ� ������ƮǮ
*/
template<typename Type>
class ReuseObjectPool
{

};