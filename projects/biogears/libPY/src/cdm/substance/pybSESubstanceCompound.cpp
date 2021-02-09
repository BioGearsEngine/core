#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/cdm/substance/SESubstanceCompound.h>
#include <biogears/cdm/substance/SESubstanceManager.h>

namespace py = pybind11;

void define_pybSESubstanceCompound(py::module_ &m)
{

  py::class_<biogears::SESubstanceCompound, biogears::Loggable>(m, "SESubstanceCompound")
    .def(py::init<biogears::Logger*>())
    .def(py::init<const char*, biogears::Logger*>())
    .def(py::init<const std::string&, biogears::Logger*>())

    .def("make_unique", &biogears::SESubstanceCompound::make_unique)
    .def("Clear", &biogears::SESubstanceCompound::Clear)
    .def("Load", &biogears::SESubstanceCompound::Load)
    .def("Unload", py::overload_cast<>(&biogears::SESubstanceCompound::Unload, py::const_))

    .def("GetName", &biogears::SESubstanceCompound::GetName)
    .def("GetName_cStr", &biogears::SESubstanceCompound::GetName_cStr)

    .def("SetName", py::overload_cast<const char*>(&biogears::SESubstanceCompound::SetName))
    .def("SetName", py::overload_cast<const std::string&>(&biogears::SESubstanceCompound::SetName))
    .def("HasName", &biogears::SESubstanceCompound::HasName)
    .def("InvalidateName", &biogears::SESubstanceCompound::InvalidateName)

    .def("GetClassification", &biogears::SESubstanceCompound::GetClassification)
    .def("SetClassification", &biogears::SESubstanceCompound::SetClassification)
    .def("HasClassification", &biogears::SESubstanceCompound::HasClassification)
    .def("InvalidateClassification", &biogears::SESubstanceCompound::InvalidateClassification)
    .def("GetRhFactor", &biogears::SESubstanceCompound::GetRhFactor)
    .def("SetRhFactor", &biogears::SESubstanceCompound::SetRhFactor)
    .def("HasRhFactor", &biogears::SESubstanceCompound::HasRhFactor)

    .def("HasComponent", py::overload_cast<>(&biogears::SESubstanceCompound::HasComponent, py::const_))
    .def("HasComponent", py::overload_cast<const biogears::SESubstance&>(&biogears::SESubstanceCompound::HasComponent, py::const_))

    .def("GetComponents", py::overload_cast<>(&biogears::SESubstanceCompound::GetComponents))
    .def("GetComponents", py::overload_cast<>(&biogears::SESubstanceCompound::GetComponents, py::const_))

    .def("GetComponent", py::overload_cast<biogears::SESubstance&>(&biogears::SESubstanceCompound::GetComponent))
    .def("GetComponent", py::overload_cast<biogears::SESubstance&>(&biogears::SESubstanceCompound::GetComponent, py::const_))

    .def("RemoveComponent", &biogears::SESubstanceCompound::RemoveComponent);

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}
