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

#include <biogears/cdm/properties/SEScalar0To1.h>

namespace py = pybind11;
using biogears::SEScalar;

void define_pybSEScalar0To1(pybind11::module_ &m)
{

    py::class_<biogears::SEScalar0To1>(m, "SEScalar0To1")
    .def(py::init<>())
    .def("Unload", &biogears::SEScalar0To1::Unload)
    .def("GetValue", py::overload_cast<>(&biogears::SEScalar0To1::GetValue, py::const_))
    .def("GetValue", py::overload_cast<const biogears::NoUnit&>(&biogears::SEScalar0To1::GetValue, py::const_))
    .def("SetValue", py::overload_cast<double>(&biogears::SEScalar0To1::SetValue))
    .def("SetValue", py::overload_cast<double, const biogears::NoUnit&>(&biogears::SEScalar0To1::SetValue));

#ifdef VERSION_INFO
  m.attr("__version__") == VERSION_INFO
#else
  m.attr("__version__") = "dev";
#endif
}