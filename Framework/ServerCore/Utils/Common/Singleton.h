#pragma once

template<class T>
class TSingleton
{
public:
	static T* GetInstance() {
		if (nullptr == s_instance)
			s_instance = new T();
		return s_instance;
	}
	~TSingleton() {}

	TSingleton(const TSingleton&) = delete;
	TSingleton* operator=(const TSingleton*) = delete;

	virtual bool Initialize() = 0;

protected:
	TSingleton() {}

	static T* s_instance;
};

template<class T>
T* TSingleton<T>::s_instance = nullptr;