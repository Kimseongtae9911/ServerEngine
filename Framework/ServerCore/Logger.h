#pragma once

//파일 로그 매크로
#define LDebug(msg, ...)		Logger::GetInstance()->WriteLogToFile(LogLevel::Debug, msg, ##__VA_ARGS__)
#define LInfo(msg, ...)		Logger::GetInstance()->WriteLogToFile(LogLevel::Info, msg, ##__VA_ARGS__)
#define LWarning(msg, ...)	Logger::GetInstance()->WriteLogToFile(LogLevel::Warning, msg, ##__VA_ARGS__)
#define LError(msg, ...)		Logger::GetInstance()->WriteLogToFile(LogLevel::Error, msg, ##__VA_ARGS__)
#define LFatal(msg, ...)		Logger::GetInstance()->WriteLogToFile(LogLevel::Fatal, msg, ##__VA_ARGS__)

//콘솔 로그 매크로
#define CLLog(msg)		Logger::GetInstance()->WriteLogToConsole(msg)
#define CLDebug(msg, ...)	Logger::GetInstance()->WriteLogToConsole(LogLevel::Debug, msg, ##__VA_ARGS__)
#define CLInfo(msg, ...)		Logger::GetInstance()->WriteLogToConsole(LogLevel::Info, msg, ##__VA_ARGS__)
#define CLWarning(msg, ...)	Logger::GetInstance()->WriteLogToConsole(LogLevel::Warning, msg, ##__VA_ARGS__)
#define CLError(msg, ...)	Logger::GetInstance()->WriteLogToConsole(LogLevel::Error, msg, ##__VA_ARGS__)
#define CLFatal(msg, ...)	Logger::GetInstance()->WriteLogToConsole(LogLevel::Fatal, msg, ##__VA_ARGS__)

enum class LogLevel : std::uint8_t
{
	None = 0,
	Debug = 1,		//디버그용
	Info = 2,		//정보제공
	Warning = 3,	//발생가능, 많이 발생하면 확인필요
	Error = 4,		//발생하면안됨
	Fatal = 5,		//서버 크래시
};

class Logger : public TSingleton<Logger>
{
	friend class TSingleton<Logger>;
public:
	bool Initialize() override final;
	bool Release();

	template<typename... Args>
	void WriteLogToFile(LogLevel _level, std::string_view _msg, const Args&... _args);

	template<typename... Args>
	void WriteLogToConsole(const std::string& _msg, const Args&... _args) { WriteLogToConsole(LogLevel::None, _msg, _args...); }

	template<typename... Args>
	void WriteLogToConsole(LogLevel _level, const std::string& _msg, const Args&... _args);

	void WriteLogToConsole(const std::string& _msg);

	template<typename... Args>
	std::string ParseLogMsg(std::string_view _msg, const Args&... _args);

private:
	void PushLog(std::string _msg) { m_logQueue.push(_msg); }

private:
	std::ofstream m_logFile;
	LogLevel m_currentLogLevel = LogLevel::Info;
	tbb::concurrent_queue<std::string> m_logQueue;
	std::mutex m_consoleLock;
	std::atomic_bool m_isRunning = false;
};

template<typename ...Args>
inline void Logger::WriteLogToFile(LogLevel _level, std::string_view _msg, const Args & ..._args)
{
	if (_level < m_currentLogLevel)
		return;

	auto now = std::chrono::system_clock::now();
	auto now_c = std::chrono::system_clock::to_time_t(now);
	std::tm tm;
	localtime_s(&tm, &now_c);

	std::stringstream timeStream;
	timeStream << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");

	std::string levelStr = [&]() {
		switch (_level) {
		case LogLevel::Info: return "INFO";
		case LogLevel::Debug: return "DEBUG";
		case LogLevel::Warning: return "WARNING";
		case LogLevel::Error: return "ERROR";
		case LogLevel::Fatal: return "FATAL";
		default: return "UNKNOWN";
		}
		}();

	std::string logMessage = std::format("[{}] [{}] {}", timeStream.str(), levelStr, ParseLogMsg(_msg, _args...));

	PushLog(std::move(logMessage));
}

template<typename ...Args>
inline void Logger::WriteLogToConsole(LogLevel _level, const std::string& _msg, const Args & ..._args)
{
	std::unique_lock<std::mutex> lock(m_consoleLock);

	switch (_level)
	{
	case LogLevel::Info:
		std::cout << "[INFO] ";
		break;
	case LogLevel::Debug:
		std::cout << "[DEBUG] ";
		break;
	case LogLevel::Warning:
		std::cout << "[WARNING] ";
		break;
	case LogLevel::Error:
		std::cout << "[ERROR] ";
		break;
	case LogLevel::Fatal:
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