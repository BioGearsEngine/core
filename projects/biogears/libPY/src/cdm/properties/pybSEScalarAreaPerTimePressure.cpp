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
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/properties/SEScalarAreaPerTimePressure.h>

namespace py = pybind11;

using biogears::AreaPerTimePressureUnit;
using biogears::SEScalar;
using biogears::SEScalarAreaPerTimePressure;

void define_pybSEScalarAreaPerTimePressure(pybind11::module_& m)
{

  py::class_<biogears::AreaPerTimePressureUnit>(m, "AreaPerTimePressureUnit")
    .def_readonly_static("m2_Per_s_mmHg", &biogears::AreaPerTimePressureUnit::m2_Per_s_mmHg)
    .def_readonly_static("cm2_Per_s_mmHg", &biogears::AreaPerTimePressureUnit::cm2_Per_s_mmHg)
    .def_readonly_static("m2_Per_min_mmHg", &biogears::AreaPerTimePressureUnit::m2_Per_min_mmHg)
    .def_readonly_static("cm2_Per_min_mmHg", &biogears::AreaPerTimePressureUnit::cm2_Per_min_mmHg);

  pybind11::class_<biogears::SEScalarAreaPerTimePressure>(m, "SEScalarAreaPerTimePressure")
    .def(py::init<>())

    .def("Unload", py::overload_cast<>(&SEScalarAreaPerTimePressure::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarAreaPerTimePressure::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarAreaPerTimePressure::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarAreaPerTimePressure::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarAreaPerTimePressure::GetValue, py::const_))
    .def("GetValue", py::overload_cast<AreaPerTimePressureUnit const&>(&SEScalarAreaPerTimePressure::GetValue, py::const_))
    .def("SetValue", &SEScalarAreaPerTimePressure::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarAreaPerTimePressure::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarAreaPerTimePressure::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const AreaPerTimePressureUnit&>(&SEScalarAreaPerTimePressure::IncrementValue))
    .def("Increment", &SEScalarAreaPerTimePressure::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarAreaPerTimePressure::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarAreaPerTimePressure::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const AreaPerTimePressureUnit&>(&SEScalarAreaPerTimePressure::DecrementValue))
    .def("Decrement", &SEScalarAreaPerTimePressure::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarAreaPerTimePressure::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarAreaPerTimePressure::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarAreaPerTimePressure::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarAreaPerTimePressure::Divide))

    //.def("Equals", &SEScalarAreaPerTimePressure::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarAreaPerTimePressure::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarAreaPerTimePressure::GetCompoundUnit, py::const_))
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
