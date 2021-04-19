// Add necessary symbols to generated header
%{
  #include <biogears/cdm/utils/Logger.h>
%}

%include "windows.i"
#if defined(SWIGCSHARP)
  %include "csharp/std_string.i"
  %include "csharp/std_vector.i"
  %include "csharp/std_except.i"

  namespace std 
{
  %ignore runtime_error;
  struct runtime_error {};
}

#elif defind(SWIGPYTHON)
  %include "python/std_string.i"
  %include "python/std_vector.i"
  %include "python/std_except.i"
#else
  %include "std_string.i"
  %include "std_vector.i"
  %include "std_except.i"
#endif 
%include "exception.i"
%include <biogears/exports.h>

%nspace biogears::Logger;
%nspace biogears::LoggerForward;
%nspace biogears::Loggable;

namespace biogears {

  class Logger {
  public:
    Logger(const char* logFilename, const char* working_dir = Loggable::empty_cStr);
    virtual ~Logger();

    virtual void Debug(const std::string& msg, std::string const& origin = Loggable::empty) const;
    virtual void Info(const std::string& msg, std::string const& origin = Loggable::empty) const;
    virtual void Warning(const std::string& msg, std::string const& origin = Loggable::empty) const;
    virtual void Error(const std::string& msg, std::string const& origin = Loggable::empty) const;
    virtual void Fatal(const std::string& msg, std::string const& origin = Loggable::empty) const;
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
    virtual void ForwardDebug(const std::string& msg, const std::string& origin) = 0;
    virtual void ForwardInfo(const std::string& msg, const std::string& origin) = 0;
    virtual void ForwardWarning(const std::string& msg, const std::string& origin) = 0;
    virtual void ForwardError(const std::string& msg, const std::string& origin) = 0;
    virtual void ForwardFatal(const std::string& msg, const std::string& origin) = 0;
  };


  }

