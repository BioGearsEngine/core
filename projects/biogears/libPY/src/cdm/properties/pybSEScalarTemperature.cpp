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

#include <biogears/cdm/properties/SEScalarTemperature.h>

namespace py = pybind11;

using biogears::TemperatureUnit;
using biogears::SEScalar;
using biogears::SEScalarTemperature;

void define_pybSEScalarTemperature(pybind11::module_& m)
{
  py::class_<biogears::TemperatureUnit>(m, "TemperatureUnit")
    .def_readonly_static("F", &biogears::TemperatureUnit::F)
    .def_readonly_static("C", &biogears::TemperatureUnit::C)
    .def_readonly_static("K", &biogears::TemperatureUnit::K)
    .def_readonly_static("R", &biogears::TemperatureUnit::R);

  pybind11::class_<biogears::SEScalarTemperature>(m, "SEScalarTemperature")
    .def(py::init<>())
      
    .def("Unload", py::overload_cast<>(&SEScalarTemperature::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarTemperature::ToString, py::const_))
    //.def("GetValue", py::overload_cast<std::string const&>(&SEScalarTemperature::GetValue, py::const_))
    //.def("GetValue", py::overload_cast<char const*>(&SEScalarTemperature::GetValue, py::const_))
    //.def("GetValue", py::overload_cast<>(&SEScalarTemperature::GetValue, py::const_))
    .def("GetValue", py::overload_cast<TemperatureUnit const&>(&SEScalarTemperature::GetValue, py::const_))
    .def("SetValue", &SEScalarTemperature::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarTemperature::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarTemperature::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const TemperatureUnit&>(&SEScalarTemperature::IncrementValue))
    .def("Increment", &SEScalarTemperature::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarTemperature::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarTemperature::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const TemperatureUnit&>(&SEScalarTemperature::DecrementValue))
    .def("Decrement", &SEScalarTemperature::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarTemperature::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarTemperature::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarTemperature::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarTemperature::Divide))

    //.def("Equals", &SEScalarTemperature::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarTemperature::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarTemperature::GetCompoundUnit, py::const_))
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
