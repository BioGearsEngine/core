
%{
#include <biogears/cdm/patient/actions/SEAcuteRespiratoryDistress.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace biogears::SEAcuteRespiratoryDistress;

%ignore biogears::SEAcuteRespiratoryDistress::Unload;
%ignore biogears::SEAcuteRespiratoryDistress::Load;

#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SEAcuteRespiratoryDistress::ToString() const "public override";
%csmethodmodifiers biogears::SEAcuteRespiratoryDistress::ToString(std::ostream& str) const "public override";
#endif  

%include <biogears/cdm/patient/actions/SEAcuteRespiratoryDistress.h>
