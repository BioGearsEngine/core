#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/patient/actions/SEAcuteStress.h>

namespace py = pybind11;

void define_pybSEAcuteStress(py::module_ &m) {


    py::class_<biogears::SEAcuteStress>(m, "SEAcuteStress")
    .def(py::init<>())    
    .def("TypeTag",&biogears::SEAcuteStress::TypeTag)
    .def("classname",py::overload_cast<>(&biogears::SEAcuteStress::classname,py::const_))
    .def("Clear",py::overload_cast<>(&biogears::SEAcuteStress::Clear))
    .def("IsValid",py::overload_cast<>(&biogears::SEAcuteStress::IsValid,py::const_))
    .def("IsActive",py::overload_cast<>(&biogears::SEAcuteStress::IsActive,py::const_))
    .def("Load",&biogears::SEAcuteStress::Load)
    .def("Unload",py::overload_cast<>(&biogears::SEAcuteStress::Unload,py::const_))
    .def("HasSeverity",&biogears::SEAcuteStress::HasSeverity)
    .def("GetSeverity",&biogears::SEAcuteStress::GetSeverity)
    .def("ToString",py::overload_cast<std::ostream&>(&biogears::SEAcuteStress::ToString,py::const_));


    
  
#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}
