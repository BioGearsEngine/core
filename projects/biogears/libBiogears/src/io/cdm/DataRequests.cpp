#include "DataRequests.h"

#include "Property.h"

#include <biogears/BiogearsEnums.h>
#include <biogears/cdm/scenario/requests/SEDataRequestManager.h>
#include <biogears/cdm/scenario/requests/SEEnvironmentDataRequest.h>
#include <biogears/cdm/scenario/requests/SEEquipmentDataRequest.h>
#include <biogears/cdm/scenario/requests/SEGasCompartmentDataRequest.h>
#include <biogears/cdm/scenario/requests/SELiquidCompartmentDataRequest.h>
#include <biogears/cdm/scenario/requests/SEPatientDataRequest.h>
#include <biogears/cdm/scenario/requests/SEPhysiologyDataRequest.h>
#include <biogears/cdm/scenario/requests/SESubstanceDataRequest.h>
#include <biogears/cdm/scenario/requests/SEThermalCompartmentDataRequest.h>
#include <biogears/cdm/scenario/requests/SETissueCompartmentDataRequest.h>

#include <biogears/cdm/substance/SESubstanceManager.h>

namespace biogears {
namespace io {
  //-----------------------------------------------------------------------------
  // class SEDataRequestManager
  void DataRequests::UnMarshall(const CDM::DataRequestManagerData& in, SESubstanceManager const& subMgr, SEDataRequestManager& out, std::default_random_engine* re)
  {
    out.Clear();
    if (in.Filename().present())
      out.m_ResultsFile = in.Filename().get();
    if (in.WorkingDir().present())
      out.m_WorkingDir = in.WorkingDir().get();
    if (in.SamplesPerSecond().present())
      out.m_SamplesPerSecond = in.SamplesPerSecond().get();
    if (in.DefaultDecimalFormatting().present())
      io::Property::UnMarshall(in.DefaultDecimalFormatting(), out.GetDefaultDecimalFormatting());
    if (in.OverrideDecimalFormatting().present())
      io::Property::UnMarshall(in.OverrideDecimalFormatting(), out.GetOverrideDecimalFormatting());

    for (auto& request : in.DataRequest()) {
      auto dr = factory(&request, subMgr, out.m_DefaultDecimalFormatting);
      if (dr != nullptr) {
        if (out.HasOverrideDecimalFormatting())
          static_cast<SEDecimalFormat*>(dr.get())->Set(*out.m_OverrideDecimalFormatting);
        out.m_Requests.push_back(dr.release());
      }
    }
  }
  void DataRequests::Marshall(const SEDataRequestManager& in, CDM::DataRequestManagerData& out)
  {
    out.SamplesPerSecond(in.m_SamplesPerSecond);
    if (in.HasResultsFilename())
      out.Filename(in.m_ResultsFile);
    if (in.HasWorkingDir())
      out.WorkingDir(in.m_WorkingDir);
    if (in.HasDefaultDecimalFormatting())
      io::Property::Marshall(*in.m_DefaultDecimalFormatting, out.DefaultDecimalFormatting());
    if (in.HasOverrideDecimalFormatting())
      io::Property::Marshall(*in.m_OverrideDecimalFormatting, out.OverrideDecimalFormatting());
    for (auto* dr : in.m_Requests) {
      CDM::DataRequestData data;
      out.DataRequest().push_back(io::DataRequests::factory(dr));
    }
  }
  //-----------------------------------------------------------------------------
  // class SEDataRequest
  void DataRequests::UnMarshall(const CDM::DataRequestData& in, SEDataRequest& out, std::default_random_engine* re)
  {
    io::Property::UnMarshall(static_cast<CDM::DecimalFormatData const&>(in), static_cast<SEDecimalFormat&>(out));
    out.m_Name = in.Name();
    if (in.Unit().present()) {
      out.m_RequestedUnit = in.Unit().get();
    }
  }
  void DataRequests::Marshall(const SEDataRequest& in, CDM::DataRequestData& out)
  {
    io::Property::Marshall(static_cast<SEDecimalFormat const&>(in), static_cast<CDM::DecimalFormatData&>(out));
    out.Name(in.m_Name);
    if (in.HasUnit()) {
      out.Unit(in.m_Unit->GetString());
    } else if (in.HasRequestedUnit()) {

      out.Unit(in.m_RequestedUnit);
    }
  }
  //-----------------------------------------------------------------------------
  // class SEPatientDataRequest
  void DataRequests::UnMarshall(const CDM::PatientDataRequestData& in, SEPatientDataRequest& out, std::default_random_engine* re)
  {
    io::DataRequests::UnMarshall(static_cast<CDM::DataRequestData const&>(in), static_cast<SEDataRequest&>(out));
  }
  void DataRequests::Marshall(const SEPatientDataRequest& in, CDM::PatientDataRequestData& out)
  {
    io::DataRequests::Marshall(static_cast<SEDataRequest const&>(in), static_cast<CDM::DataRequestData&>(out));
  }
  //-----------------------------------------------------------------------------
  // class SEPhysiologyDataRequest
  void DataRequests::UnMarshall(const CDM::PhysiologyDataRequestData& in, SEPhysiologyDataRequest& out, std::default_random_engine* re)
  {
    io::DataRequests::UnMarshall(static_cast<CDM::DataRequestData const&>(in), static_cast<SEDataRequest&>(out));
  }
  void DataRequests::Marshall(const SEPhysiologyDataRequest& in, CDM::PhysiologyDataRequestData& out)
  {
    io::DataRequests::Marshall(static_cast<SEDataRequest const&>(in), static_cast<CDM::DataRequestData&>(out));
  }
  //-----------------------------------------------------------------------------
  // class SEEnvironmentDataRequest
  void DataRequests::UnMarshall(const CDM::EnvironmentDataRequestData& in, SEEnvironmentDataRequest& out, std::default_random_engine* re)
  {
    io::DataRequests::UnMarshall(static_cast<CDM::DataRequestData const&>(in), static_cast<SEDataRequest&>(out));
  }
  void DataRequests::Marshall(const SEEnvironmentDataRequest& in, CDM::EnvironmentDataRequestData& out)
  {
    io::DataRequests::Marshall(static_cast<SEDataRequest const&>(in), static_cast<CDM::DataRequestData&>(out));
  }
  //-----------------------------------------------------------------------------
  // class SEEquipmentDataRequest
  void DataRequests::UnMarshall(const CDM::EquipmentDataRequestData& in, SEEquipmentDataRequest& out, std::default_random_engine* re)
  {
    io::DataRequests::UnMarshall(static_cast<CDM::DataRequestData const&>(in), static_cast<SEDataRequest&>(out));
    out.SetType(in.Type());
  }
  void DataRequests::Marshall(const SEEquipmentDataRequest& in, CDM::EquipmentDataRequestData& out)
  {
    io::DataRequests::Marshall(static_cast<SEDataRequest const&>(in), static_cast<CDM::DataRequestData&>(out));
    if (in.HasType()) {
      out.Type(in.m_Type);
    }
  }
  //-----------------------------------------------------------------------------
  // class SECompartmentDataRequest
  void DataRequests::UnMarshall(const CDM::CompartmentDataRequestData& in, SECompartmentDataRequest& out, std::default_random_engine* re)
  {
    io::DataRequests::UnMarshall(static_cast<CDM::DataRequestData const&>(in), static_cast<SEDataRequest&>(out));
    out.SetCompartment(in.Compartment());
  }
  void DataRequests::Marshall(const SECompartmentDataRequest& in, CDM::CompartmentDataRequestData& out)
  {
    io::DataRequests::Marshall(static_cast<SEDataRequest const&>(in), static_cast<CDM::DataRequestData&>(out));
    out.Compartment(in.m_Compartment);
  }
  //-----------------------------------------------------------------------------
  // class SECompartmentSubstanceDataRequest
  void DataRequests::UnMarshall(const CDM::CompartmentSubstanceDataRequestData& in, SESubstanceManager const& substances, SECompartmentSubstanceDataRequest& out, std::default_random_engine* re)
  {
    io::DataRequests::UnMarshall(static_cast<CDM::CompartmentDataRequestData const&>(in), static_cast<SECompartmentDataRequest&>(out));
    if (in.Substance().present()) {
      out.SetSubstance(substances.GetSubstance(in.Substance().get()));
    }
  }

