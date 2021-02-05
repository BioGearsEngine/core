#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/actions/SEChestCompressionForceScale.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

namespace py = pybind11;

PYBIND11_MODULE(pybSEChestCompressionForceScale, m)
{
  py::class_<biogears::SEChestCompressionForceScale, biogears::SEChestCompression>(m, "SEChestCompressionForceScale")
    .def(py::init<>())
    .def("Clear", &biogears::SEChestCompressionForceScale::Clear)
    .def("IsValid", &biogears::SEChestCompressionForceScale::IsValid)
    .def("IsActive", &biogears::SEChestCompressionForceScale::IsActive)
    .def("Load", &biogears::SEChestCompressionForceScale::Load)
    .def("Unload", py::overload_cast<>(&biogears::SEChestCompressionForceScale::Unload, py::const_))
    .def("HasForceScale", &biogears::SEChestCompressionForceScale::HasForceScale)
    .def("GetForceScale", &biogears::SEChestCompressionForceScale::GetForceScale)
    .def("HasForcePeriod", &biogears::SEChestCompressionForceScale::HasForcePeriod)
    .def("GetForcePeriod", &biogears::SEChestCompressionForceScale::GetForcePeriod)
    .def("ToString", &biogears::SEChestCompressionForceScale::ToString);

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}