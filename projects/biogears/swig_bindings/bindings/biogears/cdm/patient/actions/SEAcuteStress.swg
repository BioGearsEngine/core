%{
#include <biogears/cdm/patient/actions/SEAcuteStress.h>
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
) SEAcuteStress
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace BIOGEARS_NAMESPACE SEAcuteStress;

%ignore BIOGEARS_NAMESPACE SEAcuteStress::Unload;
%ignore BIOGEARS_NAMESPACE SEAcuteStress::Load;

%include <biogears/cdm/patient/actions/SEAcuteStress.h>

%nspace  std::vector<BIOGEARS_NAMESPACE SEAcuteStress*>;
%traits_swigtype(SEAcuteStress);
%fragment(SWIG_Traits_frag(SEAcuteStress));
%template (vector_SEAcuteStress_ptr)  std::vector<BIOGEARS_NAMESPACE SEAcuteStress*>;
