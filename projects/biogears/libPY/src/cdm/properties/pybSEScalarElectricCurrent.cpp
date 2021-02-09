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

#include <biogears/cdm/properties/SEScalarElectricCurrent.h>

namespace py = pybind11;

using biogears::ElectricCurrentUnit;
using biogears::SEScalar;
using biogears::SEScalarElectricCurrent;

void define_pybSEScalarElectricCurrent(pybind11::module_ &m)
{
  py::class_<biogears::ElectricCurrentUnit>(m, "ElectricCurrentUnit")
    .def_readonly_static("A", &biogears::ElectricCurrentUnit::A);

  pybind11::class_<biogears::SEScalarElectricCurrent>(m, "SEScalarElectricCurrent")
    .def(py::init<>())
   
    .def("Unload", py::overload_cast<>(&SEScalarElectricCurrent::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarElectricCurrent::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarElectricCurrent::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarElectricCurrent::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarElectricCurrent::GetValue, py::const_))
    .def("GetValue", py::overload_cast<ElectricCurrentUnit const&>(&SEScalarElectricCurrent::GetValue, py::const_))
    .def("SetValue", &SEScalarElectricCurrent::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarElectricCurrent::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarElectricCurrent::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const ElectricCurrentUnit&>(&SEScalarElectricCurrent::IncrementValue))
    .def("Increment", &SEScalarElectricCurrent::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarElectricCurrent::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarElectricCurrent::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const ElectricCurrentUnit&>(&SEScalarElectricCurrent::DecrementValue))
    .def("Decrement", &SEScalarElectricCurrent::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarElectricCurrent::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarElectricCurrent::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarElectricCurrent::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarElectricCurrent::Divide))

    //.def("Equals", &SEScalarElectricCurrent::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarElectricCurrent::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarElectricCurrent::GetCompoundUnit, py::const_))
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
