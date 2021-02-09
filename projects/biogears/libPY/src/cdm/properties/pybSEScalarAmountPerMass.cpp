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

#include <biogears/cdm/properties/SEScalarAmountPerMass.h>

namespace py = pybind11;

using biogears::AmountPerMassUnit;
using biogears::SEScalar;
using biogears::SEScalarAmountPerMass;

void define_pybSEScalarAmountPerMass(pybind11::module_& m)
{

  py::class_<biogears::AmountPerMassUnit>(m, "AmountPerMassUnit")
    .def_readonly_static("ct_Per_g", &biogears::AmountPerMassUnit::ct_Per_g)
    .def_readonly_static("ct_Per_ug", &biogears::AmountPerMassUnit::ct_Per_ug);

  pybind11::class_<biogears::SEScalarAmountPerMass>(m, "SEScalarAmountPerMass")
    .def(py::init<>())
    .def("Unload", py::overload_cast<>(&SEScalarAmountPerMass::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarAmountPerMass::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarAmountPerMass::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarAmountPerMass::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarAmountPerMass::GetValue, py::const_))
    .def("GetValue", py::overload_cast<AmountPerMassUnit const&>(&SEScalarAmountPerMass::GetValue, py::const_))
    .def("SetValue", &SEScalarAmountPerMass::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarAmountPerMass::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarAmountPerMass::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const AmountPerMassUnit&>(&SEScalarAmountPerMass::IncrementValue))
    .def("Increment", &SEScalarAmountPerMass::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarAmountPerMass::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarAmountPerMass::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const AmountPerMassUnit&>(&SEScalarAmountPerMass::DecrementValue))
    .def("Decrement", &SEScalarAmountPerMass::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarAmountPerMass::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarAmountPerMass::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarAmountPerMass::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarAmountPerMass::Divide))

    //.def("Equals", &SEScalarAmountPerMass::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarAmountPerMass::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarAmountPerMass::GetCompoundUnit, py::const_))
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
  ;

#ifdef VERSION_INFO
  m.attr("__version__") == VERSION_INFO
#else
  m.attr("__version__") = "dev";
#endif
}
