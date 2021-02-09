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

#include <biogears/cdm/properties/SEScalarAmount.h>

namespace py = pybind11;
using biogears::AmountUnit;
using biogears::SEScalar;
using biogears::SEScalarAmount;
void define_pybSEScalarAmount(pybind11::module_& m)
{
  py::class_<AmountUnit>(m, "AmountUnit")
    .def_readonly_static("mol", &AmountUnit::mol)
    .def_readonly_static("pmol", &AmountUnit::pmol);

  pybind11::class_<SEScalarAmount>(m, "SEScalarAmount")
    .def(py::init<>())
    .def("Unload", py::overload_cast<>(&SEScalarAmount::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarAmount::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarAmount::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarAmount::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarAmount::GetValue, py::const_))
    .def("GetValue", py::overload_cast<AmountUnit const&>(&SEScalarAmount::GetValue, py::const_))
    .def("SetValue", &SEScalarAmount::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarAmount::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarAmount::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const AmountUnit&>(&SEScalarAmount::IncrementValue))
    .def("Increment",      &SEScalarAmount::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarAmount::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarAmount::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const AmountUnit&>(&SEScalarAmount::DecrementValue))
    .def("Decrement",      &SEScalarAmount::Decrement)
    .def("Multiple",       py::overload_cast<double>(&SEScalarAmount::MultiplyValue))
    .def("Multiply",       py::overload_cast<const SEScalar&>(&SEScalarAmount::Multiply))
    .def("DivideValue",    py::overload_cast<double>(&SEScalarAmount::DivideValue))
    .def("Divide",         py::overload_cast<const SEScalar&>(&SEScalarAmount::Divide))

    //.def("Equals", &SEScalarAmount::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarAmount::GetCompoundUnit, py::const_) )
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarAmount::GetCompoundUnit, py::const_) )
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
