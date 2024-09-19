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

private:
	std::ofstream m_logFile;
	LogLevel m_currentLogLevel = LogLevel::INFO;
};

template<typename ...Args>
inline void Logger::WriteLogToFile(LogLevel _level, std::string_view _msg, const Args & ..._args)
{
	if (_level < m_currentLogLevel)
		return;

	if (!m_logFile.is_open())
		return;

	auto now = std::chrono::system_clock::now();
	auto now_c = std::chrono::system_clock::to_time_t(now);
	std::tm tm;
	localtime_s(&tm, &now_c);

	// 시간 문자열 생성
	std::stringstream timeStream;
	timeStream << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");

	std::stringstream logStream;
	std::string levelStr;

	switch (_level)
	{
	case LogLevel::INFO:
		levelStr = "INFO";
		break;
	case LogLevel::DEBUG:
		levelStr = "DEBUG";
		break;
	case LogLevel::WARNING:
		levelStr = "WARNING";
		break;
	case LogLevel::FATAL:
		levelStr = "FATAL";
		break;
	default:
		levelStr = "UNKNOWN";
		break;
	}
	logStream << "[" << timeStream.str() << "] [" << levelStr << "] " << ParseLogMsg(_msg, _args...);

	m_logFile << logStream.str() << std::endl;
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