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

#include <biogears/cdm/properties/SEScalarMassPerAmount.h>

namespace py = pybind11;

using biogears::MassPerAmountUnit;
using biogears::SEScalar;
using biogears::SEScalarMassPerAmount;

void define_pybSEScalarMassPerAmount(pybind11::module_& m)
{
  py::class_<biogears::MassPerAmountUnit>(m, "MassPerAmountUnit")
    .def_readonly_static("g_Per_ct", &biogears::MassPerAmountUnit::g_Per_ct)
    .def_readonly_static("g_Per_mol", &biogears::MassPerAmountUnit::g_Per_mol)
    .def_readonly_static("g_Per_mmol", &biogears::MassPerAmountUnit::g_Per_mmol)
    .def_readonly_static("g_Per_umol", &biogears::MassPerAmountUnit::g_Per_umol)
    .def_readonly_static("mg_Per_mol", &biogears::MassPerAmountUnit::mg_Per_mol)
    .def_readonly_static("mg_Per_mmol", &biogears::MassPerAmountUnit::mg_Per_mmol)
    .def_readonly_static("kg_Per_mol", &biogears::MassPerAmountUnit::kg_Per_mol)
    .def_readonly_static("ug_Per_mmol", &biogears::MassPerAmountUnit::ug_Per_mmol)
    .def_readonly_static("ug_Per_mol", &biogears::MassPerAmountUnit::ug_Per_mol)
    .def_readonly_static("ug_Per_ct", &biogears::MassPerAmountUnit::ug_Per_ct)
    .def_readonly_static("pg_Per_ct", &biogears::MassPerAmountUnit::pg_Per_ct);

  pybind11::class_<biogears::SEScalarMassPerAmount>(m, "SEScalarMassPerAmount")
    .def(py::init<>())
      
    .def("Unload", py::overload_cast<>(&SEScalarMassPerAmount::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarMassPerAmount::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarMassPerAmount::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarMassPerAmount::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarMassPerAmount::GetValue, py::const_))
    .def("GetValue", py::overload_cast<MassPerAmountUnit const&>(&SEScalarMassPerAmount::GetValue, py::const_))
    .def("SetValue", &SEScalarMassPerAmount::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarMassPerAmount::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarMassPerAmount::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const MassPerAmountUnit&>(&SEScalarMassPerAmount::IncrementValue))
    .def("Increment", &SEScalarMassPerAmount::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarMassPerAmount::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarMassPerAmount::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const MassPerAmountUnit&>(&SEScalarMassPerAmount::DecrementValue))
    .def("Decrement", &SEScalarMassPerAmount::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarMassPerAmount::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarMassPerAmount::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarMassPerAmount::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarMassPerAmount::Divide))

    //.def("Equals", &SEScalarMassPerAmount::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarMassPerAmount::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarMassPerAmount::GetCompoundUnit, py::const_))
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
