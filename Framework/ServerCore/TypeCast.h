#pragma once

/*
	todo: release��忡�� ���� ��Ȯ�� �ʿ�

	������ Ÿ�� ���̳��� ĳ��Ʈ
	����� ���� ������..

	Ŭ���� ��Ӱ��迡 ���� TypeList����, �ֻ�� Ŭ������ USE_TL(TLName) ��ũ�� ���
	���� ��� Ŭ������ �����ڿ� INIT_TL(ClassName); ��ũ�� ���
*/


#pragma region TypeList
/*
	Ŭ���� ��� �������� ��� Ŭ���� Type��
	TypeList�� ���� �̾������� �Ѵ�
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
	enum�� ���� ���� ǥ���� ������ ������ Ÿ�ӿ� ���� ���������� �ϱ�����
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
	index�� �ش��ϴ� TypeList�� Type�� �������� �˾Ƴ���
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
	TypeList ���� Ŭ������ ���° �ε����� ���ϴ���
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
	From�� To�� ��ȯ�� �����ϴٸ� Success�� ������ ��
	From�� To�� ��ȯ�� �Ұ��ϴٸ� Fail�� ������ ��
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
	������ Ÿ�ӿ� ����� ���������ϹǷ� ������ for���� ���� �� ����
	���� ���ø��� ���� for���� ������ �ļ�..
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
===========================================================================��� ����===========================================================================
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