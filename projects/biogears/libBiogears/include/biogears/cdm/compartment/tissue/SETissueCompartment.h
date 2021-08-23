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
#include <vector>

#include <biogears/exports.h>

#include <biogears/cdm/CommonDataModel.h>
#include <biogears/cdm/compartment/SECompartment.h>
#include <biogears/schema/cdm/Compartment.hxx>

#include <vector>
#include <map>
#include <string>

namespace biogears {
class SEScalar0To1;
class SESubstance;
class SESubstanceManager;
class SELiquidCompartment;
class SEScalarMassPerMass;
class MassPerMassUnit;
class SEScalarElectricPotential;
class ElectricPotentialUnit;
class SEScalarMass;
class MassUnit;
class SECompartmentManager;

namespace io {
  class Compartment;
}
class BIOGEARS_API SETissueCompartment : public SECompartment {
  friend class SECompartmentManager;
  friend io::Compartment;

protected:
  SETissueCompartment(const char* name, Logger* logger);
  SETissueCompartment(const std::string& name, Logger* logger);

public:
  virtual ~SETissueCompartment();

  virtual void Clear() override;

  virtual bool Load(const CDM::TissueCompartmentData& in, SESubstanceManager& subMgr, SECircuitManager* circuits = nullptr);
  virtual CDM::TissueCompartmentData* Unload() override;

protected:
  virtual void Unload(CDM::TissueCompartmentData& data);

public:
  virtual const SEScalar* GetScalar(const char* name) override;
  virtual const SEScalar* GetScalar(const std::string& name) override;

  virtual bool HasChildren() const override { return !m_Children.empty(); } // Children on tissue is not supported at this time
  virtual const std::vector<SETissueCompartment*>& GetChildren() { return m_Children; } // Children on tissue is not supported at this time
  virtual const std::vector<SETissueCompartment*>& GetLeaves() { return m_Leaves; } // Children on tissue is not supported at this time

  virtual void StateChange() override;

  virtual bool HasAcidicPhospohlipidConcentration() const;
  virtual SEScalarMassPerMass& GetAcidicPhospohlipidConcentration();
  virtual double GetAcidicPhospohlipidConcentration(const MassPerMassUnit& unit) const;

  virtual bool HasMatrixVolume() const;
  virtual SEScalarVolume& GetMatrixVolume();
  virtual double GetMatrixVolume(const VolumeUnit& unit) const;

  virtual bool HasMembranePotential() const;
  virtual SEScalarElectricPotential& GetMembranePotential();
  virtual double GetMembranePotential(const ElectricPotentialUnit& unit) const;

  virtual bool HasNeutralLipidsVolumeFraction() const;
  virtual SEScalarFraction& GetNeutralLipidsVolumeFraction();
  virtual double GetNeutralLipidsVolumeFraction() const;

  virtual bool HasNeutralPhospholipidsVolumeFraction() const;
  virtual SEScalarFraction& GetNeutralPhospholipidsVolumeFraction();
  virtual double GetNeutralPhospholipidsVolumeFraction() const;

  virtual bool HasReflectionCoefficient() const;
  virtual SEScalar0To1& GetReflectionCoefficient();
  virtual double GetReflectionCoefficient() const;

  virtual bool HasTissueToPlasmaAlbuminRatio() const;
  virtual SEScalar& GetTissueToPlasmaAlbuminRatio();
  virtual double GetTissueToPlasmaAlbuminRatio() const;

  virtual bool HasTissueToPlasmaAlphaAcidGlycoproteinRatio() const;
  virtual SEScalar& GetTissueToPlasmaAlphaAcidGlycoproteinRatio();
  virtual double GetTissueToPlasmaAlphaAcidGlycoproteinRatio() const;

  virtual bool HasTissueToPlasmaLipoproteinRatio() const;
  virtual SEScalar& GetTissueToPlasmaLipoproteinRatio();
  virtual double GetTissueToPlasmaLipoproteinRatio() const;

  virtual bool HasTotalMass() const;
  virtual SEScalarMass& GetTotalMass();
  virtual double GetTotalMass(const MassUnit& unit) const;

protected:
  SEScalarMassPerMass* m_AcidicPhospohlipidConcentration;
  SEScalarVolume* m_MatrixVolume;
  SEScalarElectricPotential* m_MembranePotential;
  SEScalarFraction* m_NeutralLipidsVolumeFraction;
  SEScalarFraction* m_NeutralPhospholipidsVolumeFraction;
  SEScalar0To1* m_ReflectionCoefficient;
  SEScalar* m_TissueToPlasmaAlbuminRatio;
  SEScalar* m_TissueToPlasmaAlphaAcidGlycoproteinRatio;
  SEScalar* m_TissueToPlasmaLipoproteinRatio;
  SEScalarMass* m_TotalMass;

  std::vector<SETissueCompartment*> m_Children;
  std::vector<SETissueCompartment*> m_Leaves;
};
}                                     //namespac biogears

namespace std {
  BG_EXT template class BIOGEARS_API vector<biogears::SETissueCompartment*> ;
  BG_EXT template class BIOGEARS_API map<string, biogears::SETissueCompartment*> ;
}