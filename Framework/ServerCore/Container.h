#pragma once

#include "Types.h"
#include "Allocator.h"
#include <array>
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <tbb/concurrent_queue.h>

/*
	STL �����̳ʵ��� allocator�� Ŀ�������� ���
	�Ź� ����Ҷ����� allocator�� �ִ� ���� ����� ������ �����̳ʵ� ����
	���̹� �������� �켱 ������ �ձ��� st..
*/

template<typename Type>
using StVector = std::vector<Type, StlAllocator<Type>>;

template<typename Type>
using StList = std::list<Type, StlAllocator<Type>>;

template<typename Key, typename Type, typename Pred = std::less<Key>>
using StMap = std::map<Key, Type, Pred, StlAllocator<std::pair<Key, Type>>>;

template<typename Key, typename Pred = std::less<Key>>
using StSet = std::set<Key, Pred, StlAllocator<Key>>;

template<typename Type>
using StDeque = std::deque<Type, StlAllocator<Type>>;

template<typename Type, typename Container = StDeque<Type>>
using StQueue = std::queue<Type, Container>;

template<typename Type, typename Container = StDeque<Type>>
using StStack = std::stack<Type, Container>;

template<typename Type, typename Container = StVector<Type>, typename Pred = std::less<typename Container::value_type>>
using StPriorityQueue = std::priority_queue<Type, Container, Pred>;

using StString = std::basic_string<char, std::char_traits<char>, StlAllocator<char>>;
using StWString = std::basic_string<wchar_t, std::char_traits<wchar_t>, StlAllocator<wchar_t>>;

template<typename Key, typename Type, typename Hasher = std::hash<Key>, typename KeyEq = std::equal_to<Key>>
using StUMap = std::unordered_map<Key, Type, Hasher, KeyEq, StlAllocator<std::pair<Key, Type>>>;

template<typename Key, typename Hasher = std::hash<Key>, typename KeyEq = std::equal_to<Key>>
using StUSet = std::unordered_set<Key, Hasher, KeyEq, StlAllocator<Key>>;