/**************************************************************************************
Copyright 2015 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the License); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under

the License is distributed on an AS IS BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/
#include <pybind11/pybind11.h>  
#include <pybind11/operators.h>

#include <string>

#include <biogears/cdm/properties/SEScalarTime.h>

namespace py = pybind11;

using biogears::TimeUnit;
using biogears::SEScalar;
using biogears::SEScalarTime;

void define_pybSEScalarTime(pybind11::module_ &m)
{
  py::class_<biogears::TimeUnit>(m, "TimeUnit", py::module_local())
    .def_readonly_static("s", &biogears::TimeUnit::s)
    .def_readonly_static("min", &biogears::TimeUnit::min)
    .def_readonly_static("hr", &biogears::TimeUnit::hr)
    .def_readonly_static("day", &biogears::TimeUnit::day)
    .def_readonly_static("yr", &biogears::TimeUnit::yr);

  pybind11::class_<biogears::SEScalarTime>(m, "SEScalarTime")
    .def(py::init<>())
  
    .def("Unload", py::overload_cast<>(&SEScalarTime::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarTime::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarTime::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarTime::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarTime::GetValue, py::const_))
    .def("GetValue", py::overload_cast<TimeUnit const&>(&SEScalarTime::GetValue, py::const_))
    .def("SetValue", &SEScalarTime::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarTime::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarTime::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const TimeUnit&>(&SEScalarTime::IncrementValue))
    .def("Increment", &SEScalarTime::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarTime::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarTime::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const TimeUnit&>(&SEScalarTime::DecrementValue))
    .def("Decrement", &SEScalarTime::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarTime::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarTime::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarTime::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarTime::Divide))

    //.def("Equals", &SEScalarTime::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarTime::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarTime::GetCompoundUnit, py::const_))
    .def(py::self < py::self)
    .def(py::self <= py::self)
    .def(py::self > py::self)
    .def(py::self >= py::self)
    .def(py::self == py::self)
    .def(py::self != py::self)
    .def(py::self + py::self)
    .def(py::self += py::self)
    .def(py::self - py::self)
    .def(py::self -= py::self)
    .def(py::self * py::self)
    .def(py::self *= py::self)
    .def(py::self /= py::self)
    .def(py::self / py::self);


#ifdef VERSION_INFO
  m.attr("__version__") == VERSION_INFO
#else
  m.attr("__version__") = "dev";
#endif
}
