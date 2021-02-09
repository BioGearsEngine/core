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

#include <biogears/cdm/properties/SEScalarFlowCompliance.h>

namespace py = pybind11;

using biogears::FlowComplianceUnit;
using biogears::SEScalar;
using biogears::SEScalarFlowCompliance;

void define_pybSEScalarFlowCompliance(pybind11::module_ &m)
{
  py::class_<biogears::FlowComplianceUnit>(m, "FlowComplianceUnit")
    .def_readonly_static("L_Per_cmH2O", &biogears::FlowComplianceUnit::L_Per_cmH2O)
    .def_readonly_static("mL_Per_mmHg", &biogears::FlowComplianceUnit::mL_Per_mmHg)
    .def_readonly_static("mL_Per_cmH2O", &biogears::FlowComplianceUnit::mL_Per_cmH2O)
    .def_readonly_static("m3_Per_Pa", &biogears::FlowComplianceUnit::m3_Per_Pa);


  pybind11::class_<biogears::SEScalarFlowCompliance>(m, "SEScalarFlowCompliance")
    .def(py::init<>())
  
    .def("Unload", py::overload_cast<>(&SEScalarFlowCompliance::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarFlowCompliance::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarFlowCompliance::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarFlowCompliance::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarFlowCompliance::GetValue, py::const_))
    .def("GetValue", py::overload_cast<FlowComplianceUnit const&>(&SEScalarFlowCompliance::GetValue, py::const_))
    .def("SetValue", &SEScalarFlowCompliance::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarFlowCompliance::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarFlowCompliance::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const FlowComplianceUnit&>(&SEScalarFlowCompliance::IncrementValue))
    .def("Increment", &SEScalarFlowCompliance::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarFlowCompliance::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarFlowCompliance::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const FlowComplianceUnit&>(&SEScalarFlowCompliance::DecrementValue))
    .def("Decrement", &SEScalarFlowCompliance::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarFlowCompliance::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarFlowCompliance::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarFlowCompliance::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarFlowCompliance::Divide))

    //.def("Equals", &SEScalarFlowCompliance::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarFlowCompliance::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarFlowCompliance::GetCompoundUnit, py::const_))
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