  void DataRequests::Marshall(const SECompartmentSubstanceDataRequest& in, CDM::CompartmentSubstanceDataRequestData& out)
  {
    io::DataRequests::Marshall(static_cast<SECompartmentDataRequest const&>(in), static_cast<CDM::CompartmentDataRequestData&>(out));
    if (in.HasSubstance()) {
      out.Substance(in.m_Substance->GetName());
    }
  }
  //-----------------------------------------------------------------------------
  // class SEGasCompartmentDataRequest
  void DataRequests::UnMarshall(const CDM::GasCompartmentDataRequestData& in, SESubstanceManager const& substances, SEGasCompartmentDataRequest& out, std::default_random_engine* re)
  {
    io::DataRequests::UnMarshall(static_cast<CDM::CompartmentSubstanceDataRequestData const&>(in), substances, static_cast<SECompartmentSubstanceDataRequest&>(out));
  }
  void DataRequests::Marshall(const SEGasCompartmentDataRequest& in, CDM::GasCompartmentDataRequestData& out)
  {
    io::DataRequests::Marshall(static_cast<SECompartmentSubstanceDataRequest const&>(in), static_cast<CDM::CompartmentSubstanceDataRequestData&>(out));
  }
  //-----------------------------------------------------------------------------
  // class SELiquidCompartmentDataRequest0
  void DataRequests::UnMarshall(const CDM::LiquidCompartmentDataRequestData& in, SESubstanceManager const& substances, SELiquidCompartmentDataRequest& out, std::default_random_engine* re)
  {
    io::DataRequests::UnMarshall(static_cast<CDM::CompartmentSubstanceDataRequestData const&>(in), substances, static_cast<SECompartmentSubstanceDataRequest&>(out));
  }
  void DataRequests::Marshall(const SELiquidCompartmentDataRequest& in, CDM::LiquidCompartmentDataRequestData& out)
  {
    io::DataRequests::Marshall(static_cast<SECompartmentSubstanceDataRequest const&>(in), static_cast<CDM::CompartmentSubstanceDataRequestData&>(out));
  }
  //-----------------------------------------------------------------------------
  // class SEThermalCompartmentDataRequest
  void DataRequests::UnMarshall(const CDM::ThermalCompartmentDataRequestData& in, SEThermalCompartmentDataRequest& out, std::default_random_engine* re)
  {
    io::DataRequests::UnMarshall(static_cast<CDM::CompartmentDataRequestData const&>(in), static_cast<SECompartmentDataRequest&>(out));
  }
  void DataRequests::Marshall(const SEThermalCompartmentDataRequest& in, CDM::ThermalCompartmentDataRequestData& out)
  {
    io::DataRequests::Marshall(static_cast<SECompartmentDataRequest const&>(in), static_cast<CDM::CompartmentDataRequestData&>(out));
  }
  //-----------------------------------------------------------------------------
  // class SETissueCompartmentDataRequest
  void DataRequests::UnMarshall(const CDM::TissueCompartmentDataRequestData& in, SETissueCompartmentDataRequest& out, std::default_random_engine* re)
  {
    io::DataRequests::UnMarshall(static_cast<CDM::CompartmentDataRequestData const&>(in), static_cast<SECompartmentDataRequest&>(out));
  }
  void DataRequests::Marshall(const SETissueCompartmentDataRequest& in, CDM::TissueCompartmentDataRequestData& out)
  {
    io::DataRequests::Marshall(static_cast<SECompartmentDataRequest const&>(in), static_cast<CDM::CompartmentDataRequestData&>(out));
  }
  // class SESubstanceDataRequest
  void DataRequests::UnMarshall(const CDM::SubstanceDataRequestData& in, SESubstanceManager const& substances, SESubstanceDataRequest& out, std::default_random_engine* re)
  {
    io::DataRequests::UnMarshall(static_cast<CDM::DataRequestData const&>(in), static_cast<SEDataRequest&>(out));
    if (in.Compartment().present()) {
      out.SetCompartment(in.Compartment().get());
    }
    out.SetSubstance(substances.GetSubstance(in.Substance()));
  }
  void DataRequests::Marshall(const SESubstanceDataRequest& in, CDM::SubstanceDataRequestData& out)
  {
    io::DataRequests::Marshall(static_cast<SEDataRequest const&>(in), static_cast<CDM::DataRequestData&>(out));
    if (in.HasCompartment()) {
      out.Compartment(in.m_Compartment);
    }
    if (in.HasSubstance()) {
      out.Substance(in.m_Substance->GetName());
    }
  }
  //-----------------------------------------------------------------------------
  std::vector<std::unique_ptr<SEDataRequest>> DataRequests ::data_request_factory(const CDM::DataRequestManagerData& in, SESubstanceManager const& substances, const SEDecimalFormat* df)
  {
    std::vector<std::unique_ptr<SEDataRequest>> r_vec;
    for (auto request_data : in.DataRequest()) {
      r_vec.emplace_back(factory(&request_data, substances, df));
    }
    return r_vec;
  }

