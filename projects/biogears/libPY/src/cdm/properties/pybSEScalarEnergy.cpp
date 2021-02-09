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

#include <biogears/cdm/properties/SEScalarEnergy.h>

namespace py = pybind11;

using biogears::EnergyUnit;
using biogears::SEScalar;
using biogears::SEScalarEnergy;

void define_pybSEScalarEnergy(pybind11::module_ &m)
{
  py::class_<biogears::EnergyUnit>(m, "EnergyUnit")
    .def_readonly_static("J", &biogears::EnergyUnit::J)
    .def_readonly_static("mJ", &biogears::EnergyUnit::mJ)
    .def_readonly_static("kJ", &biogears::EnergyUnit::kJ)
    .def_readonly_static("kcal", &biogears::EnergyUnit::kcal);

  pybind11::class_<biogears::SEScalarEnergy>(m, "SEScalarEnergy")
    .def(py::init<>())
  
    .def("Unload", py::overload_cast<>(&SEScalarEnergy::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarEnergy::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarEnergy::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarEnergy::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarEnergy::GetValue, py::const_))
    .def("GetValue", py::overload_cast<EnergyUnit const&>(&SEScalarEnergy::GetValue, py::const_))
    .def("SetValue", &SEScalarEnergy::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarEnergy::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarEnergy::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const EnergyUnit&>(&SEScalarEnergy::IncrementValue))
    .def("Increment", &SEScalarEnergy::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarEnergy::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarEnergy::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const EnergyUnit&>(&SEScalarEnergy::DecrementValue))
    .def("Decrement", &SEScalarEnergy::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarEnergy::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarEnergy::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarEnergy::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarEnergy::Divide))

    //.def("Equals", &SEScalarEnergy::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarEnergy::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarEnergy::GetCompoundUnit, py::const_))
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
