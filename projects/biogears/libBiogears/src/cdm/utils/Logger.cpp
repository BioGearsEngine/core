
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

namespace biogears {
std::string const Loggable::empty("");
const char* Loggable::empty_cStr("");

constexpr auto MESSAGES_TO_FLUSH = 25;

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

  LogLevel volatile_filter_level = Logger::INFO;
  LogLevel persistant_filter_level = Logger::INFO;
  std::string format_buffer;

  std::string persistant_message_pattern = ":datetime: <:priority:> :origin: [:simtime:] :message: :endline:";
  std::string volatile_message_pattern = ":datetime: <:priority:> [:simtime:] :message: :endline:";

  std::unique_ptr<std::ostream> persistantStream; //!< Ostream to some Persistant device like a FILE or Database
  std::unique_ptr<std::ostream> volatileStream; //!< Ostream to some volatile device like a console or webserver

  std::shared_ptr<IOManager> io;
  LoggerForward* userDefinedLogger = nullptr;

  SEScalarTime const* time = nullptr;

  bool formatMessages = true;
  bool logToConsole = true;

  unsigned int message_count = 0;
};

Logger::Implementation::Implementation()
  : Implementation(IOManager())
{
}
Logger::Implementation::Implementation(IOManager const& t_io)
  : io(new IOManager(t_io))
  , message_count(0)
{
  format_buffer.reserve(255);
}

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
  m_impl->persistant_message_pattern = pattern;
}
//-------------------------------------------------------------------------
//!  \input Pattern [IN] -- String which will be used to determine the message pattern
//!                  when any of the log message types are called
//!
void Logger::SetConsoleConversionPattern(std::string const& pattern)
{
  m_impl->volatile_message_pattern = pattern;
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
//! :datetime: Equivilant of strftime (%Y-%m-%d %H:%M)
//! :priority: Message Priority text representation (i.e Warning,Info,Error)
//! :simtime:  Time in the current simulation if known
//! :message:  Test given to the logger when called
//! :endline:  A new Line
//!
//! This function accepts all valid format strings for std::put_time.
//! If the presence of a %[a-zA-Z] is found it will be passed   to std::put_time for final processing.
//! As this might cause a problem for some message formats. the longform alternatives are avaliable and
//! will be expanded over time

void FormatLogMessage(std::string::const_iterator pattern_start, std::string::const_iterator pattern_end,
                      std::istreambuf_iterator<char> message_start, std::istreambuf_iterator<char> message_end,
                      Logger::LogLevel priority, std::string origin, SEScalarTime const* simtime,
                      std::string& workspace, std::ostream& destination)
{

  workspace.clear();

  char time_buffer[80];

  std::string::const_iterator scanner = pattern_start, processed = pattern_start;
  std::string::const_iterator token_start, token_end, t_current;

  std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  int t_seconds = 0;
  int seconds = 0, minutes = 0, hours = 0, days = 0, weeks = 0;

  bool time_placeholders_found = false;
  while (scanner != pattern_end) {

    if (*scanner == '%') {
      time_placeholders_found = true;
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
          workspace.append(processed, token_start);
          processed = token_start;
        }
        auto token = std::string(token_start + 1, token_end);
        if (token == "datetime") {
          std::strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M", std::localtime(&now));
          workspace += time_buffer;
          scanner = token_end;
          processed = scanner + 1;
        } else if (token == "origin") {
          workspace += origin;
          scanner = token_end;
          processed = scanner + 1;
        } else if (token == "priority") {
          workspace += ToString(priority);
          scanner = token_end;
          processed = scanner + 1;
        } else if (token == "simseconds") {
          if (&simtime != nullptr) {
            workspace += std::to_string(simtime->GetValue(TimeUnit::s)) + "s";
          }
          scanner = token_end;
          processed = scanner + 1;
        } else if (token == "simtime") {
          if (&simtime != nullptr) {
            t_seconds = static_cast<int>(simtime->GetValue(TimeUnit::s));

            minutes = t_seconds / 60;
            hours = minutes / 60;

            if (hours) {
              workspace.append(std::to_string(hours % 24) + "h" + std::to_string(minutes % 60) + "m" + std::to_string(t_seconds % 60) + "s");
            } else if (minutes) {
              workspace.append(std::to_string(minutes % 60) + "m" + std::to_string(t_seconds % 60) + "s");
            } else {
              workspace.append(std::to_string(t_seconds % 60) + "s");
            }
            scanner = token_end;
            processed = scanner + 1;
          }
        } else if (token == "message") {
          workspace.append(message_start, message_end);
          scanner = token_end;
          processed = scanner + 1;
        } else if (token == "space") {
          workspace += " ";
          scanner = token_end;
          processed = scanner + 1;
        } else if (token == "tab") {
          workspace += "\t";
          scanner = token_end;
          processed = scanner + 1;
        } else if (token == "endline") {
          workspace += "\n";
          scanner = token_end;
          processed = scanner + 1;
          ;
        }
      }
    }
    ++scanner;
  }

  if (scanner != pattern_end) {
    workspace.append(scanner, pattern_end);
    scanner = pattern_end;
  }

  if (time_placeholders_found) {
    destination << std::put_time(std::localtime(&now), workspace.c_str());
  } else {
    destination << workspace;
  }
}

