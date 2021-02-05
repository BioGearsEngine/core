#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/substance/SESubstanceTissuePharmacokinetics.h>

namespace py = pybind11;

PYBIND11_MODULE(pybSESubstanceTissuePharmacokinetics, m)
{

  py::class_<biogears::SESubstanceTissuePharmacokinetics, biogears::Loggable>(m, "SESubstanceTissuePharmacokinetics")
    .def(py::init<const std::string&, biogears::Logger*>())

    .def("Clear", &biogears::SESubstanceTissuePharmacokinetics::Clear)
    .def("Load", &biogears::SESubstanceTissuePharmacokinetics::Load)
    .def("Unload", py::overload_cast<>(&biogears::SESubstanceTissuePharmacokinetics::Unload, py::const_))

    .def("GetScalar", py::overload_cast<const char*>(&biogears::SESubstanceTissuePharmacokinetics::GetScalar))
    .def("GetScalar", py::overload_cast<const std::string&>(&biogears::SESubstanceTissuePharmacokinetics::GetScalar))

    .def("GetName", &biogears::SESubstanceTissuePharmacokinetics::GetName)
    .def("GetName_cStr", &biogears::SESubstanceTissuePharmacokinetics::GetName_cStr)
    .def("HasPartitionCoefficient", &biogears::SESubstanceTissuePharmacokinetics::HasPartitionCoefficient)
    .def("GetPartitionCoefficient", py::overload_cast<>(&biogears::SESubstanceTissuePharmacokinetics::GetPartitionCoefficient))
    .def("GetPartitionCoefficient", py::overload_cast<>(&biogears::SESubstanceTissuePharmacokinetics::GetPartitionCoefficient, py::const_));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}