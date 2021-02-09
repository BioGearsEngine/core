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

#include <biogears/cdm/properties/SEScalarPressureTimePerVolumeArea.h>

namespace py = pybind11;

using biogears::PressureTimePerVolumeAreaUnit;
using biogears::SEScalar;
using biogears::SEScalarPressureTimePerVolumeArea;

void define_pybSEScalarPressureTimePerVolumeArea(pybind11::module_& m)
{
  py::class_<biogears::PressureTimePerVolumeAreaUnit>(m, "PressureTimePerVolumeAreaUnit")
    .def_readonly_static("mmHg_min_Per_mL_m2", &biogears::PressureTimePerVolumeAreaUnit::mmHg_min_Per_mL_m2)
    .def_readonly_static("mmHg_s_Per_mL_m2", &biogears::PressureTimePerVolumeAreaUnit::mmHg_s_Per_mL_m2)
    .def_readonly_static("dyn_s_Per_cm5_m2", &biogears::PressureTimePerVolumeAreaUnit::dyn_s_Per_cm5_m2);

  pybind11::class_<biogears::SEScalarPressureTimePerVolumeArea>(m, "SEScalarPressureTimePerVolumeArea")
    .def(py::init<>())
      
    .def("Unload", py::overload_cast<>(&SEScalarPressureTimePerVolumeArea::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarPressureTimePerVolumeArea::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarPressureTimePerVolumeArea::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarPressureTimePerVolumeArea::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarPressureTimePerVolumeArea::GetValue, py::const_))
    .def("GetValue", py::overload_cast<PressureTimePerVolumeAreaUnit const&>(&SEScalarPressureTimePerVolumeArea::GetValue, py::const_))
    .def("SetValue", &SEScalarPressureTimePerVolumeArea::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarPressureTimePerVolumeArea::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarPressureTimePerVolumeArea::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const PressureTimePerVolumeAreaUnit&>(&SEScalarPressureTimePerVolumeArea::IncrementValue))
    .def("Increment", &SEScalarPressureTimePerVolumeArea::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarPressureTimePerVolumeArea::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarPressureTimePerVolumeArea::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const PressureTimePerVolumeAreaUnit&>(&SEScalarPressureTimePerVolumeArea::DecrementValue))
    .def("Decrement", &SEScalarPressureTimePerVolumeArea::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarPressureTimePerVolumeArea::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarPressureTimePerVolumeArea::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarPressureTimePerVolumeArea::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarPressureTimePerVolumeArea::Divide))

    //.def("Equals", &SEScalarPressureTimePerVolumeArea::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarPressureTimePerVolumeArea::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarPressureTimePerVolumeArea::GetCompoundUnit, py::const_))
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
