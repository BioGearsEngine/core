
%{
  #include <biogears/cdm/utils/unitconversion/CompoundUnitElement.h>
  #include <biogears/cdm/utils/unitconversion/UnitDimension.h>
  #include <biogears/cdm/utils/unitconversion/CompoundUnit.h>
  #include <biogears/cdm/utils/unitconversion/CompoundUnit.h>
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

%nspace biogears::CCompoundUnitElement;
%nspace biogears::CUnitDimension;
%nspace biogears::CCompoundUnit;

#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::CCompoundUnit::ToString() const "public override";
#endif 

namespace biogears {
   class CCompoundUnit {
   
   public:
     std::string ToString() const{
        return GetString();
      }
   };
}



