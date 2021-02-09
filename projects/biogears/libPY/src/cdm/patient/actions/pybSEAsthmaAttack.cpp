#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/patient/actions/SEAsthmaAttack.h>

namespace py = pybind11;

void define_pybSEAsthmaAttack(py::module_ &m) {


    py::class_<biogears::SEAsthmaAttack>(m, "SEAsthmaAttack")
    .def(py::init<>())    
    .def("TypeTag",&biogears::SEAsthmaAttack::TypeTag)
    .def("classname",py::overload_cast<>(&biogears::SEAsthmaAttack::classname,py::const_))
    .def("Clear",py::overload_cast<>(&biogears::SEAsthmaAttack::Clear))
    .def("IsValid",py::overload_cast<>(&biogears::SEAsthmaAttack::IsValid,py::const_))
    .def("IsActive",py::overload_cast<>(&biogears::SEAsthmaAttack::IsActive,py::const_))
    .def("Load",&biogears::SEAsthmaAttack::Load)
    .def("Unload",py::overload_cast<>(&biogears::SEAsthmaAttack::Unload,py::const_))
    .def("HasSeverity",&biogears::SEAsthmaAttack::HasSeverity)
    .def("GetSeverity",&biogears::SEAsthmaAttack::GetSeverity)
    .def("ToString",py::overload_cast<std::ostream&>(&biogears::SEAsthmaAttack::ToString,py::const_));


    
  
#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}
