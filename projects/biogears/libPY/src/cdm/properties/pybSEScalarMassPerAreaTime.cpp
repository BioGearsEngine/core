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

#include <biogears/cdm/properties/SEScalarMassPerAreaTime.h>

namespace py = pybind11;

using biogears::MassPerAreaTimeUnit;
using biogears::SEScalar;
using biogears::SEScalarMassPerAreaTime;

void define_pybSEScalarMassPerAreaTime(pybind11::module_& m)
{
  py::class_<biogears::MassPerAreaTimeUnit>(m, "MassPerAreaTimeUnit")
    .def_readonly_static("g_Per_cm2_s", &biogears::MassPerAreaTimeUnit::g_Per_cm2_s);

  pybind11::class_<biogears::SEScalarMassPerAreaTime>(m, "SEScalarMassPerAreaTime")
    .def(py::init<>())
      
    .def("Unload", py::overload_cast<>(&SEScalarMassPerAreaTime::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarMassPerAreaTime::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarMassPerAreaTime::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarMassPerAreaTime::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarMassPerAreaTime::GetValue, py::const_))
    .def("GetValue", py::overload_cast<MassPerAreaTimeUnit const&>(&SEScalarMassPerAreaTime::GetValue, py::const_))
    .def("SetValue", &SEScalarMassPerAreaTime::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarMassPerAreaTime::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarMassPerAreaTime::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const MassPerAreaTimeUnit&>(&SEScalarMassPerAreaTime::IncrementValue))
    .def("Increment", &SEScalarMassPerAreaTime::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarMassPerAreaTime::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarMassPerAreaTime::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const MassPerAreaTimeUnit&>(&SEScalarMassPerAreaTime::DecrementValue))
    .def("Decrement", &SEScalarMassPerAreaTime::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarMassPerAreaTime::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarMassPerAreaTime::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarMassPerAreaTime::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarMassPerAreaTime::Divide))

    //.def("Equals", &SEScalarMassPerAreaTime::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarMassPerAreaTime::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarMassPerAreaTime::GetCompoundUnit, py::const_))
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
