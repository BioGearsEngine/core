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

#include <biogears/cdm/properties/SEScalarLengthPerTime.h>

namespace py = pybind11;

using biogears::LengthPerTimeUnit;
using biogears::SEScalar;
using biogears::SEScalarLengthPerTime;

void define_pybSEScalarLengthPerTime(pybind11::module_& m)
{
  py::class_<biogears::LengthPerTimeUnit>(m, "LengthPerTimeUnit")
    .def_readonly_static("m_Per_s", &biogears::LengthPerTimeUnit::m_Per_s)
    .def_readonly_static("cm_Per_s", &biogears::LengthPerTimeUnit::cm_Per_s)
    .def_readonly_static("m_Per_min", &biogears::LengthPerTimeUnit::m_Per_min)
    .def_readonly_static("cm_Per_min", &biogears::LengthPerTimeUnit::cm_Per_min)
    .def_readonly_static("ft_Per_s", &biogears::LengthPerTimeUnit::ft_Per_s)
    .def_readonly_static("ft_Per_min", &biogears::LengthPerTimeUnit::ft_Per_min);

  pybind11::class_<biogears::SEScalarLengthPerTime>(m, "SEScalarLengthPerTime")
    .def(py::init<>())
      
    .def("Unload", py::overload_cast<>(&SEScalarLengthPerTime::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarLengthPerTime::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarLengthPerTime::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarLengthPerTime::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarLengthPerTime::GetValue, py::const_))
    .def("GetValue", py::overload_cast<LengthPerTimeUnit const&>(&SEScalarLengthPerTime::GetValue, py::const_))
    .def("SetValue", &SEScalarLengthPerTime::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarLengthPerTime::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarLengthPerTime::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const LengthPerTimeUnit&>(&SEScalarLengthPerTime::IncrementValue))
    .def("Increment", &SEScalarLengthPerTime::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarLengthPerTime::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarLengthPerTime::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const LengthPerTimeUnit&>(&SEScalarLengthPerTime::DecrementValue))
    .def("Decrement", &SEScalarLengthPerTime::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarLengthPerTime::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarLengthPerTime::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarLengthPerTime::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarLengthPerTime::Divide))

    //.def("Equals", &SEScalarLengthPerTime::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarLengthPerTime::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarLengthPerTime::GetCompoundUnit, py::const_))
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
