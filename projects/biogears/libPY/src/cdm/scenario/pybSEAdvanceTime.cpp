#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/cdm/scenario/SEAdvanceTime.h>

namespace py = pybind11;

void define_pybSEAdvanceTime(py::module_ &m)
{

  py::class_<biogears::SEAdvanceTime, biogears::SEAction>(m, "SEAdvanceTime")
    .def(py::init<>())
    .def("TypeTag",&biogears::SEAdvanceTime::TypeTag)
    .def("classname", py::overload_cast<>(&biogears::SEAdvanceTime::classname,py::const_))

    .def("Clear", py::overload_cast<>(&biogears::SEAdvanceTime::Clear))
    .def("IsValid", py::overload_cast<>(&biogears::SEAdvanceTime::IsValid,py::const_))
    .def("Load", &biogears::SEAdvanceTime::Load)
    .def("Unload", py::overload_cast<>(&biogears::SEAdvanceTime::Unload,py::const_))
    .def("ToString",py::overload_cast<std::ostream&>(&biogears::SEAdvanceTime::ToString,py::const_))
    .def("HasTime", &biogears::SEAdvanceTime::HasTime)
    .def("GetTime",py::overload_cast<>(&biogears::SEAdvanceTime::GetTime))
    .def("GetTime",py::overload_cast<const biogears::TimeUnit&>(&biogears::SEAdvanceTime::GetTime,py::const_));


#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}
