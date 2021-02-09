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

#include <biogears/cdm/properties/SEScalarVolumePerTimePressureArea.h>

namespace py = pybind11;

using biogears::VolumePerTimePressureAreaUnit;
using biogears::SEScalar;
using biogears::SEScalarVolumePerTimePressureArea;

void define_pybSEScalarVolumePerTimePressureArea(pybind11::module_ &m)
{
  py::class_<biogears::VolumePerTimePressureAreaUnit>(m, "VolumePerTimePressureAreaUnit")
    .def_readonly_static("mL_Per_min_mmHg_m2", &biogears::VolumePerTimePressureAreaUnit::mL_Per_min_mmHg_m2)
    .def_readonly_static("mL_Per_s_mmHg_m2", &biogears::VolumePerTimePressureAreaUnit::mL_Per_s_mmHg_m2);
  
  pybind11::class_<biogears::SEScalarVolumePerTimePressureArea>(m, "SEScalarVolumePerTimePressureArea")
    .def(py::init<>())
  
    .def("Unload", py::overload_cast<>(&SEScalarVolumePerTimePressureArea::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarVolumePerTimePressureArea::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarVolumePerTimePressureArea::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarVolumePerTimePressureArea::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarVolumePerTimePressureArea::GetValue, py::const_))
    .def("GetValue", py::overload_cast<VolumePerTimePressureAreaUnit const&>(&SEScalarVolumePerTimePressureArea::GetValue, py::const_))
    .def("SetValue", &SEScalarVolumePerTimePressureArea::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarVolumePerTimePressureArea::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarVolumePerTimePressureArea::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const VolumePerTimePressureAreaUnit&>(&SEScalarVolumePerTimePressureArea::IncrementValue))
    .def("Increment", &SEScalarVolumePerTimePressureArea::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarVolumePerTimePressureArea::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarVolumePerTimePressureArea::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const VolumePerTimePressureAreaUnit&>(&SEScalarVolumePerTimePressureArea::DecrementValue))
    .def("Decrement", &SEScalarVolumePerTimePressureArea::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarVolumePerTimePressureArea::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarVolumePerTimePressureArea::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarVolumePerTimePressureArea::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarVolumePerTimePressureArea::Divide))

    //.def("Equals", &SEScalarVolumePerTimePressureArea::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarVolumePerTimePressureArea::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarVolumePerTimePressureArea::GetCompoundUnit, py::const_))
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
