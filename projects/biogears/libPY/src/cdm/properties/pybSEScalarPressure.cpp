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

#include <biogears/cdm/properties/SEScalarPressure.h>

namespace py = pybind11;

using biogears::PressureUnit;
using biogears::SEScalar;
using biogears::SEScalarPressure;

void define_pybSEScalarPressure(pybind11::module_ &m)
{
  py::class_<biogears::PressureUnit>(m, "PressureUnit")
    .def_readonly_static("Pa", &biogears::PressureUnit::Pa)
    .def_readonly_static("mmHg", &biogears::PressureUnit::mmHg)
    .def_readonly_static("cmH2O", &biogears::PressureUnit::cmH2O)
    .def_readonly_static("psi", &biogears::PressureUnit::psi)
    .def_readonly_static("atm", &biogears::PressureUnit::atm);

  pybind11::class_<biogears::SEScalarPressure>(m, "SEScalarPressure")
    .def(py::init<>())
  
    .def("Unload", py::overload_cast<>(&SEScalarPressure::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarPressure::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarPressure::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarPressure::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarPressure::GetValue, py::const_))
    .def("GetValue", py::overload_cast<PressureUnit const&>(&SEScalarPressure::GetValue, py::const_))
    .def("SetValue", &SEScalarPressure::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarPressure::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarPressure::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const PressureUnit&>(&SEScalarPressure::IncrementValue))
    .def("Increment", &SEScalarPressure::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarPressure::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarPressure::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const PressureUnit&>(&SEScalarPressure::DecrementValue))
    .def("Decrement", &SEScalarPressure::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarPressure::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarPressure::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarPressure::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarPressure::Divide))

    //.def("Equals", &SEScalarPressure::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarPressure::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarPressure::GetCompoundUnit, py::const_))
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
