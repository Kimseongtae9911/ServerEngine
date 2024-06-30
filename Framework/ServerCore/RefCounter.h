#pragma once

/*
해당 클래스를 상속받아야 하기 떄문에 외부 라이브러리에는 사용할 수 없다
외부 라이브러리에 사용하기 위해서는 코드 수정 필요
*/
class RefCounter
{
public:
	RefCounter() {}
	virtual ~RefCounter() {}

	int32 GetRefCount() { return m_refCount; }

	int32 AddRef() { return ++m_refCount; }
	int32 ReleaseRef() {
		int32 refCount = --m_refCount;
		if (refCount == 0)
			delete this;
		return refCount;
	}

protected:
	std::atomic<int32> m_refCount = 1;
};

/*
RefCounter에서 AddRef를 하는 순간 다른 스레드에서 날릴 수 있다
위 현상으로 인한 댕글링 포인터를 방지하기 위한 랩핑 클래스
*/
template<typename T>
class TSharedPtr
{
public:
	TSharedPtr() {}
	TSharedPtr(T* _ptr) { SetPtr(_ptr); }

	TSharedPtr(const TSharedPtr& _rhs) { SetPtr(_rhs.m_ptr); }
	TSharedPtr(TSharedPtr&& _rhs) { m_ptr = _rhs.m_ptr; _rhs.m_ptr = nullptr; }

	template<typename U>	// 상속 관계 복사
	TSharedPtr(const TSharedPtr<U>& _rhs) { SetPtr(static_cast<T*>(_rhs.m_ptr)); }

	~TSharedPtr() { Release(); }

public:
	TSharedPtr& operator=(const TSharedPtr& _rhs) {
		if (m_ptr != _rhs.m_ptr) {
			Release();
			SetPtr(_rhs.m_ptr);
		}
		return *this;
	}

	TSharedPtr& operator=(TSharedPtr&& _rhs) {
		Release();
		SetPtr(_rhs.m_ptr);
		_rhs.Release();
		return *this;
	}

	bool		operator==(const TSharedPtr& _rhs) const { return m_ptr == _rhs.m_ptr; }
	bool		operator==(T* _ptr) const { return m_ptr == _ptr; }
	bool		operator!=(const TSharedPtr& _rhs) const { return m_ptr == _rhs.m_ptr; }
	bool		operator!=(T* _ptr) const { return m_ptr == _ptr; }
	bool		operator<(const TSharedPtr& _rhs) const { return m_ptr < _rhs.m_ptr; }
	T*			operator*() { return m_ptr; }
	const T*	operator*() { return m_ptr; }
				operator T*() const { return m_ptr; }
	T*			operator->() { return m_ptr; }
	const T*	operator->() { return m_ptr; }

	bool IsValid() { return m_ptr != nullptr; }

private:
	void SetPtr(T* _ptr) {
		m_ptr = _ptr;
		if (_ptr)
			_ptr->AddRef();
	}

	void Release() {
		if (m_ptr != nullptr) {
			m_ptr->ReleaseRef();
			m_ptr = nullptr;
		}
	}

private:
	T* m_ptr = nullptr;
};