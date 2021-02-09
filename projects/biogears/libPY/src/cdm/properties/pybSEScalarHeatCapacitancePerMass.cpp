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

#include <biogears/cdm/properties/SEScalarHeatCapacitancePerMass.h>

namespace py = pybind11;

using biogears::HeatCapacitancePerMassUnit;
using biogears::SEScalar;
using biogears::SEScalarHeatCapacitancePerMass;

void define_pybSEScalarHeatCapacitancePerMass(pybind11::module_ &m)
{
  py::class_<biogears::HeatCapacitancePerMassUnit>(m, "HeatCapacitancePerMassUnit")
    .def_readonly_static("J_Per_K_kg", &biogears::HeatCapacitancePerMassUnit::J_Per_K_kg)
    .def_readonly_static("kJ_Per_K_kg", &biogears::HeatCapacitancePerMassUnit::kJ_Per_K_kg)
    .def_readonly_static("kcal_Per_K_kg", &biogears::HeatCapacitancePerMassUnit::kcal_Per_K_kg)
    .def_readonly_static("kcal_Per_C_kg", &biogears::HeatCapacitancePerMassUnit::kcal_Per_C_kg);

  
  pybind11::class_<biogears::SEScalarHeatCapacitancePerMass>(m, "SEScalarHeatCapacitancePerMass")
    .def(py::init<>())
  
    .def("Unload", py::overload_cast<>(&SEScalarHeatCapacitancePerMass::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarHeatCapacitancePerMass::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarHeatCapacitancePerMass::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarHeatCapacitancePerMass::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarHeatCapacitancePerMass::GetValue, py::const_))
    .def("GetValue", py::overload_cast<HeatCapacitancePerMassUnit const&>(&SEScalarHeatCapacitancePerMass::GetValue, py::const_))
    .def("SetValue", &SEScalarHeatCapacitancePerMass::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarHeatCapacitancePerMass::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarHeatCapacitancePerMass::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const HeatCapacitancePerMassUnit&>(&SEScalarHeatCapacitancePerMass::IncrementValue))
    .def("Increment", &SEScalarHeatCapacitancePerMass::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarHeatCapacitancePerMass::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarHeatCapacitancePerMass::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const HeatCapacitancePerMassUnit&>(&SEScalarHeatCapacitancePerMass::DecrementValue))
    .def("Decrement", &SEScalarHeatCapacitancePerMass::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarHeatCapacitancePerMass::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarHeatCapacitancePerMass::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarHeatCapacitancePerMass::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarHeatCapacitancePerMass::Divide))

    //.def("Equals", &SEScalarHeatCapacitancePerMass::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarHeatCapacitancePerMass::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarHeatCapacitancePerMass::GetCompoundUnit, py::const_))
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
