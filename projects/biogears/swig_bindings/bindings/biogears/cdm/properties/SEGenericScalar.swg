%{
#include <biogears/cdm/properties/SEGenericScalar.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/utils/unitconversion/CompoundUnit.swg>
%import <biogears/cdm/properties/SEScalar.swg>

%nspace biogears::SEGenericScalar;
%nspace biogears::SEScalar;

%ignore biogears::SEGenericScalar::GetCompoundUnit(std::string const &) const ;

//%ignore biogears::SEGenericScalar::GetValue() const;
//%rename (GetValueAs) biogears::SEGenericScalar::GetValue(const char* unit) const;
%ignore biogears::SEGenericScalar::GetValue(std::string const &) const ;
%ignore biogears::SEGenericScalar::IncrementValue(double,std::string const &) ;
%ignore biogears::SEGenericScalar::DecrementValue(double,std::string const &) ;

%ignore operator==(const SEGenericScalar&) const;
%ignore operator!=(const SEGenericScalar&) const;

#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SEGenericScalar::ToString() const "public override";
#endif 

%include <biogears/cdm/properties/SEGenericScalar.h>
