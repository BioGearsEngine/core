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

#include <biogears/cdm/properties/SEScalarOsmolarity.h>

namespace py = pybind11;

using biogears::OsmolarityUnit;
using biogears::SEScalar;
using biogears::SEScalarOsmolarity;

void define_pybSEScalarOsmolarity(pybind11::module_ &m)
{
  py::class_<biogears::OsmolarityUnit>(m, "OsmolarityUnit")
    .def_readonly_static("Osm_Per_L", &biogears::OsmolarityUnit::Osm_Per_L)
    .def_readonly_static("mOsm_Per_L", &biogears::OsmolarityUnit::mOsm_Per_L);

  pybind11::class_<biogears::SEScalarOsmolarity>(m, "SEScalarOsmolarity")
    .def(py::init<>())
  
    .def("Unload", py::overload_cast<>(&SEScalarOsmolarity::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarOsmolarity::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarOsmolarity::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarOsmolarity::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarOsmolarity::GetValue, py::const_))
    .def("GetValue", py::overload_cast<OsmolarityUnit const&>(&SEScalarOsmolarity::GetValue, py::const_))
    .def("SetValue", &SEScalarOsmolarity::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarOsmolarity::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarOsmolarity::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const OsmolarityUnit&>(&SEScalarOsmolarity::IncrementValue))
    .def("Increment", &SEScalarOsmolarity::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarOsmolarity::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarOsmolarity::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const OsmolarityUnit&>(&SEScalarOsmolarity::DecrementValue))
    .def("Decrement", &SEScalarOsmolarity::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarOsmolarity::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarOsmolarity::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarOsmolarity::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarOsmolarity::Divide))

    //.def("Equals", &SEScalarOsmolarity::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarOsmolarity::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarOsmolarity::GetCompoundUnit, py::const_))
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
