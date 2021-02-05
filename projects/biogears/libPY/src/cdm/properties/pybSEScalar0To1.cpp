#include <pybind11/pybind11.h>

#include <biogears/cdm/properties/SEScalar.h>
#include <biogears/cdm/properties/SEScalar0To1.h>

namespace py = pybind11;

PYBIND11_MODULE(pybSEScalar0To1, m)
{

  py::class_<biogears::SEScalar0To1>(m, "SEScalar0To1")
    .def(py::init<>())
    .def("Unload", &biogears::SEScalar0To1::Unload)
    .def("GetValue", py::overload_cast<>(&biogears::SEScalar0To1::GetValue, py::const_))
    .def("GetValue", py::overload_cast<const biogears::NoUnit&>(&biogears::SEScalar0To1::GetValue, py::const_))
    .def("SetValue", py::overload_cast<double>(&biogears::SEScalar0To1::SetValue))
    .def("SetValue", py::overload_cast<double, const biogears::NoUnit&>(&biogears::SEScalar0To1::SetValue));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}