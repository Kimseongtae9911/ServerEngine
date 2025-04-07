#pragma once
class BufferWriter
{
public:
	BufferWriter();
	BufferWriter(uint8* _buffer, uint32 _size, uint32 _pos = 0);
	~BufferWriter();

	uint8* GetBuffer() { return m_buffer; }
	uint32 GetSize() { return m_size; }
	uint32 GetWriteSize() { return m_pos; }
	uint32 GetFreeSize() { return m_size - m_pos; }

	bool Write(void* _source, uint32 _len);

	template<typename T>
	bool Write(T* _source) { return Write(_source, sizeof(T)); }

	template<typename T>
	T* Reserve()
	{
		if (GetFreeSize() < sizeof(T))
			return nullptr;

		T* ret = reinterpret_cast<T*>(&m_buffer[m_pos]);
		m_pos += sizeof(T);
		return ret;
	}

	template<typename T>
	BufferWriter& operator<<(T&& _source)
	{
		using Type = std::remove_reference_t<T>;
		*reinterpret_cast<Type*>(&m_buffer[m_pos]) = std::forward<Type>(_source);
		m_pos += sizeof(T);
		return *this;
	}

private:
	uint8* m_buffer = nullptr;
	uint32 m_size = 0;
	uint32 m_pos = 0;
};

