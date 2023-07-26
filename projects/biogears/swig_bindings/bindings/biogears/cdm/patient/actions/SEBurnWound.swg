%{
#include <biogears/cdm/patient/actions/SEBurnWound.h>
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
) SEBurnWound
#endif

%include <biogears/exports.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace BIOGEARS_NAMESPACE SEBurnWound;

%ignore BIOGEARS_NAMESPACE SEBurnWound::Unload;
%ignore BIOGEARS_NAMESPACE SEBurnWound::Load;

%ignore BIOGEARS_NAMESPACE SEBurnWound::SetCompartment(std::string const &);

%include <biogears/cdm/patient/actions/SEBurnWound.h>

%nsspace  std::vector<BIOGEARS_NAMESPACE SEBurnWound*>;
%template (vector_SEBurnWound_ptr)  std::vector<BIOGEARS_NAMESPACE SEBurnWound*>;
