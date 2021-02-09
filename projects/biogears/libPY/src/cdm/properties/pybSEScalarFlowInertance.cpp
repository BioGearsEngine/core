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

#include <biogears/cdm/properties/SEScalarFlowInertance.h>

namespace py = pybind11;

using biogears::FlowInertanceUnit;
using biogears::SEScalar;
using biogears::SEScalarFlowInertance;

void define_pybSEScalarFlowInertance(pybind11::module_& m)
{
  py::class_<biogears::FlowInertanceUnit>(m, "FlowInertanceUnit")
    .def_readonly_static("mmHg_s2_Per_mL", &biogears::FlowInertanceUnit::mmHg_s2_Per_mL)
    .def_readonly_static("mmHg_s2_Per_L", &biogears::FlowInertanceUnit::mmHg_s2_Per_L)
    .def_readonly_static("cmH2O_s2_Per_mL", &biogears::FlowInertanceUnit::cmH2O_s2_Per_mL)
    .def_readonly_static("cmH2O_s2_Per_L", &biogears::FlowInertanceUnit::cmH2O_s2_Per_L)
    .def_readonly_static("Pa_s2_Per_m3", &biogears::FlowInertanceUnit::Pa_s2_Per_m3);

  pybind11::class_<biogears::SEScalarFlowInertance>(m, "SEScalarFlowInertance")
    .def(py::init<>())
      
    .def("Unload", py::overload_cast<>(&SEScalarFlowInertance::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarFlowInertance::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarFlowInertance::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarFlowInertance::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarFlowInertance::GetValue, py::const_))
    .def("GetValue", py::overload_cast<FlowInertanceUnit const&>(&SEScalarFlowInertance::GetValue, py::const_))
    .def("SetValue", &SEScalarFlowInertance::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarFlowInertance::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarFlowInertance::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const FlowInertanceUnit&>(&SEScalarFlowInertance::IncrementValue))
    .def("Increment", &SEScalarFlowInertance::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarFlowInertance::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarFlowInertance::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const FlowInertanceUnit&>(&SEScalarFlowInertance::DecrementValue))
    .def("Decrement", &SEScalarFlowInertance::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarFlowInertance::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarFlowInertance::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarFlowInertance::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarFlowInertance::Divide))

    //.def("Equals", &SEScalarFlowInertance::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarFlowInertance::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarFlowInertance::GetCompoundUnit, py::const_))
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
