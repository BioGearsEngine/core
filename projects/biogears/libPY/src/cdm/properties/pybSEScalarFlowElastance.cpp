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

#include <biogears/cdm/properties/SEScalarFlowElastance.h>

namespace py = pybind11;

using biogears::FlowElastanceUnit;
using biogears::SEScalar;
using biogears::SEScalarFlowElastance;

void define_pybSEScalarFlowElastance(pybind11::module_& m)
{
  py::class_<biogears::FlowElastanceUnit>(m, "FlowElastanceUnit")
    .def_readonly_static("cmH2O_Per_L", &biogears::FlowElastanceUnit::cmH2O_Per_L)
    .def_readonly_static("mmHg_Per_mL", &biogears::FlowElastanceUnit::mmHg_Per_mL)
    .def_readonly_static("Pa_Per_m3", &biogears::FlowElastanceUnit::Pa_Per_m3);

  pybind11::class_<biogears::SEScalarFlowElastance>(m, "SEScalarFlowElastance")
    .def(py::init<>())
  
    .def("Unload", py::overload_cast<>(&SEScalarFlowElastance::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarFlowElastance::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarFlowElastance::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarFlowElastance::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarFlowElastance::GetValue, py::const_))
    .def("GetValue", py::overload_cast<FlowElastanceUnit const&>(&SEScalarFlowElastance::GetValue, py::const_))
    .def("SetValue", &SEScalarFlowElastance::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarFlowElastance::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarFlowElastance::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const FlowElastanceUnit&>(&SEScalarFlowElastance::IncrementValue))
    .def("Increment", &SEScalarFlowElastance::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarFlowElastance::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarFlowElastance::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const FlowElastanceUnit&>(&SEScalarFlowElastance::DecrementValue))
    .def("Decrement", &SEScalarFlowElastance::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarFlowElastance::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarFlowElastance::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarFlowElastance::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarFlowElastance::Divide))

    //.def("Equals", &SEScalarFlowElastance::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarFlowElastance::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarFlowElastance::GetCompoundUnit, py::const_))
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
