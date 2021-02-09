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

#include <biogears/cdm/properties/SEScalarHeatConductancePerArea.h>

namespace py = pybind11;

using biogears::HeatConductancePerAreaUnit;
using biogears::SEScalar;
using biogears::SEScalarHeatConductancePerArea;

void define_pybSEScalarHeatConductancePerArea(pybind11::module_& m)
{
  py::class_<biogears::HeatConductancePerAreaUnit>(m, "HeatConductancePerAreaUnit")
    .def_readonly_static("W_Per_m2_K", &biogears::HeatConductancePerAreaUnit::W_Per_m2_K)
    .def_readonly_static("W_Per_m2_C", &biogears::HeatConductancePerAreaUnit::W_Per_m2_C)
    .def_readonly_static("BTU_Per_hr_ft2_C", &biogears::HeatConductancePerAreaUnit::BTU_Per_hr_ft2_C);

  pybind11::class_<biogears::SEScalarHeatConductancePerArea>(m, "SEScalarHeatConductancePerArea")
    .def(py::init<>())
      
    .def("Unload", py::overload_cast<>(&SEScalarHeatConductancePerArea::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarHeatConductancePerArea::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarHeatConductancePerArea::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarHeatConductancePerArea::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarHeatConductancePerArea::GetValue, py::const_))
    .def("GetValue", py::overload_cast<HeatConductancePerAreaUnit const&>(&SEScalarHeatConductancePerArea::GetValue, py::const_))
    .def("SetValue", &SEScalarHeatConductancePerArea::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarHeatConductancePerArea::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarHeatConductancePerArea::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const HeatConductancePerAreaUnit&>(&SEScalarHeatConductancePerArea::IncrementValue))
    .def("Increment", &SEScalarHeatConductancePerArea::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarHeatConductancePerArea::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarHeatConductancePerArea::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const HeatConductancePerAreaUnit&>(&SEScalarHeatConductancePerArea::DecrementValue))
    .def("Decrement", &SEScalarHeatConductancePerArea::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarHeatConductancePerArea::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarHeatConductancePerArea::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarHeatConductancePerArea::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarHeatConductancePerArea::Divide))

    //.def("Equals", &SEScalarHeatConductancePerArea::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarHeatConductancePerArea::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarHeatConductancePerArea::GetCompoundUnit, py::const_))
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
