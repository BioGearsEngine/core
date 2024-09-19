#include "Actions.h"

#include "AnesthesiaActions.h"
#include "EnvironmentActions.h"
#include "InhalerActions.h"
#include "PatientActions.h"
#include "Property.h"

#include <biogears/cdm/patient/actions/SEPatientAction.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/scenario/SEAction.h>
#include <biogears/cdm/scenario/SEActionManager.h>
#include <biogears/cdm/scenario/SEAdvanceTime.h>
#include <biogears/cdm/scenario/SESerializeState.h>
#include <biogears/cdm/system/environment/actions/SEEnvironmentAction.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEAnesthesiaMachineAction.h>
#include <biogears/cdm/system/equipment/Inhaler/actions/SEInhalerAction.h>

namespace biogears {
namespace io {

  std::vector<std::unique_ptr<SEAction>> Actions::action_factory(const CDM::ActionListData& in, SESubstanceManager& substances, std::default_random_engine* rd)
  {
    std::vector<std::unique_ptr<SEAction>> r_vec;
    for (auto action_data : in.Action()) {
      r_vec.emplace_back(factory(&action_data, substances, rd));
    }
    return std::move(r_vec);
  }
  std::unique_ptr<SEAction> Actions::factory(CDM::ActionData const* actionData, SESubstanceManager& substances, std::default_random_engine* rd)
  {
    if (auto advData = dynamic_cast<CDM::AdvanceTimeData const*>(actionData); advData) {
      auto advanceTime = std::make_unique<SEAdvanceTime>();
      Actions::UnMarshall(*advData, *advanceTime, rd);
      return advanceTime;
    }

    if (auto stData = dynamic_cast<CDM::SerializeStateData const*>(actionData); stData) {
      auto serializeState = std::make_unique<SESerializeState>();
      Actions::UnMarshall(*stData, *serializeState);
      return serializeState;
    }

    if (auto patient_action_data = dynamic_cast<const CDM::PatientActionData*>(actionData)) {
      return io::PatientActions::factory(patient_action_data, substances, rd);
    }

    if (auto environment_action_data = dynamic_cast<const CDM::EnvironmentActionData*>(actionData)) {
      return io::EnvironmentActions::factory(environment_action_data, substances, rd);
    }

    if (auto inhaller_action_data = dynamic_cast<const CDM::InhalerActionData*>(actionData)) {
      return io::InhalerActions::factory(inhaller_action_data, substances, rd);
    }

    if (auto anesthesia_action_data = dynamic_cast<const CDM::AnesthesiaMachineActionData*>(actionData)) {
      return io::AnesthesiaActions::factory(anesthesia_action_data, substances, rd);
    }

    throw biogears::CommonDataModelException("Actions:Factory - Unsupported Action Received.");
  }
  std::unique_ptr<CDM::ActionData> Actions::factory(const SEAction* data)
  {
    if (auto advData = dynamic_cast<SEAdvanceTime const*>(data); advData) {
      auto advanceTime = std::make_unique<CDM::AdvanceTimeData>();
      Actions::Marshall(*advData, *advanceTime);
      return advanceTime;
    }

    if (auto stData = dynamic_cast<SESerializeState const*>(data); stData) {
      auto serializeState = std::make_unique<CDM::SerializeStateData>();
      Actions::Marshall(*stData, *serializeState);
      return serializeState;
    }

    if (auto patient_action_data = dynamic_cast<SEPatientAction const*>(data)) {
      return io::PatientActions::factory(patient_action_data);
    }

    if (auto environment_action_data = dynamic_cast<SEEnvironmentAction const*>(data)) {
      return io::EnvironmentActions::factory(environment_action_data);
    }

    if (auto inhaller_action_data = dynamic_cast<SEInhalerAction const*>(data)) {
      return io::InhalerActions::factory(inhaller_action_data);
    }

    if (auto anesthesia_action_data = dynamic_cast<SEAnesthesiaMachineAction const*>(data)) {
      return io::AnesthesiaActions::factory(anesthesia_action_data);
    }

    throw biogears::CommonDataModelException("Actions::factory does not support the derived Action. If you are not a developer contact upstream for support.");
  }

  void Actions::Marshall(SEActionManager const& in, std::vector<std::unique_ptr<CDM::ActionData>>& out)
  {
    io::PatientActions::Marshall(in.m_PatientActions, out);
    io::EnvironmentActions::Marshall(in.m_EnvironmentActions, out);
    io::AnesthesiaActions::Marshall(in.m_AnesthesiaMachineActions, out);
    io::InhalerActions::Marshall(in.m_InhalerActions, out);
  }

  //-----------------------------------------------------------------------------
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
  // void Actions::UnMarshall(const CDM::ActionListData& in, SEActionList& out, std::default_random_engine* rd = nullptr)
  //{
  //}
  // void Actions::Marshall(const SEActionList& in, CDM::ActionListData& out)
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
    } catch (xsd::cxx::tree::unexpected_enumerator<char>) {
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
    if (in.Filename().size() > 0)
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
  // void Actions::UnMarshall(const CDM::RandomSeed& in, SERandomSeed& out, std::default_random_engine* rd = nullptr)
  //{
  //}
  // void Actions::Marshall(const SERandomSeed& in, CDM::RandomSeed& out)
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
