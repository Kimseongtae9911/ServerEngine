#pragma once
#include "Allocator.h"

/*
C++�� new�� delete�� ����ϰ� �Ǹ� ���ؽ�Ʈ ����Ī�� �߻��� �� �ִ�
�̸� �����ϱ� ���� Ŀ���� Allocator ���
*/

template<typename Type, typename... Args>
Type* stnew(Args&&... args) {
	//�޸� �Ҵ�
	Type* memory = static_cast<Type*>(BaseAllocator::Alloc(sizeof(Type)));

	//������ ȣ��
	new(memory)Type(std::forward<Args>(args));
	return memory;
}

template<typename Type>
void stdelete(Type* _obj) {
	//�Ҹ��� ȣ��
	_obj->~Type();

	//�޸� �ݳ�
	BaseAllocator::Release(_obj);
}