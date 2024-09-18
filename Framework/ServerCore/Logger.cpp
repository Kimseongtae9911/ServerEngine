#include "pch.h"
#include "Logger.h"

bool Logger::Initialize()
{
	try {
		std::string filPath = "../out/log/";

		auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		std::tm tm;
		localtime_s(&tm, &now);

		std::stringstream currentTime;
		currentTime << std::put_time(&tm, "%H%M%S");

		std::string fileName = filPath + "logfile_" + currentTime.str() + ".log";

		boost::log::add_file_log(
			boost::log::keywords::file_name = fileName,
			boost::log::keywords::open_mode = std::ios::app
		);

		boost::log::core::get()->set_filter(
			boost::log::trivial::severity >= boost::log::trivial::info
		);

		boost::log::add_common_attributes();

		return true;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Fail to Initailize Logger: " << e.what() << std::endl;
		return false;
	}
}

void Logger::WriteLogToConsole(const std::string& _msg)
{
	std::cout << _msg << std::endl;
}