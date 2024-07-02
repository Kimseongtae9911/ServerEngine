#pragma once
#include "Allocator.h"

class MemoryPool;

/*
	todo: �޸�Ǯ�� ����, ������ ������ �� ���� �ڵ带 �����������
	-> ������ ���� ���� �ٲ� �� ����ǵ��� ���� �ʿ�
*/
class MemoryManager
{
	// 1024������ 32����, 2048->128, 2096->256
	// ����� Ŭ ���� ���� ���̱� ����... ���� �����ϸ鼭 ���� �ʿ� �Ҽ���(todo:check)
	enum {
		POOL_COUNT = (1024 / 32) + (1024 / 128) + (2048 / 256),
		MAX_ALLOC_SIZE = 4096
	};

public:
	MemoryManager();
	~MemoryManager();

	void* Allocate(int32 _size);
	void Release(void* _ptr);

private:
	std::vector<MemoryPool*> m_pools;
	std::array<MemoryPool*, MAX_ALLOC_SIZE + 1> m_poolTable;	// O(1)�� �޸� ã�� ���� ���̺�, 0~32�� ���� Ǯ, 33~64�� ���� Ǯ
};


/*
	C++�� new�� delete�� ����ϰ� �Ǹ� ���ؽ�Ʈ ����Ī�� �߻��� �� �ִ�
	�̸� �����ϱ� ���� Ŀ���� Allocator ���, ���̹��� �ϴ� st..
*/
template<typename Type, typename... Args>
Type* stnew(Args&&... args) {
	//�޸� �Ҵ�
	Type* memory = static_cast<Type*>(PoolAllocator::Alloc(sizeof(Type)));

	//������ ȣ��S
	new(memory)Type(std::forward<Args>(args));
	return memory;
}

template<typename Type>
void stdelete(Type* _obj) {
	//�Ҹ��� ȣ��
	_obj->~Type();

	//�޸� �ݳ�
	PoolAllocator::Release(_obj);
}

/*
	������ƮǮ�� ������� �ʰ� �޸�Ǯ�� ����Ͽ� ������Ʈ ������ ��
*/
template<typename Type>
std::shared_ptr<Type> CreateSharedObj()
{
	return std::shared_ptr<Type>{stnew<Type>(), stdelete<Type>};
}