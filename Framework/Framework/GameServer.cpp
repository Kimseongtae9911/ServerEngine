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
		//std::cout << "Knight()" << std::endl;
	}

	~Knight() {
		//std::cout << "~Knight()" << std::endl;
	}
};

uint64 numIterations = 100;

void TestCustomAllocator()
{
    for (int32 i = 0; i < 5; ++i) {
        GThreadManager->RunThreads([]() {
            for (int j = 0; j < numIterations; ++j) {
                StVector<Knight> v(10);
                StMap<int32, Knight> m;
                m[100] = Knight();
            }
            });
    }

    GThreadManager->JoinThreads();
}

void TestSTLAllocator()
{
    for (int32 i = 0; i < 5; ++i) {
        GThreadManager->RunThreads([]() {
            for (int j = 0; j < numIterations; ++j) {
                std::vector<Knight> v(10);
                std::map<int32, Knight> m;
                m[100] = Knight();
            }
            });
    }

    GThreadManager->JoinThreads();
}

int main()
{
    for (int i = 0; i < 10; ++i) {
        std::cout << "Test Case#" << std::to_string(i + 1) <<", AllocateNum : " << numIterations << std::endl;
        std::cout << "Start Custom Allocator" << std::endl;
        auto start = std::chrono::high_resolution_clock::now();
        TestCustomAllocator();
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        std::cout << "Custom Allocator Duration: " << duration.count() << " seconds" << std::endl;

        std::cout << "Start STL Allocator" << std::endl;
        start = std::chrono::high_resolution_clock::now();
        TestSTLAllocator();
        end = std::chrono::high_resolution_clock::now();
        duration = end - start;
        std::cout << "STL Allocator Duration: " << duration.count() << " seconds" << std::endl;
        std::cout << "============================================================" << std::endl << std::endl;
        numIterations *= 10;
    }

    return 0;
}