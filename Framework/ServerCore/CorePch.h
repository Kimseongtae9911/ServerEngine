#pragma once
#define BOOST_LIB_DIAGNOSTIC
#define BOOST_ALL_NO_LIB

//boost
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include <boost/log/core.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/expressions/formatters/date_time.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

#include <boost/date_time/c_local_time_adjustor.hpp>
#include <boost/date_time/local_time/local_time.hpp>

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

#include "Singleton.h"
#include "Logger.h"

#include "Lock.h"
#include "MemoryManager.h"

#include "ObjectPool.h"
#include "TypeCast.h"