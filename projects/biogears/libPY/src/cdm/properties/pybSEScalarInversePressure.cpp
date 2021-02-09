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

#include <biogears/cdm/properties/SEScalarInversePressure.h>

namespace py = pybind11;

using biogears::InversePressureUnit;
using biogears::SEScalar;
using biogears::SEScalarInversePressure;

void define_pybSEScalarInversePressure(pybind11::module_& m)
{
  py::class_<biogears::InversePressureUnit>(m, "InversePressureUnit")
    .def_readonly_static("Inverse_Pa", &biogears::InversePressureUnit::Inverse_Pa)
    .def_readonly_static("Inverse_mmHg", &biogears::InversePressureUnit::Inverse_mmHg)
    .def_readonly_static("Inverse_cmH2O", &biogears::InversePressureUnit::Inverse_cmH2O)
    .def_readonly_static("Inverse_atm", &biogears::InversePressureUnit::Inverse_atm);

  pybind11::class_<biogears::SEScalarInversePressure>(m, "SEScalarInversePressure")
    .def(py::init<>())
      
    .def("Unload", py::overload_cast<>(&SEScalarInversePressure::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarInversePressure::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarInversePressure::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarInversePressure::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarInversePressure::GetValue, py::const_))
    .def("GetValue", py::overload_cast<InversePressureUnit const&>(&SEScalarInversePressure::GetValue, py::const_))
    .def("SetValue", &SEScalarInversePressure::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarInversePressure::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarInversePressure::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const InversePressureUnit&>(&SEScalarInversePressure::IncrementValue))
    .def("Increment", &SEScalarInversePressure::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarInversePressure::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarInversePressure::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const InversePressureUnit&>(&SEScalarInversePressure::DecrementValue))
    .def("Decrement", &SEScalarInversePressure::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarInversePressure::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarInversePressure::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarInversePressure::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarInversePressure::Divide))

    //.def("Equals", &SEScalarInversePressure::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarInversePressure::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarInversePressure::GetCompoundUnit, py::const_))
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
