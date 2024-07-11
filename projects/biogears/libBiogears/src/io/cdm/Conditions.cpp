#include "Conditions.h"

#include "Property.h"
#include "Actions.h"

#include <biogears/cdm/scenario/SECondition.h>
#include <biogears/cdm/scenario/SEConditionManager.h>


#include <biogears/BiogearsEnums.h>

namespace biogears {
namespace io {
  // class SECondition;
  void Conditions::UnMarshall(const CDM::ConditionData& in, SECondition& out)
  {
    out.Clear();
    if (in.Comment().present()) {
      out.m_Comment = in.Comment().get();
    }
  }
  void Conditions::Marshall(const SECondition& in, CDM::ConditionData& out)
  {
    if (in.HasComment()) {
      out.Comment(in.m_Comment);
    }
  }
} // namespace io
} // namespace biogears