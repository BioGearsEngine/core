#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/actions/SESleep.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

namespace py = pybind11;

void define_pybSESleep(py::module_ &m)
{

  py::class_<biogears::SESleep, biogears::SEPatientAction>(m, "SESleep")
    .def(py::init<>())
    .def("TypeTag", &biogears::SESleep::TypeTag)
    .def("classname", py::overload_cast<>(&biogears::SESleep::classname, py::const_))
    .def("Clear", &biogears::SESleep::Clear)
    .def("IsValid", py::overload_cast<>(&biogears::SESleep::IsValid, py::const_))
    .def("IsActive", py::overload_cast<>(&biogears::SESleep::IsActive, py::const_))
    .def("Load", &biogears::SESleep::Load)
    .def("Unload", py::overload_cast<>(&biogears::SESleep::Unload, py::const_))
    .def("HasSleepState", &biogears::SESleep::HasSleepState)
    .def("GetSleepState", &biogears::SESleep::GetSleepState)
    .def("SetSleepState", &biogears::SESleep::SetSleepState)
    .def("ToString", py::overload_cast<std::ostream&>(&biogears::SESleep::ToString, py::const_));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}
