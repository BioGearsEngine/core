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

#include <biogears/cdm/properties/SEScalarVolumePerPressure.h>

namespace py = pybind11;

using biogears::VolumePerPressureUnit;
using biogears::SEScalar;
using biogears::SEScalarVolumePerPressure;

void define_pybSEScalarVolumePerPressure(pybind11::module_ &m)
{
  py::class_<biogears::VolumePerPressureUnit>(m, "VolumePerPressureUnit")
    .def_readonly_static("L_Per_Pa", &biogears::VolumePerPressureUnit::L_Per_Pa)
    .def_readonly_static("L_Per_cmH2O", &biogears::VolumePerPressureUnit::L_Per_cmH2O);

  pybind11::class_<biogears::SEScalarVolumePerPressure>(m, "SEScalarVolumePerPressure")
    .def(py::init<>())
  
    .def("Unload", py::overload_cast<>(&SEScalarVolumePerPressure::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarVolumePerPressure::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarVolumePerPressure::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarVolumePerPressure::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarVolumePerPressure::GetValue, py::const_))
    .def("GetValue", py::overload_cast<VolumePerPressureUnit const&>(&SEScalarVolumePerPressure::GetValue, py::const_))
    .def("SetValue", &SEScalarVolumePerPressure::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarVolumePerPressure::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarVolumePerPressure::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const VolumePerPressureUnit&>(&SEScalarVolumePerPressure::IncrementValue))
    .def("Increment", &SEScalarVolumePerPressure::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarVolumePerPressure::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarVolumePerPressure::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const VolumePerPressureUnit&>(&SEScalarVolumePerPressure::DecrementValue))
    .def("Decrement", &SEScalarVolumePerPressure::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarVolumePerPressure::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarVolumePerPressure::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarVolumePerPressure::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarVolumePerPressure::Divide))

    //.def("Equals", &SEScalarVolumePerPressure::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarVolumePerPressure::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarVolumePerPressure::GetCompoundUnit, py::const_))
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
