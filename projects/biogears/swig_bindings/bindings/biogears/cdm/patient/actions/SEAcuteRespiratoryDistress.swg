
%{
#include <biogears/cdm/patient/actions/SEAcuteRespiratoryDistress.h>
%}

%include <biogears/exports.h>

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
