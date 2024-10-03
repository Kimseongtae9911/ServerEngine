#pragma once
#include <stack>
extern thread_local uint32_t LThreadId;
extern thread_local std::stack<int32> LLockStack;
extern thread_local SendBufChunkRef LSendBufferChunk;
