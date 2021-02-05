#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>
namespace py = pybind11;

PYBIND11_MODULE(pybBioGearsEngine, m)
{
  // m.def("CreateBioGearsEngine")
  //testing
  m.def("CreateBioGearsEngine", py::overload_cast<biogears::Logger*>(&biogears::CreateBioGearsEngine));
  m.def("CreateBioGearsEngine", py::overload_cast<const std::string>(&biogears::CreateBioGearsEngine));
  m.def("CreateBioGearsEngine", py::overload_cast<const char*>(&biogears::CreateBioGearsEngine));
  m.def("CreateBioGearsEngine", py::overload_cast<const std::string, const std::string>(&biogears::CreateBioGearsEngine));
  m.def("CreateBioGearsEngine", py::overload_cast<const char*, const char*>(&biogears::CreateBioGearsEngine));
  m.def("CreateBioGearsEngine", py::overload_cast<const std::string, biogears::Logger*>(&biogears::CreateBioGearsEngine));
  m.def("CreateBioGearsEngine", py::overload_cast<const char*, biogears::Logger*>(&biogears::CreateBioGearsEngine));

  // Testing starts

  // Testing ends here

  py::class_<biogears::PhysiologyEngine>(m, "PhysiologyEngine");
}