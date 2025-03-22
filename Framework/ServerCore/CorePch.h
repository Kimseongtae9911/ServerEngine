#pragma once

//boost
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "CoreMacro.h"
#include "Container.h"
#include "CoreTLS.h"
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

#include "ErrorCode.h"
#include "Singleton.h"
#include "Logger.h"

#include "Lock.h"
#include "MemoryManager.h"

#include "ObjectPool.h"
#include "TypeCast.h"
#include "SendBuffer.h"
#include "Session.h"
#include "JobSerializer.h"

#include "packet.h"