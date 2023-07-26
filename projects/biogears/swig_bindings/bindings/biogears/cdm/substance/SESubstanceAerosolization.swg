%{
#include <biogears/cdm/substance/SESubstanceAerosolization.h>
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
) SESubstanceAerosolization
#endif

%include <biogears/exports.swg>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <biogears/cdm/properties/SEScalarNeg1To1.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/properties/SEHistogramFractionVsLength.swg>
%import <biogears/cdm/properties/SEScalarMassPerVolume.swg>


%nspace BIOGEARS_NAMESPACE SESubstanceAerosolization;

%ignore BIOGEARS_NAMESPACE SESubstanceAerosolization::Unload;
%ignore BIOGEARS_NAMESPACE SESubstanceAerosolization::Load;

%ignore BIOGEARS_NAMESPACE SESubstanceAerosolization::operator==(SESubstanceAerosolization const& rhs) const;
%ignore BIOGEARS_NAMESPACE SESubstanceAerosolization::operator!=(SESubstanceAerosolization const& rhs) const;
%ignore BIOGEARS_NAMESPACE SESubstanceAerosolization::operator=(SESubstanceAerosolization const& rhs);

%ignore BIOGEARS_NAMESPACE SESubstanceAerosolization::GetScalar(std::string const &);
%rename (GetBronchioleModifier_const) BIOGEARS_NAMESPACE SESubstanceAerosolization::GetBronchioleModifier() const;
%rename (GetInflammationCoefficient_const) BIOGEARS_NAMESPACE SESubstanceAerosolization::GetInflammationCoefficient() const;
%rename (GetParticulateSizeDistribution_const) BIOGEARS_NAMESPACE SESubstanceAerosolization::GetParticulateSizeDistribution() const;

%include <biogears/cdm/substance/SESubstanceAerosolization.h>
