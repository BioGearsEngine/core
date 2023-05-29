%{
#include <biogears/cdm/substance/SESubstanceConcentration.h>
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

%import <biogears/cdm/substance/SESubstance.swg>
%import <biogears/cdm/properties/SEScalar.swg>
%import <biogears/cdm/properties/SEScalarMassPerVolume.swg>

%nspace BIOGEARS_NAMESPACE SESubstanceConcentration;

%ignore BIOGEARS_NAMESPACE SESubstanceConcentration::Unload;
%ignore BIOGEARS_NAMESPACE SESubstanceConcentration::Load;

%ignore BIOGEARS_NAMESPACE SESubstanceConcentration::GetScalar(std::string const &);
%ignore BIOGEARS_NAMESPACE SESubstanceConcentration::SetName(const std::string& name);

%ignore BIOGEARS_NAMESPACE SESubstanceConcentration::make_unique(const std::string& name, Logger* logger);

%ignore BIOGEARS_NAMESPACE SESubstanceConcentration::operator==(SESubstanceConcentration const& rhs) const;
%ignore BIOGEARS_NAMESPACE SESubstanceConcentration::operator!=(SESubstanceConcentration const& rhs) const;
%ignore BIOGEARS_NAMESPACE SESubstanceConcentration::operator=(SESubstanceConcentration const& rhs);

OPEN_BIOGEARS_NAMESPACE
   class SESubstance;
CLOSE_BIOGEARS_NAMESPACE

%include <biogears/cdm/substance/SESubstanceConcentration.h>

%nspace std::vector<BIOGEARS_NAMESPACE SESubstanceConcentration>;
%template (vector_SESubstanceConcetration) std::vector<BIOGEARS_NAMESPACE SESubstanceConcentration>;

%nspace std::vector<BIOGEARS_NAMESPACE SESubstanceConcentration*>;
%nspace std::vector<const BIOGEARS_NAMESPACE SESubstanceConcentration*>;

%template(vector_SESubstanceConcentration_ptr)  std::vector<BIOGEARS_NAMESPACE SESubstanceConcentration*>;
%template(vector_const_SESubstanceConcentration_ptr)  std::vector<const BIOGEARS_NAMESPACE SESubstanceConcentration*>;
