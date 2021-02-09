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

#include <biogears/cdm/properties/SEScalarHeatInductance.h>

namespace py = pybind11;

using biogears::HeatInductanceUnit;
using biogears::SEScalar;
using biogears::SEScalarHeatInductance;

void define_pybSEScalarHeatInductance(pybind11::module_ &m)
{
  py::class_<biogears::HeatInductanceUnit>(m, "HeatInductanceUnit")
    .def_readonly_static("K_s_Per_W", &biogears::HeatInductanceUnit::K_s_Per_W);

  pybind11::class_<biogears::SEScalarHeatInductance>(m, "SEScalarHeatInductance")
    .def(py::init<>())
  
    .def("Unload", py::overload_cast<>(&SEScalarHeatInductance::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarHeatInductance::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarHeatInductance::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarHeatInductance::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarHeatInductance::GetValue, py::const_))
    .def("GetValue", py::overload_cast<HeatInductanceUnit const&>(&SEScalarHeatInductance::GetValue, py::const_))
    .def("SetValue", &SEScalarHeatInductance::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarHeatInductance::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarHeatInductance::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const HeatInductanceUnit&>(&SEScalarHeatInductance::IncrementValue))
    .def("Increment", &SEScalarHeatInductance::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarHeatInductance::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarHeatInductance::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const HeatInductanceUnit&>(&SEScalarHeatInductance::DecrementValue))
    .def("Decrement", &SEScalarHeatInductance::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarHeatInductance::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarHeatInductance::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarHeatInductance::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarHeatInductance::Divide))

    //.def("Equals", &SEScalarHeatInductance::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarHeatInductance::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarHeatInductance::GetCompoundUnit, py::const_))
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
