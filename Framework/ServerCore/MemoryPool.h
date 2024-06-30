#pragma once

/*
	메모리의 크기, 다음 메모리의 힙 주소 등의 정보를 담은 헤더
	[MemoryHeader][Data]
*/
struct MemoryHeader
{
	MemoryHeader(int32 _size) : allocSize(_size) {}

	static void* AttachHeader(MemoryHeader* _header, int32 _size) {
		new(_header)MemoryHeader(_size);
		return reinterpret_cast<void*>(++_header);	//Data의 시작 주소 반환
	}

	static MemoryHeader* DetachHeader(void* _ptr) {
		MemoryHeader* header = reinterpret_cast<MemoryHeader*>(_ptr) - 1;	// MemoryHeader 시작 주소 반환
		return header;
	}

	int32 allocSize = 0;
	// todo : 디버깅에 필요한 추가 정보
};

/*
	각 메모리 풀은 allocSize이하의 크기를 가진 메모리들의 집합
	ex) [32][64][128] 각각 32, 64, 128이하의 메모리들의 집합
*/
class MemoryPool
{
public:
	MemoryPool(int32 _allocSize);
	~MemoryPool();

	void PushMemory(MemoryHeader* _ptr);
	MemoryHeader* PopMemory();

private:
	int32 m_allocSize = 0;	// 담당하고 있는 메모리 크기
	std::atomic<int32> m_allocCount = 0;	// 메모리풀에서 관리 중인 메모리 개수

	USE_LOCK;
	std::queue<MemoryHeader*> m_memoryQueue;
};

