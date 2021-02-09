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

#include <biogears/cdm/properties/SEScalarVolumePerTime.h>

namespace py = pybind11;

using biogears::VolumePerTimeUnit;
using biogears::SEScalar;
using biogears::SEScalarVolumePerTime;

void define_pybSEScalarVolumePerTime(pybind11::module_& m)
{
  py::class_<biogears::VolumePerTimeUnit>(m, "VolumePerTimeUnit")
    .def_readonly_static("L_Per_s", &biogears::VolumePerTimeUnit::L_Per_s)
    .def_readonly_static("mL_Per_s", &biogears::VolumePerTimeUnit::mL_Per_s)
    .def_readonly_static("mL_Per_day", &biogears::VolumePerTimeUnit::mL_Per_day)
    .def_readonly_static("L_Per_day", &biogears::VolumePerTimeUnit::L_Per_day)
    .def_readonly_static("L_Per_min", &biogears::VolumePerTimeUnit::L_Per_min)
    .def_readonly_static("m3_Per_s", &biogears::VolumePerTimeUnit::m3_Per_s)
    .def_readonly_static("mL_Per_min", &biogears::VolumePerTimeUnit::mL_Per_min)
    .def_readonly_static("mL_Per_hr", &biogears::VolumePerTimeUnit::mL_Per_hr);

  pybind11::class_<biogears::SEScalarVolumePerTime>(m, "SEScalarVolumePerTime")
    .def(py::init<>())
      
    .def("Unload", py::overload_cast<>(&SEScalarVolumePerTime::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarVolumePerTime::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarVolumePerTime::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarVolumePerTime::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarVolumePerTime::GetValue, py::const_))
    .def("GetValue", py::overload_cast<VolumePerTimeUnit const&>(&SEScalarVolumePerTime::GetValue, py::const_))
    .def("SetValue", &SEScalarVolumePerTime::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarVolumePerTime::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarVolumePerTime::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const VolumePerTimeUnit&>(&SEScalarVolumePerTime::IncrementValue))
    .def("Increment", &SEScalarVolumePerTime::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarVolumePerTime::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarVolumePerTime::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const VolumePerTimeUnit&>(&SEScalarVolumePerTime::DecrementValue))
    .def("Decrement", &SEScalarVolumePerTime::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarVolumePerTime::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarVolumePerTime::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarVolumePerTime::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarVolumePerTime::Divide))

    //.def("Equals", &SEScalarVolumePerTime::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarVolumePerTime::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarVolumePerTime::GetCompoundUnit, py::const_))
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
