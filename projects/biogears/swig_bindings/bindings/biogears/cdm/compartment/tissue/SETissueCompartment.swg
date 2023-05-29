%{
#include <biogears/cdm/compartment/tissue/SETissueCompartment.h>
#include <biogears/cdm/properties/SEScalarMass.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/compartment/SECompartment.swg>

%import <biogears/cdm/properties/SEScalar.swg>
%import <biogears/cdm/properties/SEScalarMassPerMass.swg>
%import <biogears/cdm/properties/SEScalarVolume.swg>
%import <biogears/cdm/properties/SEScalarElectricPotential.swg>
%import <biogears/cdm/properties/SEScalarFraction.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/properties/SEScalarMass.swg>

%nspace BIOGEARS_NAMESPACE SETissueCompartment;

%ignore BIOGEARS_NAMESPACE SETissueCompartment::Unload;
%ignore BIOGEARS_NAMESPACE SETissueCompartment::Load;

%ignore BIOGEARS_NAMESPACE SETissueCompartment::GetScalar(const std::string& name);

%rename (GetAcidicPhospohlipidConcentration_const) BIOGEARS_NAMESPACE SETissueCompartment::GetAcidicPhospohlipidConcentration(const MassPerMassUnit& unit) const;
%rename (GetMatrixVolume_const) BIOGEARS_NAMESPACE SETissueCompartment::GetMatrixVolume(const VolumeUnit& unit) const;
%rename (GetMembranePotential_const) BIOGEARS_NAMESPACE SETissueCompartment::GetMembranePotential(const ElectricPotentialUnit& unit) const;
%rename (GetNeutralLipidsVolumeFraction_const) BIOGEARS_NAMESPACE SETissueCompartment::GetNeutralLipidsVolumeFraction() const;
%rename (GetNeutralPhospholipidsVolumeFraction_const) BIOGEARS_NAMESPACE SETissueCompartment::GetNeutralPhospholipidsVolumeFraction() const;
%rename (GetReflectionCoefficient_const) BIOGEARS_NAMESPACE SETissueCompartment::GetReflectionCoefficient() const;
%rename (GetTissueToPlasmaAlbuminRatio_const) BIOGEARS_NAMESPACE SETissueCompartment::GetTissueToPlasmaAlbuminRatio() const;
%rename (GetTissueToPlasmaAlphaAcidGlycoproteinRatio_const) BIOGEARS_NAMESPACE SETissueCompartment::GetTissueToPlasmaAlphaAcidGlycoproteinRatio() const;
%rename (GetTissueToPlasmaLipoproteinRatio_const) BIOGEARS_NAMESPACE SETissueCompartment::GetTissueToPlasmaLipoproteinRatio() const;
%rename (GetTotalMass_const) BIOGEARS_NAMESPACE SETissueCompartment::GetTotalMass(const MassUnit& unit) const;

#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SETissueCompartment::HashCode() const "public override";
#endif 

%include <biogears/cdm/compartment/tissue/SETissueCompartment.h>

%nspace std::vector<BIOGEARS_NAMESPACE SETissueCompartment*>;
%template (vector_SETissueCompartment_ptr) std::vector<BIOGEARS_NAMESPACE SETissueCompartment*>;
