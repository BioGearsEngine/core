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

#include <biogears/cdm/properties/SEScalarElectricCharge.h>

namespace py = pybind11;

using biogears::ElectricChargeUnit;
using biogears::SEScalar;
using biogears::SEScalarElectricCharge;

void define_pybSEScalarElectricCharge(pybind11::module_ &m)
{
  py::class_<biogears::ElectricChargeUnit>(m, "ElectricChargeUnit")
    .def_readonly_static("C", &biogears::ElectricChargeUnit::C);

  pybind11::class_<biogears::SEScalarElectricCharge>(m, "SEScalarElectricCharge")
    .def(py::init<>())
  
    .def("Unload", py::overload_cast<>(&SEScalarElectricCharge::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarElectricCharge::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarElectricCharge::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarElectricCharge::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarElectricCharge::GetValue, py::const_))
    .def("GetValue", py::overload_cast<ElectricChargeUnit const&>(&SEScalarElectricCharge::GetValue, py::const_))
    .def("SetValue", &SEScalarElectricCharge::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarElectricCharge::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarElectricCharge::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const ElectricChargeUnit&>(&SEScalarElectricCharge::IncrementValue))
    .def("Increment", &SEScalarElectricCharge::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarElectricCharge::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarElectricCharge::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const ElectricChargeUnit&>(&SEScalarElectricCharge::DecrementValue))
    .def("Decrement", &SEScalarElectricCharge::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarElectricCharge::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarElectricCharge::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarElectricCharge::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarElectricCharge::Divide))

    //.def("Equals", &SEScalarElectricCharge::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarElectricCharge::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarElectricCharge::GetCompoundUnit, py::const_))
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
