%{
#include <biogears/cdm/substance/SESubstancePhysicochemicals.h>
#include <biogears/cdm/properties/SEScalar.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/substance/SESubstancePharmacodynamics.h>
#include <biogears/cdm/substance/SESubstanceCompound.h>
#include <biogears/cdm/substance/SESubstanceTissuePharmacokinetics.h>
#include <biogears/cdm/substance/SESubstancePharmacokinetics.h>
#include <biogears/cdm/substance/SESubstanceConcentration.h>
#include <biogears/cdm/substance/SESubstanceAerosolization.h>
#include <biogears/cdm/substance/SESubstanceClearance.h>
#include <biogears/cdm/substance/SESubstancePhysicochemicals.h>
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <biogears/cdm/properties/SEScalar.swg>
%import <biogears/cdm/properties/SEScalarFraction.swg>


%nspace BIOGEARS_NAMESPACE SESubstancePhysicochemicals;
%nspace BIOGEARS_NAMESPACE SEPharmacodynamicModifier;

%ignore BIOGEARS_NAMESPACE SESubstancePhysicochemicals::GetScalar(const std::string& name);
%ignore BIOGEARS_NAMESPACE SESubstancePhysicochemicals::Unload;
%ignore BIOGEARS_NAMESPACE SESubstancePhysicochemicals::Load;

%rename (GetPrimaryPKA_const)  BIOGEARS_NAMESPACE SESubstancePhysicochemicals::GetPrimaryPKA() const;
%rename (GetSecondaryPKA_const)  BIOGEARS_NAMESPACE SESubstancePhysicochemicals::GetSecondaryPKA() const;
%rename (GetBloodPlasmaRatio_const)  BIOGEARS_NAMESPACE SESubstancePhysicochemicals::GetBloodPlasmaRatio() const;
%rename (GetFractionUnboundInPlasma_const)  BIOGEARS_NAMESPACE SESubstancePhysicochemicals::GetFractionUnboundInPlasma() const;
%rename (GetLogP_const)  BIOGEARS_NAMESPACE SESubstancePhysicochemicals::GetLogP() const;
%rename (GetHydrogenBondCount_const)  BIOGEARS_NAMESPACE SESubstancePhysicochemicals::GetHydrogenBondCount() const;
%rename (GetPolarSurfaceArea_const)  BIOGEARS_NAMESPACE SESubstancePhysicochemicals::GetPolarSurfaceArea() const;

%include <biogears/cdm/substance/SESubstancePhysicochemicals.h>
