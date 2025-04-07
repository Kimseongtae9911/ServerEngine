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
	int32 m_capacity = 0;		//실제 버퍼의 크기
	int32 m_bufferSize = 0;		//한번에 사용 가능한 버퍼 크기
	int32 m_readPos = 0;		//현재 읽고 있는 버퍼 위치
	int32 m_writePos = 0;		//현재 쓰고 있는 버퍼 위치
	StVector<uint8> m_buffer;
};