void FormatLogMessage(std::string::const_iterator pattern_start, std::string::const_iterator pattern_end,
                      std::string::const_iterator message_start, std::string::const_iterator message_end,
                      Logger::LogLevel priority, std::string origin, SEScalarTime const* simtime,
                      std::string& workspace, std::ostream& destination)
{

  workspace.clear();

  char time_buffer[80];

  std::string::const_iterator scanner = pattern_start, processed = pattern_start;
  std::string::const_iterator token_start, token_end, t_current;
  std::string::const_iterator;

  std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  int t_seconds = 0;
  int seconds = 0, minutes = 0, hours = 0, days = 0, weeks = 0;

  bool time_placeholders_found = false;
  while (scanner != pattern_end) {

    if (*scanner == '%') {
      time_placeholders_found = true;
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
          workspace.append(processed, token_start);
          processed = token_start;
        }
        auto token = std::string(token_start + 1, token_end);
        if (token == "datetime") {
          std::strftime(&time_buffer[0], sizeof(time_buffer), "%Y-%m-%d %H:%M", std::localtime(&now));
          workspace.append(&time_buffer[0]);
          scanner = token_end;
          processed = token_end + 1;
        } else if (token == "origin") {
          workspace += origin;
          scanner = token_end;
          processed = token_end + 1;
        } else if (token == "priority") {
          workspace += ToString(priority);
          scanner = token_end;
          processed = token_end + 1;
        } else if (token == "simseconds") {
          if (&simtime != nullptr) {
            workspace += std::to_string(simtime->GetValue(TimeUnit::s)) + "s";
          }
          scanner = token_end;
          processed = token_end + 1;
        } else if (token == "simtime") {
          if (&simtime != nullptr) {
            t_seconds = static_cast<int>(simtime->GetValue(TimeUnit::s));

            minutes = t_seconds / 60;
            hours = minutes / 60;

            if (hours) {
              workspace.append(std::to_string(hours % 24) + "h" + std::to_string(minutes % 60) + "m" + std::to_string(t_seconds % 60) + "s");
            } else if (minutes) {
              workspace.append(std::to_string(minutes % 60) + "m" + std::to_string(t_seconds % 60) + "s");
            } else {
              workspace.append(std::to_string(t_seconds % 60) + "s");
            }
            scanner = token_end;
            processed = token_end + 1;
          }
        } else if (token == "message") {
          workspace.append(message_start, message_end);
          scanner = token_end;
          processed = token_end + 1;
        } else if (token == "space") {
          workspace += " ";
          scanner = token_end;
          processed = token_end + 1;
        } else if (token == "tab") {
          workspace += "\t";
          scanner = token_end;
          processed = token_end + 1;
        } else if (token == "endline") {
          workspace += "\n";
          scanner = token_end;
          processed = token_end + 1;
        }
      }
    }
    ++scanner;
  }

  if (scanner != pattern_end) {
    workspace.append(scanner, pattern_end);
    scanner = pattern_end;
  }

  if (time_placeholders_found) {
    destination << std::put_time(std::localtime(&now), workspace.c_str());
  } else {
    destination << workspace;
  }
}