  //-----------------------------------------------------------------------------
  std::unique_ptr<SEDataRequest> DataRequests::factory(CDM::DataRequestData const* requestData, SESubstanceManager const& substances, const SEDecimalFormat* df)
  {
    CDM::DataRequestData const* drData = requestData;
    CDM::PhysiologyDataRequestData const* physSysData = dynamic_cast<const CDM::PhysiologyDataRequestData*>(drData);
    if (physSysData != nullptr) {
      auto sys = std::make_unique<SEPhysiologyDataRequest>(df);
      DataRequests::UnMarshall(*physSysData, *sys);
      return sys;
    }
    const CDM::GasCompartmentDataRequestData* gasData = dynamic_cast<const CDM::GasCompartmentDataRequestData*>(drData);
    if (gasData != nullptr) {
      auto Comp = std::make_unique<SEGasCompartmentDataRequest>(df);
      DataRequests::UnMarshall(*gasData, substances, *Comp);
      return Comp;
    }
    const CDM::LiquidCompartmentDataRequestData* liquidData = dynamic_cast<const CDM::LiquidCompartmentDataRequestData*>(drData);
    if (liquidData != nullptr) {
      auto Comp = std::make_unique<SELiquidCompartmentDataRequest>(df);
      DataRequests::UnMarshall(*liquidData, substances, *Comp);
      return Comp;
    }
    const CDM::ThermalCompartmentDataRequestData* thermData = dynamic_cast<const CDM::ThermalCompartmentDataRequestData*>(drData);
    if (thermData != nullptr) {
      auto Comp = std::make_unique<SEThermalCompartmentDataRequest>(df);
      DataRequests::UnMarshall(*thermData, *Comp);
      return Comp;
    }
    const CDM::TissueCompartmentDataRequestData* tissueData = dynamic_cast<const CDM::TissueCompartmentDataRequestData*>(drData);
    if (tissueData != nullptr) {
      auto Comp = std::make_unique<SETissueCompartmentDataRequest>(df);
      DataRequests::UnMarshall(*tissueData, *Comp);
      return Comp;
    }
    const CDM::PatientDataRequestData* patData = dynamic_cast<const CDM::PatientDataRequestData*>(drData);
    if (patData != nullptr) {
      auto sys = std::make_unique<SEPatientDataRequest>(df);
      DataRequests::UnMarshall(*patData, *sys);
      return sys;
    }
    const CDM::SubstanceDataRequestData* subData = dynamic_cast<const CDM::SubstanceDataRequestData*>(drData);
    if (subData != nullptr) {
      auto sub = std::make_unique<SESubstanceDataRequest>(df);
      DataRequests::UnMarshall(*subData, substances, *sub);
      return sub;
    }
    const CDM::EnvironmentDataRequestData* envData = dynamic_cast<const CDM::EnvironmentDataRequestData*>(drData);
    if (envData != nullptr) {
      auto env = std::make_unique<SEEnvironmentDataRequest>(df);
      DataRequests::UnMarshall(*envData, *env);
      return env;
    }
    const CDM::EquipmentDataRequestData* equipSysData = dynamic_cast<const CDM::EquipmentDataRequestData*>(drData);
    if (equipSysData != nullptr) {
      auto sys = std::make_unique<SEEquipmentDataRequest>(df);
      DataRequests::UnMarshall(*equipSysData, *sys);
      return sys;
    }

    if (substances.GetLogger() != nullptr)
      substances.GetLogger()->Error("Unsupported DataRequest Received", "SEDataRequest::newFromBind");
    return nullptr;
  }
  //-----------------------------------------------------------------------------
  std::unique_ptr<CDM::DataRequestData> DataRequests::factory(const SEDataRequest* in)
  {

    auto physSysData = dynamic_cast<const SEPhysiologyDataRequest*>(in);
    if (physSysData != nullptr) {
      auto sys = std::make_unique<CDM::PhysiologyDataRequestData>();
      DataRequests::Marshall(*physSysData, *sys);
      return sys;
    }
    auto gasData = dynamic_cast<const SEGasCompartmentDataRequest*>(in);
    if (gasData != nullptr) {
      auto Comp = std::make_unique<CDM::GasCompartmentDataRequestData>();
      DataRequests::Marshall(*gasData, *Comp);
      return Comp;
    }
    auto liquidData = dynamic_cast<const SELiquidCompartmentDataRequest*>(in);
    if (liquidData != nullptr) {
      auto Comp = std::make_unique<CDM::LiquidCompartmentDataRequestData>();
      DataRequests::Marshall(*liquidData, *Comp);
      return Comp;
    }
    auto thermData = dynamic_cast<const SEThermalCompartmentDataRequest*>(in);
    if (thermData != nullptr) {
      auto Comp = std::make_unique<CDM::ThermalCompartmentDataRequestData>();
      DataRequests::Marshall(*thermData, *Comp);
      return Comp;
    }
    auto tissueData = dynamic_cast<const SETissueCompartmentDataRequest*>(in);
    if (tissueData != nullptr) {
      auto Comp = std::make_unique<CDM::TissueCompartmentDataRequestData>();
      DataRequests::Marshall(*tissueData, *Comp);
      return Comp;
    }
    auto patData = dynamic_cast<const SEPatientDataRequest*>(in);
    if (patData != nullptr) {
      auto sys = std::make_unique<CDM::PatientDataRequestData>();
      DataRequests::Marshall(*patData, *sys);
      return sys;
    }
    auto subData = dynamic_cast<const SESubstanceDataRequest*>(in);
    if (subData != nullptr) {
      auto sub = std::make_unique<CDM::SubstanceDataRequestData>();
      DataRequests::Marshall(*subData, *sub);
      return sub;
    }
    auto envData = dynamic_cast<const SEEnvironmentDataRequest*>(in);
    if (envData != nullptr) {
      auto env = std::make_unique<CDM::EnvironmentDataRequestData>();
      DataRequests::Marshall(*envData, *env);
      return env;
    }
    auto equipSysData = dynamic_cast<const SEEquipmentDataRequest*>(in);
    if (equipSysData != nullptr) {
      auto sys = std::make_unique<CDM::EquipmentDataRequestData>();
      DataRequests::Marshall(*equipSysData, *sys);
      return sys;
    }

    throw CommonDataModelException("No Acceptable DataRequest Conversion Found");
  }
} // namespace io
} // namespace biogears