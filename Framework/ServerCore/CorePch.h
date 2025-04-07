#pragma once

//boost
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "CoreMacro.h"
#include "Container.h"
#include "Thread/ThreadLocal/CoreTLS.h"
#include "CoreGlobal.h"

#include <iostream>
#include <memory>
#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <format>
#include <functional>
#include <utility>
#include <filesystem>

#include "Thread/Lock.h"
#include "Thread/ThreadManager.h"
#include "Thread/DeadLockProfiler.h"

#include "Utils/Enum/ErrorCode.h"
#include "Utils/Common/Singleton.h"
#include "Utils/Log/Logger.h"
#include "Utils/Common/TypeCast.h"
#include "Utils/Buffer/BufferReader.h"
#include "Utils/Buffer/BufferWriter.h"
#include "Utils/Buffer/RecvBuffer.h"
#include "Utils/Buffer/SendBuffer.h"

#include "Network/Packet/packet.h"
#include "Network/NetAddress.h"
#include "Network/SocketUtils.h"
#include "Network/Acceptor.h"
#include "Network/Service.h"
#include "Network/Timer/Timer.h"

#include "Memory/RefCounter.h"
#include "Memory/MemoryManager.h"
#include "Memory/Pool/MemoryPool.h"
#include "Memory/Pool/ObjectPool.h"

#include "Job/Job.h"
#include "Job/JobQueue.h"
#include "Job/JobSerializer.h"
#include "Network/Session/Session.h"