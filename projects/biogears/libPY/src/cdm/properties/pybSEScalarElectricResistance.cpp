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
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/properties/SEScalarElectricResistance.h>

namespace py = pybind11;

using biogears::ElectricResistanceUnit;
using biogears::SEScalar;
using biogears::SEScalarElectricResistance;

void define_pybSEScalarElectricResistance(pybind11::module_ &m)
{
  py::class_<biogears::ElectricResistanceUnit>(m, "ElectricResistanceUnit")
    .def_readonly_static("Ohm", &biogears::ElectricResistanceUnit::Ohm);

  pybind11::class_<biogears::SEScalarElectricResistance>(m, "SEScalarElectricResistance")
    .def(py::init<>())
  
    .def("Unload", py::overload_cast<>(&SEScalarElectricResistance::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarElectricResistance::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarElectricResistance::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarElectricResistance::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarElectricResistance::GetValue, py::const_))
    .def("GetValue", py::overload_cast<ElectricResistanceUnit const&>(&SEScalarElectricResistance::GetValue, py::const_))
    .def("SetValue", &SEScalarElectricResistance::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarElectricResistance::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarElectricResistance::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const ElectricResistanceUnit&>(&SEScalarElectricResistance::IncrementValue))
    .def("Increment", &SEScalarElectricResistance::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarElectricResistance::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarElectricResistance::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const ElectricResistanceUnit&>(&SEScalarElectricResistance::DecrementValue))
    .def("Decrement", &SEScalarElectricResistance::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarElectricResistance::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarElectricResistance::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarElectricResistance::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarElectricResistance::Divide))

    //.def("Equals", &SEScalarElectricResistance::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarElectricResistance::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarElectricResistance::GetCompoundUnit, py::const_))
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
