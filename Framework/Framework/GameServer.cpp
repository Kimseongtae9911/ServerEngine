#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include "CoreMacro.h"
#include "ThreadManager.h"

CoreGlobal Core;

void ThreadMain()
{
	while (true)
	{
		std::cout << "Hello! I am thread... " << LThreadId << std::endl;
		std::this_thread::sleep_for(TimeSec(1));
	}
}

int main()
{
	for (int32 i = 0; i < 5; ++i)
		GThreadManager->RunThreads(ThreadMain);

	GThreadManager->JoinThreads();
}