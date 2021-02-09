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

#include <biogears/cdm/properties/SEScalarLength.h>

namespace py = pybind11;

using biogears::LengthUnit;
using biogears::SEScalar;
using biogears::SEScalarLength;

void define_pybSEScalarLength(pybind11::module_ &m)
{
  py::class_<biogears::LengthUnit>(m, "LengthUnit")
    .def_readonly_static("m", &biogears::LengthUnit::m)
    .def_readonly_static("cm", &biogears::LengthUnit::cm)
    .def_readonly_static("mm", &biogears::LengthUnit::mm)
    .def_readonly_static("um", &biogears::LengthUnit::um)
    .def_readonly_static("in", &biogears::LengthUnit::in)
    .def_readonly_static("ft", &biogears::LengthUnit::ft);

  pybind11::class_<biogears::SEScalarLength>(m, "SEScalarLength")
    .def(py::init<>())
  
    .def("Unload", py::overload_cast<>(&SEScalarLength::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarLength::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarLength::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarLength::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarLength::GetValue, py::const_))
    .def("GetValue", py::overload_cast<LengthUnit const&>(&SEScalarLength::GetValue, py::const_))
    .def("SetValue", &SEScalarLength::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarLength::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarLength::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const LengthUnit&>(&SEScalarLength::IncrementValue))
    .def("Increment", &SEScalarLength::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarLength::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarLength::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const LengthUnit&>(&SEScalarLength::DecrementValue))
    .def("Decrement", &SEScalarLength::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarLength::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarLength::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarLength::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarLength::Divide))

    //.def("Equals", &SEScalarLength::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarLength::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarLength::GetCompoundUnit, py::const_))
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
