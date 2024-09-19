#include "pch.h"
#include "ThreadManager.h"
#include "Memory.h"
#include "NetworkCore.h"

int main()
{
    if (false == Logger::GetInstance()->Initialize())
    {
        ASSERT_CRASH("Logger Initailize Fail");
    }

    CLLog("Logger Initialized");

    NetworkCore server;
    server.RunObject();        

    GThreadManager->JoinThreads();    
}