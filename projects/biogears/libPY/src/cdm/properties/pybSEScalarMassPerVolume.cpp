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

#include <biogears/cdm/properties/SEScalarMassPerVolume.h>

namespace py = pybind11;

using biogears::MassPerVolumeUnit;
using biogears::SEScalar;
using biogears::SEScalarMassPerVolume;

void define_pybSEScalarMassPerVolume(pybind11::module_& m)
{
  py::class_<biogears::MassPerVolumeUnit>(m, "MassPerVolumeUnit")
    .def_readonly_static("g_Per_dL", &biogears::MassPerVolumeUnit::g_Per_dL)
    .def_readonly_static("g_Per_cm3", &biogears::MassPerVolumeUnit::g_Per_cm3)
    .def_readonly_static("g_Per_m3", &biogears::MassPerVolumeUnit::g_Per_m3)
    .def_readonly_static("ug_Per_mL", &biogears::MassPerVolumeUnit::ug_Per_mL)
    .def_readonly_static("mg_Per_m3", &biogears::MassPerVolumeUnit::mg_Per_m3)
    .def_readonly_static("kg_Per_m3", &biogears::MassPerVolumeUnit::kg_Per_m3)
    .def_readonly_static("ug_Per_L", &biogears::MassPerVolumeUnit::ug_Per_L)
    .def_readonly_static("g_Per_L", &biogears::MassPerVolumeUnit::g_Per_L)
    .def_readonly_static("g_Per_mL", &biogears::MassPerVolumeUnit::g_Per_mL)
    .def_readonly_static("mg_Per_mL", &biogears::MassPerVolumeUnit::mg_Per_mL)
    .def_readonly_static("mg_Per_L", &biogears::MassPerVolumeUnit::mg_Per_L)
    .def_readonly_static("mg_Per_dL", &biogears::MassPerVolumeUnit::mg_Per_dL)
    .def_readonly_static("kg_Per_mL", &biogears::MassPerVolumeUnit::kg_Per_mL)
    .def_readonly_static("kg_Per_L", &biogears::MassPerVolumeUnit::kg_Per_L);

  pybind11::class_<biogears::SEScalarMassPerVolume>(m, "SEScalarMassPerVolume")
    .def(py::init<>())
      
    .def("Unload", py::overload_cast<>(&SEScalarMassPerVolume::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarMassPerVolume::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarMassPerVolume::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarMassPerVolume::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarMassPerVolume::GetValue, py::const_))
    .def("GetValue", py::overload_cast<MassPerVolumeUnit const&>(&SEScalarMassPerVolume::GetValue, py::const_))
    .def("SetValue", &SEScalarMassPerVolume::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarMassPerVolume::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarMassPerVolume::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const MassPerVolumeUnit&>(&SEScalarMassPerVolume::IncrementValue))
    .def("Increment", &SEScalarMassPerVolume::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarMassPerVolume::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarMassPerVolume::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const MassPerVolumeUnit&>(&SEScalarMassPerVolume::DecrementValue))
    .def("Decrement", &SEScalarMassPerVolume::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarMassPerVolume::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarMassPerVolume::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarMassPerVolume::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarMassPerVolume::Divide))

    //.def("Equals", &SEScalarMassPerVolume::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarMassPerVolume::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarMassPerVolume::GetCompoundUnit, py::const_))
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
