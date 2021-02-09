#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/patient/actions/SEAcuteRespiratoryDistress.h>

namespace py = pybind11;

void define_pybSEAcuteRespiratoryDistress(py::module_ &m) {


    py::class_<biogears::SEAcuteRespiratoryDistress>(m, "SEAcuteRespiratoryDistress")
    .def(py::init<>())    
    .def("TypeTag",&biogears::SEAcuteRespiratoryDistress::TypeTag)
    .def("classname",py::overload_cast<>(&biogears::SEAcuteRespiratoryDistress::classname,py::const_))
    .def("Clear",py::overload_cast<>(&biogears::SEAcuteRespiratoryDistress::Clear))
    .def("IsValid",py::overload_cast<>(&biogears::SEAcuteRespiratoryDistress::IsValid,py::const_))
    .def("IsActive",py::overload_cast<>(&biogears::SEAcuteRespiratoryDistress::IsActive,py::const_))
    .def("Load",&biogears::SEAcuteRespiratoryDistress::Load)
    .def("Unload",py::overload_cast<>(&biogears::SEAcuteRespiratoryDistress::Unload,py::const_))
    .def("HasSeverity",&biogears::SEAcuteRespiratoryDistress::HasSeverity)
    .def("GetSeverity",&biogears::SEAcuteRespiratoryDistress::GetSeverity)
    .def("ToString",py::overload_cast<std::ostream&>(&biogears::SEAcuteRespiratoryDistress::ToString,py::const_));


    
  
#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}
