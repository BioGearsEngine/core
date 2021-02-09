#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/actions/SEUseInhaler.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

namespace py = pybind11;

void define_pybSEUseInhaler(py::module_ &m)
{

  py::class_<biogears::SEUseInhaler, biogears::SEConsciousRespirationCommand>(m, "SEUseInhaler")
    // .def(py::init<>())
    .def("Clear", &biogears::SEUseInhaler::Clear)
    .def("IsValid", &biogears::SEUseInhaler::IsValid)
    .def("IsActive", &biogears::SEUseInhaler::IsActive)
    .def("Load", &biogears::SEUseInhaler::Load)
    .def("Unload", py::overload_cast<>(&biogears::SEUseInhaler::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&biogears::SEUseInhaler::ToString, py::const_));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}
