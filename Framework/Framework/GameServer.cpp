#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include "CoreMacro.h"
#include "ThreadManager.h"
#include "Memory.h"

class Knight
{
public:
	Knight() {
		std::cout << "Knight()" << std::endl;
	}

	~Knight() {
		std::cout << "~Knight()" << std::endl;
	}
};

int main() 
{
	for (int32 i = 0; i < 5; ++i) {
		GThreadManager->RunThreads([]() {
			while (true) {
				StVector<Knight> v(10);

				StMap<int32, Knight> m;
				m[100] = Knight();

				std::this_thread::sleep_for(TimeMS(10));
			}
			});
	}
	
	GThreadManager->JoinThreads();
}