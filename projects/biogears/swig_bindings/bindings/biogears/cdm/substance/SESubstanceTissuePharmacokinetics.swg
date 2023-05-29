%{
#include <biogears/cdm/substance/SESubstanceTissuePharmacokinetics.h>
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

%nspace BIOGEARS_NAMESPACE SESubstanceTissuePharmacokinetics;

%ignore BIOGEARS_NAMESPACE SESubstanceTissuePharmacokinetics::GetScalar(const std::string& name);
%ignore BIOGEARS_NAMESPACE SESubstanceTissuePharmacokinetics::Unload;
%ignore BIOGEARS_NAMESPACE SESubstanceTissuePharmacokinetics::Load;

%rename (GetPartitionCoefficient_const) BIOGEARS_NAMESPACE SESubstanceTissuePharmacokinetics::GetPartitionCoefficient() const;

%include <biogears/cdm/substance/SESubstanceTissuePharmacokinetics.h>