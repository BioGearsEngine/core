%{
#include <biogears/cdm/properties/SEGenericScalar.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/utils/unitconversion/CompoundUnit.swg>
%import <biogears/cdm/properties/SEScalar.swg>

%nspace BIOGEARS_NAMESPACE SEGenericScalar;
%nspace BIOGEARS_NAMESPACE SEScalar;

%ignore BIOGEARS_NAMESPACE SEGenericScalar::GetCompoundUnit(std::string const &) const ;

//%ignore BIOGEARS_NAMESPACE SEGenericScalar::GetValue() const;
//%rename (GetValueAs) BIOGEARS_NAMESPACE SEGenericScalar::GetValue(const char* unit) const;
%ignore BIOGEARS_NAMESPACE SEGenericScalar::GetValue(std::string const &) const ;
%ignore BIOGEARS_NAMESPACE SEGenericScalar::IncrementValue(double,std::string const &) ;
%ignore BIOGEARS_NAMESPACE SEGenericScalar::DecrementValue(double,std::string const &) ;

%ignore operator==(const SEGenericScalar&) const;
%ignore operator!=(const SEGenericScalar&) const;

#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEGenericScalar::ToString() const "public override";
#endif 

%include <biogears/cdm/properties/SEGenericScalar.h>
