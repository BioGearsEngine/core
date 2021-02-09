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

#include <biogears/cdm/properties/SEScalarElectricInductance.h>

namespace py = pybind11;

using biogears::ElectricInductanceUnit;
using biogears::SEScalar;
using biogears::SEScalarElectricInductance;

void define_pybSEScalarElectricInductance(pybind11::module_ &m)
{
  py::class_<biogears::ElectricInductanceUnit>(m, "ElectricInductanceUnit")
    .def_readonly_static("H", &biogears::ElectricInductanceUnit::H);

  pybind11::class_<biogears::SEScalarElectricInductance>(m, "SEScalarElectricInductance")
    .def(py::init<>())
  
    .def("Unload", py::overload_cast<>(&SEScalarElectricInductance::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarElectricInductance::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarElectricInductance::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarElectricInductance::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarElectricInductance::GetValue, py::const_))
    .def("GetValue", py::overload_cast<ElectricInductanceUnit const&>(&SEScalarElectricInductance::GetValue, py::const_))
    .def("SetValue", &SEScalarElectricInductance::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarElectricInductance::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarElectricInductance::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const ElectricInductanceUnit&>(&SEScalarElectricInductance::IncrementValue))
    .def("Increment", &SEScalarElectricInductance::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarElectricInductance::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarElectricInductance::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const ElectricInductanceUnit&>(&SEScalarElectricInductance::DecrementValue))
    .def("Decrement", &SEScalarElectricInductance::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarElectricInductance::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarElectricInductance::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarElectricInductance::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarElectricInductance::Divide))

    //.def("Equals", &SEScalarElectricInductance::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarElectricInductance::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarElectricInductance::GetCompoundUnit, py::const_))
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
