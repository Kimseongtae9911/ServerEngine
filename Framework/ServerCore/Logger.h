#pragma once

enum class LogLevel : std::uint8_t
{
	NONE = 0,
	INFO = 1,
	DEBUG = 2,
	WARNING = 3,
	FATAL = 4,
};

class Logger : public TSingleton<Logger>
{
	friend class TSingleton<Logger>;
public:
	bool Initialize() override final;

	template<typename... Args>
	void WriteLogToFile(LogLevel _level, std::string_view _msg, const Args&... _args);

	template<typename... Args>
	void WriteLogToConsole(const std::string& _msg, const Args&... _args) { WriteLogToConsole(LogLevel::NONE, _msg, _args...); }

	template<typename... Args>
	void WriteLogToConsole(LogLevel _level, const std::string& _msg, const Args&... _args);

	void WriteLogToConsole(const std::string& _msg);

	Logger() { if (!Initialize()) { throw std::runtime_error("Logger initailization fail"); } }

	template<typename... Args>
	std::string ParseLogMsg(std::string_view _msg, const Args&... _args);
};

template<typename ...Args>
inline void Logger::WriteLogToFile(LogLevel _level, std::string_view _msg, const Args & ..._args)
{
	switch (_level)
	{
	case LogLevel::INFO:
		BOOST_LOG_TRIVIAL(info) << ParseLogMsg(_msg, _args...);
		break;
	case LogLevel::DEBUG:
		BOOST_LOG_TRIVIAL(debug) << ParseLogMsg(_msg, _args...);
		break;
	case LogLevel::WARNING:
		BOOST_LOG_TRIVIAL(warning) << ParseLogMsg(_msg, _args...);
		break;
	case LogLevel::FATAL:
		BOOST_LOG_TRIVIAL(fatal) << ParseLogMsg(_msg, _args...);
		break;
	default:
		break;
	}
}

template<typename ...Args>
inline void Logger::WriteLogToConsole(LogLevel _level, const std::string& _msg, const Args & ..._args)
{
	switch (_level)
	{
	case LogLevel::INFO:
		std::cout << "[INFO] ";
		break;
	case LogLevel::DEBUG:
		std::cout << "[DEBUG] ";
		break;
	case LogLevel::WARNING:
		std::cout << "[WARNING] ";
		break;
	case LogLevel::FATAL:
		std::cout << "[FATAL] ";
		break;
	default:
		break;
	}

	std::cout << ParseLogMsg(_msg, _args...) << std::endl;
}

template<typename ...Args>
inline std::string Logger::ParseLogMsg(std::string_view _msg, const Args & ..._args)
{
	return std::vformat(_msg, std::make_format_args(_args...));
}