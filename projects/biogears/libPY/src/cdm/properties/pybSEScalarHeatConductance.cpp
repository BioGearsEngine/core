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

#include <biogears/cdm/properties/SEScalarHeatConductance.h>

namespace py = pybind11;

using biogears::HeatConductanceUnit;
using biogears::SEScalar;
using biogears::SEScalarHeatConductance;

void define_pybSEScalarHeatConductance(pybind11::module_ &m)
{
  py::class_<biogears::HeatConductanceUnit>(m, "HeatConductanceUnit")
    .def_readonly_static("W_Per_K", &biogears::HeatConductanceUnit::W_Per_K)
    .def_readonly_static("W_Per_C", &biogears::HeatConductanceUnit::W_Per_C)
    .def_readonly_static("kcal_Per_K_s", &biogears::HeatConductanceUnit::kcal_Per_K_s)
    .def_readonly_static("kcal_Per_C_s", &biogears::HeatConductanceUnit::kcal_Per_C_s);

  
  pybind11::class_<biogears::SEScalarHeatConductance>(m, "SEScalarHeatConductance")
    .def(py::init<>())
  
    .def("Unload", py::overload_cast<>(&SEScalarHeatConductance::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarHeatConductance::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarHeatConductance::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarHeatConductance::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarHeatConductance::GetValue, py::const_))
    .def("GetValue", py::overload_cast<HeatConductanceUnit const&>(&SEScalarHeatConductance::GetValue, py::const_))
    .def("SetValue", &SEScalarHeatConductance::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarHeatConductance::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarHeatConductance::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const HeatConductanceUnit&>(&SEScalarHeatConductance::IncrementValue))
    .def("Increment", &SEScalarHeatConductance::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarHeatConductance::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarHeatConductance::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const HeatConductanceUnit&>(&SEScalarHeatConductance::DecrementValue))
    .def("Decrement", &SEScalarHeatConductance::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarHeatConductance::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarHeatConductance::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarHeatConductance::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarHeatConductance::Divide))

    //.def("Equals", &SEScalarHeatConductance::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarHeatConductance::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarHeatConductance::GetCompoundUnit, py::const_))
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
