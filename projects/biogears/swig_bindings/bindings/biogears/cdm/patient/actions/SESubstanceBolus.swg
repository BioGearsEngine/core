%{
#include <biogears/cdm/patient/actions/SESubstanceBolus.h>
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

#if defined(SWIGPYTHON)
%module(package="biogears", 
moduleimport=
   "# Import the low-level C/C++ module\n"
    "if __package__ or \".\" in __name__:\n"
    "    from . import _biogears\n"
    "else:\n"
    "    import _biogears\n"
) SESubstanceBolus
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SESubstanceAdministration.swg>

%import <biogears/cdm/substance/SESubstance.swg>
%import <biogears/cdm/substance/SESubstanceCompound.swg>
%import <biogears/cdm/properties/SEScalarVolume.swg>
%import <biogears/cdm/properties/SEScalarVolumePerTime.swg>
%import <biogears/cdm/properties/SEScalarMassPerVolume.swg>
%import <biogears/cdm/properties/SEScalarTime.swg>


%nspace BIOGEARS_NAMESPACE SESubstanceBolus;

%ignore BIOGEARS_NAMESPACE SESubstanceBolus::Unload;
%ignore BIOGEARS_NAMESPACE SESubstanceBolus::Load;
%ignore BIOGEARS_NAMESPACE SESubstanceBolus::ToString(std::ostream& str) const;

%include <biogears/cdm/patient/actions/SESubstanceBolus.h>

%nspace  std::vector<BIOGEARS_NAMESPACE SESubstanceBolus*>;
%template (vector_SESubstanceBolus_ptr)  std::vector<BIOGEARS_NAMESPACE SESubstanceBolus*>;