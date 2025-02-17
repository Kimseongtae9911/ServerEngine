#pragma once
#include <stack>
extern thread_local uint32 LThreadId;
extern thread_local uint64 LEndTickCount;

extern thread_local std::stack<int32> LLockStack;
extern thread_local SendBufChunkRef LSendBufferChunk;
extern thread_local JobSerializer* LCurrentJobSerializer;	//현재 실행중인 잡큐