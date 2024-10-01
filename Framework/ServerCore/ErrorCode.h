#pragma once

enum class EResultCode
{
	Success = 0,
	Disconnect = 1,	//이미 연결이 끊겼다
	BufferOverflow = 2, //버퍼 오버플로우
};