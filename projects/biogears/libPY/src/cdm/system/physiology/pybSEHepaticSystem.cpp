#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/system/physiology/SEHepaticSystem.h>          
#include <biogears/container/Tree.tci.h>


namespace py = pybind11;

void define_pybSEHepaticSystem(py::module_ &m)
{
  py::module_::import("pybSESystem");

  py::class_<biogears::SEHepaticSystem, biogears::SESystem>(m, "SEHepaticSystem")
    .def(py::init<biogears::Logger*>())
    .def("HasKetoneProductionRate",&biogears::SEHepaticSystem::HasKetoneProductionRate)
    .def("GetKetoneProductionRate", py::overload_cast<>(&biogears::SEHepaticSystem::GetKetoneProductionRate))
    .def("GetKetoneProductionRate", py::overload_cast<const biogears::AmountPerTimeUnit&>(&biogears::SEHepaticSystem::GetKetoneProductionRate, py::const_))
    .def("HasHepaticGluconeogenesisRate",&biogears::SEHepaticSystem::HasHepaticGluconeogenesisRate)
    .def("GetHepaticGluconeogenesisRate", py::overload_cast<>(&biogears::SEHepaticSystem::GetHepaticGluconeogenesisRate))
    .def("GetHepaticGluconeogenesisRate", py::overload_cast<const biogears::MassPerTimeUnit&>(&biogears::SEHepaticSystem::GetHepaticGluconeogenesisRate, py::const_));

#ifdef VERSION_INFO
      m.attr("__version__")
    = VERSION_INFO;
#else
      m.attr("__version__")
    = "dev";
#endif
}
