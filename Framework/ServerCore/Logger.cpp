#include "pch.h"
#include "Logger.h"
#include "ThreadManager.h"

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
        
        if (!std::filesystem::exists(fileName)) {
            std::filesystem::create_directories(std::filesystem::path(fileName).parent_path());
            std::ofstream createFile(fileName);
            createFile.close();
        }
        
        m_logFile.open(fileName, std::ios::app);


        if (!m_logFile.is_open())
        {
            throw std::runtime_error("Failed to open log file.");
        }
        m_isRunning = true;

        GThreadManager->RunThreads([this]() {
            WriteLogToConsole(LogLevel::Info, "Logger Thread Start. ThreadId={}", LThreadId);
            std::string logMessage;
            while (m_isRunning)
            {
                while (m_logQueue.try_pop(logMessage))
                {
                    m_logFile << logMessage << std::endl;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }

            while (m_logQueue.try_pop(logMessage))
            {
                m_logFile << logMessage << std::endl;
            }
            });

        return true;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Fail to Initialize Logger: " << e.what() << std::endl;
        return false;
    }
}

bool Logger::Release()
{
    m_isRunning = false;
}

void Logger::WriteLogToConsole(const std::string& _msg)
{
    std::unique_lock<std::mutex> lock(m_consoleLock);

	std::cout << _msg << std::endl;
}