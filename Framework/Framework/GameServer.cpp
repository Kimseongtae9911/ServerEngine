#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include "CoreMacro.h"
#include "ThreadManager.h"
#include "Memory.h"

class Knight
{
public:
    int64 hp = 0;
};

class Monster
{
public:
    int64 id = 0;
};

int main()
{
    Knight* knights[100];

    for (int i = 0; i < 100; ++i) {
        knights[i] = ObjectPool<Knight>::PopObject();
    }

    for (int i = 0; i < 100; ++i) {
        ObjectPool<Knight>::PushObject(knights[i]);
        knights[i] = nullptr;
    }
    std::shared_ptr<Knight> ptr = ObjectPool<Knight>::MakeShared();

    Knight* k = ObjectPool<Knight>::PopObject();
    ObjectPool<Knight>::PushObject(k);

    return 0;
}