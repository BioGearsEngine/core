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

#include <biogears/cdm/properties/SEScalarForce.h>

namespace py = pybind11;

using biogears::ForceUnit;
using biogears::SEScalar;
using biogears::SEScalarForce;

void define_pybSEScalarForce(pybind11::module_ &m)
{
  py::class_<biogears::ForceUnit>(m, "ForceUnit")
    .def_readonly_static("N", &biogears::ForceUnit::N)
    .def_readonly_static("lbf", &biogears::ForceUnit::lbf)
    .def_readonly_static("dyn", &biogears::ForceUnit::dyn);

  pybind11::class_<biogears::SEScalarForce>(m, "SEScalarForce")
    .def(py::init<>())
  
    .def("Unload", py::overload_cast<>(&SEScalarForce::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarForce::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarForce::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarForce::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarForce::GetValue, py::const_))
    .def("GetValue", py::overload_cast<ForceUnit const&>(&SEScalarForce::GetValue, py::const_))
    .def("SetValue", &SEScalarForce::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarForce::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarForce::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const ForceUnit&>(&SEScalarForce::IncrementValue))
    .def("Increment", &SEScalarForce::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarForce::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarForce::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const ForceUnit&>(&SEScalarForce::DecrementValue))
    .def("Decrement", &SEScalarForce::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarForce::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarForce::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarForce::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarForce::Divide))

    //.def("Equals", &SEScalarForce::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarForce::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarForce::GetCompoundUnit, py::const_))
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
