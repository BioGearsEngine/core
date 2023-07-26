%{
#include <biogears/cdm/substance/SESubstanceFraction.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/substance/SESubstanceAerosolization.h>
#include <biogears/cdm/substance/SESubstanceClearance.h>
#include <biogears/cdm/substance/SESubstanceCompound.h>
#include <biogears/cdm/substance/SESubstanceConcentration.h>
#include <biogears/cdm/substance/SESubstancePharmacodynamics.h>
#include <biogears/cdm/substance/SESubstancePharmacokinetics.h>
#include <biogears/cdm/substance/SESubstancePhysicochemicals.h>
#include <biogears/cdm/substance/SESubstanceTissuePharmacokinetics.h>
#include <biogears/cdm/system/environment/conditions/SEEnvironmentCondition.h>
#include <biogears/cdm/substance/SESubstancePharmacokinetics.h>
#include <biogears/cdm/substance/SESubstanceTissuePharmacokinetics.h>
%}

#if defined(SWIGPYTHON)
%module(package="biogears", 
moduleimport=
   "# Import the low-level C/C++ module\n"
    "if __package__ or \".\" in __name__:\n"
    "    from . import _biogears\n"
    "else:\n"
    "    import _biogears\n"
) SESubstanceFraction
#endif

%include <biogears/exports.swg>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <biogears/cdm/substance/SESubstance.swg>
%import <biogears/cdm/properties/SEScalarFraction.swg>

%nspace BIOGEARS_NAMESPACE SESubstanceFraction;

%ignore BIOGEARS_NAMESPACE SESubstanceFraction::Unload;
%ignore BIOGEARS_NAMESPACE SESubstanceFraction::Load;

%rename (GetFractionAmount_const) BIOGEARS_NAMESPACE SESubstanceFraction::GetFractionAmount() const;

%include <biogears/cdm/substance/SESubstanceFraction.h>

%nspace std::vector<BIOGEARS_NAMESPACE SESubstanceFraction*>;
%nspace std::vector<const BIOGEARS_NAMESPACE SESubstanceFraction*>;

%template(vector_SESubstanceFraction_ptr)  std::vector<BIOGEARS_NAMESPACE SESubstanceFraction*>;
%template(vector_const_SSESubstanceFraction_ptr)  std::vector<const BIOGEARS_NAMESPACE SESubstanceFraction*>;
