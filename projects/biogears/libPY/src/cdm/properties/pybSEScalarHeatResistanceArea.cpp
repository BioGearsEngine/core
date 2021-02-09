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

#include <biogears/cdm/properties/SEScalarHeatResistanceArea.h>

namespace py = pybind11;

using biogears::HeatResistanceAreaUnit;
using biogears::SEScalar;
using biogears::SEScalarHeatResistanceArea;

void define_pybSEScalarHeatResistanceArea(pybind11::module_& m)
{
  py::class_<biogears::HeatResistanceAreaUnit>(m, "HeatResistanceAreaUnit")
    .def_readonly_static("rsi", &biogears::HeatResistanceAreaUnit::rsi)
    .def_readonly_static("clo", &biogears::HeatResistanceAreaUnit::clo)
    .def_readonly_static("rValue", &biogears::HeatResistanceAreaUnit::rValue)
    .def_readonly_static("tog", &biogears::HeatResistanceAreaUnit::tog);

  pybind11::class_<biogears::SEScalarHeatResistanceArea>(m, "SEScalarHeatResistanceArea")
    .def(py::init<>())
      
    .def("Unload", py::overload_cast<>(&SEScalarHeatResistanceArea::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarHeatResistanceArea::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarHeatResistanceArea::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarHeatResistanceArea::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarHeatResistanceArea::GetValue, py::const_))
    .def("GetValue", py::overload_cast<HeatResistanceAreaUnit const&>(&SEScalarHeatResistanceArea::GetValue, py::const_))
    .def("SetValue", &SEScalarHeatResistanceArea::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarHeatResistanceArea::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarHeatResistanceArea::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const HeatResistanceAreaUnit&>(&SEScalarHeatResistanceArea::IncrementValue))
    .def("Increment", &SEScalarHeatResistanceArea::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarHeatResistanceArea::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarHeatResistanceArea::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const HeatResistanceAreaUnit&>(&SEScalarHeatResistanceArea::DecrementValue))
    .def("Decrement", &SEScalarHeatResistanceArea::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarHeatResistanceArea::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarHeatResistanceArea::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarHeatResistanceArea::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarHeatResistanceArea::Divide))

    //.def("Equals", &SEScalarHeatResistanceArea::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarHeatResistanceArea::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarHeatResistanceArea::GetCompoundUnit, py::const_))
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
