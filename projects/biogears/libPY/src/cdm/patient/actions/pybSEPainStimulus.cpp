#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/actions/SEPainStimulus.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

namespace py = pybind11;

void define_pybSEPainStimulus(py::module_ &m)
{

  py::class_<biogears::SEPainStimulus, biogears::SEPatientAction>(m, "SEPainStimulus")
    .def(py::init<>())
    .def("TypeTag", &biogears::SEPainStimulus::TypeTag)
    .def("classname", py::overload_cast<>(&biogears::SEPainStimulus::classname, py::const_))
    .def("Clear", py::overload_cast<>(&biogears::SEPainStimulus::Clear))
    .def("IsValid", py::overload_cast<>(&biogears::SEPainStimulus::IsValid, py::const_))
    .def("IsActive", py::overload_cast<>(&biogears::SEPainStimulus::IsActive, py::const_))
    .def("Load", &biogears::SEPainStimulus::Load)
    .def("Unload", py::overload_cast<>(&biogears::SEPainStimulus::Unload, py::const_))
    .def("HasSeverity", &biogears::SEPainStimulus::HasSeverity)
    .def("GetSeverity", &biogears::SEPainStimulus::GetSeverity)
    .def("HasHalfLife", &biogears::SEPainStimulus::HasHalfLife)
    .def("GetHalfLife", &biogears::SEPainStimulus::GetHalfLife)
    .def("HasLocation", &biogears::SEPainStimulus::HasLocation)
    .def("GetLocation_cStr", &biogears::SEPainStimulus::GetLocation_cStr)
    .def("GetLocation", &biogears::SEPainStimulus::GetLocation)
    .def("SetLocation", py::overload_cast<const char*>(&biogears::SEPainStimulus::SetLocation))
    .def("SetLocation", py::overload_cast<const std::string&>(&biogears::SEPainStimulus::SetLocation))

    .def("ToString", py::overload_cast<std::ostream&>(&biogears::SEPainStimulus::ToString, py::const_));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}
