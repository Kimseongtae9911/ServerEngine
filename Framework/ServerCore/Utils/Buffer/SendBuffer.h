#pragma once

class SendBufferChunk;
class SendBuffer
{
public:
	SendBuffer(SendBufChunkRef _owner, uint8* _buffer, uint32 _allocSize);
	~SendBuffer();

	uint8_t* GetBuffer() { return m_buffer; }
	uint32 GetAllocSize() { return m_allocSize; }
	uint32 GetWriteSize() { return m_writeSize; }
	void Close(uint32 _writeSize);

private:
	uint8* m_buffer;
	uint32 m_allocSize = 0;
	uint32 m_writeSize = 0;	// 한번에 최대로 보낼 수 있는 버퍼 크기
	SendBufChunkRef m_owner = nullptr;
};


//Chunk에서 필요한만큼 SendBuffer를 가져다 쓴다
class SendBufferChunk : public std::enable_shared_from_this<SendBufferChunk>
{
	enum {
		SEND_BUFFER_CHUNK_SIZE = 6000
	};
	
public:
	SendBufferChunk() {}
	~SendBufferChunk() {}

	void Reset();
	SendBufRef Open(uint32 _allocSize);
	void Close(uint32 _writeSize);

	bool IsOpen() { return m_isOpen; }
	uint8* GetBuffer() { return &m_buffer[m_usedSize]; }
	uint32 GetFreeSize() { return static_cast<uint32>(m_buffer.size() - m_usedSize); }

private:
	StArray<uint8, SEND_BUFFER_CHUNK_SIZE> m_buffer = {};
	bool m_isOpen = false;
	uint32 m_usedSize = 0;
};

class SendBufferPool
{
public:
	SendBufRef UseChunk(uint32 _size);

private:
	SendBufChunkRef Pop();
	void Push(SendBufChunkRef _buffer);

	static void ChunkRelease(SendBufferChunk* _buffer);

private:
	USE_LOCK;
	StVector<SendBufChunkRef> m_sendBufferChunks;
};