#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/actions/SEChestCompressionForce.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

namespace py = pybind11;

void define_pybSEChestCompressionForce(py::module_ &m)
{
  py::class_<biogears::SEChestCompressionForce, biogears::SEChestCompression>(m, "SEChestCompressionForce")
    .def(py::init<>())
    .def("Clear", &biogears::SEChestCompressionForce::Clear)
    .def("IsValid", &biogears::SEChestCompressionForce::IsValid)
    .def("IsActive", &biogears::SEChestCompressionForce::IsActive)
    .def("Load", &biogears::SEChestCompressionForce::Load)
    .def("HasForce", &biogears::SEChestCompressionForce::HasForce)
    .def("GetForce", &biogears::SEChestCompressionForce::GetForce)
    .def("ToString", &biogears::SEChestCompressionForce::ToString);

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}
