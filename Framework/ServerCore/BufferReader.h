#pragma once
class BufferReader
{
public:
	BufferReader();
	BufferReader(uint8* _buffer, uint32 _size, uint32 _pos = 0);
	~BufferReader();

	uint8* GetBuffer() { return m_buffer; }
	uint32 GetSize() { return m_size; }
	uint32 GetReadSize() { return m_pos; }
	uint32 GetFreeSize() { return m_size - m_pos; }

	bool Peek(void* _dest, uint32 _len);
	bool Read(void* _dest, uint32 _len);
	
	template<typename T>
	bool Peek(T* _dest) { return Peek(_dest, sizeof(T)); }
	template<typename T>
	bool Read(void* _dest, uint32 _len) { return Read(_dest, sizeof(T)); }

	template<typename T>
	BufferReader& operator>>(OUT T& _dest)
	{
		_dest = *reinterpret_cast<T*>(&m_buffer[m_pos]);
		m_pos += sizeof(T);
		return *this;
	}

private:
	uint8* m_buffer;
	uint32 m_size = 0;
	uint32 m_pos = 0;
};

