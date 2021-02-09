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

#include <biogears/cdm/properties/SEScalarMassPerMass.h>

namespace py = pybind11;

using biogears::MassPerMassUnit;
using biogears::SEScalar;
using biogears::SEScalarMassPerMass;

void define_pybSEScalarMassPerMass(pybind11::module_& m)
{
  py::class_<biogears::MassPerMassUnit>(m, "MassPerMassUnit")
    .def_readonly_static("ug_Per_kg", &biogears::MassPerMassUnit::ug_Per_kg)
    .def_readonly_static("mg_Per_g", &biogears::MassPerMassUnit::mg_Per_g);

  pybind11::class_<biogears::SEScalarMassPerMass>(m, "SEScalarMassPerMass")
    .def(py::init<>())
      
    .def("Unload", py::overload_cast<>(&SEScalarMassPerMass::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarMassPerMass::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarMassPerMass::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarMassPerMass::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarMassPerMass::GetValue, py::const_))
    .def("GetValue", py::overload_cast<MassPerMassUnit const&>(&SEScalarMassPerMass::GetValue, py::const_))
    .def("SetValue", &SEScalarMassPerMass::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarMassPerMass::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarMassPerMass::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const MassPerMassUnit&>(&SEScalarMassPerMass::IncrementValue))
    .def("Increment", &SEScalarMassPerMass::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarMassPerMass::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarMassPerMass::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const MassPerMassUnit&>(&SEScalarMassPerMass::DecrementValue))
    .def("Decrement", &SEScalarMassPerMass::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarMassPerMass::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarMassPerMass::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarMassPerMass::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarMassPerMass::Divide))

    //.def("Equals", &SEScalarMassPerMass::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarMassPerMass::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarMassPerMass::GetCompoundUnit, py::const_))
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
