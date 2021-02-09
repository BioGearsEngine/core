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

#include <biogears/cdm/properties/SEScalarTimeMassPerVolume.h>

namespace py = pybind11;

using biogears::TimeMassPerVolumeUnit;
using biogears::SEScalar;
using biogears::SEScalarTimeMassPerVolume;

void define_pybSEScalarTimeMassPerVolume(pybind11::module_& m)
{
  py::class_<biogears::TimeMassPerVolumeUnit>(m, "TimeMassPerVolumeUnit")
    .def_readonly_static("s_g_Per_L", &biogears::TimeMassPerVolumeUnit::s_g_Per_L)
    .def_readonly_static("min_g_Per_L", &biogears::TimeMassPerVolumeUnit::min_g_Per_L)
    .def_readonly_static("hr_g_Per_L", &biogears::TimeMassPerVolumeUnit::hr_g_Per_L)
    .def_readonly_static("s_ug_Per_mL", &biogears::TimeMassPerVolumeUnit::s_ug_Per_mL)
    .def_readonly_static("min_ug_Per_mL", &biogears::TimeMassPerVolumeUnit::min_ug_Per_mL)
    .def_readonly_static("hr_ug_Per_mL", &biogears::TimeMassPerVolumeUnit::hr_ug_Per_mL);

  pybind11::class_<biogears::SEScalarTimeMassPerVolume>(m, "SEScalarTimeMassPerVolume")
    .def(py::init<>())
      
    .def("Unload", py::overload_cast<>(&SEScalarTimeMassPerVolume::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarTimeMassPerVolume::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarTimeMassPerVolume::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarTimeMassPerVolume::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarTimeMassPerVolume::GetValue, py::const_))
    .def("GetValue", py::overload_cast<TimeMassPerVolumeUnit const&>(&SEScalarTimeMassPerVolume::GetValue, py::const_))
    .def("SetValue", &SEScalarTimeMassPerVolume::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarTimeMassPerVolume::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarTimeMassPerVolume::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const TimeMassPerVolumeUnit&>(&SEScalarTimeMassPerVolume::IncrementValue))
    .def("Increment", &SEScalarTimeMassPerVolume::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarTimeMassPerVolume::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarTimeMassPerVolume::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const TimeMassPerVolumeUnit&>(&SEScalarTimeMassPerVolume::DecrementValue))
    .def("Decrement", &SEScalarTimeMassPerVolume::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarTimeMassPerVolume::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarTimeMassPerVolume::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarTimeMassPerVolume::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarTimeMassPerVolume::Divide))

    //.def("Equals", &SEScalarTimeMassPerVolume::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarTimeMassPerVolume::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarTimeMassPerVolume::GetCompoundUnit, py::const_))
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
