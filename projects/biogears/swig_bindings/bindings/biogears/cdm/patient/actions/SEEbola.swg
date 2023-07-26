%{
#include <biogears/cdm/patient/actions/SEEbola.h>
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
) SEEbola
#endif

%include <biogears/exports.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace BIOGEARS_NAMESPACE SEEbola;

%ignore BIOGEARS_NAMESPACE SEEbola::Unload;
%ignore BIOGEARS_NAMESPACE SEEbola::Load;

%include <biogears/cdm/patient/actions/SEEbola.h>

%nsspace  std::vector<BIOGEARS_NAMESPACE SEEbola*>;
%template (vector_SEEbola_ptr)  std::vector<BIOGEARS_NAMESPACE SEEbola*>;