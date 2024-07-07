#include "pch.h"
#include "ThreadManager.h"
#include "Memory.h"
#include "NetworkCore.h"

int main()
{
    for (int32 i = 0; i < 6; ++i) {
        GThreadManager->RunThreads([]() {
            NetworkCore server;
            server.RunObject();
            });
    }

    GThreadManager->JoinThreads();    
}