%{
#include <biogears/cdm/substance/SESubstanceCompound.h>
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <biogears/cdm/substance/SESubstance.swg>
%import <biogears/cdm/substance/SESubstanceConcentration.swg>
%import <biogears/cdm/substance/SESubstancePharmacodynamics.swg>

%import <biogears/cdm/properties/SEScalarAmountPerVolume.swg>
%import <biogears/cdm/properties/SEScalarMass.swg>
%import <biogears/cdm/properties/SEScalarMassPerAmount.swg>
%import <biogears/cdm/properties/SEScalarVolumePerTimePressure.swg>
%import <biogears/cdm/properties/SEScalarInversePressure.swg>
%import <biogears/cdm/properties/SEScalarMassPerAreaTime.swg>
%import <biogears/cdm/properties/SEScalarElectricResistance.swg>
%import <biogears/cdm/properties/SEScalarVolumePerTime.swg>
%import <biogears/cdm/properties/SEScalarPressure.swg>
%import <biogears/cdm/properties/SEScalarTimeMassPerVolume.swg>

%nspace biogears::SESubstanceCompound;

%ignore biogears::SESubstanceCompound::Unload;
%ignore biogears::SESubstanceCompound::Load;

%ignore SESubstanceCompound::GetScalar(std::string const &);
%ignore SESubstanceCompound::SetName(const std::string& name);

%rename (GetComponents_const) biogears::SESubstanceCompound::GetComponents() const;
%rename (GetComponent_const)  biogears::SESubstanceCompound::GetComponent(SESubstance& substance) const;

%ignore make_unique(const std::string& name, Logger* logger);

%ignore SESubstanceCompound(std::string const &,Logger *);
%ignore SetName(std::string const &);

%include <biogears/cdm/substance/SESubstanceCompound.h>

%nsspace  std::vector<biogears::SESubstanceCompound*>;
%template (vector_SESubstanceCompound_ptr)  std::vector<biogears::SESubstanceCompound*>;