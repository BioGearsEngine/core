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
#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/utils/Logger.h>

namespace py = pybind11;
using namespace pybind11::literals; 
using biogears::Loggable;
using biogears::Logger;

PYBIND11_MODULE(pybiogears, m)
{
  py::class_<biogears::Logger>(m, "Logger")
    .def(py::init<const char*, const char*>(), "Default construcctor for a logger.", "logFilename"_a = Loggable::empty_cStr, "working_dir"_a = Loggable::empty_cStr)
    .def("LogToConsole", &Logger::LogToConsole, "Set Console logging on/off", "value"_a)
    .def("FormatMessages", &Logger::FormatMessages, "Set MSG Formatting on/off", "value"_a)
    .def("ResetLogFile"  ,   static_cast<void (Logger::*)(const std::string&, const std::string&)> (&Logger::ResetLogFile),   "Reset the logfile initial conditions."
               , "logFilename"_a = Loggable::empty_cStr
               , "working_dir"_a = Loggable::empty_cStr)
    .def("SetLogLevel", &Logger::SetLogLevel, "Set Log Filtering level for log4cpp backend", "level"_a)
    .def("SetConsoleLogLevel", &Logger::SetConsoleLogLevel, "Set the console log filtering level for the log4cpp backend", "level"_a)
    .def("GetLogLevel", &Logger::GetLogLevel)
    .def("SetsetConversionPattern", &Logger::SetsetConversionPattern, "Set conversion pattern for file logging", "pattern"_a = Loggable::empty)
    .def("SetConsolesetConversionPattern", &Logger::SetConsolesetConversionPattern, "Set conversion pattern for console logging", "pattern"_a = Loggable::empty)
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
     m.doc() = "Python Bindings for BioGears Human Physiology Engine"; // optional module docstring
}