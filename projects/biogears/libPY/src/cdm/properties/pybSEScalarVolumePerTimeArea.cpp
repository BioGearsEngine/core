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

#include <biogears/cdm/properties/SEScalarVolumePerTimeArea.h>

namespace py = pybind11;

using biogears::VolumePerTimeAreaUnit;
using biogears::SEScalar;
using biogears::SEScalarVolumePerTimeArea;

void define_pybSEScalarVolumePerTimeArea(pybind11::module_ &m)
{
  py::class_<biogears::VolumePerTimeAreaUnit>(m, "VolumePerTimeAreaUnit")
    .def_readonly_static("mL_Per_min_m2", &biogears::VolumePerTimeAreaUnit::mL_Per_min_m2)
    .def_readonly_static("mL_Per_s_m2", &biogears::VolumePerTimeAreaUnit::mL_Per_s_m2)
    .def_readonly_static("L_Per_min_m2", &biogears::VolumePerTimeAreaUnit::L_Per_min_m2);

  pybind11::class_<biogears::SEScalarVolumePerTimeArea>(m, "SEScalarVolumePerTimeArea")
    .def(py::init<>())
  
    .def("Unload", py::overload_cast<>(&SEScalarVolumePerTimeArea::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarVolumePerTimeArea::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarVolumePerTimeArea::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarVolumePerTimeArea::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarVolumePerTimeArea::GetValue, py::const_))
    .def("GetValue", py::overload_cast<VolumePerTimeAreaUnit const&>(&SEScalarVolumePerTimeArea::GetValue, py::const_))
    .def("SetValue", &SEScalarVolumePerTimeArea::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarVolumePerTimeArea::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarVolumePerTimeArea::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const VolumePerTimeAreaUnit&>(&SEScalarVolumePerTimeArea::IncrementValue))
    .def("Increment", &SEScalarVolumePerTimeArea::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarVolumePerTimeArea::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarVolumePerTimeArea::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const VolumePerTimeAreaUnit&>(&SEScalarVolumePerTimeArea::DecrementValue))
    .def("Decrement", &SEScalarVolumePerTimeArea::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarVolumePerTimeArea::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarVolumePerTimeArea::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarVolumePerTimeArea::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarVolumePerTimeArea::Divide))

    //.def("Equals", &SEScalarVolumePerTimeArea::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarVolumePerTimeArea::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarVolumePerTimeArea::GetCompoundUnit, py::const_))
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
