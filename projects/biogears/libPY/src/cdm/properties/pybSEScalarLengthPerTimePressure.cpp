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

#include <biogears/cdm/properties/SEScalarLengthPerTimePressure.h>

namespace py = pybind11;

using biogears::LengthPerTimePressureUnit;
using biogears::SEScalar;
using biogears::SEScalarLengthPerTimePressure;

void define_pybSEScalarLengthPerTimePressure(pybind11::module_& m)
{
  py::class_<biogears::LengthPerTimePressureUnit>(m, "LengthPerTimePressureUnit")
    .def_readonly_static("m_Per_s_mmHg", &biogears::LengthPerTimePressureUnit::m_Per_s_mmHg)
    .def_readonly_static("cm_Per_s_mmHg", &biogears::LengthPerTimePressureUnit::cm_Per_s_mmHg)
    .def_readonly_static("m_Per_min_mmHg", &biogears::LengthPerTimePressureUnit::m_Per_min_mmHg)
    .def_readonly_static("cm_Per_min_mmHg", &biogears::LengthPerTimePressureUnit::cm_Per_min_mmHg);

  pybind11::class_<biogears::SEScalarLengthPerTimePressure>(m, "SEScalarLengthPerTimePressure")
    .def(py::init<>())
      
    .def("Unload", py::overload_cast<>(&SEScalarLengthPerTimePressure::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarLengthPerTimePressure::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarLengthPerTimePressure::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarLengthPerTimePressure::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarLengthPerTimePressure::GetValue, py::const_))
    .def("GetValue", py::overload_cast<LengthPerTimePressureUnit const&>(&SEScalarLengthPerTimePressure::GetValue, py::const_))
    .def("SetValue", &SEScalarLengthPerTimePressure::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarLengthPerTimePressure::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarLengthPerTimePressure::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const LengthPerTimePressureUnit&>(&SEScalarLengthPerTimePressure::IncrementValue))
    .def("Increment", &SEScalarLengthPerTimePressure::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarLengthPerTimePressure::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarLengthPerTimePressure::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const LengthPerTimePressureUnit&>(&SEScalarLengthPerTimePressure::DecrementValue))
    .def("Decrement", &SEScalarLengthPerTimePressure::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarLengthPerTimePressure::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarLengthPerTimePressure::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarLengthPerTimePressure::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarLengthPerTimePressure::Divide))

    //.def("Equals", &SEScalarLengthPerTimePressure::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarLengthPerTimePressure::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarLengthPerTimePressure::GetCompoundUnit, py::const_))
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
