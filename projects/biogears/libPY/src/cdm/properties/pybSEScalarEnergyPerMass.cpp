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

#include <biogears/cdm/properties/SEScalarEnergyPerMass.h>

namespace py = pybind11;

using biogears::EnergyPerMassUnit;
using biogears::SEScalar;
using biogears::SEScalarEnergyPerMass;

void define_pybSEScalarEnergyPerMass(pybind11::module_ &m)
{
  py::class_<biogears::EnergyPerMassUnit>(m, "EnergyPerMassUnit")
    .def_readonly_static("J_Per_kg", &biogears::EnergyPerMassUnit::J_Per_kg)
    .def_readonly_static("kJ_Per_kg", &biogears::EnergyPerMassUnit::kJ_Per_kg)
    .def_readonly_static("mJ_Per_kg", &biogears::EnergyPerMassUnit::mJ_Per_kg)
    .def_readonly_static("kcal_Per_kg", &biogears::EnergyPerMassUnit::kcal_Per_kg);

  pybind11::class_<biogears::SEScalarEnergyPerMass>(m, "SEScalarEnergyPerMass")
    .def(py::init<>())
  
    .def("Unload", py::overload_cast<>(&SEScalarEnergyPerMass::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarEnergyPerMass::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarEnergyPerMass::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarEnergyPerMass::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarEnergyPerMass::GetValue, py::const_))
    .def("GetValue", py::overload_cast<EnergyPerMassUnit const&>(&SEScalarEnergyPerMass::GetValue, py::const_))
    .def("SetValue", &SEScalarEnergyPerMass::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarEnergyPerMass::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarEnergyPerMass::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const EnergyPerMassUnit&>(&SEScalarEnergyPerMass::IncrementValue))
    .def("Increment", &SEScalarEnergyPerMass::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarEnergyPerMass::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarEnergyPerMass::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const EnergyPerMassUnit&>(&SEScalarEnergyPerMass::DecrementValue))
    .def("Decrement", &SEScalarEnergyPerMass::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarEnergyPerMass::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarEnergyPerMass::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarEnergyPerMass::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarEnergyPerMass::Divide))

    //.def("Equals", &SEScalarEnergyPerMass::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarEnergyPerMass::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarEnergyPerMass::GetCompoundUnit, py::const_))
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
