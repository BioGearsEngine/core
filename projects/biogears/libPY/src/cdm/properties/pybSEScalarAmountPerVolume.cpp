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

#include <biogears/cdm/properties/SEScalarAmountPerVolume.h>

namespace py = pybind11;

using biogears::AmountPerVolumeUnit;
using biogears::SEScalar;
using biogears::SEScalarAmountPerVolume;

void define_pybSEScalarAmountPerVolume(pybind11::module_ &m)
{

  py::class_<biogears::AmountPerVolumeUnit>(m, "AmountPerVolumeUnit")
    .def_readonly_static("mol_Per_L", &biogears::AmountPerVolumeUnit::mol_Per_L)
    .def_readonly_static("mol_Per_mL", &biogears::AmountPerVolumeUnit::mol_Per_mL)
    .def_readonly_static("mmol_Per_L", &biogears::AmountPerVolumeUnit::mmol_Per_L)
    .def_readonly_static("mmol_Per_mL", &biogears::AmountPerVolumeUnit::mmol_Per_mL)
    .def_readonly_static("ct_Per_L", &biogears::AmountPerVolumeUnit::ct_Per_L)
    .def_readonly_static("ct_Per_uL", &biogears::AmountPerVolumeUnit::ct_Per_uL);


  pybind11::class_<biogears::SEScalarAmountPerVolume>(m, "SEScalarAmountPerVolume")
    .def(py::init<>())
    
    .def("Unload", py::overload_cast<>(&SEScalarAmountPerVolume::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&SEScalarAmountPerVolume::ToString, py::const_))
    .def("GetValue", py::overload_cast<std::string const&>(&SEScalarAmountPerVolume::GetValue, py::const_))
    .def("GetValue", py::overload_cast<char const*>(&SEScalarAmountPerVolume::GetValue, py::const_))
    .def("GetValue", py::overload_cast<>(&SEScalarAmountPerVolume::GetValue, py::const_))
    .def("GetValue", py::overload_cast<AmountPerVolumeUnit const&>(&SEScalarAmountPerVolume::GetValue, py::const_))
    .def("SetValue", &SEScalarAmountPerVolume::SetValue)
    .def("IncrementValue", py::overload_cast<double, const char*>(&SEScalarAmountPerVolume::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const std::string&>(&SEScalarAmountPerVolume::IncrementValue))
    .def("IncrementValue", py::overload_cast<double, const AmountPerVolumeUnit&>(&SEScalarAmountPerVolume::IncrementValue))
    .def("Increment", &SEScalarAmountPerVolume::Increment)
    .def("DecrementValue", py::overload_cast<double, const char*>(&SEScalarAmountPerVolume::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const std::string&>(&SEScalarAmountPerVolume::DecrementValue))
    .def("DecrementValue", py::overload_cast<double, const AmountPerVolumeUnit&>(&SEScalarAmountPerVolume::DecrementValue))
    .def("Decrement", &SEScalarAmountPerVolume::Decrement)
    .def("Multiple", py::overload_cast<double>(&SEScalarAmountPerVolume::MultiplyValue))
    .def("Multiply", py::overload_cast<const SEScalar&>(&SEScalarAmountPerVolume::Multiply))
    .def("DivideValue", py::overload_cast<double>(&SEScalarAmountPerVolume::DivideValue))
    .def("Divide", py::overload_cast<const SEScalar&>(&SEScalarAmountPerVolume::Divide))

    //.def("Equals", &SEScalarAmountPerVolume::Equals, py::const_)   //I can't seem to wrap this correctly
    .def("GetCompoundUnit", py::overload_cast<const char*>(&SEScalarAmountPerVolume::GetCompoundUnit, py::const_))
    .def("GetCompoundUnit", py::overload_cast<const std::string&>(&SEScalarAmountPerVolume::GetCompoundUnit, py::const_))
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
