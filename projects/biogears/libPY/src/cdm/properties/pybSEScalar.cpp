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

#include <string>

#include <biogears/cdm/properties/SEScalar.h>

namespace py = pybind11;
using biogears::SEScalar;

void define_pybSEScalar(pybind11::module_ &m)
{
  pybind11::class_<biogears::SEScalar>(m, "SEScalar")
    .def(py::init<>())
    .def("Unload",py::overload_cast<>(&biogears::SEScalar::Unload,py::const_))
    .def("ToString",py::overload_cast<std::ostream&>(&biogears::SEScalar::ToString,py::const_))
    .def("GetValue", &biogears::SEScalar::GetValue)
    .def("GetValue", &biogears::SEScalar::GetValue)
    .def("SetValue", &biogears::SEScalar::SetValue)
    .def("SetValue",&biogears::SEScalar::SetValue);
  ;

#ifdef VERSION_INFO
  m.attr("__version__") == VERSION_INFO
#else
  m.attr("__version__") = "dev";
#endif
}