void Logger::LogMessage(std::istream& msg, std::string const& origin, LogLevel priority) const
{
  auto& impl = *m_impl;
  m_impl->message_count += 1;
  std::string message;
  if (priority <= impl.persistant_filter_level) {
    if (impl.formatMessages) {
      std::istreambuf_iterator<char> msg_start { msg }, msg_end;
      FormatLogMessage(impl.persistant_message_pattern.begin(), impl.persistant_message_pattern.end(),
                       msg_start, msg_end,
                       priority, "", impl.time,
                       impl.format_buffer, *impl.persistantStream);
    } else {
      *impl.persistantStream << msg.rdbuf();
    }

    *impl.persistantStream << message;
    if (impl.message_count % MESSAGES_TO_FLUSH == 0) {
      impl.persistantStream->flush();
    }
  }
  if (priority <= impl.volatile_filter_level) {
    if (impl.formatMessages) {
      std::istreambuf_iterator<char> msg_start { msg }, msg_end;
      FormatLogMessage(impl.volatile_message_pattern.begin(), impl.volatile_message_pattern.end(),
                       msg_start, msg_end,
                       priority, "", impl.time,
                       impl.format_buffer, *impl.volatileStream);
    } else {
      *impl.volatileStream << msg.rdbuf();
    }
  }

  if (impl.userDefinedLogger != nullptr) {
    switch (priority) {
    case Logger::DEBUG:
      impl.userDefinedLogger->Debug(message);
      break;
    case Logger::ERROR:
      impl.userDefinedLogger->Error(message);
      break;
    case Logger::EXCEPTION:
      break;
    case Logger::WARNING:
      impl.userDefinedLogger->Warning(message);
      break;
    case Logger::INFO:
      impl.userDefinedLogger->Info(message);
      break;
    case Logger::STABILIZATION:
      break;
    }
  }
}
void Logger::LogMessage(std::istream&& msg, std::string const& origin, LogLevel priority) const
{
  auto& impl = *m_impl;
  m_impl->message_count += 1;
  std::string message;
  if (priority <= impl.persistant_filter_level) {
    if (impl.formatMessages) {
      std::istreambuf_iterator<char> msg_start { msg }, msg_end;
      FormatLogMessage(impl.persistant_message_pattern.begin(), impl.persistant_message_pattern.end(),
                       msg_start, msg_end,
                       priority, "", impl.time,
                       impl.format_buffer, *impl.persistantStream);
    } else {
      *impl.persistantStream << msg.rdbuf();
    }

    *impl.persistantStream << message;
    if (impl.message_count % MESSAGES_TO_FLUSH == 0) {
      impl.persistantStream->flush();
    }
  }
  if (priority <= impl.volatile_filter_level) {
    if (impl.formatMessages) {
      std::istreambuf_iterator<char> msg_start { msg }, msg_end;
      FormatLogMessage(impl.volatile_message_pattern.begin(), impl.volatile_message_pattern.end(),
                       msg_start, msg_end,
                       priority, "", impl.time,
                       impl.format_buffer, *impl.volatileStream);
    } else {
      *impl.volatileStream << msg.rdbuf();
    }
  }

  if (impl.userDefinedLogger != nullptr) {
    switch (priority) {
    case Logger::DEBUG:
      impl.userDefinedLogger->Debug(message);
      break;
    case Logger::ERROR:
      impl.userDefinedLogger->Error(message);
      break;
    case Logger::EXCEPTION:
      break;
    case Logger::WARNING:
      impl.userDefinedLogger->Warning(message);
      break;
    case Logger::INFO:
      impl.userDefinedLogger->Info(message);
      break;
    case Logger::STABILIZATION:
      break;
    }
  }
}

void Logger::LogMessage(std::string const& msg, std::string const& origin, LogLevel priority) const
{
  auto& impl = *m_impl;
  m_impl->message_count += 1;
  std::string message;
  if (priority <= impl.persistant_filter_level) {
    if (impl.formatMessages) {

      FormatLogMessage(impl.persistant_message_pattern.begin(), impl.persistant_message_pattern.end(),
                       msg.begin(), msg.end(),
                       priority, origin, impl.time,
                       impl.format_buffer, *impl.persistantStream);
    } else {
      *impl.persistantStream << msg;
    }

    *impl.persistantStream << message;
    if (impl.message_count % MESSAGES_TO_FLUSH == 0) {
      impl.persistantStream->flush();
    }
  }
  if (priority <= impl.volatile_filter_level) {
    if (impl.formatMessages) {

      FormatLogMessage(impl.volatile_message_pattern.begin(), impl.volatile_message_pattern.end(),
                       msg.begin(), msg.end(),
                       priority, origin, impl.time,
                       impl.format_buffer, *impl.volatileStream);
    } else {
      *impl.volatileStream << msg;
    }
  }

  if (impl.userDefinedLogger != nullptr) {
    switch (priority) {
    case Logger::DEBUG:
      impl.userDefinedLogger->Debug(message);
      break;
    case Logger::ERROR:
      impl.userDefinedLogger->Error(message);
      break;
    case Logger::EXCEPTION:
      break;
    case Logger::WARNING:
      impl.userDefinedLogger->Warning(message);
      break;
    case Logger::INFO:
      impl.userDefinedLogger->Info(message);
      break;
    case Logger::STABILIZATION:
      break;
    }
  }
}
void Logger::LogMessage(std::string&& msg, std::string const& origin, LogLevel priority) const
{
  auto& impl = *m_impl;
  m_impl->message_count += 1;
  std::string message;
  if (priority <= impl.persistant_filter_level) {
    if (impl.formatMessages) {

      FormatLogMessage(impl.persistant_message_pattern.begin(), impl.persistant_message_pattern.end(),
                       msg.begin(), msg.end(),
                       priority, origin, impl.time,
                       impl.format_buffer, *impl.persistantStream);
    } else {
      *impl.persistantStream << msg;
    }

    *impl.persistantStream << message;
    if (impl.message_count % MESSAGES_TO_FLUSH == 0) {
      impl.persistantStream->flush();
    }
  }
  if (priority <= impl.volatile_filter_level) {
    if (impl.formatMessages) {

      FormatLogMessage(impl.volatile_message_pattern.begin(), impl.volatile_message_pattern.end(),
                       msg.begin(), msg.end(),
                       priority, origin, impl.time,
                       impl.format_buffer, *impl.volatileStream);
    } else {
      *impl.volatileStream << msg;
    }
  }

  if (impl.userDefinedLogger != nullptr) {
    switch (priority) {
    case Logger::DEBUG:
      impl.userDefinedLogger->Debug(message);
      break;
    case Logger::ERROR:
      impl.userDefinedLogger->Error(message);
      break;
    case Logger::EXCEPTION:
      break;
    case Logger::WARNING:
      impl.userDefinedLogger->Warning(message);
      break;
    case Logger::INFO:
      impl.userDefinedLogger->Info(message);
      break;
    case Logger::STABILIZATION:
      break;
    }
  }
}

