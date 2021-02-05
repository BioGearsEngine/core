#include <pybind11/pybind11.h>

#include <biogears/cdm/utils/Logger.h>
#include <biogears/cdm/properties/SEScalarTime.h>

namespace py = pybind11;

PYBIND11_MODULE(pybLogger, m) {

    std::cout<<"pybLogger called";
    py::class_<biogears::Logger>(m, "Logger")

    .def(py::init<std::string&, std::string&>())
    .def(py::init<char*, char*>())
    .def("LogToConsole",&biogears::Logger::LogToConsole)
    .def("FormatMessages",&biogears::Logger::FormatMessages)
    .def("ResetLogFile",py::overload_cast<const std::string&, const std::string&>(&biogears::Logger::ResetLogFile))
    .def("ResetLogFile",py::overload_cast<const char*, const char*>(&biogears::Logger::ResetLogFile))
    .def("SetLogLevel",&biogears::Logger::SetLogLevel)
    .def("SetConsoleLogLevel",&biogears::Logger::SetConsoleLogLevel)
    .def("GetLogLevel",&biogears::Logger::GetLogLevel)
    .def("SetLogTime",&biogears::Logger::SetLogTime)
    .def("SetsetConversionPattern",&biogears::Logger::SetsetConversionPattern)
    .def("SetConsolesetConversionPattern",&biogears::Logger::SetConsolesetConversionPattern)
    .def("SetForward",&biogears::Logger::SetForward)
    .def("HasForward",&biogears::Logger::HasForward)
    .def("HasForward",&biogears::Logger::HasForward)
    .def("Debug",py::overload_cast<const std::string&,const std::string&>(&biogears::Logger::Debug,py::const_))
    .def("Debug",py::overload_cast<const std::ostream&,const std::string&>(&biogears::Logger::Debug,py::const_))
    .def("Info",py::overload_cast<const std::string&,const std::string&>(&biogears::Logger::Info,py::const_),py::arg("msg")="",py::arg("origin") ="")
    .def("Info",py::overload_cast<const std::ostream&,const std::string&>(&biogears::Logger::Info,py::const_),py::arg("msg")="",py::arg("origin") ="")
    .def("Warning",py::overload_cast<const std::string&,const std::string&>(&biogears::Logger::Warning,py::const_))
    .def("Warning",py::overload_cast<const std::ostream&,const std::string&>(&biogears::Logger::Warning,py::const_))
    .def("Error",py::overload_cast<const std::string&,const std::string&>(&biogears::Logger::Error,py::const_))
    .def("Error",py::overload_cast<const std::ostream&,const std::string&>(&biogears::Logger::Error,py::const_))
    .def("Fatal",py::overload_cast<const std::string&,const std::string&>(&biogears::Logger::Fatal,py::const_))
    .def("Fatal",py::overload_cast<const std::ostream&,const std::string&>(&biogears::Logger::Fatal,py::const_));

    py::class_<biogears::Loggable>(m, "Loggable")
    .def(py::init<>())
    .def(py::init<biogears::Logger*>())
    .def("GetLogger",&biogears::Loggable::GetLogger);

 
#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}