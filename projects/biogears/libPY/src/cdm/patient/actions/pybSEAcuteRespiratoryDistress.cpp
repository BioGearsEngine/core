#include <pybind11/pybind11.h>
#include <pybind11/functional.h>

#include <string>

#include <biogears/cdm/patient/actions/SEAcuteRespiratoryDistress.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

namespace py = pybind11;
using biogears::SEAction;
using biogears::SEAcuteRespiratoryDistress;
using biogears::SEPatientAction;

void define_pybSEAcuteRespiratoryDistress(py::module_& m)
{

  py::class_<SEAcuteRespiratoryDistress>(m, "SEAcuteRespiratoryDistress")
    .def(py::init<>())
    .def_static("TypeTag", &SEAcuteRespiratoryDistress::TypeTag)
    .def("classname", py::overload_cast<>(&SEAcuteRespiratoryDistress::classname, py::const_))
    .def("Clear", &SEAcuteRespiratoryDistress::Clear)
    .def("IsValid", py::overload_cast<>(&SEAcuteRespiratoryDistress::IsValid, py::const_))
    .def("IsActive", py::overload_cast<>(&SEAcuteRespiratoryDistress::IsActive, py::const_))
    .def("Load", &SEAcuteRespiratoryDistress::Load)
    .def("Unload", py::overload_cast<>(&SEAcuteRespiratoryDistress::Unload, py::const_))
    .def("HasSeverity", &SEAcuteRespiratoryDistress::HasSeverity)
    .def("GetSeverity", &SEAcuteRespiratoryDistress::GetSeverity, py::return_value_policy::reference)
    .def("ToString", py::overload_cast<>(&SEAction::ToString, py::const_))
    .def("__repr__", py::overload_cast<>(&SEAction::ToString, py::const_))
    .def_property("Severity", &SEAcuteRespiratoryDistress::GetSeverity, nullptr);
}
