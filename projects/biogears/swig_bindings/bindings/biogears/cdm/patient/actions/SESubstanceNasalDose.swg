%{
#include <biogears/cdm/patient/actions/SESubstanceNasalDose.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/substance/SESubstancePharmacodynamics.h>
#include <biogears/cdm/substance/SESubstanceTissuePharmacokinetics.h>
#include <biogears/cdm/substance/SESubstancePharmacokinetics.h>
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
) SESubstanceNasalDose
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SESubstanceAdministration.swg>

%import <biogears/cdm/substance/SESubstance.swg>
%import <biogears/cdm/properties/SEScalarMass.swg>
%import <biogears/cdm/properties/SEScalarMassPerVolume.swg>

%nspace BIOGEARS_NAMESPACE SESubstanceNasalDose;

%ignore BIOGEARS_NAMESPACE SESubstanceNasalDose::Unload;
%ignore BIOGEARS_NAMESPACE SESubstanceNasalDose::Load;

%include <biogears/cdm/patient/actions/SESubstanceNasalDose.h>

%nsspace  std::vector<BIOGEARS_NAMESPACE SESubstanceNasalDose*>;
%template (vector_SESubstanceNasalDose_ptr)  std::vector<BIOGEARS_NAMESPACE SESubstanceNasalDose*>;