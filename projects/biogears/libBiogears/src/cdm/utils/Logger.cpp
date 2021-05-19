
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
#include <ostream>
#include <regex>

#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/filesystem/path.h>

namespace biogears {
std::string const Loggable::empty("");
const char* Loggable::empty_cStr("");

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
  mutable std::stringstream formatStream;

  std::string persistant_message_pattern = ":datetime: <:priority:> :origin: [:simtime:] :message: :endline:";
  std::string volatile_message_pattern = ":datetime: <:priority:> [:simtime:] :message: :endline:";

  std::unique_ptr<std::ostream> persistantStream; //!< Ostream to some Persistant device like a FILE or Database
  std::unique_ptr<std::ostream> volatileStream; //!< Ostream to some volatile device like a console or webserver

  std::shared_ptr<IOManager> io;
  LoggerForward* userDefinedLogger = nullptr;

  SEScalarTime const* time = nullptr;

  bool formatMessages = true;
  bool logToConsole = true;

  std::atomic<unsigned int> message_count;
};

Logger::Implementation::Implementation()
  : Implementation(IOManager())
{
}
Logger::Implementation::Implementation(IOManager const& t_io)
  : io(new IOManager(t_io))
{
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
std::string Logger::FormatLogMessage(LogLevel priority, std::string const& pattern, std::string const& message, std::string const& origin_str) const
{

  auto processed_itr = pattern.begin();
  auto current = pattern.begin();
  auto end = pattern.end();

  std::stringstream& message_stream = m_impl->formatStream;
  message_stream.str("");

  std::string token;
  auto t_start = current;
  auto t_end = current;

  bool time_placeholders_found = false;

  auto parse_token = [&](std::string::const_iterator t_start, std::string::const_iterator m_end) {
    auto current = t_start + 1;
    auto t_end = current;

    while (t_start != m_end) {
      if (*current == '%') {
        time_placeholders_found = true;
      }
      if (!std::isalpha(*current)) {
        return current;
      }
      ++current;
    }
    return t_start;
  };

  while (current != end) {
    if (*current == '%') {
      time_placeholders_found = true;
    }
    if (*current == ':') {
      t_start = current;
      t_end = parse_token(t_start, end);
      if (t_start != t_end) {
        if (processed_itr != t_start) {
          message_stream << std::string(processed_itr, t_start);
          processed_itr = t_start;
        }
        token = std::string(t_start + 1, t_end);
        if (token == "datetime") {
          current = t_end;
          std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
          message_stream << std::put_time(std::localtime(&now), "%Y-%m-%d %H:%M");
          current = t_end;
          processed_itr = t_end + 1;
        } else if (token == "origin") {
          current = t_end;
          message_stream << origin_str;
          current = t_end;
          processed_itr = t_end + 1;
        } else if (token == "priority") {
          message_stream << ToString(priority);
          current = t_end;
          processed_itr = t_end + 1;
        } else if (token == "datetime") {
          message_stream << "stub";
          current = t_end;
          processed_itr = t_end + 1;
        } else if (token == "simseconds") {
          if (m_impl->time) {
            message_stream << m_impl->time->GetValue(TimeUnit::s) << "s";
          }
          current = t_end;
          processed_itr = t_end + 1;
        } else if (token == "simtime") {
          if (m_impl->time) {
            int t_seconds = static_cast<int>(m_impl->time->GetValue(TimeUnit::s));
            int weeks = t_seconds / 604800;
            int days = (t_seconds / 86400) % 7;
            int hours = (t_seconds / 3600) % 24;
            int minutes = (t_seconds / 60) % 60;
            int seconds = t_seconds % 60;
            if (weeks != 0) {
              message_stream << weeks << "w";
            }
            current = t_end;
            processed_itr = t_end + 1;
            if (days != 0) {
              message_stream << days << "d";
            }
            current = t_end;
            if (hours != 0) {
              message_stream << hours << "h";
            }
            current = t_end;
            if (minutes != 0) {
              message_stream << minutes << "m";
            }

            message_stream << seconds << "s";

            current = t_end;
          }
        } else if (token == "message") {
          message_stream << message;
          current = t_end;
          processed_itr = t_end + 1;
        } else if (token == "space") {
          message_stream << " ";
          current = t_end;
          processed_itr = t_end + 1;
        } else if (token == "tab") {
          message_stream << "\t";
          current = t_end;
          processed_itr = t_end + 1;
        } else if (token == "endline") {
          message_stream << "\n";
          current = t_end;
          processed_itr = t_end + 1;
        }
      }
    }
    ++current;
  }

  if (processed_itr != end) {
    message_stream << std::string(processed_itr, end);
    processed_itr = end;
  }

  if (time_placeholders_found) {
    std::string processed_message = message_stream.str();
    message_stream.str("");
    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    message_stream << std::put_time(std::localtime(&now), processed_message.c_str());
  }
  return message_stream.str();
}

constexpr auto MESSAGES_TO_FLUSH = 25;
void Logger::Debug(std::string const& msg, std::string const& origin) const
{
  m_impl->message_count += 1;
  std::string message;
  if (Logger::DEBUG <= m_impl->persistant_filter_level) {
    message = (m_impl->formatMessages) ? FormatLogMessage(Logger::DEBUG, m_impl->persistant_message_pattern, msg, origin) : msg;
    *m_impl->persistantStream << message;
    if (m_impl->message_count % MESSAGES_TO_FLUSH == 0) {
      m_impl->persistantStream->flush();
    }
  }
  if (Logger::DEBUG <= m_impl->volatile_filter_level) {
    message = (m_impl->formatMessages) ? FormatLogMessage(Logger::DEBUG, m_impl->volatile_message_pattern, msg, origin) : msg;
    *m_impl->volatileStream << message;
  }

  if (m_impl->userDefinedLogger != nullptr) {
    m_impl->userDefinedLogger->Debug(message);
  }
}
void Logger::Info(std::string const& msg, std::string const& origin) const
{
  m_impl->message_count += 1;
  std::string message;
  if (Logger::INFO <= m_impl->persistant_filter_level) {
    message = (m_impl->formatMessages) ? FormatLogMessage(Logger::INFO, m_impl->persistant_message_pattern, msg, origin) : msg;
    *m_impl->persistantStream << message;
    if (m_impl->message_count % MESSAGES_TO_FLUSH == 0) {
      m_impl->persistantStream->flush();
    }
  }
  if (Logger::INFO <= m_impl->volatile_filter_level) {
    message = (m_impl->formatMessages) ? FormatLogMessage(Logger::INFO, m_impl->volatile_message_pattern, msg, origin) : msg;
    *m_impl->volatileStream << message;
  }

  if (m_impl->userDefinedLogger != nullptr) {
    m_impl->userDefinedLogger->Info(message);
  }
}
void Logger::Warning(std::string const& msg, std::string const& origin) const
{
  m_impl->message_count += 1;
  std::string message;
  if (Logger::WARNING <= m_impl->persistant_filter_level) {
    message = (m_impl->formatMessages) ? FormatLogMessage(Logger::WARNING, m_impl->persistant_message_pattern, msg, origin) : msg;
    *m_impl->persistantStream << message;
    if (m_impl->message_count % MESSAGES_TO_FLUSH == 0) {
      m_impl->persistantStream->flush();
    }
  }
  if (Logger::WARNING <= m_impl->volatile_filter_level) {
    message = (m_impl->formatMessages) ? FormatLogMessage(Logger::WARNING, m_impl->volatile_message_pattern, msg, origin) : msg;
    *m_impl->volatileStream << message;
  }

  if (m_impl->userDefinedLogger != nullptr) {
    m_impl->userDefinedLogger->Warning(message);
  }
}
void Logger::Error(std::string const& msg, std::string const& origin) const
{
  m_impl->message_count += 1;
  std::string message;
  if (Logger::ERROR <= m_impl->persistant_filter_level) {
    message = (m_impl->formatMessages) ? FormatLogMessage(Logger::DEBUG, m_impl->persistant_message_pattern, msg, origin) : msg;
    *m_impl->persistantStream << message;
    if (m_impl->message_count % MESSAGES_TO_FLUSH == 0) {
      m_impl->persistantStream->flush();
    }
  }
  if (Logger::ERROR <= m_impl->volatile_filter_level) {
    message = (m_impl->formatMessages) ? FormatLogMessage(Logger::DEBUG, m_impl->volatile_message_pattern, msg, origin) : msg;
    *m_impl->volatileStream << message;
  }

  if (m_impl->userDefinedLogger != nullptr) {
    m_impl->userDefinedLogger->Error(message);
  }
}
void Logger::Fatal(std::string const& msg, std::string const& origin) const
{
  m_impl->message_count += 1;
  std::string message;
  if (Logger::FATAL <= m_impl->persistant_filter_level) {
    message = (m_impl->formatMessages) ? FormatLogMessage(Logger::FATAL, m_impl->persistant_message_pattern, msg, origin) : msg;
    *m_impl->persistantStream << message;
    if (m_impl->message_count % MESSAGES_TO_FLUSH == 0) {
      m_impl->persistantStream->flush();
    }
  }
  if (Logger::FATAL <= m_impl->volatile_filter_level) {
    message = (m_impl->formatMessages) ? FormatLogMessage(Logger::FATAL, m_impl->volatile_message_pattern, msg, origin) : msg;
    *m_impl->volatileStream << message;
  }

  if (m_impl->userDefinedLogger != nullptr) {
    m_impl->userDefinedLogger->Fatal(message);
  }
}

void Logger::Debug(std::ostream& msg) const
{
std::stringstream ss;
  ss << msg.rdbuf();
  Warning(ss.str());
}
void Logger::Info(std::ostream& msg) const
{
std::stringstream ss;
  ss << msg.rdbuf();
  Warning(ss.str());
}
void Logger::Warning(std::ostream& msg) const
{
std::stringstream ss;
  ss << msg.rdbuf();
  Warning(ss.str());
}
void Logger::Error(std::ostream& msg) const
{
std::stringstream ss;
  ss << msg.rdbuf();
  Warning(ss.str());
}
void Logger::Fatal(std::ostream& msg) const
{
std::stringstream ss;
  ss << msg.rdbuf();
  Warning(ss.str());
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
//! OStream implementation I'm hoping we can replace this with a
//! varadic implementation. Such that all types have a ToString call but
//! for now this is a good work around
//!
void Loggable::Debug(std::ostream& msg) const
{
  if (m_Logger) {
    m_Logger->Debug(msg);
  }
}
void Loggable::Info(std::ostream& msg) const
{
  if (m_Logger) {
    m_Logger->Info(msg);
  }
}
void Loggable::Warning(std::ostream& msg) const
{
  if (m_Logger) {
    m_Logger->Warning(msg);
  }
}
void Loggable::Error(std::ostream& msg) const
{
  if (m_Logger) {
    m_Logger->Error(msg);
  }
}
void Loggable::Fatal(std::ostream& msg) const
{
  if (m_Logger) {
    m_Logger->Fatal(msg);
  }
}

Logger* Loggable::GetLogger() const { return m_Logger; }
}
