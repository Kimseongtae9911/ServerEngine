#pragma once

/*
�ش� Ŭ������ ��ӹ޾ƾ� �ϱ� ������ �ܺ� ���̺귯������ ����� �� ����
�ܺ� ���̺귯���� ����ϱ� ���ؼ��� �ڵ� ���� �ʿ�
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
RefCounter���� AddRef�� �ϴ� ���� �ٸ� �����忡�� ���� �� �ִ�
�� �������� ���� ��۸� �����͸� �����ϱ� ���� ���� Ŭ����
*/
template<typename T>
class TSharedPtr
{
public:
	TSharedPtr() {}
	TSharedPtr(T* _ptr) { SetPtr(_ptr); }

	TSharedPtr(const TSharedPtr& _rhs) { SetPtr(_rhs.m_ptr); }
	TSharedPtr(TSharedPtr&& _rhs) { m_ptr = _rhs.m_ptr; _rhs.m_ptr = nullptr; }

	template<typename U>	// ��� ���� ����
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