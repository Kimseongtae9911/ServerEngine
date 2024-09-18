#include "pch.h"
#include "ThreadManager.h"
#include "Memory.h"
#include "NetworkCore.h"

int main()
{
    Logger::GetInstance()->Initialize();

    NetworkCore server;
    server.RunObject();        

    GThreadManager->JoinThreads();    
}