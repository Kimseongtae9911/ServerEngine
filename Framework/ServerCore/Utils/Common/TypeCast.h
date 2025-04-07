#pragma once

/*
	todo: release모드에서 성능 재확인 필요

	컴파일 타임 다이나믹 캐스트
	사용이 조금 귀찮다..

	클래스 상속관계에 따른 TypeList선언, 최상단 클래스에 USE_TL(TLName) 매크로 사용
	이후 모든 클래스의 생성자에 INIT_TL(ClassName); 매크로 사용
*/


#pragma region TypeList
/*
	클래스 상속 구조에서 모든 클래스 Type이
	TypeList를 통해 이어지도록 한다
*/

template<typename... T>
struct TypeList;

template<typename T, typename U>
struct TypeList<T, U>
{
	using Head = T;
	using Tail = U;
};

template<typename T, typename... U>
struct TypeList<T, U...>
{
	using Head = T;
	using Tail = TypeList<U...>;
};
#pragma endregion

/*
====================================================================================
*/

#pragma region Length
/*
	enum을 통해 값을 표한한 이유는 컴파일 타임에 값이 정해지도록 하기위해
*/

template<typename T>
struct Length;

template<>
struct Length<TypeList<>>
{
	enum { value = 0 };
};

template<typename T, typename... U>
struct Length<TypeList<T, U...>>
{
	enum { value = 1 + Length<TypeList<U...>>::value };
};
#pragma endregion

/*
====================================================================================
*/

#pragma region TypeAt
/*
	index에 해당하는 TypeList의 Type은 무엇인지 알아내기
*/
template<typename TL, int32 index>
struct TypeAt;

template<typename Head, typename... Tail>
struct TypeAt<TypeList<Head, Tail...>, 0>
{
	using Result = Head;
};

template<typename Head, typename... Tail, int32 index>
struct TypeAt<TypeList<Head, Tail...>, index>
{
	using Result = typename TypeAt<TypeList<Tail...>, index -1>::Result;
};
#pragma endregion

/*
====================================================================================
*/

#pragma region IndexOf
/*
	TypeList 내의 클래스가 몇번째 인덱스에 속하는지
*/

template<typename TL, typename T>
struct IndexOf;

template<typename... Tail, typename T>
struct IndexOf<TypeList<T, Tail...>, T>
{
	enum { value = 0 };
};

template<typename T>
struct IndexOf<TypeList<>, T>
{
	enum { value = -1 };
};

template<typename Head, typename... Tail, typename T>
struct IndexOf<TypeList<Head, Tail...>, T>
{
private:
	enum { temp = IndexOf<TypeList<Tail...>, T>::value };

public:
	enum { value = (temp == -1) ? -1 : temp + 1 };
};
#pragma endregion

/*
====================================================================================
*/

#pragma region Conversion
/*
	From이 To로 변환이 가능하다면 Success을 리턴할 것
	From이 To로 변환이 불가하다면 Fail를 리턴할 것
*/

template<typename From, typename To>
class Conversion
{
private:
	using Success = int8;
	using Fail = int32;

	static Success Convert(const To&) { return 0; }
	static Fail Convert(...) { return 0; }
	static From MakeFrom() { return 0; }

public:
	enum {
		exists = sizeof(Convert(MakeFrom())) == sizeof(Success)
	};
};
#pragma endregion

/*
====================================================================================
*/

#pragma region TypeCast
/*
	컴파일 타임에 상수가 정해져야하므로 기존의 for문을 돌릴 수 없음
	따라서 템플릿을 통해 for문을 돌리는 꼼수..
*/
template<uint8 v>
struct Int2Type
{
	enum { value = v };
};


template<typename TL>
class TypeConversion
{
public:
	static inline bool CanConvert(uint8 _from, uint8 _to) {
		static TypeConversion conversion;
		return s_convert[_from][_to];
	}

private:
	enum {
		length = Length<TL>::value
	};

	TypeConversion()
	{
		MakeTable(Int2Type<0>(), Int2Type<0>());
	}

	template<uint8 i, uint8 j>
	static void MakeTable(Int2Type<i>, Int2Type<j>) {
		using FromType = typename TypeAt<TL, i>::Result;
		using ToType = typename TypeAt<TL, j>::Result;

		if (Conversion<const FromType*, const ToType*>::exists)
			s_convert[i][j] = true;
		else
			s_convert[i][j] = false;

		MakeTable(Int2Type<i>(), Int2Type<j + 1>());
	}

	template<uint8 i>
	static void MakeTable(Int2Type<i>, Int2Type<length>) {
		MakeTable(Int2Type<i + 1>(), Int2Type<0>());
	}

	template<uint8 j>
	static void MakeTable(Int2Type<length>, Int2Type<j>) {}

public:
	static bool s_convert[length][length];
};

template<typename TL>
bool TypeConversion<TL>::s_convert[length][length];

template<typename To, typename From>
To TypeCast(From* ptr) {
	if (ptr == nullptr)
		return nullptr;

	using TL = typename From::TL;

	if (TypeConversion<TL>::CanConvert(ptr->_typeId, IndexOf<TL, std::remove_pointer_t<To>>::value))
		return static_cast<To>(ptr);

	return nullptr;
}

template<typename To, typename From>
std::shared_ptr<To> TypeCast(std::shared_ptr<From> _ptr) {
	if (_ptr == nullptr)
		return nullptr;

	using TL = typename From::TL;

	if (TypeConversion<TL>::CanConvert(_ptr->_typeId, IndexOf<TL, std::remove_pointer_t<To>>::value))
		return std::static_pointer_cast<To>(_ptr);

	return nullptr;
}

template<typename To, typename From>
bool CanCast(From* ptr) {
	if (ptr == nullptr)
		return false;

	using TL = typename From::TL;
	return TypeConversion<TL>::CanConvert(ptr->_typeId, IndexOf<TL, std::remove_pointer_t<To>>::value);
}

template<typename To, typename From>
bool CanCast(std::shared_ptr<From> ptr) {
	if (ptr == nullptr)
		return false;

	using TL = typename From::TL;
	return TypeConversion<TL>::CanConvert(ptr->_typeId, IndexOf<TL, std::remove_pointer_t<To>>::value);
}
#pragma endregion

#define USE_TL(TypeList) public: using TL = TypeList; uint8 _typeId;
#define INIT_TL(Type) _typeId = IndexOf<TL, Type>::value;

/*
===========================================================================사용 예제===========================================================================
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
	{
		Player* player = new Player();
		Player* knight = new Knight();

		bool canCast = CanCast<Knight*>(player);
		Knight* k = TypeCast<Knight*>(player);

		bool canCast2 = CanCast<Player*>(knight);
		Player* p = TypeCast<Player*>(knight);

		delete player;
		delete knight;
	}

	{
		std::shared_ptr<Player> player = std::make_shared<Player>();
		std::shared_ptr<Player> knight = std::make_shared<Knight>();

		bool canCast = CanCast<Knight>(player);
		auto k = TypeCast<Knight>(player);

		bool canCast2 = CanCast<Player>(knight);
		auto p = TypeCast<Player>(knight);
	}
}
*/