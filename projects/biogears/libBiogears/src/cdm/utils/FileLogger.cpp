#include <biogears/cdm/utils/FileLogger.h>
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

namespace biogears {

FileLogger::FileLogger(const std::string& logFilename) : Logger("FileLogger") {}
FileLogger::~FileLogger() {}

void FileLogger::ResetLogFile(const std::string& logFilename)
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
  LogToConsole(false);
}

} // end namespace biogears