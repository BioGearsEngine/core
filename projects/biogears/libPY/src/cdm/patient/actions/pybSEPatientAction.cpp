#include <pybind11/pybind11.h>

#include <iostream>
#include <string>

#include <biogears/cdm/patient/actions/SEPatientAction.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

namespace py = pybind11;

class PySEAction : public biogears::SEAction {
public:
  /* Inherit the constructors */
  using biogears::SEAction::SEAction;

  char const* classname() const override
  {
    PYBIND11_OVERRIDE_PURE(
      char const*,
      biogears::SEAction,
      classname,

    );
  }
  /* Trampoline (need one for each virtual function) */
  void ToString(std::ostream& str) const override
  {
    PYBIND11_OVERRIDE_PURE(
      void,
      biogears::SEAction,
      ToString,
      str);
  }
};

class PySEPatientAction : public biogears::SEPatientAction {
public:
  /* Inherit the constructors */
  using biogears::SEPatientAction::SEPatientAction;

  /* Trampoline (need one for each virtual function) */
  char const* classname() const override
  {
    PYBIND11_OVERRIDE_PURE(
      char const*,
      biogears::SEAction,
      classname);
  }

  /* Trampoline (need one for each virtual function) */
  CDM::PatientActionData* Unload() const override
  {
    PYBIND11_OVERRIDE_PURE(
      CDM::PatientActionData*,
      biogears::SEPatientAction,
      Unload);
  }

  /* Trampoline (need one for each virtual function) */
  void ToString(std::ostream& str) const override
  {
    PYBIND11_OVERRIDE_PURE(
      void,
      biogears::SEPatientAction,
      ToString,
      str);
  }
};

void define_pybSEPatientAction(py::module_& m)
{

  py::class_<biogears::SEAction, PySEAction>(m, "SEAction")
    .def(py::init<>())
    .def("Clear", &biogears::SEAction::Clear)
    .def("IsValid", &biogears::SEAction::IsValid)
    .def("Load", &biogears::SEAction::Load)
    .def("Unload", py::overload_cast<>(&biogears::SEAction::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&biogears::SEAction::ToString, py::const_));

  py::class_<biogears::SEPatientAction, PySEPatientAction>(m, "SEPatientAction")
    .def(py::init<>())
    .def("Clear", &biogears::SEPatientAction::Clear)
    .def("IsValid", &biogears::SEPatientAction::IsValid)
    .def("Load", &biogears::SEPatientAction::Load)
    .def("Unload", py::overload_cast<>(&biogears::SEPatientAction::Unload, py::const_))
    .def("ToString", py::overload_cast<std::ostream&>(&biogears::SEPatientAction::ToString, py::const_));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}
