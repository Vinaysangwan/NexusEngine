#include "Logger.h"

#include <chrono>
#include <ctime>
#include <format>
#include <iostream>
#include <Windows.h>
#include <sstream>

constexpr WORD GREEN = 2;
constexpr WORD RED = 4;
constexpr WORD YELLOW = 6;
constexpr WORD WHITE = 7;

namespace NEXUS_LOGGER
{
  template <typename... Args>
  inline void Logger::Log(const std::string &msg, Args &&...args)
  {
    assert(_Initialized && "The logger must be initialized before it is used!");

    std::stringstream ss;
    ss << "NEXUS [INFO]: " << CurrentDateTime() << " - "<<std::vformat(msg, std::make_format_args(args...))<<"\n";

    if (_ConsoleLog)
    {
      HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
      SetConsoleTextAttribute(hConsole, GREEN);
      std::cout<<ss.str();
      SetConsoleTextAttribute(hConsole, WHITE);
    }

    if (_RetainLogs)
    {
      _LogEntries.emplace_back(LogEntry::LogType::INFO, ss.str());
    }
  }

  template <typename... Args>
  inline void Logger::Warn(const std::string &msg, Args &&...args)
  {
    assert(_Initialized && "The logger must be initialized before it is used!");

    std::stringstream ss;
    ss << "NEXUS [WARN]: " << CurrentDateTime() << " - "<<std::vformat(msg, std::make_format_args(args...))<<"\n";

    if (_ConsoleLog)
    {
      HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
      SetConsoleTextAttribute(hConsole, YELLOW);
      std::cout<<ss.str();
      SetConsoleTextAttribute(hConsole, WHITE);
    }

    if (_RetainLogs)
    {
      _LogEntries.emplace_back(LogEntry::LogType::WARN, ss.str());
    }
  }

  template <typename... Args>
  inline void Logger::Error(std::source_location location, const std::string &msg, Args &&...args)
  {
    assert(_Initialized && "The logger must be initialized before it is used!");

    std::stringstream ss;
    ss << "NEXUS [ERROR]: " << CurrentDateTime() << " - "<<std::vformat(msg, std::make_format_args(args...))<<
    "\nFUNC: " << location.function_name()<<
    "\nLINE: "<<location.line()<<"\n\n";

    if (_ConsoleLog)
    {
      HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
      SetConsoleTextAttribute(hConsole, RED);
      std::cout<<ss.str();
      SetConsoleTextAttribute(hConsole, WHITE);
    }

    if (_RetainLogs)
    {
      _LogEntries.emplace_back(LogEntry::LogType::ERR, ss.str());
    }
  }
}
