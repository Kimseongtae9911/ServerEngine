#include "../../pch.h"

BufferWriter::BufferWriter()
{
}

BufferWriter::BufferWriter(uint8* _buffer, uint32 _size, uint32 _pos) :
	m_buffer(_buffer), m_size(_size), m_pos(_pos)
{
}

BufferWriter::~BufferWriter()
{
}

bool BufferWriter::Write(void* _source, uint32 _len)
{
	if (GetFreeSize() < _len)
		return false;

	::memcpy_s(&m_buffer[m_pos], m_size, _source, _len);
	m_pos += _len;
	return true;
}
