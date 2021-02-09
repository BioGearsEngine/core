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

#include <biogears/cdm/properties/SEScalarAmountPerTime.h>

namespace py = pybind11;

using biogears::AmountPerTimeUnit;
using biogears::SEScalar;
using biogears::SEScalarAmountPerTime;

void define_pybSEScalarAmountPerTime(pybind11::module_& m)
{
  py::class_<biogears::AmountPerTimeUnit>(m, "AmountPerTimeUnit")
    .def_readonly_static("mol_Per_day", &biogears::AmountPerTimeUnit::mol_Per_day)
    .def_readonly_static("mol_Per_s", &biogears::AmountPerTimeUnit::mol_Per_s)
    .def_readonly_static("umol_Per_s", &biogears::AmountPerTimeUnit::umol_Per_s)
    .def_readonly_static("mmol_Per_min", &biogears::AmountPerTimeUnit::mmol_Per_min)
    .def_readonly_static("pmol_Per_min", &biogears::AmountPerTimeUnit::pmol_Per_min)
    .def_readonly_static("umol_Per_min", &biogears::AmountPerTimeUnit::umol_Per_min);

  pybind11::class_<biogears::SEScalarAmountPerTime>(m, "SEScalarAmountPerTime")
    .def(py::init<>())
    
    .def("Unload", py::overload_cast<>(&SEScalarAmountPerTime::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarAmountPerTime::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarAmountPerTime::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarAmountPerTime::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarAmountPerTime::GetValue, py::const_))
    .def("GetValue", py::overload_cast<AmountPerTimeUnit const&>(&SEScalarAmountPerTime::GetValue, py::const_))
    .def("SetValue", &SEScalarAmountPerTime::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarAmountPerTime::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarAmountPerTime::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const AmountPerTimeUnit&>(&SEScalarAmountPerTime::IncrementValue))
    .def("Increment", &SEScalarAmountPerTime::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarAmountPerTime::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarAmountPerTime::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const AmountPerTimeUnit&>(&SEScalarAmountPerTime::DecrementValue))
    .def("Decrement", &SEScalarAmountPerTime::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarAmountPerTime::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarAmountPerTime::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarAmountPerTime::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarAmountPerTime::Divide))

    //.def("Equals", &SEScalarAmountPerTime::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarAmountPerTime::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarAmountPerTime::GetCompoundUnit, py::const_))
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
