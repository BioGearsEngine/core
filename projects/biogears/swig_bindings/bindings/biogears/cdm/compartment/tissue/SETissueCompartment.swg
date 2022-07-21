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

%nspace biogears::SETissueCompartment;

%ignore biogears::SETissueCompartment::Unload;
%ignore biogears::SETissueCompartment::Load;

%ignore biogears::SETissueCompartment::GetScalar(const std::string& name);

%rename (GetAcidicPhospohlipidConcentration_const) biogears::SETissueCompartment::GetAcidicPhospohlipidConcentration(const MassPerMassUnit& unit) const;
%rename (GetMatrixVolume_const) biogears::SETissueCompartment::GetMatrixVolume(const VolumeUnit& unit) const;
%rename (GetMembranePotential_const) biogears::SETissueCompartment::GetMembranePotential(const ElectricPotentialUnit& unit) const;
%rename (GetNeutralLipidsVolumeFraction_const) biogears::SETissueCompartment::GetNeutralLipidsVolumeFraction() const;
%rename (GetNeutralPhospholipidsVolumeFraction_const) biogears::SETissueCompartment::GetNeutralPhospholipidsVolumeFraction() const;
%rename (GetReflectionCoefficient_const) biogears::SETissueCompartment::GetReflectionCoefficient() const;
%rename (GetTissueToPlasmaAlbuminRatio_const) biogears::SETissueCompartment::GetTissueToPlasmaAlbuminRatio() const;
%rename (GetTissueToPlasmaAlphaAcidGlycoproteinRatio_const) biogears::SETissueCompartment::GetTissueToPlasmaAlphaAcidGlycoproteinRatio() const;
%rename (GetTissueToPlasmaLipoproteinRatio_const) biogears::SETissueCompartment::GetTissueToPlasmaLipoproteinRatio() const;
%rename (GetTotalMass_const) biogears::SETissueCompartment::GetTotalMass(const MassUnit& unit) const;

#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SETissueCompartment::HashCode() const "public override";
#endif 

%include <biogears/cdm/compartment/tissue/SETissueCompartment.h>

%nspace std::vector<biogears::SETissueCompartment*>;
%template (vector_SETissueCompartment_ptr) std::vector<biogears::SETissueCompartment*>;
