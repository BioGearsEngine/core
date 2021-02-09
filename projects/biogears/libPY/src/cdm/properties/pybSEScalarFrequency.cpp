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

#include <biogears/cdm/properties/SEScalarFrequency.h>

namespace py = pybind11;

using biogears::FrequencyUnit;
using biogears::SEScalar;
using biogears::SEScalarFrequency;

void define_pybSEScalarFrequency(pybind11::module_ &m)
{
  py::class_<biogears::FrequencyUnit>(m, "FrequencyUnit")
    .def_readonly_static("Per_min", &biogears::FrequencyUnit::Per_min)
    .def_readonly_static("Per_s", &biogears::FrequencyUnit::Per_s)
    .def_readonly_static("Hz", &biogears::FrequencyUnit::Hz)
    .def_readonly_static("Per_hr", &biogears::FrequencyUnit::Per_hr);


  pybind11::class_<biogears::SEScalarFrequency>(m, "SEScalarFrequency")
    .def(py::init<>())
  
    .def("Unload", py::overload_cast<>(&SEScalarFrequency::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarFrequency::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarFrequency::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarFrequency::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarFrequency::GetValue, py::const_))
    .def("GetValue", py::overload_cast<FrequencyUnit const&>(&SEScalarFrequency::GetValue, py::const_))
    .def("SetValue", &SEScalarFrequency::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarFrequency::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarFrequency::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const FrequencyUnit&>(&SEScalarFrequency::IncrementValue))
    .def("Increment", &SEScalarFrequency::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarFrequency::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarFrequency::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const FrequencyUnit&>(&SEScalarFrequency::DecrementValue))
    .def("Decrement", &SEScalarFrequency::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarFrequency::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarFrequency::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarFrequency::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarFrequency::Divide))

    //.def("Equals", &SEScalarFrequency::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarFrequency::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarFrequency::GetCompoundUnit, py::const_))
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
