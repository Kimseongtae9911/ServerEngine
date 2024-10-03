#include "pch.h"
#include "ThreadManager.h"
#include "Memory.h"
#include "NetworkCore.h"
#include "Session.h"
#include "Service.h"

class GameTimer : public Timer
{
public:
    void OnTimer() override
    {
        if (nullptr == m_service)
        {
            StopTimer();
            return;
        }

        for (auto session : m_service->GetAllSession())
            session->OnTimer();
    }
};

int main()
{
    if (false == Logger::GetInstance()->Initialize())
    {
        ASSERT_CRASH("Logger Initailize Fail");
    }

    CLLog("Logger Initialized");

    NetworkCore server;
    server.RunObject(CreateSharedObj<GameTimer>());        

    GThreadManager->JoinThreads();    
}