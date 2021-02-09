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

#include <biogears/cdm/properties/SEScalarHeatResistance.h>

namespace py = pybind11;

using biogears::HeatResistanceUnit;
using biogears::SEScalar;
using biogears::SEScalarHeatResistance;

void define_pybSEScalarHeatResistance(pybind11::module_& m)
{
  py::class_<biogears::HeatResistanceUnit>(m, "HeatResistanceUnit")
    .def_readonly_static("K_Per_W", &biogears::HeatResistanceUnit::K_Per_W)
    .def_readonly_static("C_Per_W", &biogears::HeatResistanceUnit::C_Per_W)
    .def_readonly_static("K_s_Per_kcal", &biogears::HeatResistanceUnit::K_s_Per_kcal)
    .def_readonly_static("C_s_Per_kcal", &biogears::HeatResistanceUnit::C_s_Per_kcal);

  pybind11::class_<biogears::SEScalarHeatResistance>(m, "SEScalarHeatResistance")
    .def(py::init<>())
      
    .def("Unload", py::overload_cast<>(&SEScalarHeatResistance::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarHeatResistance::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarHeatResistance::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarHeatResistance::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarHeatResistance::GetValue, py::const_))
    .def("GetValue", py::overload_cast<HeatResistanceUnit const&>(&SEScalarHeatResistance::GetValue, py::const_))
    .def("SetValue", &SEScalarHeatResistance::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarHeatResistance::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarHeatResistance::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const HeatResistanceUnit&>(&SEScalarHeatResistance::IncrementValue))
    .def("Increment", &SEScalarHeatResistance::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarHeatResistance::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarHeatResistance::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const HeatResistanceUnit&>(&SEScalarHeatResistance::DecrementValue))
    .def("Decrement", &SEScalarHeatResistance::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarHeatResistance::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarHeatResistance::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarHeatResistance::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarHeatResistance::Divide))

    //.def("Equals", &SEScalarHeatResistance::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarHeatResistance::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarHeatResistance::GetCompoundUnit, py::const_))
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
