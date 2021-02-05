#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/actions/SENeedleDecompression.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

namespace py = pybind11;

PYBIND11_MODULE(pybSENeedleDecompression, m)
{

  py::class_<biogears::SENeedleDecompression, biogears::SEPatientAction>(m, "SENeedleDecompression")
    .def(py::init<>())
    .def("TypeTag", &biogears::SENeedleDecompression::TypeTag)
    .def("classname", py::overload_cast<>(&biogears::SENeedleDecompression::classname, py::const_))
    .def("Clear", py::overload_cast<>(&biogears::SENeedleDecompression::Clear))
    .def("IsValid", py::overload_cast<>(&biogears::SENeedleDecompression::IsValid, py::const_))
    .def("IsActive", py::overload_cast<>(&biogears::SENeedleDecompression::IsActive, py::const_))
    .def("SetActive", &biogears::SENeedleDecompression::SetActive)
    .def("Load", &biogears::SENeedleDecompression::Load)
    .def("Unload", py::overload_cast<>(&biogears::SENeedleDecompression::Unload, py::const_))
    .def("GetSide", &biogears::SENeedleDecompression::GetSide)
    .def("SetSide", &biogears::SENeedleDecompression::SetSide)
    .def("HasSide", &biogears::SENeedleDecompression::HasSide)
    .def("InvalidateSide", &biogears::SENeedleDecompression::InvalidateSide)

    .def("ToString", py::overload_cast<std::ostream&>(&biogears::SENeedleDecompression::ToString, py::const_));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}