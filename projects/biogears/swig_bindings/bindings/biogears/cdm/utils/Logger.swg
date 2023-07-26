%{
#include <biogears/cdm/utils/Logger.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

#if defined(SWIGPYTHON)
%module(package="biogears", 
moduleimport=
   "# Import the low-level C/C++ module\n"
    "if __package__ or \".\" in __name__:\n"
    "    from . import _biogears\n"
    "else:\n"
    "    import _biogears\n"
) Logger
#endif

%import <biogears/cdm/CommonDataModel.swg>

#include <biogears/config.h>

%nspace BIOGEARS_NAMESPACE Logger;
%nspace BIOGEARS_NAMESPACE LoggerForward;
%nspace BIOGEARS_NAMESPACE Loggable;

OPEN_BIOGEARS_NAMESPACE

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
  CLOSE_BIOGEARS_NAMESPACE