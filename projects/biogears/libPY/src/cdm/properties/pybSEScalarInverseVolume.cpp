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

#include <biogears/cdm/properties/SEScalarInverseVolume.h>

namespace py = pybind11;

using biogears::InverseVolumeUnit;
using biogears::SEScalar;
using biogears::SEScalarInverseVolume;

void define_pybSEScalarInverseVolume(pybind11::module_& m)
{
  py::class_<biogears::InverseVolumeUnit>(m, "InverseVolumeUnit")
    .def_readonly_static("Inverse_L", &biogears::InverseVolumeUnit::Inverse_L)
    .def_readonly_static("Inverse_mL", &biogears::InverseVolumeUnit::Inverse_mL);

  pybind11::class_<biogears::SEScalarInverseVolume>(m, "SEScalarInverseVolume")
    .def(py::init<>())
      
    .def("Unload", py::overload_cast<>(&SEScalarInverseVolume::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarInverseVolume::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarInverseVolume::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarInverseVolume::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarInverseVolume::GetValue, py::const_))
    .def("GetValue", py::overload_cast<InverseVolumeUnit const&>(&SEScalarInverseVolume::GetValue, py::const_))
    .def("SetValue", &SEScalarInverseVolume::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarInverseVolume::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarInverseVolume::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const InverseVolumeUnit&>(&SEScalarInverseVolume::IncrementValue))
    .def("Increment", &SEScalarInverseVolume::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarInverseVolume::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarInverseVolume::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const InverseVolumeUnit&>(&SEScalarInverseVolume::DecrementValue))
    .def("Decrement", &SEScalarInverseVolume::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarInverseVolume::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarInverseVolume::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarInverseVolume::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarInverseVolume::Divide))

    //.def("Equals", &SEScalarInverseVolume::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarInverseVolume::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarInverseVolume::GetCompoundUnit, py::const_))
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
