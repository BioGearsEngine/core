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

#include <biogears/cdm/properties/SEScalarOsmolality.h>

namespace py = pybind11;

using biogears::OsmolalityUnit;
using biogears::SEScalar;
using biogears::SEScalarOsmolality;

void define_pybSEScalarOsmolality(pybind11::module_& m)
{
  py::class_<biogears::OsmolalityUnit>(m, "OsmolalityUnit")
    .def_readonly_static("Osm_Per_kg", &biogears::OsmolalityUnit::Osm_Per_kg)
    .def_readonly_static("mOsm_Per_kg", &biogears::OsmolalityUnit::mOsm_Per_kg);

  pybind11::class_<biogears::SEScalarOsmolality>(m, "SEScalarOsmolality")
    .def(py::init<>())
      
    .def("Unload", py::overload_cast<>(&SEScalarOsmolality::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarOsmolality::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarOsmolality::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarOsmolality::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarOsmolality::GetValue, py::const_))
    .def("GetValue", py::overload_cast<OsmolalityUnit const&>(&SEScalarOsmolality::GetValue, py::const_))
    .def("SetValue", &SEScalarOsmolality::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarOsmolality::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarOsmolality::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const OsmolalityUnit&>(&SEScalarOsmolality::IncrementValue))
    .def("Increment", &SEScalarOsmolality::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarOsmolality::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarOsmolality::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const OsmolalityUnit&>(&SEScalarOsmolality::DecrementValue))
    .def("Decrement", &SEScalarOsmolality::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarOsmolality::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarOsmolality::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarOsmolality::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarOsmolality::Divide))

    //.def("Equals", &SEScalarOsmolality::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarOsmolality::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarOsmolality::GetCompoundUnit, py::const_))
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
