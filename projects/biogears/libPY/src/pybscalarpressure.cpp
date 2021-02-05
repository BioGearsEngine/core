#include "../include/biogears/cdm/properties/SEScalarPressure.h"
#include "../include/biogears/cdm/properties/SEScalarQuantity.h"
#include "../include/biogears/cdm/utils/unitconversion/CompoundUnit.h"

#include <pybind11/pybind11.h>


namespace py = pybind11;



PYBIND11_MODULE(pybscalarpressure, m) {

//    py::class_<biogears::PressureUnit,biogears::CCompoundUnit>(m, "PressureUnit");

//    py::class_<biogears::SEScalarPressure,biogears::SEScalarQuantity<biogears::PressureUnit>>(m, "SEScalarPressure")
//     .def(py::init<>());

   py::class_<biogears::SEScalarPressure>(m, "SEScalarPressure")
    .def(py::init<>());

}
