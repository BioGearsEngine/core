/**************************************************************************************
Copyright 2015 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/

#pragma once
#include <biogears/cdm/CommonDataModel.h>
#include <biogears/exports.h>

#include <biogears/cdm/patient/actions/SEPatientAction.h>
#include <biogears/cdm/properties/SEScalarFlowResistance.h>
#include <biogears/schema/cdm/PatientActions.hxx>

namespace biogears {
class SEScalarVolumePerTime;
namespace io {
  class PatientActions;
}
class BIOGEARS_API SEHemorrhage : public SEPatientAction {
  friend io::PatientActions;

  //! \class SEHemorrhage
  //! \brief Patient Action for submitting hemorrhage events
  //!
  //! Two major fields Compartment and InitialRate
  //! SEScalarVolumePerTime InitialRate
  //! std::string Compartment "Map Key for referring to the Hemorrhage in the future"
public:
  SEHemorrhage();
  virtual ~SEHemorrhage() override;

  static constexpr const char* TypeTag() { return "SEHemorrhage"; };
  const char* classname() const override { return TypeTag(); }

  virtual void Clear() override; //clear memory

  virtual bool IsValid() const override;
  virtual bool IsActive() const override;

  virtual bool Load(const CDM::HemorrhageData& in);
  virtual CDM::HemorrhageData* Unload() const override;

  virtual const char* GetCompartment_cStr() const;
  virtual std::string GetCompartment() const;
  virtual bool HasCompartment() const;
  virtual void SetCompartment(const char* name);
  virtual void SetCompartment(const std::string& name);
  virtual void InvalidateCompartment();

  virtual bool HasMCIS() const;
  virtual std::vector<unsigned int>& GetMCIS() { return m_MCIS; }
  virtual std::vector<unsigned int> const & GetMCIS() const { return m_MCIS; };
  virtual void SetMCIS();

  virtual bool HasInitialRate() const;
  virtual SEScalarVolumePerTime& GetInitialRate();
  virtual SEScalarVolumePerTime const& GetInitialRate() const;

  virtual bool HasBleedResistance() const;
  virtual SEScalarFlowResistance& GetBleedResistance();
  virtual SEScalarFlowResistance const& GetBleedResistance() const;

  virtual void ToString(std::ostream& str) const override;

  bool operator==( const SEHemorrhage& rhs) const;
  bool operator!=( const SEHemorrhage& rhs) const;

protected:
  virtual void Unload(CDM::HemorrhageData& data) const;

protected:
  std::string m_Compartment;
  std::vector<unsigned int> m_MCIS;
  SEScalarVolumePerTime* m_InitialRate;
  SEScalarFlowResistance* m_BleedResistance;
  std::map<std::string, std::vector<unsigned int>> m_OrganMap;
};
}