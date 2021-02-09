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

#include <biogears/cdm/properties/SEScalarMassPerTime.h>

namespace py = pybind11;

using biogears::MassPerTimeUnit;
using biogears::SEScalar;
using biogears::SEScalarMassPerTime;

void define_pybSEScalarMassPerTime(pybind11::module_& m)
{
  py::class_<biogears::MassPerTimeUnit>(m, "MassPerTimeUnit")
    .def_readonly_static("g_Per_s", &biogears::MassPerTimeUnit::g_Per_s)
    .def_readonly_static("g_Per_min", &biogears::MassPerTimeUnit::g_Per_min)
    .def_readonly_static("g_Per_day", &biogears::MassPerTimeUnit::g_Per_day)
    .def_readonly_static("mg_Per_s", &biogears::MassPerTimeUnit::mg_Per_s)
    .def_readonly_static("mg_Per_min", &biogears::MassPerTimeUnit::mg_Per_min)
    .def_readonly_static("ug_Per_s", &biogears::MassPerTimeUnit::ug_Per_s)
    .def_readonly_static("kg_Per_s", &biogears::MassPerTimeUnit::kg_Per_s)
    .def_readonly_static("ug_Per_min", &biogears::MassPerTimeUnit::ug_Per_min);

  pybind11::class_<biogears::SEScalarMassPerTime>(m, "SEScalarMassPerTime")
    .def(py::init<>())
      
    .def("Unload", py::overload_cast<>(&SEScalarMassPerTime::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarMassPerTime::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarMassPerTime::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarMassPerTime::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarMassPerTime::GetValue, py::const_))
    .def("GetValue", py::overload_cast<MassPerTimeUnit const&>(&SEScalarMassPerTime::GetValue, py::const_))
    .def("SetValue", &SEScalarMassPerTime::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarMassPerTime::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarMassPerTime::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const MassPerTimeUnit&>(&SEScalarMassPerTime::IncrementValue))
    .def("Increment", &SEScalarMassPerTime::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarMassPerTime::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarMassPerTime::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const MassPerTimeUnit&>(&SEScalarMassPerTime::DecrementValue))
    .def("Decrement", &SEScalarMassPerTime::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarMassPerTime::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarMassPerTime::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarMassPerTime::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarMassPerTime::Divide))

    //.def("Equals", &SEScalarMassPerTime::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarMassPerTime::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarMassPerTime::GetCompoundUnit, py::const_))
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
