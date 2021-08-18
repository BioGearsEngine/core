
/**************************************************************************************
Copyright 2015 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the
License at: http://www.apache.org/licenses/LICENSE-2.0 Unless required by
applicable law or agreed to in writing, software distributed under the License
is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
KIND, either express or implied. See the License for the specific language
governing permissions and limitations under the License.
**************************************************************************************/

#include <biogears/cdm/utils/Logger.h>

#include <biogears/io/io-manager.h>

#include <atomic>
#include <cctype>
#include <chrono>
#include <cstdarg>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <istream>
#include <iterator>
#include <ostream>
#include <regex>

#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/filesystem/path.h>

#ifdef ANDROID
#include <android/log.h>
#endif

#ifdef ANDROID
#define forward_to_logcat(LEVEL, CONTENT) __android_log_print(LEVEL, "BIOGEARS", "%s", CONTENT);
#else
#define forward_to_logcat(LEVEL,CONTENT)
#endif

using namespace std::chrono_literals;

namespace biogears {
std::string const Loggable::empty("");
const char* Loggable::empty_cStr("");

constexpr auto TIME_TO_FLUSH = 1s;

using CallStack
  = std::vector<std::function<void(std::ostream& os, std::string const& message, std::string const& origin, Logger::LogLevel priority, SEScalarTime const* simtime, tm const* tp)>>;

void string_handler(std::ostream& os, std::string const& /*message*/, std::string const& str, std::string const& /*origin*/, Logger::LogLevel /*prioriy*/, SEScalarTime const* /*simtime*/, tm const* /*timepoint*/);
void datetime_handler(std::ostream& os, std::string const& message, std::string const& origin, Logger::LogLevel /*prioriy*/, SEScalarTime const* /*simtime*/, tm const* /*timepoint*/);
void origin_handler(std::ostream& os, std::string const& message, std::string const& origin, Logger::LogLevel /*prioriy*/, SEScalarTime const* /*simtime*/, tm const* /*timepoint*/);
void priority_handler(std::ostream& os, std::string const& message, std::string const& origin, Logger::LogLevel /*prioriy*/, SEScalarTime const* /*simtime*/, tm const* /*timepoint*/);
void simseconds_handler(std::ostream& os, std::string const& message, std::string const& origin, Logger::LogLevel /*prioriy*/, SEScalarTime const* /*simtime*/, tm const* /*timepoint*/);
void simtime_handler(std::ostream& os, std::string const& message, std::string const& origin, Logger::LogLevel /*prioriy*/, SEScalarTime const* /*simtime*/, tm const* /*timepoint*/);
void message_handler(std::ostream& os, std::string const& message, std::string const& origin, Logger::LogLevel /*prioriy*/, SEScalarTime const* /*simtime*/, tm const* /*timepoint*/);
void puttime_handler(std::ostream& os, std::string const& format, std::string const& /*message*/, std::string const& /*origin*/, Logger::LogLevel /*prioriy*/, SEScalarTime const* /*simtime*/, tm const* datetime);
void flush_handler(std::ostream& os, std::string const& /*message*/, std::string const& /*origin*/, Logger::LogLevel /*prioriy*/, SEScalarTime const* /*simtime*/, tm const* /*datetime*/);

auto process_message_pattern(std::string pattern) -> CallStack;
auto process_message_pattern(std::string::const_iterator scanner, std::string::const_iterator pattern_end) -> CallStack;

//!
//! Logger Implementation member Functions
//!
//! \brief This is a quick refactor to remove log4cpp
//!        We will likely be loosing some of the current feature sets but I think
//!        longterm we can gain a lot more. Specifcially I plan a future patch that
//!        allos the persistantand volatile streams to be replaced by the user allowing
//!        non file based streaming.
//!
struct Logger::Implementation {

public:
  Implementation();
  Implementation(const IOManager& io);

  LogLevel volatile_filter_level = Logger::eInfo;
  LogLevel persistant_filter_level = Logger::eInfo;
  std::string format_buffer;

  CallStack persistant_message_callstack;
  CallStack volatile_message_callstack;

  std::unique_ptr<std::ostream> persistantStream; //!< Ostream to some Persistant device like a FILE or Database
  std::unique_ptr<std::ostream> volatileStream; //!< Ostream to some volatile device like a console or webserver

