%{
#include <biogears/cdm/patient/actions/SEPainStimulus.h>
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
) SEPainStimulus
#endif


%include <biogears/exports.swg>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/properties/SEScalarTime.swg>

%nspace BIOGEARS_NAMESPACE SEPainStimulus;

%ignore BIOGEARS_NAMESPACE SEPainStimulus::Unload;
%ignore BIOGEARS_NAMESPACE SEPainStimulus::Load;

%ignore BIOGEARS_NAMESPACE SEPainStimulus::SetLocation(const std::string& name);

%include <biogears/cdm/patient/actions/SEPainStimulus.h>


%nsspace  std::vector<BIOGEARS_NAMESPACE SEPainStimulus*>;
%traits_swigtype(SEPainStimulus);
%fragment(SWIG_Traits_frag(SEPainStimulus));
%template (vector_SEPainStimulus_ptr)  std::vector<BIOGEARS_NAMESPACE SEPainStimulus*>;