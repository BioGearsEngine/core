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

#include <biogears/cdm/properties/SEScalarPowerPerAreaTemperatureToTheFourth.h>

namespace py = pybind11;

using biogears::PowerPerAreaTemperatureToTheFourthUnit;
using biogears::SEScalar;
using biogears::SEScalarPowerPerAreaTemperatureToTheFourth;

void define_pybSEScalarPowerPerAreaTemperatureToTheFourth(pybind11::module_ &m)
{
  py::class_<biogears::PowerPerAreaTemperatureToTheFourthUnit>(m, "PowerPerAreaTemperatureToTheFourthUnit")
    .def_readonly_static("W_Per_m2_K4", &biogears::PowerPerAreaTemperatureToTheFourthUnit::W_Per_m2_K4);

  pybind11::class_<biogears::SEScalarPowerPerAreaTemperatureToTheFourth>(m, "SEScalarPowerPerAreaTemperatureToTheFourth")
    .def(py::init<>())
  
    .def("Unload", py::overload_cast<>(&SEScalarPowerPerAreaTemperatureToTheFourth::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarPowerPerAreaTemperatureToTheFourth::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarPowerPerAreaTemperatureToTheFourth::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarPowerPerAreaTemperatureToTheFourth::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarPowerPerAreaTemperatureToTheFourth::GetValue, py::const_))
    .def("GetValue", py::overload_cast<PowerPerAreaTemperatureToTheFourthUnit const&>(&SEScalarPowerPerAreaTemperatureToTheFourth::GetValue, py::const_))
    .def("SetValue", &SEScalarPowerPerAreaTemperatureToTheFourth::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarPowerPerAreaTemperatureToTheFourth::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarPowerPerAreaTemperatureToTheFourth::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const PowerPerAreaTemperatureToTheFourthUnit&>(&SEScalarPowerPerAreaTemperatureToTheFourth::IncrementValue))
    .def("Increment", &SEScalarPowerPerAreaTemperatureToTheFourth::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarPowerPerAreaTemperatureToTheFourth::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarPowerPerAreaTemperatureToTheFourth::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const PowerPerAreaTemperatureToTheFourthUnit&>(&SEScalarPowerPerAreaTemperatureToTheFourth::DecrementValue))
    .def("Decrement", &SEScalarPowerPerAreaTemperatureToTheFourth::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarPowerPerAreaTemperatureToTheFourth::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarPowerPerAreaTemperatureToTheFourth::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarPowerPerAreaTemperatureToTheFourth::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarPowerPerAreaTemperatureToTheFourth::Divide))

    //.def("Equals", &SEScalarPowerPerAreaTemperatureToTheFourth::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarPowerPerAreaTemperatureToTheFourth::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarPowerPerAreaTemperatureToTheFourth::GetCompoundUnit, py::const_))
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
