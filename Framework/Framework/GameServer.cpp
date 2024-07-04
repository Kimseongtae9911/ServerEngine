#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include "CoreMacro.h"
#include "ThreadManager.h"
#include "Memory.h"
#include "ObjectPool.h"

using PlayerTL = TypeList<class Player, class Mage, class Knight, class Archer>;

class Player
{
    USE_TL(PlayerTL)
public:
    Player()
    {
        INIT_TL(Player);
    }
    virtual ~Player() {}

    int m_hp = 0;
};

class Knight : public Player
{   
public:
    Knight()
    {
        INIT_TL(Knight);
    }
    virtual ~Knight() {}
};

class Mage : public Player
{
public:
    Mage()
    {
        INIT_TL(Mage);
    }
    virtual ~Mage() {}
};

class Archer : public Player
{
public:
    Archer()
    {
        INIT_TL(Archer);
    }
    virtual ~Archer() {}
};

int main()
{
    int castingNum = 100000;

    Knight* knight = new Knight();
    Mage* mage = new Mage();
    Archer* archer = new Archer();

    for (int j = 0; j < 4; ++j) {
        {
            std::cout << "Casting Num : " << castingNum << std::endl;
            std::cout << "Static Cast" << std::endl;
            auto startTime = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < castingNum; ++i) {
                
                Player* p1 = static_cast<Player*>(knight);
                Player* p2 = static_cast<Player*>(mage);
                Player* p3 = static_cast<Player*>(archer);
                p1->m_hp = 10 + i;
                p2->m_hp = 20 + i;
                p3->m_hp = 30 + i;

            }
            auto endTime = std::chrono::high_resolution_clock::now();

            std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << "ms" << std::endl;
            std::cout << "===================================" << std::endl;
        }
        {
            std::cout << "Reinterpret Cast" << std::endl;
            auto startTime = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < castingNum; ++i) {
                Player* p1 = reinterpret_cast<Player*>(knight);
                Player* p2 = reinterpret_cast<Player*>(mage);
                Player* p3 = reinterpret_cast<Player*>(archer);
                p1->m_hp = 10 + i;
                p2->m_hp = 20 + i;
                p3->m_hp = 30 + i;
            }
            auto endTime = std::chrono::high_resolution_clock::now();

            std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << "ms" << std::endl;
            std::cout << "===================================" << std::endl;
        }
        {
            std::cout << "Dynamic Cast" << std::endl;
            auto startTime = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < castingNum; ++i) {
                Player* p1 = dynamic_cast<Player*>(knight);
                Player* p2 = dynamic_cast<Player*>(mage);
                Player* p3 = dynamic_cast<Player*>(archer);
                p1->m_hp = 10 + i;
                p2->m_hp = 20 + i;
                p3->m_hp = 30 + i;
            }
            auto endTime = std::chrono::high_resolution_clock::now();

            std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << "ms" << std::endl;
            std::cout << "===================================" << std::endl;
        }
        {
            std::cout << "Custom Cast" << std::endl;
            auto startTime = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < castingNum; ++i) {
                Player* p1 = TypeCast<Player*>(knight);
                Player* p2 = TypeCast<Player*>(mage);
                Player* p3 = TypeCast<Player*>(archer);
                p1->m_hp = 10 + i;
                p2->m_hp = 20 + i;
                p3->m_hp = 30 + i;
            }
            auto endTime = std::chrono::high_resolution_clock::now();

            std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << "ms" << std::endl;
            std::cout << "===================================" << std::endl;
        }
        castingNum *= 10;
        std::cout << std::endl;
    }

    delete knight;
    delete mage;
    delete archer;
    return 0;
}