  std::shared_ptr<IOManager> io;
  LoggerForward* userDefinedLogger = nullptr;

  SEScalarTime const* time = nullptr;

  bool formatMessages = true;
  bool logToConsole = true;

  std::chrono::steady_clock::time_point last_flush;
};

Logger::Implementation::Implementation()
  : Implementation(IOManager())
{
}
Logger::Implementation::Implementation(IOManager const& t_io)
  : io(new IOManager(t_io))
  , persistant_message_callstack(process_message_pattern(":datetime: <:priority:> :origin: [:simtime:] :message: :endline:"))
  , volatile_message_callstack(process_message_pattern(":datetime: <:priority:> [:simtime:] :message: :endline:"))
  , last_flush(std::chrono::steady_clock::now())

{
  format_buffer.reserve(255);
}

//------------------------------ Token Handler Functions ------------------------------------------------------
void string_handler(std::ostream& os, std::string const& str, std::string const& /*message*/, std::string const& /*origin*/, Logger::LogLevel /*prioriy*/, SEScalarTime const* /*simtime*/, tm const* /*datetime*/)
{
  os << str;
};
void message_handler(std::ostream& os, std::string const& message, std::string const& /*origin*/, Logger::LogLevel /*prioriy*/, SEScalarTime const* /*simtime*/, tm const* /*datetime*/)
{
  os << message;
};
void origin_handler(std::ostream& os, std::string const& /*message*/, std::string const& origin, Logger::LogLevel /*prioriy*/, SEScalarTime const* /*simtime*/, tm const* /*datetime*/)
{
  os << origin;
};
void priority_handler(std::ostream& os, std::string const& /*message*/, std::string const& /*origin*/, Logger::LogLevel prioriy, SEScalarTime const* /*simtime*/, tm const* /*datetime*/)
{
  os << prioriy;
};
void simseconds_handler(std::ostream& os, std::string const& /*message*/, std::string const& /*origin*/, Logger::LogLevel /*prioriy*/, SEScalarTime const* simtime, tm const* /*datetime*/)
{
  if (simtime) {
    os << simtime->GetValue(TimeUnit::s) << "s";
  }
};
void simtime_handler(std::ostream& os, std::string const& /*message*/, std::string const& /*origin*/, Logger::LogLevel /*prioriy*/, SEScalarTime const* simtime, tm const* /*datetime*/)
{
  if (simtime) {
    auto const t_seconds = static_cast<int>(simtime->GetValue(TimeUnit::s));

    auto const minutes = t_seconds / 60;
    auto const hours = minutes / 60;

#if defined(ANDROID)
    std::stringstream ss;
    if (hours) {
      ss << hours % 24 << "h";
      os << ss.str();
      ss.str("");
      ss << minutes % 60 << "m";
      os << ss.str();
      ss.str("");
      ss << t_seconds % 60 << "s";
      os << ss.str();
      ss.str("");
    } else if (minutes) {
      ss << minutes % 60 << "m";
      os << ss.str();
      ss.str("");
      ss << t_seconds % 60 << "s";
      os << ss.str();
      ss.str("");
    } else {
      ss << t_seconds % 60 << "s";
      os << ss.str();
      ss.str("");
    }
#else
    if (hours) {
      os << std::to_string(hours % 24) << "h" << std::to_string(minutes % 60) << "m" << std::to_string(t_seconds % 60) << "s";
    } else if (minutes) {
      os << std::to_string(minutes % 60) << "m" << std::to_string(t_seconds % 60) << "s";
    } else {
      os << std::to_string(t_seconds % 60) << "s";
    }
#endif
  }
};
void datetime_handler(std::ostream& os, std::string const& /*message*/, std::string const& /*origin*/, Logger::LogLevel /*prioriy*/, SEScalarTime const* /*simtime*/, tm const* datetime)
{
  os << std::put_time(datetime, "%Y-%m-%d %H:%M");
};
void puttime_handler(std::ostream& os, std::string const& format, std::string const& /*message*/, std::string const& /*origin*/, Logger::LogLevel /*prioriy*/, SEScalarTime const* /*simtime*/, tm const* datetime)
{
  os << std::put_time(datetime, format.c_str());
};
void flush_handler(std::ostream& os, std::string const& /*message*/, std::string const& /*origin*/, Logger::LogLevel /*prioriy*/, SEScalarTime const* /*simtime*/, tm const* /*datetime*/)
{
  os.flush();
};
// Call Stack Builder
auto process_message_pattern(std::string pattern) -> CallStack
{
  return process_message_pattern(pattern.begin(), pattern.end());
}
auto process_message_pattern(std::string::const_iterator scanner, std::string::const_iterator pattern_end) -> CallStack
{
  CallStack call_stack;

  auto token_start = scanner;
  auto t_current = scanner;
  auto processed = scanner;

  bool time_placeholders_found = false;
  while (scanner != pattern_end) {

    if (*scanner == '{') {
      token_start = scanner;
      t_current = scanner;
      auto token_end = t_current;

      while (token_start != pattern_end) {
        if (*t_current == '}') {
          token_end = t_current;
          break;
        }
        ++t_current;
      }
      if (processed != token_start) {
        call_stack.push_back(std::bind(string_handler, std::placeholders::_1, std::string(processed, token_start), std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6));
        processed = token_start;
      }
      if (token_start != token_end) {
        auto const s = std::string(token_start + 1, token_end);
        call_stack.push_back(std::bind(puttime_handler, std::placeholders::_1, s, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6));
        scanner = token_end;
        processed = scanner + 1;
      }
    }
    if (*scanner == ':') {
      token_start = scanner;
      t_current = token_start + 1;
      auto token_end = t_current;

      while (token_start != pattern_end) {
        if (*t_current == '%') {
          time_placeholders_found = true;
        }
        if (!std::isalpha(*t_current)) {
          token_end = t_current;
          break;
        }
        ++t_current;
      }

      if (token_start != token_end) {
        if (processed != token_start) {
          call_stack.push_back(std::bind(string_handler, std::placeholders::_1, std::string(processed, token_start), std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6));
          processed = token_start;
        }
        auto token = std::string(token_start + 1, token_end);
        if (token == "datetime") {
          call_stack.push_back(datetime_handler);
          scanner = token_end;
          processed = scanner + 1;
        } else if (token == "origin") {
          call_stack.push_back(origin_handler);
          scanner = token_end;
          processed = scanner + 1;
        } else if (token == "priority") {
          call_stack.push_back(priority_handler);
          scanner = token_end;
          processed = scanner + 1;
        } else if (token == "simseconds") {
          call_stack.push_back(simseconds_handler);
          scanner = token_end;
          processed = scanner + 1;
        } else if (token == "simtime") {
          call_stack.push_back(simtime_handler);
          scanner = token_end;
          processed = scanner + 1;
        } else if (token == "message") {
          call_stack.push_back(message_handler);
          scanner = token_end;
          processed = scanner + 1;
        } else if (token == "space") {
          call_stack.push_back(std::bind(string_handler, std::placeholders::_1, " ", std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6));
          scanner = token_end;
          processed = scanner + 1;
        } else if (token == "tab") {
          call_stack.push_back(std::bind(string_handler, std::placeholders::_1, "\t", std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6));
          scanner = token_end;
          processed = scanner + 1;
        } else if (token == "endline" || token == "newline") {
          call_stack.push_back(std::bind(string_handler, std::placeholders::_1, "\n", std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6));
          scanner = token_end;
          processed = scanner + 1;
        } else if (token == "flush") {
          call_stack.push_back(flush_handler);
          scanner = token_end;
          processed = scanner + 1;
        }
      }
    }
    ++scanner;
  }

  if (scanner != pattern_end) {
    call_stack.push_back(std::bind(string_handler, std::placeholders::_1, std::string(scanner, token_start), std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6));
    scanner = pattern_end;
  }
  return call_stack;
};

//!
//! Logger Member Fucntions
//!
////-------------------------------------------------------------------------------
Logger::Logger(std::string const& logFilename)
  : Logger(logFilename, IOManager())
{
}
Logger::Logger(std::string const& logFilename, IOManager const& io)
  : m_impl(std::make_unique<Implementation>(io))
{
  ResetLogFile(logFilename);
}
Logger::Logger(const char* logFilename)
  : Logger(std::string(logFilename), IOManager())
{
}
Logger::Logger(const char* logFilename, IOManager const& io)
  : Logger(std::string(logFilename), io)
{
}
//-------------------------------------------------------------------------
//!  format_messages [IN] -- toggles the volatile stream on and off
//!
void Logger::LogToConsole(bool log_to_console)
{
  m_impl->logToConsole = log_to_console;
}
//-------------------------------------------------------------------------
//!  \input format_messages [IN] -- sets the current state for formating messages
//!
//!  \brief When format messages is false the mesage_pattern for a stream
//!         is ignored the raw text sent is printed
void Logger::FormatMessages(bool format_messages)
{
  m_impl->formatMessages = format_messages;
}
//-------------------------------------------------------------------------
//!  \input filename [IN] -- Destorys the existing persistant logstream and replaces
//!                  it with a filestream to the given filepath. Additionally
//!                  resets the volatile stream to std::cout
void Logger::ResetLogFile(std::string const& filepath)
{

  m_impl->volatileStream = std::make_unique<std::ostream>(std::cout.rdbuf());

  filesystem::path qualified_path = m_impl->io->ResolveLogFileLocation(filepath);
  if (!qualified_path.empty()) {
    filesystem::create_directories(qualified_path.parent_path());

    if (!(filepath.empty() || qualified_path.empty())) {
      auto f = std::make_unique<std::fstream>(qualified_path, std::ios::trunc | std::ios::out);
      if (f->is_open()) {
        m_impl->persistantStream = std::move(f);
      } else {
        m_impl->persistantStream = std::make_unique<std::ostream>(nullptr);
        m_impl->volatileStream->operator<<("Unable to ") << qualified_path << std::endl;
      }
    }
  }
}
//-------------------------------------------------------------------------
//!  \input filename [IN] -- Destorys the existing persistant logstream and replaces
//!                  it with a filestream to the given filepath. Additionally
//!                  resets the volatile stream to std::cout
void Logger::ResetLogFile(const char* filepath)
{
  ResetLogFile(std::string(filepath));
}
//-------------------------------------------------------------------------
//!
//!  Log Deconstructor
//!
Logger::~Logger()
{
}

void Logger::SetLogTime(const SEScalarTime* time) { m_impl->time = time; }
//-------------------------------------------------------------------------
//!  \input LogLevel [IN] -- Lowest level message to be printed to the persistant log stream
//!
//!
void Logger::SetLogLevel(LogLevel logLevel) const
{
  m_impl->persistant_filter_level = logLevel;
}
//-------------------------------------------------------------------------
//!  \input Loglevel [IN] -- Lowest level message to be printed to the volatile log stream
//!
//!
void Logger::SetConsoleLogLevel(LogLevel logLevel) const
{
  m_impl->volatile_filter_level = logLevel;
}
//-------------------------------------------------------------------------
//!  \input Pattern [IN] -- String which will be used to determine the message pattern
//!                  when any of the log message types are called
//!
//!   \brief  The format string can contain any ASCII pattern but the following items are treated for expansion
//!
//! Currently the current long form placeholders are accepted
//!
//! :origin:   If passed it will print out where the message originated
//! :datetime: Equivilant of strftime (%Y-%m-%dT%H:%M)
//! :priority: Message Priority text representation (i.e Warning,Info,Error)
//! :simtime:  Time in the current simulation if known
//! :message:  Test given to the logger when called
//! :endline:  A new Line
//!
//! This function accepts all valid format strings for std::put_time.
//! If the presence of a %[a-zA-Z] is found it will be passed   to std::put_time for final processing.
//! As this might cause a problem for some message formats. the longform alternatives are avaliable and
//! will be expanded over time

void Logger::SetConversionPattern(std::string const& pattern)
{
  m_impl->persistant_message_callstack = process_message_pattern(pattern.begin(), pattern.end());
}
//-------------------------------------------------------------------------
//!  \input Pattern [IN] -- String which will be used to determine the message pattern
//!                  when any of the log message types are called
//!
void Logger::SetConsoleConversionPattern(std::string const& pattern)
{
  m_impl->volatile_message_callstack = process_message_pattern(pattern.begin(), pattern.end());
}
//-------------------------------------------------------------------------
//!  \return LogLevel [IN] -- Lowest level message to be printed to the volatile log stream
//!
//!
auto Logger::GetLogLevel() -> LogLevel
{
  return m_impl->persistant_filter_level;
}
//-------------------------------------------------------------------------
//!  \brief Sets the user defined log foward class
//!         This is often used for system integration where persistant and volitle streams
//!         need to be redirected to complex integration code. Extend LogFoward for this
//!
//!  The Logserver does not take ownership of this object, but it does require it be persistant
//!  for the lifetime of the Logger.
void Logger::SetForward(LoggerForward* forward) { m_impl->userDefinedLogger = forward; }

//-------------------------------------------------------------------------
//!  \return bool - Has a User defined LogFoward Impl been defined to this logger
bool Logger::HasForward() { return m_impl->userDefinedLogger == nullptr ? false : true; }

//-------------------------------------------------------------------------------
//! \brief
//! Currently the current long form placeholders are accepted
//!
//! :datetime: Equivilant of strftime {%Y-%m-%d %H:%M}
//! :priority: Message Priority text representation (i.e Warning,Info,Error)
//! :simtime:  Time in the current simulation if known
//! :message:  Test given to the logger when called
//! :endline:  A new Line
//!
//! This function accepts all valid format strings for std::put_time when wraped in braces {%Y-%m-%d %H:%M}.
//! The entire contents of the {} will be processed through put_time for processing.
//! As this might cause a problem for some message formats. the longform alternatives are avaliable and
//! will be expanded over time

void FormatLogMessage(CallStack callstack,
                      std::string message,
                      Logger::LogLevel priority, std::string origin, SEScalarTime const* simtime, std::ostream& destination)
{

  std::time_t np = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  auto now = std::localtime(&np);
  for (auto& func : callstack) {
    func(destination, message, origin, priority, simtime, now);
  }
}

void Logger::LogMessage(std::istream& msg, std::string const& origin, LogLevel priority) const
{
  auto& impl = *m_impl;

  std::istreambuf_iterator<char> msg_start { msg }, msg_end;
  std::string message = std::string(msg_start, msg_end);

  if (priority <= impl.persistant_filter_level && impl.persistantStream) {
    if (impl.formatMessages) {
      FormatLogMessage(impl.persistant_message_callstack,
                       message,
                       priority, "", impl.time,
                       /*impl.format_buffer,*/ *impl.persistantStream);
    } else {
      *impl.persistantStream << msg.rdbuf();
    }

    auto now = std::chrono::steady_clock::now();
    if (now - impl.last_flush > TIME_TO_FLUSH) {
      impl.last_flush = now;
      impl.persistantStream->flush();
    }
  }
  if (priority <= impl.volatile_filter_level && impl.volatileStream) {
    if (impl.formatMessages) {
      FormatLogMessage(impl.volatile_message_callstack,
                       message,
                       priority, "", impl.time,
                       /*impl.format_buffer,*/ *impl.volatileStream);
    } else {
      *impl.volatileStream << msg.rdbuf();
    }
  }

  if (impl.userDefinedLogger != nullptr) {
    switch (priority) {
    case Logger::eDebug:
      impl.userDefinedLogger->Debug(message.c_str());
      break;
    case Logger::eError:
      impl.userDefinedLogger->Error(message.c_str());
      break;
    case Logger::eException:
      break;
    case Logger::eWarning:
      impl.userDefinedLogger->Warning(message.c_str());
      break;
    case Logger::eInfo:
      impl.userDefinedLogger->Info(message.c_str());      
      break;
    case Logger::eStabilization:
      break;
    }
  }
  switch (priority) {
    case Logger::eDebug:
      forward_to_logcat(ANDROID_LOG_DEBUG, message.c_str());
      break;
    case Logger::eError:
      forward_to_logcat(ANDROID_LOG_ERROR, message.c_str());
      break;
    case Logger::eException:
      break;
    case Logger::eWarning:
      forward_to_logcat(ANDROID_LOG_WARN, message.c_str());
      break;
    case Logger::eInfo:
      forward_to_logcat(ANDROID_LOG_INFO, message.c_str());
      break;
    case Logger::eStabilization:
      break;
    }
}
void Logger::LogMessage(std::istream&& msg, std::string const& origin, LogLevel priority) const
{
  auto& impl = *m_impl;

  std::istreambuf_iterator<char> msg_start { msg }, msg_end;
  std::string message = std::string(msg_start, msg_end);

  if (priority <= impl.persistant_filter_level && impl.persistantStream) {
    if (impl.formatMessages) {
      FormatLogMessage(impl.persistant_message_callstack,
                       message,
                       priority, "", impl.time,
                       /*impl.format_buffer,*/ *impl.persistantStream);
    } else {
      *impl.persistantStream << msg.rdbuf();
    }

    *impl.persistantStream << message;
    auto now = std::chrono::steady_clock::now();
    if (now - impl.last_flush > TIME_TO_FLUSH) {
      impl.last_flush = now;
      impl.persistantStream->flush();
    }
  }
  if (priority <= impl.volatile_filter_level && impl.volatileStream) {
    if (impl.formatMessages) {
      FormatLogMessage(impl.volatile_message_callstack,
                       message,
                       priority, "", impl.time,
                       /*impl.format_buffer,*/ *impl.volatileStream);
    } else {
      *impl.volatileStream << msg.rdbuf();
    }
  }

  if (impl.userDefinedLogger != nullptr) {
    switch (priority) {
    case Logger::eDebug:
      forward_to_logcat(ANDROID_LOG_DEBUG, message.c_str());
      impl.userDefinedLogger->Debug(message.c_str());
      break;
    case Logger::eError:
      forward_to_logcat(ANDROID_LOG_ERROR, message.c_str());
      impl.userDefinedLogger->Error(message.c_str());
      break;
    case Logger::eException:
      break;
    case Logger::eWarning:
      forward_to_logcat(ANDROID_LOG_WARN, message.c_str());
      impl.userDefinedLogger->Warning(message.c_str());
      break;
    case Logger::eInfo:
      forward_to_logcat(ANDROID_LOG_INFO, message.c_str());
      impl.userDefinedLogger->Info(message.c_str());
      break;
    case Logger::eStabilization:
      break;
    }
  }
    switch (priority) {
    case Logger::eDebug:
      forward_to_logcat(ANDROID_LOG_DEBUG, message.c_str());
      break;
    case Logger::eError:
      forward_to_logcat(ANDROID_LOG_ERROR, message.c_str());
      break;
    case Logger::eException:
      break;
    case Logger::eWarning:
      forward_to_logcat(ANDROID_LOG_WARN, message.c_str());
      break;
    case Logger::eInfo:
      forward_to_logcat(ANDROID_LOG_INFO, message.c_str());
      break;
    case Logger::eStabilization:
      break;
    }
}

void Logger::LogMessage(std::string const& msg, std::string const& origin, LogLevel priority) const
{
  auto& impl = *m_impl;
  if (priority <= impl.persistant_filter_level && impl.persistantStream) {
    if (impl.formatMessages) {
      FormatLogMessage(impl.persistant_message_callstack,
                       msg,
                       priority, "", impl.time,
                       /*impl.format_buffer,*/ *impl.persistantStream);
    } else {
      *impl.persistantStream << msg;
    }

    auto now = std::chrono::steady_clock::now();
    if (now - impl.last_flush > TIME_TO_FLUSH) {
      impl.last_flush = now;
      impl.persistantStream->flush();
    }
  }
  if (priority <= impl.volatile_filter_level && impl.volatileStream) {
    if (impl.formatMessages) {
      FormatLogMessage(impl.volatile_message_callstack,
                       msg,
                       priority, "", impl.time,
                       /*impl.format_buffer,*/ *impl.volatileStream);
    } else {
      *impl.volatileStream << msg;
    }
  }

  if (impl.userDefinedLogger != nullptr) {
    switch (priority) {
    case Logger::eDebug:
      forward_to_logcat(ANDROID_LOG_DEBUG, msg.c_str());
      impl.userDefinedLogger->Debug(msg.c_str());
      break;
    case Logger::eError:
      forward_to_logcat(ANDROID_LOG_ERROR, msg.c_str());
      impl.userDefinedLogger->Error(msg.c_str());
      break;
    case Logger::eException:
      break;
    case Logger::eWarning:
      forward_to_logcat(ANDROID_LOG_WARN, msg.c_str());
      impl.userDefinedLogger->Warning(msg.c_str());
      break;
    case Logger::eInfo:
      forward_to_logcat(ANDROID_LOG_INFO, msg.c_str());
      impl.userDefinedLogger->Info(msg.c_str());
      break;
    case Logger::eStabilization:
      break;
    }
  }
    switch (priority) {
    case Logger::eDebug:
      forward_to_logcat(ANDROID_LOG_DEBUG, msg.c_str());
      break;
    case Logger::eError:
      forward_to_logcat(ANDROID_LOG_ERROR, msg.c_str());
      break;
    case Logger::eException:
      break;
    case Logger::eWarning:
      forward_to_logcat(ANDROID_LOG_WARN, msg.c_str());
      break;
    case Logger::eInfo:
      forward_to_logcat(ANDROID_LOG_INFO, msg.c_str());
      break;
    case Logger::eStabilization:
      break;
    }
}
void Logger::LogMessage(std::string&& msg, std::string const& origin, LogLevel priority) const
{
  auto& impl = *m_impl;
  if (priority <= impl.persistant_filter_level && impl.persistantStream) {
    if (impl.formatMessages) {
      FormatLogMessage(impl.persistant_message_callstack,
                       msg,
                       priority, "", impl.time,
                       /*impl.format_buffer,*/ *impl.persistantStream);
    } else {
      *impl.persistantStream << msg;
    }

    auto now = std::chrono::steady_clock::now();
    if (now - impl.last_flush > TIME_TO_FLUSH) {
      impl.last_flush = now;
      impl.persistantStream->flush();
    }
  }
  if (priority <= impl.volatile_filter_level && impl.volatileStream) {
    if (impl.formatMessages) {
      FormatLogMessage(impl.volatile_message_callstack,
                       msg,
                       priority, "", impl.time,
                       /*impl.format_buffer,*/ *impl.volatileStream);
    } else {
      *impl.volatileStream << msg;
    }
  }

  if (impl.userDefinedLogger != nullptr) {
    switch (priority) {
    case Logger::eDebug:
      forward_to_logcat(ANDROID_LOG_DEBUG, msg.c_str());
      impl.userDefinedLogger->Debug(msg.c_str());
      break;
    case Logger::eError:
      forward_to_logcat(ANDROID_LOG_ERROR, msg.c_str());
      impl.userDefinedLogger->Error(msg.c_str());
      break;
    case Logger::eException:
      break;
    case Logger::eWarning:
      forward_to_logcat(ANDROID_LOG_WARN, msg.c_str());
      impl.userDefinedLogger->Warning(msg.c_str());
      break;
    case Logger::eInfo:
      forward_to_logcat(ANDROID_LOG_INFO, msg.c_str());
      impl.userDefinedLogger->Info(msg.c_str());
      break;
    case Logger::eStabilization:
      break;
    }
  }
      switch (priority) {
    case Logger::eDebug:
      forward_to_logcat(ANDROID_LOG_DEBUG, msg.c_str());
      break;
    case Logger::eError:
      forward_to_logcat(ANDROID_LOG_ERROR, msg.c_str());
      break;
    case Logger::eException:
      break;
    case Logger::eWarning:
      forward_to_logcat(ANDROID_LOG_WARN, msg.c_str());
      break;
    case Logger::eInfo:
      forward_to_logcat(ANDROID_LOG_INFO, msg.c_str());
      break;
    case Logger::eStabilization:
      break;
    }
}

void Logger::Debug(std::string const& msg, std::string const& origin) const
{
  LogMessage(msg, origin, eDebug);
}
void Logger::Info(std::string const& msg, std::string const& origin) const
{
  LogMessage(msg, origin, eInfo);
}
void Logger::Warning(std::string const& msg, std::string const& origin) const
{
  LogMessage(msg, origin, eWarning);
}
void Logger::Error(std::string const& msg, std::string const& origin) const
{
  LogMessage(msg, origin, eError);
}
void Logger::Fatal(std::string const& msg, std::string const& origin) const
{
  LogMessage(msg, origin, eFatal);
}

void Logger::Debug(std::istream& msg) const
{
  LogMessage(msg, "", eDebug);
}
void Logger::Info(std::istream& msg) const
{
  LogMessage(msg, "", eInfo);
}
void Logger::Warning(std::istream& msg) const
{
  LogMessage(msg, "", eWarning);
}
void Logger::Error(std::istream& msg) const
{
  LogMessage(msg, "", eError);
}
void Logger::Fatal(std::istream& msg) const
{
  LogMessage(msg, "", eFatal);
}

std::weak_ptr<IOManager> Logger::GetIoManager() const
{
  return m_impl->io;
}
//!
//! Loggable Member Functions
//!
//!
Loggable::Loggable()
  : Loggable(nullptr, "")
{
}
Loggable::Loggable(Logger* logger, std::string const& origin)
  : m_Logger(logger)
  , m_origin(origin)
{
}
Loggable::~Loggable() { }

//!
//! Generic Log functions for loggable which assume the origin is
//! provided at construction
//!
void Loggable::Error(std::string const& message) const
{
  if (m_Logger) {
    m_Logger->Error(message, m_origin);
  }
}
void Loggable::Info(std::string const& message) const
{
  if (m_Logger) {
    m_Logger->Info(message, m_origin);
  }
}
void Loggable::Warning(std::string const& message) const
{
  if (m_Logger) {
    m_Logger->Warning(message, m_origin);
  }
}
void Loggable::Fatal(std::string const& message) const
{
  if (m_Logger) {
    m_Logger->Fatal(message, m_origin);
  }
}
void Loggable::Debug(std::string const& message) const
{
  if (m_Logger) {
    m_Logger->Debug(message, m_origin);
  }
}

//!
//! Log functions which allow overriding the origin provided to the loggable
//! at construction. This is used to help debug problems in our circuits
//! I'm going to replace these with a more generic fprint implimentation
//!
void Loggable::Error(std::string const& message, std::string const& origin) const
{
  if (m_Logger) {
    m_Logger->Error(message, origin);
  }
}
void Loggable::Info(std::string const& message, std::string const& origin) const
{
  if (m_Logger) {
    m_Logger->Info(message, origin);
  }
}
void Loggable::Warning(std::string const& message, std::string const& origin) const
{
  if (m_Logger) {
    m_Logger->Warning(message, origin);
  }
}
void Loggable::Fatal(std::string const& message, std::string const& origin) const
{
  if (m_Logger) {
    m_Logger->Fatal(message, origin);
  }
}
void Loggable::Debug(std::string const& message, std::string const& origin) const
{
  if (m_Logger) {
    m_Logger->Debug(message, origin);
  }
}

//!
//! istream implementation I'm hoping we can replace this with a
//! varadic implementation. Such that all types have a ToString call but
//! for now this is a good work around
//!
void Loggable::Debug(std::istream& msg) const
{
  if (m_Logger) {
    m_Logger->LogMessage(msg, "", Logger::eDebug);
  }
}
void Loggable::Info(std::istream& msg) const
{
  if (m_Logger) {
    m_Logger->LogMessage(msg, "", Logger::eInfo);
  }
}
void Loggable::Warning(std::istream& msg) const
{
  if (m_Logger) {
    m_Logger->LogMessage(msg, "", Logger::eWarning);
  }
}
void Loggable::Error(std::istream& msg) const
{
  if (m_Logger) {
    m_Logger->LogMessage(msg, "", Logger::eError);
  }
}
void Loggable::Fatal(std::istream& msg) const
{
  if (m_Logger) {
    m_Logger->LogMessage(msg, "", Logger::eFatal);
  }
}

void Loggable::Debug(std::istream&& msg) const
{
  if (m_Logger) {
    m_Logger->LogMessage(std::move(msg), "", Logger::eDebug);
  }
}
void Loggable::Info(std::istream&& msg) const
{
  if (m_Logger) {
    m_Logger->LogMessage(std::move(msg), "", Logger::eInfo);
  }
}
void Loggable::Warning(std::istream&& msg) const
{
  if (m_Logger) {
    m_Logger->LogMessage(std::move(msg), "", Logger::eWarning);
  }
}
void Loggable::Error(std::istream&& msg) const
{
  if (m_Logger) {
    m_Logger->LogMessage(std::move(msg), "", Logger::eError);
  }
}
void Loggable::Fatal(std::istream&& msg) const
{
  if (m_Logger) {
    m_Logger->LogMessage(std::move(msg), "", Logger::eFatal);
  }
}

Logger* Loggable::GetLogger() const { return m_Logger; }
}
