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

#include <biogears/cdm/properties/SEScalarHeatCapacitancePerAmount.h>

namespace py = pybind11;

using biogears::HeatCapacitancePerAmountUnit;
using biogears::SEScalar;
using biogears::SEScalarHeatCapacitancePerAmount;

void define_pybSEScalarHeatCapacitancePerAmount(pybind11::module_& m)
{
  py::class_<biogears::HeatCapacitancePerAmountUnit>(m, "HeatCapacitancePerAmountUnit")
    .def_readonly_static("J_Per_K_mol", &biogears::HeatCapacitancePerAmountUnit::J_Per_K_mol);

  pybind11::class_<biogears::SEScalarHeatCapacitancePerAmount>(m, "SEScalarHeatCapacitancePerAmount")
    .def(py::init<>())
      
    .def("Unload", py::overload_cast<>(&SEScalarHeatCapacitancePerAmount::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarHeatCapacitancePerAmount::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarHeatCapacitancePerAmount::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarHeatCapacitancePerAmount::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarHeatCapacitancePerAmount::GetValue, py::const_))
    .def("GetValue", py::overload_cast<HeatCapacitancePerAmountUnit const&>(&SEScalarHeatCapacitancePerAmount::GetValue, py::const_))
    .def("SetValue", &SEScalarHeatCapacitancePerAmount::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarHeatCapacitancePerAmount::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarHeatCapacitancePerAmount::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const HeatCapacitancePerAmountUnit&>(&SEScalarHeatCapacitancePerAmount::IncrementValue))
    .def("Increment", &SEScalarHeatCapacitancePerAmount::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarHeatCapacitancePerAmount::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarHeatCapacitancePerAmount::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const HeatCapacitancePerAmountUnit&>(&SEScalarHeatCapacitancePerAmount::DecrementValue))
    .def("Decrement", &SEScalarHeatCapacitancePerAmount::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarHeatCapacitancePerAmount::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarHeatCapacitancePerAmount::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarHeatCapacitancePerAmount::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarHeatCapacitancePerAmount::Divide))

    //.def("Equals", &SEScalarHeatCapacitancePerAmount::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarHeatCapacitancePerAmount::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarHeatCapacitancePerAmount::GetCompoundUnit, py::const_))
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
