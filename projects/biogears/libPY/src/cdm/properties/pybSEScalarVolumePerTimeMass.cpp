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

#include <biogears/cdm/properties/SEScalarVolumePerTimeMass.h>

namespace py = pybind11;

using biogears::VolumePerTimeMassUnit;
using biogears::SEScalar;
using biogears::SEScalarVolumePerTimeMass;

void define_pybSEScalarVolumePerTimeMass(pybind11::module_& m)
{
  py::class_<biogears::VolumePerTimeMassUnit>(m, "VolumePerTimeMassUnit")
    .def_readonly_static("L_Per_s_g", &biogears::VolumePerTimeMassUnit::L_Per_s_g)
    .def_readonly_static("mL_Per_s_g", &biogears::VolumePerTimeMassUnit::mL_Per_s_g)
    .def_readonly_static("mL_Per_min_kg", &biogears::VolumePerTimeMassUnit::mL_Per_min_kg)
    .def_readonly_static("mL_Per_s_kg", &biogears::VolumePerTimeMassUnit::mL_Per_s_kg)
    .def_readonly_static("uL_Per_min_kg", &biogears::VolumePerTimeMassUnit::uL_Per_min_kg);

  pybind11::class_<biogears::SEScalarVolumePerTimeMass>(m, "SEScalarVolumePerTimeMass")
    .def(py::init<>())
      
    .def("Unload", py::overload_cast<>(&SEScalarVolumePerTimeMass::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarVolumePerTimeMass::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarVolumePerTimeMass::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarVolumePerTimeMass::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarVolumePerTimeMass::GetValue, py::const_))
    .def("GetValue", py::overload_cast<VolumePerTimeMassUnit const&>(&SEScalarVolumePerTimeMass::GetValue, py::const_))
    .def("SetValue", &SEScalarVolumePerTimeMass::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarVolumePerTimeMass::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarVolumePerTimeMass::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const VolumePerTimeMassUnit&>(&SEScalarVolumePerTimeMass::IncrementValue))
    .def("Increment", &SEScalarVolumePerTimeMass::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarVolumePerTimeMass::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarVolumePerTimeMass::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const VolumePerTimeMassUnit&>(&SEScalarVolumePerTimeMass::DecrementValue))
    .def("Decrement", &SEScalarVolumePerTimeMass::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarVolumePerTimeMass::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarVolumePerTimeMass::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarVolumePerTimeMass::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarVolumePerTimeMass::Divide))

    //.def("Equals", &SEScalarVolumePerTimeMass::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarVolumePerTimeMass::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarVolumePerTimeMass::GetCompoundUnit, py::const_))
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
