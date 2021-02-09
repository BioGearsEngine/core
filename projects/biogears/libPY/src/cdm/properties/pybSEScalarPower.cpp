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

#include <biogears/cdm/properties/SEScalarPower.h>

namespace py = pybind11;

using biogears::PowerUnit;
using biogears::SEScalar;
using biogears::SEScalarPower;

void define_pybSEScalarPower(pybind11::module_& m)
{
  py::class_<biogears::PowerUnit>(m, "PowerUnit")
    .def_readonly_static("W", &biogears::PowerUnit::W)
    .def_readonly_static("kcal_Per_s", &biogears::PowerUnit::kcal_Per_s)
    .def_readonly_static("kcal_Per_hr", &biogears::PowerUnit::kcal_Per_hr)
    .def_readonly_static("kcal_Per_day", &biogears::PowerUnit::kcal_Per_day)
    .def_readonly_static("J_Per_s", &biogears::PowerUnit::J_Per_s)
    .def_readonly_static("BTU_Per_hr", &biogears::PowerUnit::BTU_Per_hr);

  pybind11::class_<biogears::SEScalarPower>(m, "SEScalarPower")
    .def(py::init<>())
      
    .def("Unload", py::overload_cast<>(&SEScalarPower::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarPower::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarPower::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarPower::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarPower::GetValue, py::const_))
    .def("GetValue", py::overload_cast<PowerUnit const&>(&SEScalarPower::GetValue, py::const_))
    .def("SetValue", &SEScalarPower::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarPower::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarPower::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const PowerUnit&>(&SEScalarPower::IncrementValue))
    .def("Increment", &SEScalarPower::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarPower::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarPower::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const PowerUnit&>(&SEScalarPower::DecrementValue))
    .def("Decrement", &SEScalarPower::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarPower::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarPower::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarPower::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarPower::Divide))

    //.def("Equals", &SEScalarPower::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarPower::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarPower::GetCompoundUnit, py::const_))
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
