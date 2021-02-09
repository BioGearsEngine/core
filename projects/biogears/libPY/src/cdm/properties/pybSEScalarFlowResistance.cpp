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

#include <biogears/cdm/properties/SEScalarFlowResistance.h>

namespace py = pybind11;

using biogears::FlowResistanceUnit;
using biogears::SEScalar;
using biogears::SEScalarFlowResistance;

void define_pybSEScalarFlowResistance(pybind11::module_& m)
{
  py::class_<biogears::FlowResistanceUnit>(m, "FlowResistanceUnit")
    .def_readonly_static("cmH2O_s_Per_L", &biogears::FlowResistanceUnit::cmH2O_s_Per_L)
    .def_readonly_static("mmHg_s_Per_mL", &biogears::FlowResistanceUnit::mmHg_s_Per_mL)
    .def_readonly_static("mmHg_min_Per_mL", &biogears::FlowResistanceUnit::mmHg_min_Per_mL)
    .def_readonly_static("mmHg_min_Per_L", &biogears::FlowResistanceUnit::mmHg_min_Per_L)
    .def_readonly_static("Pa_s_Per_m3", &biogears::FlowResistanceUnit::Pa_s_Per_m3);

  pybind11::class_<biogears::SEScalarFlowResistance>(m, "SEScalarFlowResistance")
    .def(py::init<>())
      
    .def("Unload", py::overload_cast<>(&SEScalarFlowResistance::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarFlowResistance::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarFlowResistance::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarFlowResistance::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarFlowResistance::GetValue, py::const_))
    .def("GetValue", py::overload_cast<FlowResistanceUnit const&>(&SEScalarFlowResistance::GetValue, py::const_))
    .def("SetValue", &SEScalarFlowResistance::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarFlowResistance::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarFlowResistance::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const FlowResistanceUnit&>(&SEScalarFlowResistance::IncrementValue))
    .def("Increment", &SEScalarFlowResistance::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarFlowResistance::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarFlowResistance::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const FlowResistanceUnit&>(&SEScalarFlowResistance::DecrementValue))
    .def("Decrement", &SEScalarFlowResistance::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarFlowResistance::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarFlowResistance::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarFlowResistance::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarFlowResistance::Divide))

    //.def("Equals", &SEScalarFlowResistance::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarFlowResistance::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarFlowResistance::GetCompoundUnit, py::const_))
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
