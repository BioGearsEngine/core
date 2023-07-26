%{
#include <biogears/cdm/substance/SESubstanceClearance.h>
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
) SESubstanceClearance
#endif

%include <biogears/exports.swg>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <biogears/cdm/properties/SEScalarNeg1To1.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/properties/SEHistogramFractionVsLength.swg>

%import <biogears/cdm/properties/SEScalarFrequency.swg>  
%import <biogears/cdm/properties/SEScalarFraction.swg>
%import <biogears/cdm/properties/SEScalarVolumePerTimeMass.swg>
%import <biogears/cdm/properties/SEScalar.swg>
%import <biogears/cdm/properties/SEScalarMassPerTime.swg>

%nspace BIOGEARS_NAMESPACE SESubstanceClearance;

%ignore BIOGEARS_NAMESPACE SESubstanceClearance::Unload;
%ignore BIOGEARS_NAMESPACE SESubstanceClearance::Load;

%ignore BIOGEARS_NAMESPACE SESubstanceClearance::GetScalar(const std::string& name);

%rename (GetFractionExcretedInFeces_const) BIOGEARS_NAMESPACE SESubstanceClearance::GetFractionExcretedInFeces() const;
%rename (GetFractionExcretedInUrine_const) BIOGEARS_NAMESPACE SESubstanceClearance::GetFractionExcretedInUrine() const;
%rename (GetFractionMetabolizedInGut_const) BIOGEARS_NAMESPACE SESubstanceClearance::GetFractionMetabolizedInGut() const;
%rename (GetFractionUnboundInPlasma_const) BIOGEARS_NAMESPACE SESubstanceClearance::GetFractionUnboundInPlasma() const;
%rename (GetGlomerularFilterability_const) BIOGEARS_NAMESPACE SESubstanceClearance::GetGlomerularFilterability() const;
%rename (GetRenalReabsorptionRatio_const) BIOGEARS_NAMESPACE SESubstanceClearance::GetRenalReabsorptionRatio() const;

%include <biogears/cdm/substance/SESubstanceClearance.h>