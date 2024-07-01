#include "Actions.h"

#include "Property.h"

#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/scenario/SEAction.h>
#include <biogears/cdm/scenario/SEAdvanceTime.h>
#include <biogears/cdm/scenario/SESerializeState.h>

namespace biogears {
namespace io {
//-----------------------`------------------------------------------------------
// class SEAction
void Actions::UnMarshall(const CDM::ActionData& in, SEAction& out)
{
  out.Clear();
  if (in.Comment().present()) {
    out.m_Comment = in.Comment().get();
  }
}
void Actions::Marshall(const SEAction& in, CDM::ActionData& out)
{
  if (in.HasComment()) {
    out.Comment(in.m_Comment);
  }
}
//------------------------------------------------------------------------------
// class SEActionList;
//void Actions::UnMarshall(const CDM::ActionListData& in, SEActionList& out, std::default_random_engine* rd = nullptr)
//{
//}
//void Actions::Marshall(const SEActionList& in, CDM::ActionListData& out)
//{
//}

//-----------------------------------------------------------------------------
// class SEAdvanceTime
void Actions::UnMarshall(const CDM::AdvanceTimeData& in, SEAdvanceTime& out, std::default_random_engine* re)
{
  UnMarshall(static_cast<const CDM::ActionData&>(in), static_cast<SEAction&>(out));
  io::Property::UnMarshall(in.Time(), out.GetTime(), re);
}
void Actions::Marshall(const SEAdvanceTime& in, CDM::AdvanceTimeData& out)
{
  Marshall(static_cast<const SEAction&>(in), static_cast<CDM::ActionData&>(out));
  CDM_PROPERTY_MARSHALL_HELPER(in, out, Time)
}
//-----------------------------------------------------------------------------
// class SESerializationType;
void Actions::UnMarshall(const CDM::enumSerializationType& in, SESerializationType& out)
{
  try {
    switch (in) {
      case CDM::enumSerializationType::Load:
        out = SESerializationType::Load;
        break;
      case CDM::enumSerializationType::Save:
        out = SESerializationType::Save;
        break;
      default:
        out = SESerializationType::Invalid;
        break;
    }
  } catch ( xsd::cxx::tree::unexpected_enumerator<char> ) {
    out = SESerializationType::Invalid;
  }
}
void Actions::Marshall(const SESerializationType& in, CDM::enumSerializationType& out)
{
  switch (in) {
    case SESerializationType::Load:
      out = CDM::enumSerializationType::Load;
      break;
    case SESerializationType::Save:
      out = CDM::enumSerializationType::Save;
      break;
    default:
      out = "";
      break;
  }
}
//-----------------------------------------------------------------------------
// class SESerializeState
void Actions::UnMarshall(const CDM::SerializeStateData& in, SESerializeState& out)
{
  UnMarshall(static_cast<const CDM::ActionData&>(in), static_cast<SEAction&>(out));
  UnMarshall(in.Type(), out.m_Type);
  if (in.Filename().size() > 0 )
    out.SetFilename(in.Filename());
}
void Actions::Marshall(const SESerializeState& in, CDM::SerializeStateData& out)
{
  Marshall(static_cast<const SEAction&>(in), static_cast<CDM::ActionData&>(out));
  if (in.HasFilename()) {
    out.Filename(in.m_Filename);
  } else {
    out.Filename("");
  }
  SE_ACTIONS_ENUM_MARSHALL_HELPER(in, out, Type)
}
//------------------------------------------------------------------------------
// class SERandomSeed
//void Actions::UnMarshall(const CDM::RandomSeed& in, SERandomSeed& out, std::default_random_engine* rd = nullptr)
//{
//}
//void Actions::Marshall(const SERandomSeed& in, CDM::RandomSeed& out)
//{
//}
}
bool operator==(CDM::enumSerializationType const& lhs, SESerializationType const& rhs)
{
  switch (rhs) {
    case SESerializationType ::Save:
      return (CDM::enumSerializationType ::Save == lhs);
    case SESerializationType ::Load:
      return (CDM::enumSerializationType ::Load == lhs);
    case SESerializationType ::Invalid:
      return ((CDM::enumSerializationType::value)-1 == lhs);
    default:
      return false;
  }
}
}
