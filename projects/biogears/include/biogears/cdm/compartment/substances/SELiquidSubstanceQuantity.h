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
#include <biogears/cdm/compartment/substances/SESubstanceQuantity.h>
#include <biogears/cdm/substance/SESubstanceTransport.h>
#include <biogears/cdm/properties/SEScalarAmountPerVolume.h>

namespace biogears {
class SELiquidCompartment;

enum class BalanceLiquidBy {
  Mass,
  Concentration,
  Molarity,
  PartialPressure };

class SELiquidCompartment;
class BIOGEARS_API SELiquidSubstanceQuantity : public SESubstanceQuantity, public SELiquidTransportSubstance {
  friend class SELiquidCompartment;

protected:
  SELiquidSubstanceQuantity(SESubstance& sub, SELiquidCompartment& compartment);

public:
  virtual ~SELiquidSubstanceQuantity();

  virtual void Clear() override;
  virtual void Invalidate() override;

  virtual bool Load(const CDM::LiquidSubstanceQuantityData& in);
  virtual CDM::LiquidSubstanceQuantityData* Unload() override;

protected:
  virtual void Unload(CDM::LiquidSubstanceQuantityData& data);

public:
  virtual void SetToZero();
  virtual const SEScalar* GetScalar(const char* name) override;
  virtual const SEScalar* GetScalar(const std::string& name) override;

  virtual void Balance(BalanceLiquidBy e); // Balance Substance based on a specific property

  virtual bool HasConcentration() const;
  virtual SEScalarMassPerVolume& GetConcentration();
  virtual double GetConcentration(const MassPerVolumeUnit& unit) const;

  virtual bool HasMass() const;
  virtual SEScalarMass& GetMass();
  virtual double GetMass(const MassUnit& unit) const;

  virtual bool HasMassCleared() const;
  virtual SEScalarMass& GetMassCleared();
  virtual double GetMassCleared(const MassUnit& unit) const;

  virtual bool HasMassDeposited() const;
  virtual SEScalarMass& GetMassDeposited();
  virtual double GetMassDeposited(const MassUnit& unit) const;

  virtual bool HasMassExcreted() const;
  virtual SEScalarMass& GetMassExcreted();
  virtual double GetMassExcreted(const MassUnit& unit) const;

  virtual bool HasMolarity() const;
  virtual SEScalarAmountPerVolume& GetMolarity();
  virtual double GetMolarity(const AmountPerVolumeUnit& unit) const;

  virtual bool HasPartialPressure() const;
  virtual SEScalarPressure& GetPartialPressure();
  virtual double GetPartialPressure(const PressureUnit& unit) const;

  virtual bool HasSaturation() const;
  virtual SEScalarFraction& GetSaturation();
  virtual double GetSaturation() const;
  virtual void SetHemoglobins(SESubstance& Hb, SESubstance& HbO2, SESubstance& HbCO2, SESubstance& HbO2CO2, SESubstance& HbCO);

protected:
  virtual void AddChild(SELiquidSubstanceQuantity& subQ);

  virtual bool HasExtensive() const override { return HasMass(); }
  virtual SEScalarMass& GetExtensive() override  { return GetMass(); }

  virtual bool HasIntensive() const override  { return HasConcentration(); }
  virtual SEScalarMassPerVolume& GetIntensive() override  { return GetConcentration(); }

  SEScalarMassPerVolume* m_Concentration;
  SEScalarMass* m_Mass;
  SEScalarMass* m_MassCleared;
  SEScalarMass* m_MassDeposited;
  SEScalarMass* m_MassExcreted;
  SEScalarAmountPerVolume* m_Molarity;
  SEScalarPressure* m_PartialPressure;
  SEScalarFraction* m_Saturation;

  SELiquidCompartment& m_Compartment;
  std::vector<SELiquidSubstanceQuantity*> m_Children;

  bool m_isO2;
  bool m_isCO;
  bool m_isCO2;
  // Inorder to caluclate a hierarchical saturation, we need these substances
  SESubstance* m_Hb;
  SESubstance* m_HbO2;
  SESubstance* m_HbCO2;
  SESubstance* m_HbO2CO2;
  SESubstance* m_HbCO;
};
}