void Logger::Debug(std::string const& msg, std::string const& origin) const
{
  LogMessage(msg, origin, LogLevel::DEBUG);
}
void Logger::Info(std::string const& msg, std::string const& origin) const
{
  LogMessage(msg, origin, LogLevel::INFO);
}
void Logger::Warning(std::string const& msg, std::string const& origin) const
{
  LogMessage(msg, origin, LogLevel::WARNING);
}
void Logger::Error(std::string const& msg, std::string const& origin) const
{
  LogMessage(msg, origin, LogLevel::ERROR);
}
void Logger::Fatal(std::string const& msg, std::string const& origin) const
{
  LogMessage(msg, origin, LogLevel::FATAL);
}

void Logger::Debug(std::istream& msg) const
{
  LogMessage(msg, "", LogLevel::DEBUG);
}
void Logger::Info(std::istream& msg) const
{
  LogMessage(msg, "", LogLevel::INFO);
}
void Logger::Warning(std::istream& msg) const
{
  LogMessage(msg, "", LogLevel::WARNING);
}
void Logger::Error(std::istream& msg) const
{
  LogMessage(msg, "", LogLevel::ERROR);
}
void Logger::Fatal(std::istream& msg) const
{
  LogMessage(msg, "", LogLevel::FATAL);
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
    m_Logger->LogMessage(msg, "", Logger::DEBUG);
  }
}
void Loggable::Info(std::istream& msg) const
{
  if (m_Logger) {
    m_Logger->LogMessage(msg, "", Logger::INFO);
  }
}
void Loggable::Warning(std::istream& msg) const
{
  if (m_Logger) {
    m_Logger->LogMessage(msg, "", Logger::WARNING);
  }
}
void Loggable::Error(std::istream& msg) const
{
  if (m_Logger) {
    m_Logger->LogMessage(msg, "", Logger::ERROR);
  }
}
void Loggable::Fatal(std::istream& msg) const
{
  if (m_Logger) {
    m_Logger->LogMessage(msg, "", Logger::FATAL);
  }
}

void Loggable::Debug(std::istream&& msg) const
{
  if (m_Logger) {
    m_Logger->LogMessage(std::move(msg), "", Logger::DEBUG);
  }
}
void Loggable::Info(std::istream&& msg) const
{
  if (m_Logger) {
    m_Logger->LogMessage(std::move(msg), "", Logger::INFO);
  }
}
void Loggable::Warning(std::istream&& msg) const
{
  if (m_Logger) {
    m_Logger->LogMessage(std::move(msg), "", Logger::WARNING);
  }
}
void Loggable::Error(std::istream&& msg) const
{
  if (m_Logger) {
    m_Logger->LogMessage(std::move(msg), "", Logger::ERROR);
  }
}
void Loggable::Fatal(std::istream&& msg) const
{
  if (m_Logger) {
    m_Logger->LogMessage(std::move(msg), "", Logger::FATAL);
  }
}

Logger* Loggable::GetLogger() const { return m_Logger; }
}
