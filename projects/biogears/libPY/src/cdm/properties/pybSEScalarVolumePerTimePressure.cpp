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

#include <biogears/cdm/properties/SEScalarVolumePerTimePressure.h>

namespace py = pybind11;

using biogears::VolumePerTimePressureUnit;
using biogears::SEScalar;
using biogears::SEScalarVolumePerTimePressure;

void define_pybSEScalarVolumePerTimePressure(pybind11::module_& m)
{
  py::class_<biogears::VolumePerTimePressureUnit>(m, "VolumePerTimePressureUnit")
    .def_readonly_static("L_Per_s_mmHg", &biogears::VolumePerTimePressureUnit::L_Per_s_mmHg)
    .def_readonly_static("mL_Per_s_mmHg", &biogears::VolumePerTimePressureUnit::mL_Per_s_mmHg)
    .def_readonly_static("L_Per_min_mmHg", &biogears::VolumePerTimePressureUnit::L_Per_min_mmHg)
    .def_readonly_static("mL_Per_min_mmHg", &biogears::VolumePerTimePressureUnit::mL_Per_min_mmHg);

  pybind11::class_<biogears::SEScalarVolumePerTimePressure>(m, "SEScalarVolumePerTimePressure")
    .def(py::init<>())
      
    .def("Unload", py::overload_cast<>(&SEScalarVolumePerTimePressure::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarVolumePerTimePressure::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarVolumePerTimePressure::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarVolumePerTimePressure::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarVolumePerTimePressure::GetValue, py::const_))
    .def("GetValue", py::overload_cast<VolumePerTimePressureUnit const&>(&SEScalarVolumePerTimePressure::GetValue, py::const_))
    .def("SetValue", &SEScalarVolumePerTimePressure::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarVolumePerTimePressure::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarVolumePerTimePressure::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const VolumePerTimePressureUnit&>(&SEScalarVolumePerTimePressure::IncrementValue))
    .def("Increment", &SEScalarVolumePerTimePressure::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarVolumePerTimePressure::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarVolumePerTimePressure::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const VolumePerTimePressureUnit&>(&SEScalarVolumePerTimePressure::DecrementValue))
    .def("Decrement", &SEScalarVolumePerTimePressure::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarVolumePerTimePressure::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarVolumePerTimePressure::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarVolumePerTimePressure::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarVolumePerTimePressure::Divide))

    //.def("Equals", &SEScalarVolumePerTimePressure::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarVolumePerTimePressure::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarVolumePerTimePressure::GetCompoundUnit, py::const_))
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
