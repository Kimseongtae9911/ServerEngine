#include "pch.h"
#include "Logger.h"

bool Logger::Initialize()
{
    try {
        std::string filePath = "../out/log/";

        auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::tm tm;
        localtime_s(&tm, &now);

        std::stringstream currentTime;
        currentTime << std::put_time(&tm, "%H%M%S");

        std::string fileName = filePath + "logfile_" + currentTime.str() + ".log";

        m_logFile.open(fileName, std::ios::app);

        if (!m_logFile.is_open())
        {
            throw std::runtime_error("Failed to open log file.");
        }

        return true;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Fail to Initialize Logger: " << e.what() << std::endl;
        return false;
    }
}

void Logger::WriteLogToConsole(const std::string& _msg)
{
	std::cout << _msg << std::endl;
}