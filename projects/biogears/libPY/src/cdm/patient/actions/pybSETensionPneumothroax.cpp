#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/actions/SETensionPneumothorax.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

namespace py = pybind11;

void define_pybSETensionPneumothroax(py::module_ &m)
{

  py::class_<biogears::SETensionPneumothorax, biogears::SEPatientAction>(m, "SETensionPneumothorax")
    .def(py::init<>())
    .def("TypeTag", &biogears::SETensionPneumothorax::TypeTag)
    .def("classname", py::overload_cast<>(&biogears::SETensionPneumothorax::classname, py::const_))
    .def("Clear", py::overload_cast<>(&biogears::SETensionPneumothorax::Clear))
    .def("IsValid", py::overload_cast<>(&biogears::SETensionPneumothorax::IsValid, py::const_))
    .def("IsActive", py::overload_cast<>(&biogears::SETensionPneumothorax::IsActive, py::const_))
    .def("Load", &biogears::SETensionPneumothorax::Load)
    .def("Unload", py::overload_cast<>(&biogears::SETensionPneumothorax::Unload, py::const_))
    .def("GetType", &biogears::SETensionPneumothorax::GetType)
    .def("SetType", &biogears::SETensionPneumothorax::SetType)
    .def("HasType", &biogears::SETensionPneumothorax::HasType)
    .def("InvalidateType", &biogears::SETensionPneumothorax::InvalidateType)
    .def("HasSeverity", &biogears::SETensionPneumothorax::HasSeverity)
    .def("GetSeverity", &biogears::SETensionPneumothorax::GetSeverity)

    .def("ToString", py::overload_cast<std::ostream&>(&biogears::SETensionPneumothorax::ToString, py::const_));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}
