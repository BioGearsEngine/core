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

#include <biogears/cdm/properties/SEScalarElectricCapacitance.h>

namespace py = pybind11;

using biogears::ElectricCapacitanceUnit;
using biogears::SEScalar;
using biogears::SEScalarElectricCapacitance;

void define_pybSEScalarElectricCapacitance(pybind11::module_ &m)
{
  py::class_<biogears::ElectricCapacitanceUnit>(m, "ElectricCapacitanceUnit")
    .def_readonly_static("F", &biogears::ElectricCapacitanceUnit::F);

  pybind11::class_<biogears::SEScalarElectricCapacitance>(m, "SEScalarElectricCapacitance")
    .def(py::init<>())

    .def("Unload", py::overload_cast<>(&SEScalarElectricCapacitance::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarElectricCapacitance::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarElectricCapacitance::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarElectricCapacitance::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarElectricCapacitance::GetValue, py::const_))
    .def("GetValue", py::overload_cast<ElectricCapacitanceUnit const&>(&SEScalarElectricCapacitance::GetValue, py::const_))
    .def("SetValue", &SEScalarElectricCapacitance::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarElectricCapacitance::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarElectricCapacitance::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const ElectricCapacitanceUnit&>(&SEScalarElectricCapacitance::IncrementValue))
    .def("Increment", &SEScalarElectricCapacitance::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarElectricCapacitance::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarElectricCapacitance::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const ElectricCapacitanceUnit&>(&SEScalarElectricCapacitance::DecrementValue))
    .def("Decrement", &SEScalarElectricCapacitance::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarElectricCapacitance::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarElectricCapacitance::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarElectricCapacitance::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarElectricCapacitance::Divide))

    //.def("Equals", &SEScalarElectricCapacitance::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarElectricCapacitance::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarElectricCapacitance::GetCompoundUnit, py::const_))
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
