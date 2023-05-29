
%{
  #include <biogears/cdm/utils/unitconversion/CompoundUnitElement.h>
  #include <biogears/cdm/utils/unitconversion/UnitDimension.h>
  #include <biogears/cdm/utils/unitconversion/CompoundUnit.h>
  #include <biogears/cdm/utils/unitconversion/CompoundUnit.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include "windows.i"
#if defined(SWIGCSHARP)
  %include "csharp/std_string.i"
  %include "csharp/std_vector.i"
  %include "csharp/std_except.i"

  namespace std 
{
  %ignore runtime_error;
  struct runtime_error {};
}

#elif defind(SWIGPYTHON)
  %include "python/std_string.i"
  %include "python/std_vector.i"
  %include "python/std_except.i"
#else
  %include "std_string.i"
  %include "std_vector.i"
  %include "std_except.i"
#endif 
%include "exception.i"
%include <biogears/exports.h>
%include <biogears/config.h>

%nspace BIOGEARS_NAMESPACE CCompoundUnitElement;
%nspace BIOGEARS_NAMESPACE CUnitDimension;
%nspace BIOGEARS_NAMESPACE CCompoundUnit;

#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE CCompoundUnit::ToString() const "public override";
#endif 

OPEN_BIOGEARS_NAMESPACE
   class CCompoundUnit {
   
   public:
     std::string ToString() const{
        return GetString();
      }
   };
CLOSE_BIOGEARS_NAMESPACE



