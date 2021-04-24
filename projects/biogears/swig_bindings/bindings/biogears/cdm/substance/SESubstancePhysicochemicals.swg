%{
#include <biogears/cdm/substance/SESubstancePhysicochemicals.h>
#include <biogears/cdm/properties/SEScalar.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <biogears/cdm/substance/SESubstanceTissuePharmacokinetics.h>
%import <biogears/cdm/properties/SEScalar.swg>
%import <biogears/cdm/properties/SEScalarFraction.swg>


%nspace biogears::SESubstancePhysicochemicals;
%nspace biogears::SEPharmacodynamicModifier;

%ignore biogears::SESubstancePhysicochemicals::GetScalar(const std::string& name);
%ignore biogears::SESubstancePhysicochemicals::Unload;
%ignore biogears::SESubstancePhysicochemicals::Load;

%rename (GetPrimaryPKA_const)  biogears::SESubstancePhysicochemicals::GetPrimaryPKA() const;
%rename (GetSecondaryPKA_const)  biogears::SESubstancePhysicochemicals::GetSecondaryPKA() const;
%rename (GetBloodPlasmaRatio_const)  biogears::SESubstancePhysicochemicals::GetBloodPlasmaRatio() const;
%rename (GetFractionUnboundInPlasma_const)  biogears::SESubstancePhysicochemicals::GetFractionUnboundInPlasma() const;
%rename (GetLogP_const)  biogears::SESubstancePhysicochemicals::GetLogP() const;
%rename (GetHydrogenBondCount_const)  biogears::SESubstancePhysicochemicals::GetHydrogenBondCount() const;
%rename (GetPolarSurfaceArea_const)  biogears::SESubstancePhysicochemicals::GetPolarSurfaceArea() const;

%include <biogears/cdm/substance/SESubstancePhysicochemicals.h>
