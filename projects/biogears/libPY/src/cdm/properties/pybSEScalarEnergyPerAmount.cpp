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

#include <biogears/cdm/properties/SEScalarEnergyPerAmount.h>

namespace py = pybind11;

using biogears::EnergyPerAmountUnit;
using biogears::SEScalar;
using biogears::SEScalarEnergyPerAmount;

void define_pybSEScalarEnergyPerAmount(pybind11::module_& m)
{
  pybind11::class_<biogears::EnergyPerAmountUnit>(m, "EnergyPerAmountUnit");

  py::class_<biogears::EnergyPerAmountUnit>(m, "EnergyPerAmountUnit")
    .def_readonly_static("kcal_Per_mol", &biogears::EnergyPerAmountUnit::kcal_Per_mol)
    .def_readonly_static("kJ_Per_mol", &biogears::EnergyPerAmountUnit::kJ_Per_mol);

  pybind11::class_<biogears::SEScalarEnergyPerAmount>(m, "SEScalarEnergyPerAmount")
    .def(py::init<>())
  
    .def("Unload", py::overload_cast<>(&SEScalarEnergyPerAmount::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarEnergyPerAmount::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarEnergyPerAmount::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarEnergyPerAmount::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarEnergyPerAmount::GetValue, py::const_))
    .def("GetValue", py::overload_cast<EnergyPerAmountUnit const&>(&SEScalarEnergyPerAmount::GetValue, py::const_))
    .def("SetValue", &SEScalarEnergyPerAmount::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarEnergyPerAmount::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarEnergyPerAmount::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const EnergyPerAmountUnit&>(&SEScalarEnergyPerAmount::IncrementValue))
    .def("Increment", &SEScalarEnergyPerAmount::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarEnergyPerAmount::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarEnergyPerAmount::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const EnergyPerAmountUnit&>(&SEScalarEnergyPerAmount::DecrementValue))
    .def("Decrement", &SEScalarEnergyPerAmount::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarEnergyPerAmount::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarEnergyPerAmount::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarEnergyPerAmount::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarEnergyPerAmount::Divide))

    //.def("Equals", &SEScalarEnergyPerAmount::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarEnergyPerAmount::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarEnergyPerAmount::GetCompoundUnit, py::const_))
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
