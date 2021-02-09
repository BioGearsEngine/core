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

#include <biogears/cdm/properties/SEScalarVolume.h>

namespace py = pybind11;

using biogears::VolumeUnit;
using biogears::SEScalar;
using biogears::SEScalarVolume;

void define_pybSEScalarVolume(pybind11::module_& m)
{
  py::class_<biogears::VolumeUnit>(m, "VolumeUnit")
    .def_readonly_static("L", &biogears::VolumeUnit::L)
    .def_readonly_static("dL", &biogears::VolumeUnit::dL)
    .def_readonly_static("mL", &biogears::VolumeUnit::mL)
    .def_readonly_static("uL", &biogears::VolumeUnit::uL)
    .def_readonly_static("m3", &biogears::VolumeUnit::m3);

  pybind11::class_<biogears::SEScalarVolume>(m, "SEScalarVolume")
    .def(py::init<>())
      
    .def("Unload", py::overload_cast<>(&SEScalarVolume::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarVolume::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarVolume::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarVolume::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarVolume::GetValue, py::const_))
    .def("GetValue", py::overload_cast<VolumeUnit const&>(&SEScalarVolume::GetValue, py::const_))
    .def("SetValue", &SEScalarVolume::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarVolume::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarVolume::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const VolumeUnit&>(&SEScalarVolume::IncrementValue))
    .def("Increment", &SEScalarVolume::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarVolume::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarVolume::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const VolumeUnit&>(&SEScalarVolume::DecrementValue))
    .def("Decrement", &SEScalarVolume::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarVolume::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarVolume::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarVolume::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarVolume::Divide))

    //.def("Equals", &SEScalarVolume::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarVolume::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarVolume::GetCompoundUnit, py::const_))
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
