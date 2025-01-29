#pragma once

#include "CorePch.h"
#include "Enum.pb.h"

#ifdef _DEBUG
#pragma comment(lib, "Debug\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Debug\\libprotobufd.lib")
#else
#pragma comment(lib, "Release\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Release\\libprotobuf.lib")
#endif

using GameSessionRef = std::shared_ptr<class GameSession>;
using PlayerRef = std::shared_ptr<class Player>;