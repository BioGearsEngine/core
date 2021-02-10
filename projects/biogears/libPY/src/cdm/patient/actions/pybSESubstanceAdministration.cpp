#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/patient/actions/SESubstanceAdministration.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/substance/SESubstanceManager.h>

#include "PySEAction.h"
#include "PySEPatientAction.h"

namespace py = pybind11;

class PySESubstanceAdministration : public biogears::SESubstanceAdministration {
public:
  /* Inherit the constructors */
  using SESubstanceAdministration::SESubstanceAdministration;


  char const* classname() const
  {
    PYBIND11_OVERRIDE_PURE(
      char const*,
      biogears::SEAction,
      classname);
  }

  CDM::SubstanceAdministrationData* Unload() const
  {
    PYBIND11_OVERRIDE_PURE(
      CDM::SubstanceAdministrationData*,
      biogears::SEPatientAction,
      Unload);
  }

  void ToString(std::ostream& str) const{
    PYBIND11_OVERRIDE_PURE(
      void,
      biogears::SEPatientAction,
      ToString);
  }

};

void define_pybSESubstanceAdministration(py::module_& m)
{
  py::class_<biogears::SESubstanceAdministration, biogears::SEPatientAction, PySESubstanceAdministration>(m, "SESubstanceAdministration")
    .def(py::init<>())
    .def("Clear", &biogears::SESubstanceAdministration::Clear)
    .def("IsValid", &biogears::SESubstanceAdministration::IsValid)
    .def("IsActive", &biogears::SESubstanceAdministration::IsActive)
    .def("Load", py::overload_cast<const CDM::SubstanceAdministrationData&>(&biogears::SESubstanceAdministration::Load));
}
