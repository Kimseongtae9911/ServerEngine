#include "pch.h"
#include "ThreadManager.h"
#include "Memory.h"
#include "NetworkCore.h"

int main()
{
    NetworkCore server;
    server.RunObject();        

    GThreadManager->JoinThreads();    
}