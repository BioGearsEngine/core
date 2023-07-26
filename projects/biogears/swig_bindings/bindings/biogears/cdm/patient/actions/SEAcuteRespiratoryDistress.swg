
%{
#include <biogears/cdm/patient/actions/SEAcuteRespiratoryDistress.h>
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
) SEAcuteRespiratoryDistress
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace BIOGEARS_NAMESPACE SEAcuteRespiratoryDistress;

%ignore BIOGEARS_NAMESPACE SEAcuteRespiratoryDistress::Unload;
%ignore BIOGEARS_NAMESPACE SEAcuteRespiratoryDistress::Load;

#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEAcuteRespiratoryDistress::ToString() const "public override";
%csmethodmodifiers BIOGEARS_NAMESPACE SEAcuteRespiratoryDistress::ToString(std::ostream& str) const "public override";
#endif  

%include <biogears/cdm/patient/actions/SEAcuteRespiratoryDistress.h>

%nspace  std::vector<BIOGEARS_NAMESPACE SEAcuteRespiratoryDistress*>;
%template (vector_SEAcuteRespiratoryDistress_ptr)  std::vector<BIOGEARS_NAMESPACE SEAcuteRespiratoryDistress*>;