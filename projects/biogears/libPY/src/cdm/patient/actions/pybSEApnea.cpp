#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/patient/actions/SEApnea.h>

namespace py = pybind11;

void define_pybSEApnea(py::module_ &m) {


    py::class_<biogears::SEApnea>(m, "SEApnea")
    .def(py::init<>())    
    .def("TypeTag",&biogears::SEApnea::TypeTag)
    .def("classname",py::overload_cast<>(&biogears::SEApnea::classname,py::const_))
    .def("Clear",py::overload_cast<>(&biogears::SEApnea::Clear))
    .def("IsValid",py::overload_cast<>(&biogears::SEApnea::IsValid,py::const_))
    .def("IsActive",py::overload_cast<>(&biogears::SEApnea::IsActive,py::const_))
    .def("Load",&biogears::SEApnea::Load)
    .def("Unload",py::overload_cast<>(&biogears::SEApnea::Unload,py::const_))
    .def("HasSeverity",&biogears::SEApnea::HasSeverity)
    .def("GetSeverity",&biogears::SEApnea::GetSeverity)
    .def("ToString",py::overload_cast<std::ostream&>(&biogears::SEApnea::ToString,py::const_));


    
  
#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}
