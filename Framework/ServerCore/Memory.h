#pragma once
#include "Allocator.h"

/*
C++의 new와 delete를 사용하게 되면 컨텍스트 스위칭이 발생할 수 있다
이를 방지하기 위해 커스텀 Allocator 사용
*/

template<typename Type, typename... Args>
Type* stnew(Args&&... args) {
	//메모리 할당
	Type* memory = static_cast<Type*>(BaseAllocator::Alloc(sizeof(Type)));

	//생성자 호출
	new(memory)Type(std::forward<Args>(args));
	return memory;
}

template<typename Type>
void stdelete(Type* _obj) {
	//소멸자 호출
	_obj->~Type();

	//메모리 반납
	BaseAllocator::Release(_obj);
}