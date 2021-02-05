#include "../include/biogears/cdm/system/physiology/SECardiovascularSystem.h"
#include "../include/biogears/cdm/system/SESystem.h"
#include "../include/biogears/cdm/properties/SEScalarPressure.h"


#include <pybind11/pybind11.h>


namespace py = pybind11;




PYBIND11_MODULE(pybcardio, m) {

    py::class_<biogears::SESystem >(m, "SESystem");

    py::class_<biogears::SECardiovascularSystem,biogears::SESystem >(m, "SECardiovascularSystem")

    .def(py::init<biogears::Logger *>())

    .def("Clear",&biogears::SECardiovascularSystem::Clear)
    // .def("HasArterialPressure",&biogears::SECardiovascularSystem::HasArterialPressure)
    // .def("GetArterialPressure",&biogears::SECardiovascularSystem::GetArterialPressure);
    // .def("GetArterialPressure",static_cast<biogears::SEScalarPressure& (biogears::SECardiovascularSystem::*)()>(&biogears::SECardiovascularSystem::GetArterialPressure));
     .def("GetArterialPressure",py::overload_cast<>(&biogears::SECardiovascularSystem::GetArterialPressure));

    // .def("GetArterialPressure",py::overload_cast<const biogears::PressureUnit&>(&biogears::SECardiovascularSystem::GetArterialPressure, py::const_));

#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}