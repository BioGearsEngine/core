/**************************************************************************************
Copyright 2015 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/

#pragma once
#include <biogears/cdm/CommonDataModel.h>
#include <biogears/exports.h>

#include <biogears/cdm/patient/actions/SEPatientAction.h>
#include <biogears/schema/cdm/PatientActions.hxx>

namespace biogears {
class SEScalar0To1;
class SEScalar;
namespace io {
  class PatientActions;
}

constexpr double MAXIMUM_LEFT_ARM = .09;
constexpr double MAXIMUM_RIGHT_ARM = .09;
constexpr double MAXIMUM_TRUNK = .370; // Trunk + 1%
constexpr double MAXIMUM_LEFT_LEG = .180;
constexpr double MAXIMUM_RIGHT_LEG = .180;

class BIOGEARS_API SEBurnWound : public SEPatientAction {
  friend io::PatientActions;

public:
  SEBurnWound();
  virtual ~SEBurnWound() override;

  static constexpr const char* TypeTag() { return "SEBurnWound"; };
  const char* classname() const override { return TypeTag(); }

  virtual void Clear() override; // clear memory

  virtual bool IsValid() const override;
  virtual bool IsActive() const override;

  bool Load(const CDM::BurnWoundData& in);
  virtual CDM::BurnWoundData* Unload() const override;

  bool HasTotalBodySurfaceArea() const;
  double GetTotalBodySurfaceArea() const;
  void SetTotalBodySurfaceArea(double);

  bool HasDegreeOfBurn() const;
  CDM::enumBurnDegree::value GetDegreeOfBurn() const;
  void SetDegreeOfBurn(CDM::enumBurnDegree::value value);

  double GetBurnIntensity() const;

  bool HasCompartment() const;
  bool HasCompartment(const std::string compartment) const;
  const std::vector<std::string>& GetCompartments();
  const std::string GetCompartment(const std::string compartment) const;
  void AddCompartment(const char* name);
  void AddCompartment(const std::string& name);
  void RemoveCompartment(const std::string compartment);
  void RemoveCompartments();

  bool HasInflammation() const;
  void SetInflammation(bool activate);

  void ToString(std::ostream& str) const override;

  bool operator==(const SEBurnWound& rhs) const;
  bool operator!=(const SEBurnWound& rhs) const;

  std::vector<double> GetTBSACompartmentDistribution() const;

  double getLeftArmSA() const;
  double getRightArmSA() const;
  double getTrunkSA() const;
  double getLeftLegSA() const;
  double getRightLegSA() const;

  double getLeftArmBurnIntensity() const;
  double getRightArmBurnIntensity() const;
  double getTrunkBurnIntensity() const;
  double getLeftLegBurnIntensity() const;
  double getRightLegBurnIntensity() const;

protected:
  virtual void Unload(CDM::BurnWoundData& data) const;
  void calculateCompartmentDistribution();

private:
  bool m_Inflammation;
  double m_DegreeModifier;
  CDM::enumBurnDegree::value m_DegreeOfBurn;
  SEScalar0To1* m_TBSA;
  std::vector<SEScalar0To1> m_compartments;

  double m_BurnIntensity;
  std::vector<std::string> m_compartmentsAffected;
};
}
