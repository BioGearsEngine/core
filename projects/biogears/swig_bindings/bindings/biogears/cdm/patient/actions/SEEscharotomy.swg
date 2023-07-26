%{
#include <biogears/cdm/patient/actions/SEEscharotomy.h>
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
) SEEscharotomy
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace BIOGEARS_NAMESPACE SEEscharotomy;

%ignore BIOGEARS_NAMESPACE SEEscharotomy::Unload;
%ignore BIOGEARS_NAMESPACE SEEscharotomy::Load;

%ignore BIOGEARS_NAMESPACE SEEscharotomy::SetLocation(std::string const&);

%include <biogears/cdm/patient/actions/SEEscharotomy.h>

%nsspace  std::vector<BIOGEARS_NAMESPACE SEEscharotomy*>;
%template (vector_SEEscharotomy_ptr)  std::vector<BIOGEARS_NAMESPACE SEEscharotomy*>;