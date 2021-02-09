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

#include <biogears/cdm/properties/SEScalarMass.h>

namespace py = pybind11;

using biogears::MassUnit;
using biogears::SEScalar;
using biogears::SEScalarMass;

void define_pybSEScalarMass(pybind11::module_& m)
{
  py::class_<biogears::MassUnit>(m, "MassUnit")
    .def_readonly_static("g", &biogears::MassUnit::g)
    .def_readonly_static("ug", &biogears::MassUnit::ug)
    .def_readonly_static("mg", &biogears::MassUnit::mg)
    .def_readonly_static("kg", &biogears::MassUnit::kg)
    .def_readonly_static("lb", &biogears::MassUnit::lb);

  pybind11::class_<biogears::SEScalarMass>(m, "SEScalarMass")
    .def(py::init<>())
      
    .def("Unload", py::overload_cast<>(&SEScalarMass::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarMass::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarMass::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarMass::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarMass::GetValue, py::const_))
    .def("GetValue", py::overload_cast<MassUnit const&>(&SEScalarMass::GetValue, py::const_))
    .def("SetValue", &SEScalarMass::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarMass::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarMass::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const MassUnit&>(&SEScalarMass::IncrementValue))
    .def("Increment", &SEScalarMass::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarMass::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarMass::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const MassUnit&>(&SEScalarMass::DecrementValue))
    .def("Decrement", &SEScalarMass::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarMass::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarMass::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarMass::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarMass::Divide))

    //.def("Equals", &SEScalarMass::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarMass::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarMass::GetCompoundUnit, py::const_))
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
