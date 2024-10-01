#include "pch.h"
#include "RecvBuffer.h"

RecvBuffer::RecvBuffer(int32 _bufferSize) : m_bufferSize(_bufferSize)
{
	m_capacity = m_bufferSize * BUFFER_COUNT;	// ���� ����� ���̱� ���� �� ũ�� ������ ��´�
	m_buffer.resize(m_capacity);
}

RecvBuffer::~RecvBuffer()
{
}

void RecvBuffer::ClearBuffer()
{
	int32 dataSize = GetDataSize();

	if (dataSize == 0) {
		// �б�, ���� Ŀ���� ���� ��ġ�� ��� �����͸� ó���Ϸ��Ͽ����Ƿ� ����
		m_readPos = 0;
		m_writePos = 0;
	}
	else if (GetFreeSize() < m_bufferSize) {
		// ������ ������ ������ ��� �����ִ� �����͸� ������ ����(���� ����� �ִ�)
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
