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

#include <biogears/cdm/properties/SEScalarElectricPotential.h>

namespace py = pybind11;

using biogears::ElectricPotentialUnit;
using biogears::SEScalar;
using biogears::SEScalarElectricPotential;

void define_pybSEScalarElectricPotential(pybind11::module_ &m)
{
  py::class_<biogears::ElectricPotentialUnit>(m, "ElectricPotentialUnit")
    .def_readonly_static("V", &biogears::ElectricPotentialUnit::V)
    .def_readonly_static("mV", &biogears::ElectricPotentialUnit::mV);

  pybind11::class_<biogears::SEScalarElectricPotential>(m, "SEScalarElectricPotential")
    .def(py::init<>())
  
    .def("Unload", py::overload_cast<>(&SEScalarElectricPotential::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarElectricPotential::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarElectricPotential::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarElectricPotential::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarElectricPotential::GetValue, py::const_))
    .def("GetValue", py::overload_cast<ElectricPotentialUnit const&>(&SEScalarElectricPotential::GetValue, py::const_))
    .def("SetValue", &SEScalarElectricPotential::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarElectricPotential::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarElectricPotential::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const ElectricPotentialUnit&>(&SEScalarElectricPotential::IncrementValue))
    .def("Increment", &SEScalarElectricPotential::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarElectricPotential::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarElectricPotential::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const ElectricPotentialUnit&>(&SEScalarElectricPotential::DecrementValue))
    .def("Decrement", &SEScalarElectricPotential::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarElectricPotential::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarElectricPotential::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarElectricPotential::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarElectricPotential::Divide))

    //.def("Equals", &SEScalarElectricPotential::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarElectricPotential::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarElectricPotential::GetCompoundUnit, py::const_))
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
