#include "Logger/Logger.h"

namespace NEXUS_LOGGER
{
  Logger &Logger::GetInstance()
  {
    static Logger instance {};
    return instance;
  }

  void Logger::Init(bool consoleLog, bool retainLogs)
  {
    assert(!_Initialized && "Do not call Initialize more than once!");
    if (_Initialized)
    {
      std::cout<<"Logger had already been Initialized"<<std::endl;
    }

    _ConsoleLog = consoleLog;
    _RetainLogs = retainLogs;
    _Initialized = true;
  }

  std::string Logger::CurrentDateTime()
  {
    auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    char buf[30];
    ctime_s(buf, sizeof(buf), &time);

    LogTime logTime{std::string(buf)};

    return std::format("{}-{}-{} {}", logTime.year, logTime.month, logTime.dayNumber, logTime.time);
  }

  Logger::LogTime::LogTime(const std::string &date)
    : day{date.substr(0, 3)}, dayNumber{date.substr(8, 2)}, month{date.substr(4, 3)},
      year{date.substr(20, 4)}, time{date.substr(11, 8)}
  {
  }
}

