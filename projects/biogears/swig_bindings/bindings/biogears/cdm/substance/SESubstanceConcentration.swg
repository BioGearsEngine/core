%{
#include <biogears/cdm/substance/SESubstanceConcentration.h>
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <biogears/cdm/substance/SESubstance.swg>
%import <biogears/cdm/properties/SEScalar.swg>
%import <biogears/cdm/properties/SEScalarMassPerVolume.swg>

%nspace biogears::SESubstanceConcentration;

%ignore biogears::SESubstanceConcentration::Unload;
%ignore biogears::SESubstanceConcentration::Load;

%ignore biogears::SESubstanceConcentration::GetScalar(std::string const &);
%ignore biogears::SESubstanceConcentration::SetName(const std::string& name);

%ignore biogears::SESubstanceConcentration::make_unique(const std::string& name, Logger* logger);

%ignore biogears::SESubstanceConcentration::operator==(SESubstanceConcentration const& rhs) const;
%ignore biogears::SESubstanceConcentration::operator!=(SESubstanceConcentration const& rhs) const;
%ignore biogears::SESubstanceConcentration::operator=(SESubstanceConcentration const& rhs);

namespace biogears {
   class SESubstance;
}

%include <biogears/cdm/substance/SESubstanceConcentration.h>

%nspace std::vector<biogears::SESubstanceConcentration>;
%template (vector_SESubstanceConcetration) std::vector<biogears::SESubstanceConcentration>;

%nspace std::vector<biogears::SESubstanceConcentration*>;
%nspace std::vector<const biogears::SESubstanceConcentration*>;

%template(vector_SESubstanceConcentration_ptr)  std::vector<biogears::SESubstanceConcentration*>;
%template(vector_const_SESubstanceConcentration_ptr)  std::vector<const biogears::SESubstanceConcentration*>;
