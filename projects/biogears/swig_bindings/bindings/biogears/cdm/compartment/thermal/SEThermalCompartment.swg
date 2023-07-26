%{
#include <biogears/cdm/compartment/thermal/SEThermalCompartment.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

#if defined(SWIGPYTHON)
%module(package="biogears", 
moduleimport=
   "# Import the low-level C/C++ module\n"
    "if __package__ or \".\" in __name__:\n"
    "    from . import _biogears\n"
    "else:\n"
    "    import _biogears\n"
) SEThermalCompartment
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/compartment/SECompartment.swg>

%import <biogears/cdm/properties/SEScalar.swg>
%import <biogears/cdm/properties/SEScalarPower.swg>
%import <biogears/cdm/properties/SEScalarEnergy.swg>
%import <biogears/cdm/properties/SEScalarTemperature.swg>


%nspace BIOGEARS_NAMESPACE SEThermalCompartment;

%ignore BIOGEARS_NAMESPACE SEThermalCompartment::Unload;
%ignore BIOGEARS_NAMESPACE SEThermalCompartment::Load;

%ignore BIOGEARS_NAMESPACE SEThermalCompartment::GetScalar(const std::string& name);

%ignore BIOGEARS_NAMESPACE SEThermalCompartment::HasNodeMapping() const;
%ignore BIOGEARS_NAMESPACE SEThermalCompartment::GetNodeMapping();
%ignore BIOGEARS_NAMESPACE SEThermalCompartment::MapNode(SEThermalCircuitNode& node);

%rename (GetHeatTransferRateIn_const) BIOGEARS_NAMESPACE SEThermalCompartment::GetHeatTransferRateIn(const PowerUnit& unit) const;
%rename (GetHeatTransferRateOut_const) BIOGEARS_NAMESPACE SEThermalCompartment::GetHeatTransferRateOut(const PowerUnit& unit) const;
%rename (GetHeat_const) BIOGEARS_NAMESPACE SEThermalCompartment::GetHeat(const EnergyUnit& unit) const;
%rename (GetTemperature_const) BIOGEARS_NAMESPACE SEThermalCompartment::GetTemperature(const TemperatureUnit& unit) const;

%rename (HasChild_const) BIOGEARS_NAMESPACE SEThermalCompartment::HasChild(std::string const &);

%ignore BIOGEARS_NAMESPACE SEThermalCompartment::AddLink(SEThermalCompartmentLink& link);
%ignore BIOGEARS_NAMESPACE SEThermalCompartment::RemoveLink(SEThermalCompartmentLink& link);
%ignore BIOGEARS_NAMESPACE SEThermalCompartment::RemoveLinks();
%ignore BIOGEARS_NAMESPACE SEThermalCompartment::GetLinks();

%include <biogears/cdm/compartment/thermal/SEThermalCompartment.h>

%nspace std::vector<BIOGEARS_NAMESPACE SEThermalCompartment*>;
%template (vector_SEThermalCompartment_ptr) std::vector<BIOGEARS_NAMESPACE SEThermalCompartment*>;
