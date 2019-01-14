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

#pragma warning(push,0)
#include <log4cpp/Appender.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/Priority.hh>
#pragma warning(pop)

#include <biogears/cdm/properties/SEScalarTime.h>

namespace biogears {
const std::string Loggable::empty("");

// logger constructor
Logger::Logger(const std::string& logFilename)
{
  m_Forward = nullptr;
  m_time = nullptr;
  ResetLogFile(logFilename);
}

void Logger::LogToConsole(bool b)
{
  (b) ? m_Log->addAppender(m_ConsoleAppender)
      : m_Log->removeAppender(m_ConsoleAppender);
}

void Logger::ResetLogFile(const std::string& logFilename)
{
  log4cpp::Category& category = log4cpp::Category::getInstance(logFilename);
  m_Log = &category;
  m_Log->removeAllAppenders();
  m_Log->setPriority(log4cpp::Priority::INFO);

  if (!logFilename.empty()) {
    CreateFilePath(logFilename);

    // delete previous log contents if it exists
    FILE* FilePointer = fopen(logFilename.c_str(), "wt+");
    if (FilePointer)
      fclose(FilePointer);

    m_FileAppender = log4cpp::Appender::getAppender(logFilename);
    if (m_FileAppender == nullptr) {
      m_FileAppender = new log4cpp::FileAppender(logFilename, logFilename);
      log4cpp::PatternLayout* myLayout = new log4cpp::PatternLayout();
      myLayout->setConversionPattern("%d [%p] %m%n");
      m_FileAppender->setLayout(myLayout);
      m_Log->addAppender(m_FileAppender);
    }
  }

  m_ConsoleAppender = log4cpp::Appender::getAppender(logFilename + "_console");
  if (m_ConsoleAppender == nullptr) {
    m_ConsoleAppender = new log4cpp::OstreamAppender("console", &std::cout);
    log4cpp::PatternLayout* cLayout = new log4cpp::PatternLayout();
    cLayout->setConversionPattern("%d [%p] %m%n");
    m_ConsoleAppender->setLayout(cLayout);
  }
  LogToConsole(true);
}

Logger::~Logger() {}

void Logger::SetLogTime(const SEScalarTime* time) { m_time = time; }

// TODO SetGlobalPriority(log4cpp::Priority::Value priority)
// TODO log4cpp::Priority::Value priority GetGlobalPriority()

// This function will change the priority of the logger
void Logger::SetLogLevel(log4cpp::Priority::Value priority)
{
  if (m_Log)
    m_Log->setPriority(priority);
}

// This function will return the priority of the logger
log4cpp::Priority::Value Logger::GetLogLevel()
{
  return m_Log != nullptr ? m_Log->getPriority() : log4cpp::Priority::INFO;
}

void Logger::SetForward(LoggerForward* forward) { m_Forward = forward; }

bool Logger::HasForward() { return m_Forward == nullptr ? false : true; }

std::string Logger::FormatLogMessage(const std::string& msg,
  const std::string& origin)
{
  m_ss.str("");
  m_ss.clear();
  if (m_time != nullptr && m_time->IsValid())
    m_ss << "[" << *m_time << "] " << msg;
  else
    m_ss << msg;
  if (msg.empty())
    return origin;
  return origin + " : " + m_ss.str();
}

void Logger::Debug(const std::string& msg, const std::string& origin)
{
  m_Log->debug(FormatLogMessage(msg, origin));
  // m_Log->debugStream().flush();
  if (m_Forward != nullptr)
    m_Forward->ForwardDebug(m_ss.str().c_str(), origin.c_str());
}

void Logger::Debug(std::stringstream& msg, const std::string& origin)
{
  Debug(msg.str(), origin);
  msg.str("");
  msg.clear();
}
void Logger::Debug(std::ostream& msg, const std::string& origin)
{
  std::stringstream ss;
  ss << msg.rdbuf();
  Debug(ss.str(), origin);
}

void Logger::Info(const std::string& msg, const std::string& origin)
{
  // std::cout<<"Number of appenders"<< m_Log->getAllAppenders().size() <<
  // std::endl;
  m_Log->info(FormatLogMessage(msg, origin));
  // m_Log->infoStream().flush();
  if (m_Forward != nullptr)
    m_Forward->ForwardInfo(m_ss.str().c_str(), origin.c_str());
}

void Logger::Info(std::stringstream& msg, const std::string& origin)
{
  Info(msg.str(), origin);
  msg.str("");
  msg.clear();
}

void Logger::Info(const std::stringstream& msg, const std::string& origin)
{
  Info(msg.str(), origin);
}

void Logger::Info(std::ostream& msg, const std::string& origin)
{
  std::stringstream ss;
  ss << msg.rdbuf();
  Info(ss.str(), origin);
}

void Logger::Warning(const std::string& msg, const std::string& origin)
{
  m_Log->warn(FormatLogMessage(msg, origin));
  // m_Log->warnStream().flush();
  if (m_Forward != nullptr)
    m_Forward->ForwardWarning(m_ss.str().c_str(), origin.c_str());
}
void Logger::Warning(std::stringstream& msg, const std::string& origin)
{
  Warning(msg.str(), origin);
  msg.str("");
  msg.clear();
}
void Logger::Warning(std::ostream& msg, const std::string& origin)
{
  std::stringstream ss;
  ss << msg.rdbuf();
  Warning(ss.str(), origin);
}

void Loggable::Error(const char* msg, const char* origin) const
{
  Error(std::string{ msg }, std::string{ origin });
}

void Logger::Error(const std::string& msg, const std::string& origin)
{
  m_Log->error(FormatLogMessage(msg, origin));
  // m_Log->errorStream().flush();
  if (m_Forward != nullptr)
    m_Forward->ForwardError(m_ss.str().c_str(), origin.c_str());
}
void Logger::Error(std::stringstream& msg, const std::string& origin)
{
  Error(msg.str(), origin);
  msg.str("");
  msg.clear();
}
void Logger::Error(std::ostream& msg, const std::string& origin)
{
  std::stringstream ss;
  ss << msg.rdbuf();
  Error(ss.str(), origin);
}

void Logger::Fatal(const std::string& msg, const std::string& origin)
{
  m_Log->fatal(FormatLogMessage(msg, origin));
  // m_Log->fatalStream().flush();
  if (m_Forward != nullptr)
    m_Forward->ForwardFatal(m_ss.str().c_str(), origin.c_str());
}
void Logger::Fatal(std::stringstream& msg, const std::string& origin)
{
  Fatal(msg.str(), origin);
  msg.str("");
  msg.clear();
}
void Logger::Fatal(std::ostream& msg, const std::string& origin)
{
  std::stringstream ss;
  ss << msg.rdbuf();
  Fatal(ss.str(), origin);
}

Loggable::Loggable() { m_Logger = nullptr; }

Loggable::Loggable(Logger* logger) { m_Logger = logger; }

Loggable::~Loggable() {}

Logger* Loggable::GetLogger() const { return m_Logger; }

void Loggable::Error(const std::string msg, const std::string origin) const
{
  if (m_Logger)
    m_Logger->Error(msg, origin);
  else // if(stdOut) TODO support
    std::cerr << "ERROR:" << msg << " : " << origin << std::endl;
}
void Loggable::Error(std::stringstream& msg, const std::string& origin) const
{
  Error(msg.str(), origin);
  msg.str("");
  msg.clear();
}
void Loggable::Error(std::ostream& msg, const std::string& origin) const
{
  std::stringstream ss;
  ss << msg.rdbuf();
  Error(ss.str(), origin);
}

void Loggable::Info(const std::string& msg, const std::string& origin) const
{
  if (m_Logger)
    m_Logger->Info(msg, origin);
  else // if(stdOut) TODO support
    std::cout << "INFO:" << msg << " : " << origin << std::endl;
}
void Loggable::Info(std::stringstream& msg, const std::string& origin) const
{
  Info(msg.str(), origin);
  msg.str("");
  msg.clear();
}

void Loggable::Info(const std::stringstream& msg,
  const std::string& origin) const
{
  Info(msg.str(), origin);
}

void Loggable::Info(std::ostream& msg, const std::string& origin) const
{
  std::stringstream ss;
  ss << msg.rdbuf();
  Info(ss.str(), origin);
}

void Loggable::Warning(const std::string& msg,
  const std::string& origin) const
{
  if (m_Logger)
    m_Logger->Warning(msg, origin);
  else // if(stdOut) TODO support
    std::cout << "WARN:" << msg << " : " << origin << std::endl;
}
void Loggable::Warning(std::stringstream& msg,
  const std::string& origin) const
{
  Warning(msg.str(), origin);
  msg.str("");
  msg.clear();
}
void Loggable::Warning(std::ostream& msg, const std::string& origin) const
{
  std::stringstream ss;
  ss << msg.rdbuf();
  Warning(ss.str(), origin);
}

void Loggable::Fatal(const std::string& msg, const std::string& origin) const
{
  std::cerr << "FATAL:" << msg << " : " << origin << std::endl;
  if (m_Logger)
    m_Logger->Fatal(msg, origin);
  else // if(stdOut) TODO support
    std::cerr << "FATAL:" << msg << " : " << origin << std::endl;
}
void Loggable::Fatal(std::stringstream& msg, const std::string& origin) const
{
  Fatal(msg.str(), origin);
  msg.str("");
  msg.clear();
}
void Loggable::Fatal(std::ostream& msg, const std::string& origin) const
{
  std::stringstream ss;
  ss << msg.rdbuf();
  Fatal(ss.str(), origin);
}

void Loggable::Debug(const std::string& msg, const std::string& origin) const
{
  if (m_Logger)
    m_Logger->Debug(msg, origin);
  // Not writing out DEBUG to console, only to log
  // else// if(stdOut) TODO support
  //  std::cout<<"DEBUG:"<<msg<<" : "<<origin<<std::endl;
}
void Loggable::Debug(std::stringstream& msg, const std::string& origin) const
{
  Debug(msg.str(), origin);
  msg.str("");
  msg.clear();
}
void Loggable::Debug(std::ostream& msg, const std::string& origin) const
{
  std::stringstream ss;
  ss << msg.rdbuf();
  Debug(ss.str(), origin);
}
}