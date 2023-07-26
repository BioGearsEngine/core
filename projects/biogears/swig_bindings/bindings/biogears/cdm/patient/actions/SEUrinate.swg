%{
#include <biogears/cdm/patient/actions/SEUrinate.h>
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
) SEUrinate
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%import <biogears/cdm/properties/SEScalarMassPerVolume.swg>

%nspace BIOGEARS_NAMESPACE SEUrinate;

%ignore BIOGEARS_NAMESPACE SEUrinate::Unload;
%ignore BIOGEARS_NAMESPACE SEUrinate::Load;

%include <biogears/cdm/patient/actions/SEUrinate.h>

%nsspace  std::vector<BIOGEARS_NAMESPACE SEUrinate*>;
%template (vector_SEUrinate_ptr)  std::vector<BIOGEARS_NAMESPACE SEUrinate*>;
