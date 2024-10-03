#include "pch.h"
#include "SendBuffer.h"

SendBuffer::SendBuffer(int32 _bufferSize) : m_buffer(_bufferSize)
{
}

SendBuffer::~SendBuffer()
{
}

void SendBuffer::CopyData(void* _data, int32 _len)
{
	ASSERT_CRASH(GetCapacity() >= _len);

	::memcpy_s(m_buffer.data(), m_buffer.size(), _data, _len);
	m_writeSize = _len;
}
