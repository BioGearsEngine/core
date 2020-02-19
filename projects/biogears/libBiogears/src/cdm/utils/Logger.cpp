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

#include <biogears/cdm/utils/FileUtils.h>
#include <biogears/cdm/utils/Logger.h>

#pragma warning(push, 0)
#include <log4cpp/Appender.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/Priority.hh>
#pragma warning(pop)

#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/filesystem/path.h>

namespace biogears {
const std::string Loggable::empty("");
const char* Loggable::empty_cStr("");
// logger constructor
Logger::Logger(const std::string& logFilename, const std::string& working_dir)
  : m_Forward(nullptr)
  , m_time(nullptr)
  , m_FormatMessages(true)
{
  ResetLogFile(logFilename, working_dir);
}

Logger::Logger(const char* logFilename, const char* working_dir)
  : m_Forward(nullptr)
  , m_time(nullptr)
  , m_FormatMessages(true)
{
  ResetLogFile(logFilename, working_dir);
}

void Logger::LogToConsole(bool log_to_console)
{
  if (log_to_console) {
    m_Log->addAppender(*m_ConsoleAppender);
  } else {
    m_Log->removeAppender(m_ConsoleAppender);
  }
}

void Logger::FormatMessages(bool format_messages)
{
  m_FormatMessages = format_messages;
}
void Logger::ResetLogFile(const std::string& logFilename, const std::string& working_dir)
{

  std::string key = logFilename;
  if (logFilename.empty()) {
    static int i = 0;
    key = "biogears_logger" + std::to_string(++i);
  }
  log4cpp::Category& category = log4cpp::Category::getInstance(key);
  m_Log = &category;
  m_Log->removeAllAppenders();
  m_Log->setPriority(log4cpp::Priority::INFO);

  std::string qulaified_path = ResolvePath((filesystem::path(working_dir) / logFilename).str());
  if (!qulaified_path.empty()) {
    CreateFilePath(qulaified_path);

    // delete previous log contents if it exists
    FILE* FilePointer = fopen(qulaified_path.c_str(), "wt+");
    if (FilePointer) {
      fclose(FilePointer);
    }

    m_FileAppender = log4cpp::Appender::getAppender(logFilename);
    if (m_FileAppender == nullptr && !logFilename.empty()) {
      m_FileAppender = new log4cpp::FileAppender(logFilename, qulaified_path);
      log4cpp::PatternLayout* myLayout = new log4cpp::PatternLayout();
      myLayout->setConversionPattern("%d [%p] %m%n");
      m_FileAppender->setLayout(myLayout);
      m_Log->addAppender(m_FileAppender);
    }
  }

  m_ConsoleAppender = log4cpp::Appender::getAppender(logFilename + "_console");
  if (m_ConsoleAppender == nullptr) {
    m_ConsoleAppender = new log4cpp::OstreamAppender(logFilename + "_console", &std::cout);
    log4cpp::PatternLayout* cLayout = new log4cpp::PatternLayout();
    cLayout->setConversionPattern("%d [%p] %m%n");
    m_ConsoleAppender->setLayout(cLayout);
    m_ConsoleAppender->setThreshold(log4cpp::Priority::INFO);
  }
  LogToConsole(true);
}

void Logger::ResetLogFile(const char* logFilename, const char* working_dir)
{
  const std::string logFileName_str{ logFilename };
  const std::string working_dir_str{ working_dir };
  ResetLogFile(logFileName_str, working_dir_str);
}
Logger::~Logger() {}

void Logger::SetLogTime(const SEScalarTime* time) { m_time = time; }

// TODO SetGlobalPriority(log4cpp::Priority::Value priority)
// TODO log4cpp::Priority::Value priority GetGlobalPriority()

// This function will change the priority of the logger
void Logger::SetLogLevel(log4cpp::Priority::Value priority) const
{
  if (m_Log) {
    m_Log->setPriority(priority);
  }
}

// This function will change the priority of the m_ConsoleAppender
void Logger::SetConsoleLogLevel(log4cpp::Priority::Value priority) const
{
  if (m_ConsoleAppender) {
    m_ConsoleAppender->setThreshold(priority);
  }
}
void Logger::SetsetConversionPattern(const std::string& layout)
{
  log4cpp::PatternLayout* cLayout = new log4cpp::PatternLayout();
  cLayout->setConversionPattern(layout);
  m_FileAppender->setLayout(cLayout);
}
void Logger::SetConsolesetConversionPattern(const std::string& layout)
{
  log4cpp::PatternLayout* cLayout = new log4cpp::PatternLayout();
  cLayout->setConversionPattern(layout);
  m_ConsoleAppender->setLayout(cLayout);
}
// This function will return the priority of the logger
log4cpp::Priority::Value Logger::GetLogLevel()
{
  return m_Log != nullptr ? m_Log->getPriority() : log4cpp::Priority::INFO;
}

void Logger::SetForward(LoggerForward* forward) { m_Forward = forward; }

bool Logger::HasForward() { return m_Forward == nullptr ? false : true; }

std::string Logger::FormatLogMessage(const std::string& msg, const std::string& origin) const
{
  if (m_FormatMessages) {
    m_ss.str("");
    m_ss.clear();
    if (m_time != nullptr && m_time->IsValid()) {
      m_ss << "[" << *m_time << "] " << msg;
    } else {
      m_ss << msg;
    }
    if (msg.empty()) {
      return origin;
    }
    return origin + " : " + m_ss.str();
  } else {
    return msg;
  }
}
void Logger::Debug(const std::string& msg, const std::string& origin) const
{
  m_Log->debug(FormatLogMessage(msg, origin));

  if (m_Forward != nullptr) {
    m_Forward->ForwardDebug(m_ss.str().c_str(), origin.c_str());
  }
}
void Logger::Info(const std::string& msg, const std::string& origin) const
{

  m_Log->info(FormatLogMessage(msg, origin));

  if (m_Forward != nullptr) {
    m_Forward->ForwardInfo(m_ss.str().c_str(), origin.c_str());
  }
}
void Logger::Warning(const std::string& msg, const std::string& origin) const
{
  m_Log->warn(FormatLogMessage(msg, origin));

  if (m_Forward != nullptr) {
    m_Forward->ForwardWarning(m_ss.str().c_str(), origin.c_str());
  }
}
void Logger::Error(const std::string& msg, const std::string& origin) const
{
  m_Log->error(FormatLogMessage(msg, origin));

  if (m_Forward != nullptr) {
    m_Forward->ForwardError(m_ss.str().c_str(), origin.c_str());
  }
}
void Logger::Fatal(const std::string& msg, const std::string& origin) const
{
  m_Log->fatal(FormatLogMessage(msg, origin));

  if (m_Forward != nullptr) {
    m_Forward->ForwardFatal(m_ss.str().c_str(), origin.c_str());
  }
}

void Logger::Debug(std::ostream const& msg, const std::string& origin) const
{
  std::stringstream ss;
  ss << msg.rdbuf();
  Debug(ss.str(), origin);
}
void Logger::Info(std::ostream const& msg, const std::string& origin) const
{
  std::stringstream ss;
  ss << msg.rdbuf();
  Info(ss.str(), origin);
}
void Logger::Warning(std::ostream const& msg, const std::string& origin) const
{
  std::stringstream ss;
  ss << msg.rdbuf();
  Warning(ss.str(), origin);
}
void Logger::Error(std::ostream const& msg, const std::string& origin) const
{
  std::stringstream ss;
  ss << msg.rdbuf();
  Error(ss.str(), origin);
}
void Logger::Fatal(std::ostream const& msg, const std::string& origin) const
{
  std::stringstream ss;
  ss << msg.rdbuf();
  Fatal(ss.str(), origin);
}

void Loggable::Error(const char* msg, const char* origin) const
{
  Error(std::string{ msg }, std::string{ origin });
}
void Loggable::Error(const std::string msg, const std::string origin) const
{
  if (m_Logger) {
    m_Logger->Error(msg, origin);
  } else {
    // if(stdOut) TODO support
    std::cerr << "ERROR:" << msg << " : " << origin << std::endl;
  }
}
void Loggable::Error(std::ostream& msg, const std::string& origin) const
{
  std::stringstream ss;
  ss << msg.rdbuf();
  Error(ss.str(), origin);
}

void Loggable::Info(const char* msg, const char* origin) const
{
  Error(std::string{ msg }, std::string{ origin });
}
void Loggable::Info(const std::string& msg, const std::string& origin) const
{
  if (m_Logger) {
    m_Logger->Info(msg, origin);
  } else {
    // if(stdOut) TODO support
    std::cout << "INFO:" << msg << " : " << origin << std::endl;
  }
}
void Loggable::Info(std::ostream& msg, const std::string& origin) const
{
  std::stringstream ss;
  ss << msg.rdbuf();
  Info(ss.str(), origin);
}

void Loggable::Warning(const char* msg, const char* origin) const
{
  Error(std::string{ msg }, std::string{ origin });
}
void Loggable::Warning(const std::string& msg, const std::string& origin) const
{
  if (m_Logger) {
    m_Logger->Warning(msg, origin);
  } else {
    // if(stdOut) TODO support
    std::cout << "WARN:" << msg << " : " << origin << std::endl;
  }
}
void Loggable::Warning(std::ostream& msg, const std::string& origin) const
{
  std::stringstream ss;
  ss << msg.rdbuf();
  Warning(ss.str(), origin);
}

void Loggable::Fatal(const char* msg, const char* origin) const
{
  Error(std::string{ msg }, std::string{ origin });
}
void Loggable::Fatal(const std::string& msg, const std::string& origin) const
{
  std::cerr << "FATAL:" << msg << " : " << origin << std::endl;
  if (m_Logger) {
    m_Logger->Fatal(msg, origin);
  } else {
    // if(stdOut) TODO support
    std::cerr << "FATAL:" << msg << " : " << origin << std::endl;
  }
}
void Loggable::Fatal(std::ostream& msg, const std::string& origin) const
{
  std::stringstream ss;
  ss << msg.rdbuf();
  Fatal(ss.str(), origin);
}

void Loggable::Debug(const char* msg, const char* origin) const
{
  Error(std::string{ msg }, std::string{ origin });
}
void Loggable::Debug(const std::string& msg, const std::string& origin) const
{
  if (m_Logger) {
    m_Logger->Debug(msg, origin);
  }
  // Not writing out DEBUG to console, only to log
  // else// if(stdOut) TODO support
  //  std::cout<<"DEBUG:"<<msg<<" : "<<origin<<std::endl;
}
void Loggable::Debug(std::ostream& msg, const std::string& origin) const
{
  std::stringstream ss;
  ss << msg.rdbuf();
  Debug(ss.str(), origin);
}

Loggable::Loggable() { m_Logger = nullptr; }
Loggable::Loggable(Logger* logger) { m_Logger = logger; }
Loggable::~Loggable() {}

Logger* Loggable::GetLogger() const { return m_Logger; }
}
