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

#include <biogears/cdm/properties/SEScalarPressurePerVolume.h>

namespace py = pybind11;

using biogears::PressurePerVolumeUnit;
using biogears::SEScalar;
using biogears::SEScalarPressurePerVolume;

void define_pybSEScalarPressurePerVolume(pybind11::module_ &m)
{
  py::class_<biogears::PressurePerVolumeUnit>(m, "PressurePerVolumeUnit")
    .def_readonly_static("mmHg_Per_mL", &biogears::PressurePerVolumeUnit::mmHg_Per_mL)
    .def_readonly_static("cmH2O_Per_mL", &biogears::PressurePerVolumeUnit::cmH2O_Per_mL);

  pybind11::class_<biogears::SEScalarPressurePerVolume>(m, "SEScalarPressurePerVolume")
    .def(py::init<>())
  
    .def("Unload", py::overload_cast<>(&SEScalarPressurePerVolume::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarPressurePerVolume::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarPressurePerVolume::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarPressurePerVolume::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarPressurePerVolume::GetValue, py::const_))
    .def("GetValue", py::overload_cast<PressurePerVolumeUnit const&>(&SEScalarPressurePerVolume::GetValue, py::const_))
    .def("SetValue", &SEScalarPressurePerVolume::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarPressurePerVolume::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarPressurePerVolume::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const PressurePerVolumeUnit&>(&SEScalarPressurePerVolume::IncrementValue))
    .def("Increment", &SEScalarPressurePerVolume::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarPressurePerVolume::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarPressurePerVolume::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const PressurePerVolumeUnit&>(&SEScalarPressurePerVolume::DecrementValue))
    .def("Decrement", &SEScalarPressurePerVolume::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarPressurePerVolume::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarPressurePerVolume::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarPressurePerVolume::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarPressurePerVolume::Divide))

    //.def("Equals", &SEScalarPressurePerVolume::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarPressurePerVolume::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarPressurePerVolume::GetCompoundUnit, py::const_))
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
