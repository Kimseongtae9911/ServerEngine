#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include "CoreMacro.h"
#include "ThreadManager.h"
#include "Memory.h"
#include "ObjectPool.h"

class Knight : public ReuseObject
{
public:
    void ResetObject() override {
        std::cout << "Resetting MyObject" << std::endl;
    }

private:
    void InitObjectImpl() {}

    // ���� ���ڸ� �޴� InitObject ���� (�پ��� ����)
    template<typename... Args>
    void InitObjectImpl(Args&&... args) {
        // ���� ���ڸ� ���
        (std::cout << ... << args) << std::endl;
    }

    int64 hp = 0;
};

int main()
{
    {
        auto k = ReuseObjectPool<Knight>::MakeObject();
    }

    {
        auto k = ReuseObjectPool<Knight>::MakeObject();
    }


    return 0;
}