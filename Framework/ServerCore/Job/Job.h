#pragma once

template<typename T = void()>
class Job
{
public:
	using CallbackType = std::function<void()>;

	Job(CallbackType&& _callback) : m_callback(_callback) {}

	template<typename T, typename Ret, typename... Args>
	Job(std::shared_ptr<T> _owner, Ret(T::*_memFunc)(Args...), Args&&... _args)
	{
		m_callback = [_owner, _memFunc, _args...]() {
			(_owner.get()->*_memFunc)(_args...);
			};
	}

	void Execute()
	{
		m_callback();
	}

private:
	CallbackType m_callback;
};

