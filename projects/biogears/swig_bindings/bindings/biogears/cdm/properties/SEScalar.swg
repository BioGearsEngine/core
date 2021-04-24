

%{
#include <biogears/cdm/properties/SEScalar.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEProperty.swg>

%nspace biogears::SEScalar;
%nspace biogears::NoUnit;

%rename (LessThan)  biogears::SEScalar::operator<;
%rename (LessThanEqual)  biogears::SEScalar::operator<=;
%rename (GreaterThan)  biogears::SEScalar::operator>;
%rename (GreaterThanEqual)  biogears::SEScalar::operator>=;
 
%ignore  biogears::SEScalar::ToString(std::ostream& str);
%ignore  biogears::SEScalar::operator>>;
%ignore  biogears::SEScalar::operator<<;

%ignore biogears::SEScalar::Load;
%ignore biogears::SEScalar::Unload;
%ignore unitless;

%include <biogears/cdm/properties/SEScalar.h>

%nspace std::vector<biogears::SEScalar*>;
%template(vector_SEScalar_ptr)  std::vector<biogears::SEScalar*>;


