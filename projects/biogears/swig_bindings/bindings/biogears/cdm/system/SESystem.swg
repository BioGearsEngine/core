%{
#include <biogears/cdm/system/SESystem.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/container/Tree.swg>

%import <biogears/cdm/properties/SEScalar.swg>

%nspace biogears::SESystem;

%ignore biogears::SESystem::Unload;
%ignore biogears::SESystem::Load;
%ignore newFromBind(const CDM::SystemData& action, SESubstanceManager& substances);
%ignore SetComment(std::string const &);

/** @name GetScalar
  *   @brief - A reflextion type call that will return the Scalar associated
  *            with the string. ex. GetScalar("Hematocrit") will return the0
  *            SEScalarPercent object associated with Hematocrit
  *   @details - Note this is an expensive call as it will string compare many times
  *              This is best used, and intended for, you to dynamically prepopulate
  *              a mapping data structure that will help access what you need
  */
  // virtual const SEScalar* GetScalar(const char* name) = 0;
  // virtual const SEScalar* GetScalar(const std::string& name) = 0;

  /** @name GetScalar
  *   @brief - Look for the Scalar property in the systems contained in the provided vector
  */
  // static const SEScalar* GetScalar(const char* name, std::vector<SESystem*>* systems);
  // static const SEScalar* GetScalar(const std::string& name, std::vector<SESystem*>* systems);

%ignore biogears::SESystem::GetScalar(std::string const &);
%ignore biogears::SESystem::GetScalar(std::string const &,std::vector< SESystem * > *);

namespace biogears{
  class SESystem;
}
%nspace std::vector<biogears::SESystem*>;
%template (vector_SESystem_ptr) std::vector<biogears::SESystem*>;

%ignore SESystem::ToString(std::ostream& str) const;
#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SESystem::ToString() const "public override";
#endif 

%include <biogears/cdm/system/SESystem.h>




