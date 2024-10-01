#include "pch.h"
#include "RecvBuffer.h"

RecvBuffer::RecvBuffer(int32 _bufferSize) : m_bufferSize(_bufferSize)
{
	m_capacity = m_bufferSize * BUFFER_COUNT;	// 복사 비용을 줄이기 위해 더 크게 공간을 잡는다
	m_buffer.resize(m_capacity);
}

RecvBuffer::~RecvBuffer()
{
}

void RecvBuffer::ClearBuffer()
{
	int32 dataSize = GetDataSize();

	if (dataSize == 0) {
		// 읽기, 쓰기 커서가 같은 위치면 모든 데이터를 처리완료하였으므로 리셋
		m_readPos = 0;
		m_writePos = 0;
	}
	else if (GetFreeSize() < m_bufferSize) {
		// 버퍼의 공간이 부족한 경우 남아있단 데이터를 앞으로 복사(복사 비용이 있다)
		::memcpy_s(&m_buffer[0], m_bufferSize, &m_buffer[m_readPos], dataSize);
		m_readPos = 0;
		m_writePos = dataSize;
	}
}

bool RecvBuffer::OnRead(int32 _numBytes)
{
	if (_numBytes > GetDataSize())
		return false;

	m_readPos += _numBytes;
	return true;
}

bool RecvBuffer::OnWrite(int32 _numBytes)
{
	if (_numBytes > GetFreeSize())
		return false;

	m_writePos += _numBytes;
	return true;
}
