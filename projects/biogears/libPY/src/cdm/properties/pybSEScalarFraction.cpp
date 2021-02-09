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

#include <biogears/cdm/properties/SEScalarFraction.h>

namespace py = pybind11;

using biogears::SEScalar;
using biogears::SEScalarFraction;


void define_pybSEScalarFraction(pybind11::module_& m)
{
  pybind11::class_<biogears::SEScalarFraction>(m, "SEScalarFraction")
    .def(py::init<>())
      
    .def("Unload", py::overload_cast<>(&SEScalarFraction::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarFraction::ToString, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalar::GetValue, py::const_))
    .def("GetValue", py::overload_cast<const biogears::NoUnit&>(&SEScalarFraction::GetValue, py::const_))
    .def("SetValue", py::overload_cast<double>(&SEScalar::SetValue))
    .def("SetValue", py::overload_cast<double, const biogears::NoUnit&>(&SEScalarFraction::SetValue))

    .def("Increment",&SEScalarFraction::Increment)
    .def("IncrementValue", &SEScalarFraction::IncrementValue)
    .def("Decrement", &SEScalarFraction::Decrement)
    .def("DecrementValue", &SEScalarFraction::DecrementValue)    
    .def("Multiple", &SEScalarFraction::MultiplyValue)
    .def("MultiplyValue", &SEScalarFraction::Multiply)
    .def("DivideValue", &SEScalarFraction::DivideValue)
    .def("Divide", &SEScalarFraction::Divide)

    //.def("Equals", &SEScalarFraction::Equals, py::const_)

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
    .def(py::self / py::self)
    ;


#ifdef VERSION_INFO
  m.attr("__version__") == VERSION_INFO
#else
  m.attr("__version__") = "dev";
#endif
}
