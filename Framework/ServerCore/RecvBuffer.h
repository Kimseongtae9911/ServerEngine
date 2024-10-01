#pragma once

class RecvBuffer
{
	enum { BUFFER_COUNT = 10 };

public:
	RecvBuffer(int32 _bufferSize);
	~RecvBuffer();

	void ClearBuffer();
	bool OnRead(int32 _numBytes);
	bool OnWrite(int32 _numBytes);

	uint8* GetReadPos() { return &m_buffer[m_readPos]; }
	uint8* GetWritePos() { return &m_buffer[m_writePos]; }
	int32 GetDataSize() { return m_writePos - m_readPos; }
	int32 GetFreeSize() { return m_capacity - m_writePos; }
	int32 GetBufferSize() { return m_bufferSize; }

private:
	int32 m_capacity = 0;		//���� ������ ũ��
	int32 m_bufferSize = 0;		//�ѹ��� ��� ������ ���� ũ��
	int32 m_readPos = 0;		//���� �а� �ִ� ���� ��ġ
	int32 m_writePos = 0;		//���� ���� �ִ� ���� ��ġ
	StVector<uint8> m_buffer;
};

