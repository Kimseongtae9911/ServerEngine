#pragma once
class BaseAllocator
{
public:
	static void*	Alloc(int32 _size);
	static void		Release(void* _ptr);
};

/*
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
