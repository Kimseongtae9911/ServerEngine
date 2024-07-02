#pragma once
#include "Allocator.h"

class MemoryPool;

/*
	todo: 메모리풀의 단위, 개수를 수정할 때 많은 코드를 수정해줘야함
	-> 공식을 만들어서 값만 바꿀 시 적용되도록 수정 필요
*/
class MemoryManager
{
	// 1024까지는 32단위, 2048->128, 2096->256
	// 사이즈가 클 수록 적을 것이기 때문... 이후 진행하면서 수정 필요 할수도(todo:check)
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
	std::array<MemoryPool*, MAX_ALLOC_SIZE + 1> m_poolTable;	// O(1)에 메모리 찾기 위한 테이블, 0~32는 같은 풀, 33~64는 같은 풀
};


/*
	C++의 new와 delete를 사용하게 되면 컨텍스트 스위칭이 발생할 수 있다
	이를 방지하기 위해 커스텀 Allocator 사용, 네이밍은 일단 st..
*/
template<typename Type, typename... Args>
Type* stnew(Args&&... args) {
	//메모리 할당
	Type* memory = static_cast<Type*>(PoolAllocator::Alloc(sizeof(Type)));

	//생성자 호출S
	new(memory)Type(std::forward<Args>(args));
	return memory;
}

template<typename Type>
void stdelete(Type* _obj) {
	//소멸자 호출
	_obj->~Type();

	//메모리 반납
	PoolAllocator::Release(_obj);
}

/*
	오브젝트풀은 사용하지 않고 메모리풀만 사용하여 오브젝트 생성할 때
*/
template<typename Type>
std::shared_ptr<Type> CreateSharedObj()
{
	return std::shared_ptr<Type>{stnew<Type>(), stdelete<Type>};
}