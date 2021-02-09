#include <pybind11/pybind11.h>

#include <string>

#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/scenario/SEAnesthesiaMachineActionCollection.h>

namespace py = pybind11;

void define_pybSEAnesthesiaMachineActionCollection(py::module_ &m)
{

  py::class_<biogears::SEAnesthesiaMachineActionCollection, biogears::Loggable>(m, "SEAnesthesiaMachineActionCollection")
    .def(py::init<biogears::SESubstanceManager&>())
    // .def("TypeTag",&biogears::SEAdvanceTime::TypeTag)
    // .def("classname", py::overload_cast<>(&biogears::SEAdvanceTime::classname,py::const_))
    .def("Clear", &biogears::SEAnesthesiaMachineActionCollection::Clear)
    .def("Unload", &biogears::SEAnesthesiaMachineActionCollection::Unload)
    .def("ProcessAction", py::overload_cast<const biogears::SEAnesthesiaMachineAction&>(&biogears::SEAnesthesiaMachineActionCollection::ProcessAction))
    .def("ProcessAction", py::overload_cast<const CDM::AnesthesiaMachineActionData&>(&biogears::SEAnesthesiaMachineActionCollection::ProcessAction))
    .def("HasConfiguration", &biogears::SEAnesthesiaMachineActionCollection::HasConfiguration)
    .def("GetConfiguration", &biogears::SEAnesthesiaMachineActionCollection::GetConfiguration)
    .def("RemoveConfiguration", &biogears::SEAnesthesiaMachineActionCollection::RemoveConfiguration)

    .def("HasOxygenTankPressureLoss", &biogears::SEAnesthesiaMachineActionCollection::HasOxygenTankPressureLoss)
    .def("GetOxygenTankPressureLoss", &biogears::SEAnesthesiaMachineActionCollection::GetOxygenTankPressureLoss)
    .def("RemoveOxygenTankPressureLoss", &biogears::SEAnesthesiaMachineActionCollection::RemoveOxygenTankPressureLoss)

    .def("HasOxygenWallPortPressureLoss", &biogears::SEAnesthesiaMachineActionCollection::HasOxygenWallPortPressureLoss)
    .def("GetOxygenWallPortPressureLoss", &biogears::SEAnesthesiaMachineActionCollection::GetOxygenWallPortPressureLoss)
    .def("RemoveOxygenWallPortPressureLoss", &biogears::SEAnesthesiaMachineActionCollection::RemoveOxygenWallPortPressureLoss)

    .def("HasExpiratoryValveLeak", &biogears::SEAnesthesiaMachineActionCollection::HasExpiratoryValveLeak)
    .def("GetExpiratoryValveLeak", &biogears::SEAnesthesiaMachineActionCollection::GetExpiratoryValveLeak)
    .def("RemoveExpiratoryValveLeak", &biogears::SEAnesthesiaMachineActionCollection::RemoveExpiratoryValveLeak)

    .def("HasExpiratoryValveObstruction", &biogears::SEAnesthesiaMachineActionCollection::HasExpiratoryValveObstruction)
    .def("GetExpiratoryValveObstruction", &biogears::SEAnesthesiaMachineActionCollection::GetExpiratoryValveObstruction)
    .def("RemoveExpiratoryValveObstruction", &biogears::SEAnesthesiaMachineActionCollection::RemoveExpiratoryValveObstruction)

    .def("HasInspiratoryValveLeak", &biogears::SEAnesthesiaMachineActionCollection::HasInspiratoryValveLeak)
    .def("GetInspiratoryValveLeak", &biogears::SEAnesthesiaMachineActionCollection::GetInspiratoryValveLeak)
    .def("RemoveInspiratoryValveLeak", &biogears::SEAnesthesiaMachineActionCollection::RemoveInspiratoryValveLeak)

    .def("HasInspiratoryValveObstruction", &biogears::SEAnesthesiaMachineActionCollection::HasInspiratoryValveObstruction)
    .def("GetInspiratoryValveObstruction", &biogears::SEAnesthesiaMachineActionCollection::GetInspiratoryValveObstruction)
    .def("RemoveInspiratoryValveObstruction", &biogears::SEAnesthesiaMachineActionCollection::RemoveInspiratoryValveObstruction)

    .def("HasMaskLeak", &biogears::SEAnesthesiaMachineActionCollection::HasMaskLeak)
    .def("GetMaskLeak", &biogears::SEAnesthesiaMachineActionCollection::GetMaskLeak)
    .def("RemoveMaskLeak", &biogears::SEAnesthesiaMachineActionCollection::RemoveMaskLeak)

    .def("HasSodaLimeFailure", &biogears::SEAnesthesiaMachineActionCollection::HasSodaLimeFailure)
    .def("GetSodaLimeFailure", &biogears::SEAnesthesiaMachineActionCollection::GetSodaLimeFailure)
    .def("RemoveSodaLimeFailure", &biogears::SEAnesthesiaMachineActionCollection::RemoveSodaLimeFailure)

    .def("HasTubeCuffLeak", &biogears::SEAnesthesiaMachineActionCollection::HasTubeCuffLeak)
    .def("GetTubeCuffLeak", &biogears::SEAnesthesiaMachineActionCollection::GetTubeCuffLeak)
    .def("RemoveTubeCuffLeak", &biogears::SEAnesthesiaMachineActionCollection::RemoveTubeCuffLeak)

    .def("HasVaporizerFailure", &biogears::SEAnesthesiaMachineActionCollection::HasVaporizerFailure)
    .def("GetVaporizerFailure", &biogears::SEAnesthesiaMachineActionCollection::GetVaporizerFailure)
    .def("RemoveVaporizerFailure", &biogears::SEAnesthesiaMachineActionCollection::RemoveVaporizerFailure)

    .def("HasVentilatorPressureLoss", &biogears::SEAnesthesiaMachineActionCollection::HasVentilatorPressureLoss)
    .def("GetVentilatorPressureLoss", &biogears::SEAnesthesiaMachineActionCollection::GetVentilatorPressureLoss)
    .def("RemoveVentilatorPressureLoss", &biogears::SEAnesthesiaMachineActionCollection::RemoveVentilatorPressureLoss)

    .def("HasYPieceDisconnect", &biogears::SEAnesthesiaMachineActionCollection::HasYPieceDisconnect)
    .def("GetYPieceDisconnect", &biogears::SEAnesthesiaMachineActionCollection::GetYPieceDisconnect)
    .def("RemoveYPieceDisconnect", &biogears::SEAnesthesiaMachineActionCollection::RemoveYPieceDisconnect);

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}
