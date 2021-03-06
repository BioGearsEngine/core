
%{
  #include <biogears/cdm/utils/Logger.h>
%}

%import <biogears/cdm/CommonDataModel.swg>
%include <biogears/exports.h>

%nspace biogears::Logger;
%nspace biogears::LoggerForward;
%nspace biogears::Loggable;

namespace biogears {

  class Logger {
  public:
    Logger(const char* logFilename);
    virtual ~Logger();

    virtual void Debug(const char* msg, const char* origin = "") const;
    virtual void Info(const char* msg, const char* origin = "") const;
    virtual void Warning(const char* msg, const char* origin = "") const;
    virtual void Error(const char* msg, const char* origin = "") const;
    virtual void Fatal(const char* msg, const char* origin = "") const;
  };

  class Loggable {
  public:
    Loggable();
    Loggable(Logger* log);
    virtual ~Loggable();
    virtual Logger* GetLogger() const;

  protected:
    virtual void Debug(const char* msg, const char* origin = "") const;
    virtual void Info(const char* msg, const char* origin = "") const;
    virtual void Warning(const char* msg, const char* origin = "") const;
    virtual void Error(const char* msg, const char* origin = "") const;
    virtual void Fatal(const char* msg, const char* origin = "") const;

  };

  class LoggerForward {
  public:
    virtual void Debug(const char* msg) = 0;
    virtual void Info(const char* msg) = 0;
    virtual void Warning(const char* msg) = 0;
    virtual void Error(const char* msg) = 0;
    virtual void Fatal(const char* msg) = 0;
  };


  }

