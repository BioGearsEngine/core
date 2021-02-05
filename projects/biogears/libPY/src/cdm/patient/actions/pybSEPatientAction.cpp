#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/actions/SEPatientAction.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

namespace py = pybind11;

PYBIND11_MODULE(pybSEPatientAction, m) {

  py::module_::import("pybSEAction");

    py::class_<biogears::SEPatientAction,biogears::SEAction>(m, "SEPatientAction");
    // .def(py::init<>())    
    // .def("Clear",&biogears::SEPatientAction::Clear)
    // .def("IsValid",&biogears::SEPatientAction::IsValid)
    // .def("Load",&biogears::SEPatientAction::Load);
    // // .def("Unload",py::overload_cast<>(&biogears::SEPatientAction::Unload,py::const_))
    // // .def("ToString",py::overload_cast<std::ostream&>(&biogears::SEPatientAction::ToString,py::const_));


    
  
#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}