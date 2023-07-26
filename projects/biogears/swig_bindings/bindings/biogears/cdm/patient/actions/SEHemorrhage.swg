 %{
#include <biogears/cdm/patient/actions/SEHemorrhage.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

#if defined(SWIGPYTHON)
%module(package="biogears", 
moduleimport=
   "# Import the low-level C/C++ module\n"
    "if __package__ or \".\" in __name__:\n"
    "    from . import _biogears\n"
    "else:\n"
    "    import _biogears\n"
) SEHemorrhage
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%import <biogears/cdm/properties/SEScalarVolumePerTime.swg>
%import <biogears/cdm/properties/SEScalarFlowResistance.swg>

%nspace BIOGEARS_NAMESPACE SEHemorrhage;

%ignore BIOGEARS_NAMESPACE SEHemorrhage::Unload;
%ignore BIOGEARS_NAMESPACE SEHemorrhage::Load;

%ignore BIOGEARS_NAMESPACE SEHemorrhage::SetCompartment(std::string const &);


%rename(GetMCIS_Const) BIOGEARS_NAMESPACE SEHemorrhage::GetMCIS() const;
%rename(GetInitialRate_Const) BIOGEARS_NAMESPACE SEHemorrhage::GetInitialRate() const;
%rename(GetBleedResistance_Const) BIOGEARS_NAMESPACE SEHemorrhage::GetBleedResistance() const;

%include <biogears/cdm/patient/actions/SEHemorrhage.h>

%nsspace  std::vector<BIOGEARS_NAMESPACE SEHemorrhage*>;
%template (vector_SEHemorrhage_ptr)  std::vector<BIOGEARS_NAMESPACE SEHemorrhage*>;