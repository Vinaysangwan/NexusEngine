#pragma once

#include <string>
#include <string_view>
#include <source_location>
#include <vector>
#include <cassert>

#define NEXUS_LOG(x, ...)   NEXUS_LOGGER::Logger::GetInstance().Log(x, __VA_ARGS__)
#define NEXUS_WARN(x, ...)  NEXUS_LOGGER::Logger::GetInstance().Warn(x, __VA_ARGS__)
#define NEXUS_ERROR(x, ...) NEXUS_LOGGER::Logger::GetInstance().Error(std::source_location::current(), x, __VA_ARGS__)
#define NEXUS_INIT_LOGS(console, retain) NEXUS_LOGGER::Logger::GetInstance().Init(console, retain)

namespace NEXUS_LOGGER
{
  struct LogEntry
  {
    enum class LogType
    {
      INFO,
      WARN,
      ERR,
      NONE
    };

    LogType type { LogType::INFO };
    std::string log{""};
  };

  class Logger
  {
  public:
    ~Logger() = default;
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    static Logger& GetInstance();

    template <typename ...Args>
    void Log(const std::string &msg, Args&&... args);

    template <typename ...Args>
    void Warn(const std::string &msg, Args&&... args);

    template <typename ...Args>
    void Error(std::source_location location, const std::string &msg, Args&&... args);

    void Init(bool consoleLog = true, bool retainLogs = true);

  private:
    Logger() = default;

    std::string CurrentDateTime();

  private:
    std::vector<LogEntry> _LogEntries;
    bool _LogAdded{false};
    bool _Initialized{false};
    bool _ConsoleLog{true};
    bool _RetainLogs{true};

    struct LogTime
    {
      std::string day, dayNumber, month, year, time;

      LogTime(const std::string& date);
    };
  };
}

#include "Logger.inl"
