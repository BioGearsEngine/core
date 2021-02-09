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

#include <biogears/cdm/properties/SEScalarPressureTimePerArea.h>

namespace py = pybind11;

using biogears::PressureTimePerAreaUnit;
using biogears::SEScalar;
using biogears::SEScalarPressureTimePerArea;

void define_pybSEScalarPressureTimePerArea(pybind11::module_& m)
{
  py::class_<biogears::PressureTimePerAreaUnit>(m, "PressureTimePerAreaUnit")
    .def_readonly_static("mmHg_Per_mL_m2", &biogears::PressureTimePerAreaUnit::mmHg_Per_mL_m2)
    .def_readonly_static("cmH2O_Per_mL_m2", &biogears::PressureTimePerAreaUnit::cmH2O_Per_mL_m2);

  pybind11::class_<biogears::SEScalarPressureTimePerArea>(m, "SEScalarPressureTimePerArea")
    .def(py::init<>())
      
    .def("Unload", py::overload_cast<>(&SEScalarPressureTimePerArea::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarPressureTimePerArea::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarPressureTimePerArea::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarPressureTimePerArea::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarPressureTimePerArea::GetValue, py::const_))
    .def("GetValue", py::overload_cast<PressureTimePerAreaUnit const&>(&SEScalarPressureTimePerArea::GetValue, py::const_))
    .def("SetValue", &SEScalarPressureTimePerArea::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarPressureTimePerArea::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarPressureTimePerArea::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const PressureTimePerAreaUnit&>(&SEScalarPressureTimePerArea::IncrementValue))
    .def("Increment", &SEScalarPressureTimePerArea::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarPressureTimePerArea::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarPressureTimePerArea::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const PressureTimePerAreaUnit&>(&SEScalarPressureTimePerArea::DecrementValue))
    .def("Decrement", &SEScalarPressureTimePerArea::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarPressureTimePerArea::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarPressureTimePerArea::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarPressureTimePerArea::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarPressureTimePerArea::Divide))

    //.def("Equals", &SEScalarPressureTimePerArea::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarPressureTimePerArea::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarPressureTimePerArea::GetCompoundUnit, py::const_))
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
