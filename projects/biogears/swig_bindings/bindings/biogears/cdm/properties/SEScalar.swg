%{
#include <biogears/cdm/properties/SEScalar.h>
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
) SEScalar
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/properties/SEProperty.swg>

%nspace BIOGEARS_NAMESPACE SEScalar;
%nspace BIOGEARS_NAMESPACE NoUnit;

%rename (LessThan)  BIOGEARS_NAMESPACE SEScalar::operator<;
%rename (LessThanEqual)  BIOGEARS_NAMESPACE SEScalar::operator<=;
%rename (GreaterThan)  BIOGEARS_NAMESPACE SEScalar::operator>;
%rename (GreaterThanEqual)  BIOGEARS_NAMESPACE SEScalar::operator>=;
 
%ignore  BIOGEARS_NAMESPACE SEScalar::operator>>;
%ignore  BIOGEARS_NAMESPACE SEScalar::operator<<;

%ignore BIOGEARS_NAMESPACE SEScalar::Load;
%ignore BIOGEARS_NAMESPACE SEScalar::Unload;
%ignore unitless;

#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalar::ToString() const "public override";
#endif 

%include <biogears/cdm/properties/SEScalar.h>

%nspace std::vector<BIOGEARS_NAMESPACE SEScalar*>;
%template(vector_SEScalar_ptr)  std::vector<BIOGEARS_NAMESPACE SEScalar*>;


