%{
#include <biogears/cdm/substance/SESubstanceCompound.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/substance/SESubstanceTissuePharmacokinetics.h>
#include <biogears/cdm/substance/SESubstancePharmacodynamics.h>
#include <biogears/cdm/substance/SESubstanceCompound.h>
#include <biogears/cdm/substance/SESubstanceTissuePharmacokinetics.h>
#include <biogears/cdm/substance/SESubstancePharmacokinetics.h>
#include <biogears/cdm/substance/SESubstanceConcentration.h>
#include <biogears/cdm/substance/SESubstanceAerosolization.h>
#include <biogears/cdm/substance/SESubstanceClearance.h>
#include <biogears/cdm/substance/SESubstancePhysicochemicals.h>
%}

#if defined(SWIGPYTHON)
%module(package="biogears", 
moduleimport=
   "# Import the low-level C/C++ module\n"
    "if __package__ or \".\" in __name__:\n"
    "    from . import _biogears\n"
    "else:\n"
    "    import _biogears\n"
) SESubstanceCompound
#endif

%include <biogears/exports.swg>

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

%nspace BIOGEARS_NAMESPACE SESubstanceCompound;

%ignore BIOGEARS_NAMESPACE SESubstanceCompound::Unload;
%ignore BIOGEARS_NAMESPACE SESubstanceCompound::Load;

%ignore SESubstanceCompound::GetScalar(std::string const &);
%ignore SESubstanceCompound::SetName(const std::string& name);

%rename (GetComponents_const) BIOGEARS_NAMESPACE SESubstanceCompound::GetComponents() const;
%rename (GetComponent_const)  BIOGEARS_NAMESPACE SESubstanceCompound::GetComponent(SESubstance& substance) const;

%ignore make_unique(const std::string& name, Logger* logger);

%ignore SESubstanceCompound(std::string const &,Logger *);
%ignore SetName(std::string const &);

%include <biogears/cdm/substance/SESubstanceCompound.h>

%nspace  std::vector<BIOGEARS_NAMESPACE SESubstanceCompound*>;
%template (vector_SESubstanceCompound_ptr)  std::vector<BIOGEARS_NAMESPACE SESubstanceCompound*>;