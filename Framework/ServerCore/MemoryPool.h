#pragma once

/*
	�޸��� ũ��, ���� �޸��� �� �ּ� ���� ������ ���� ���
	[MemoryHeader][Data]
*/
struct MemoryHeader
{
	MemoryHeader(int32 _size) : allocSize(_size) {}

	static void* AttachHeader(MemoryHeader* _header, int32 _size) {
		new(_header)MemoryHeader(_size);
		return reinterpret_cast<void*>(++_header);	//Data�� ���� �ּ� ��ȯ
	}

	static MemoryHeader* DetachHeader(void* _ptr) {
		MemoryHeader* header = reinterpret_cast<MemoryHeader*>(_ptr) - 1;	// MemoryHeader ���� �ּ� ��ȯ
		return header;
	}

	int32 allocSize = 0;
	// todo : ����뿡 �ʿ��� �߰� ����
};

/*
	�� �޸� Ǯ�� allocSize������ ũ�⸦ ���� �޸𸮵��� ����
	ex) [32][64][128] ���� 32, 64, 128������ �޸𸮵��� ����
*/
class MemoryPool
{
public:
	MemoryPool(int32 _allocSize);
	~MemoryPool();

	void PushMemory(MemoryHeader* _ptr);
	MemoryHeader* PopMemory();

private:
	int32 m_allocSize = 0;	// ����ϰ� �ִ� �޸� ũ��
	std::atomic<int32> m_allocCount = 0;	// �޸�Ǯ���� ���� ���� �޸� ����

	USE_LOCK;
	std::queue<MemoryHeader*> m_memoryQueue;
};

