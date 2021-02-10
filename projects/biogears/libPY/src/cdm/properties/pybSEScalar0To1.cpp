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

class SEScalar0To1Trampoline : public biogears::SEScalar0To1 {
public:
  /* Inherit the constructors */
  using SEScalar0To1::SEScalar0To1;

  void Invalidate() override
  {
    PYBIND11_OVERRIDE_PURE(
      void,
      biogears::SEProperty,
      Invalidate);
  }

  bool IsValid() const override
  {
    PYBIND11_OVERRIDE_PURE(
      bool,
      biogears::SEProperty,
      IsValid);
  }
};

using biogears::SEScalar0To1;
using biogears::NoUnit;

void define_pybSEScalar0To1(pybind11::module_& m)
{

  py::class_<SEScalar0To1, SEScalar0To1Trampoline>(m, "SEScalar0To1")
    .def(py::init<>())
    .def("Unload", &SEScalar0To1::Unload)
    .def("GetValue", py::overload_cast<>(&SEScalar0To1::GetValue, py::const_))
    .def("GetValue", py::overload_cast<const NoUnit&>(&biogears::SEScalar0To1::GetValue, py::const_))
    .def("SetValue", py::overload_cast<double>(&SEScalar0To1::SetValue))
    .def("SetValue", py::overload_cast<double, const NoUnit&>(&biogears::SEScalar0To1::SetValue))
  //.def("ToString", py::overload_cast<>(&SEAction::ToString, py::const_))
    .def("__repr__", [](const SEScalar0To1& obj){ 
    std::stringstream ss;
    obj.ToString(ss);
    return ss.str();
      
      })
    .def_property("Value", py::overload_cast<>(&SEScalar0To1::GetValue, py::const_), py::overload_cast<double>(&SEScalar0To1::SetValue))
    ;

#ifdef VERSION_INFO
  m.attr("__version__") == VERSION_INFO
#else
  m.attr("__version__") = "dev";
#endif
}