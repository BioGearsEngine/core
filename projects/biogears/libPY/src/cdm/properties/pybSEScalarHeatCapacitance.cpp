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

#include <biogears/cdm/properties/SEScalarHeatCapacitance.h>

namespace py = pybind11;

using biogears::HeatCapacitanceUnit;
using biogears::SEScalar;
using biogears::SEScalarHeatCapacitance;

void define_pybSEScalarHeatCapacitance(pybind11::module_& m)
{
  py::class_<biogears::HeatCapacitanceUnit>(m, "HeatCapacitanceUnit")
    .def_readonly_static("J_Per_K", &biogears::HeatCapacitanceUnit::J_Per_K)
    .def_readonly_static("kJ_Per_K", &biogears::HeatCapacitanceUnit::kJ_Per_K)
    .def_readonly_static("kcal_Per_K", &biogears::HeatCapacitanceUnit::kcal_Per_K)
    .def_readonly_static("kcal_Per_C", &biogears::HeatCapacitanceUnit::kcal_Per_C);

  pybind11::class_<biogears::SEScalarHeatCapacitance>(m, "SEScalarHeatCapacitance")
    .def(py::init<>())
      
    .def("Unload", py::overload_cast<>(&SEScalarHeatCapacitance::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarHeatCapacitance::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarHeatCapacitance::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarHeatCapacitance::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarHeatCapacitance::GetValue, py::const_))
    .def("GetValue", py::overload_cast<HeatCapacitanceUnit const&>(&SEScalarHeatCapacitance::GetValue, py::const_))
    .def("SetValue", &SEScalarHeatCapacitance::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarHeatCapacitance::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarHeatCapacitance::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const HeatCapacitanceUnit&>(&SEScalarHeatCapacitance::IncrementValue))
    .def("Increment", &SEScalarHeatCapacitance::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarHeatCapacitance::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarHeatCapacitance::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const HeatCapacitanceUnit&>(&SEScalarHeatCapacitance::DecrementValue))
    .def("Decrement", &SEScalarHeatCapacitance::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarHeatCapacitance::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarHeatCapacitance::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarHeatCapacitance::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarHeatCapacitance::Divide))

    //.def("Equals", &SEScalarHeatCapacitance::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarHeatCapacitance::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarHeatCapacitance::GetCompoundUnit, py::const_))
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
