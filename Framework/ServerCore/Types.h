#pragma once

#include <cstdint>
#include <string>
#include <mutex>
#include <atomic>

/*--------------
	Integers
--------------*/
using int8 = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;
using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;

/*--------------
	shared_ptr
--------------*/
using SessionRef = std::shared_ptr<class Session>;
using GameSessionRef = std::shared_ptr<class GameSession>;
using AcceptorRef = std::shared_ptr<class Acceptor>;
using SendBufRef = std::shared_ptr<class SendBuffer>;
using SendBufChunkRef = std::shared_ptr<class SendBufferChunk>;

/*--------------
	 Locks
--------------*/
template<typename T>
using Atomic = std::atomic<T>;
using Mutex = std::mutex;
using CondVar = std::condition_variable;
using UniqueLock = std::unique_lock<std::mutex>;
using LockGuard = std::lock_guard<std::mutex>;

/*--------------
	 Chrono
--------------*/
using TimeMS = std::chrono::milliseconds;
using TimeSec = std::chrono::seconds;


/*--------------
	 Boost
--------------*/
using tcpSocket = boost::asio::ip::tcp::socket;