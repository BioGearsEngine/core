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
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/properties/SEScalarArea.h>

namespace py = pybind11;

using biogears::AreaUnit;
using biogears::SEScalar;
using biogears::SEScalarArea;

void define_pybSEScalarArea(pybind11::module_ &m)
{
  py::class_<biogears::AreaUnit>(m, "AreaUnit")
    .def_readonly_static("cm2", &biogears::AreaUnit::cm2)
    .def_readonly_static("m2", &biogears::AreaUnit::m2);

  pybind11::class_<biogears::SEScalarArea>(m, "SEScalarArea")
    .def(py::init<>())
    
    .def("Unload", py::overload_cast<>(&SEScalarArea::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarArea::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarArea::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarArea::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarArea::GetValue, py::const_))
    .def("GetValue", py::overload_cast<AreaUnit const&>(&SEScalarArea::GetValue, py::const_))
    .def("SetValue", &SEScalarArea::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarArea::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarArea::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const AreaUnit&>(&SEScalarArea::IncrementValue))
    .def("Increment", &SEScalarArea::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarArea::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarArea::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const AreaUnit&>(&SEScalarArea::DecrementValue))
    .def("Decrement", &SEScalarArea::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarArea::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarArea::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarArea::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarArea::Divide))

    //.def("Equals", &SEScalarArea::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarArea::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarArea::GetCompoundUnit, py::const_))
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
    .def(py::self / py::self)
    ;

#ifdef VERSION_INFO
  m.attr("__version__") == VERSION_INFO
#else
  m.attr("__version__") = "dev";
#endif
}
