#include "pch.h"
#include "SendBuffer.h"

SendBuffer::SendBuffer(SendBufChunkRef _owner, uint8* _buffer, uint32 _allocSize) : m_owner(_owner), m_buffer(_buffer), m_allocSize(_allocSize)
{
}

SendBuffer::~SendBuffer()
{
}

void SendBuffer::Close(uint32 _writeSize)
{
	ASSERT_CRASH(m_allocSize >= _writeSize);
	m_writeSize = _writeSize;
	m_owner->Close(_writeSize);
}

/*=================
	SendBufferChunk
=================*/
void SendBufferChunk::Reset()
{
	m_isOpen = false;
	m_usedSize = 0;
}

SendBufRef SendBufferChunk::Open(uint32 _allocSize)
{
	ASSERT_CRASH(_allocSize <= SEND_BUFFER_CHUNK_SIZE);
	ASSERT_CRASH(m_isOpen == false);

	if (_allocSize > GetFreeSize())
		return nullptr;

	m_isOpen = true;
	return ObjectPool<SendBuffer>::MakeObject(shared_from_this(), GetBuffer(), _allocSize);
}

void SendBufferChunk::Close(uint32 _writeSize)
{
	ASSERT_CRASH(m_isOpen);
	m_isOpen = false;
	m_usedSize += _writeSize;
}


/*=================
	SendBufferPool
=================*/
SendBufRef SendBufferPool::UseChunk(uint32 _size)
{
	if (nullptr == LSendBufferChunk)
	{
		LSendBufferChunk = Pop();
		LSendBufferChunk->Reset();
	}

	ASSERT_CRASH(!LSendBufferChunk->IsOpen());

	//Chunk를 모두 썼다면 새걸로 교체
	if (LSendBufferChunk->GetFreeSize() < _size)
	{
		LSendBufferChunk = Pop();
		LSendBufferChunk->Reset();
	}

	CLDebug("Free Chunk Size: {}", LSendBufferChunk->GetFreeSize());

	return LSendBufferChunk->Open(_size);
}

SendBufChunkRef SendBufferPool::Pop()
{
	WRITE_LOCK;
	if (m_sendBufferChunks.empty()) {
		return SendBufChunkRef(stnew<SendBufferChunk>(), ChunkRelease);
	}

	auto sendBufferChunk = m_sendBufferChunks.back();
	m_sendBufferChunks.pop_back();
	return sendBufferChunk;
}

void SendBufferPool::Push(SendBufChunkRef _buffer)
{
	WRITE_LOCK;
	m_sendBufferChunks.push_back(_buffer);
}

void SendBufferPool::ChunkRelease(SendBufferChunk* _buffer)
{
	GSendBufferPool->Push(SendBufChunkRef(_buffer, ChunkRelease));
}