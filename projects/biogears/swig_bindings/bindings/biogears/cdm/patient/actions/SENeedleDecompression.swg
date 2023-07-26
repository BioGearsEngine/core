%{
#include <biogears/cdm/patient/actions/SENeedleDecompression.h>
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
) SENeedleDecompression
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace BIOGEARS_NAMESPACE SENeedleDecompression;

%ignore BIOGEARS_NAMESPACE SENeedleDecompression::Unload;
%ignore BIOGEARS_NAMESPACE SENeedleDecompression::Load;

%include <biogears/cdm/patient/actions/SENeedleDecompression.h>

%nsspace  std::vector<BIOGEARS_NAMESPACE SENeedleDecompression*>;
%template (vector_SENeedleDecompression_ptr)  std::vector<BIOGEARS_NAMESPACE SENeedleDecompression*>;