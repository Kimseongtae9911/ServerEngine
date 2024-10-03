#pragma once

class SendBuffer : std::enable_shared_from_this<SendBuffer>
{
public:
	SendBuffer(int32 _bufferSize);
	~SendBuffer();

	uint8_t* GetBuffer() { return m_buffer.data(); }
	int32 GetWriteSize() { return m_writeSize; }
	int32 GetCapacity() { return static_cast<int32>(m_buffer.size()); }

	void CopyData(void* _data, int32 _len);

private:
	StVector<uint8> m_buffer;
	int32 m_writeSize = 0;	// 한번에 최대로 보낼 수 있는 버퍼 크기
};

