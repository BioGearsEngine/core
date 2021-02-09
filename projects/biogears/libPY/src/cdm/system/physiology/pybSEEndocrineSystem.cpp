#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/system/physiology/SEEndocrineSystem.h>
#include <biogears/container/Tree.tci.h>


namespace py = pybind11;

void define_pybSEEndocrineSystem(py::module_ &m)
{
  py::module_::import("pybSESystem");

  py::class_<biogears::SEEndocrineSystem, biogears::SESystem>(m, "SEEndocrineSystem")
    .def(py::init<biogears::Logger*>())
    .def("HasInsulinSynthesisRate", &biogears::SEEndocrineSystem::HasInsulinSynthesisRate)
    .def("GetInsulinSynthesisRate", py::overload_cast<>(&biogears::SEEndocrineSystem::GetInsulinSynthesisRate))
    .def("GetInsulinSynthesisRate", py::overload_cast<const biogears::AmountPerTimeUnit&>(&biogears::SEEndocrineSystem::GetInsulinSynthesisRate, py::const_))
    .def("HasGlucagonSynthesisRate", &biogears::SEEndocrineSystem::HasGlucagonSynthesisRate)
    .def("GetGlucagonSynthesisRate", py::overload_cast<>(&biogears::SEEndocrineSystem::GetGlucagonSynthesisRate))
    .def("GetGlucagonSynthesisRate", py::overload_cast<const biogears::AmountPerTimeUnit&>(&biogears::SEEndocrineSystem::GetGlucagonSynthesisRate, py::const_));
 

#ifdef VERSION_INFO
      m.attr("__version__")
    = VERSION_INFO;
#else
      m.attr("__version__")
    = "dev";
#endif
}
