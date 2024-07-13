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
#include <biogears/exports.h>

#include <biogears/cdm/CommonDataModel.h>
#include <biogears/schema/cdm/Substance.hxx>
#include <biogears/cdm/enums/SESubstanceEnums.h>

CDM_BIND_DECL(SubstancePhysicochemicalData)

namespace biogears {
  class SEScalar;
  class SEScalarFraction;
namespace io {
  class Substance;
}
class BIOGEARS_API SESubstancePhysicochemical : public Loggable {
  friend io::Substance;

public:
  SESubstancePhysicochemical(Logger* logger);
  virtual ~SESubstancePhysicochemical();

  virtual void Clear();
  virtual bool IsValid() const;

  virtual const SEScalar* GetScalar(const char* name);
  virtual const SEScalar* GetScalar(const std::string& name);

  virtual bool Load(const CDM::SubstancePhysicochemicalData& in);
  virtual CDM::SubstancePhysicochemicalData* Unload() const;

  virtual bool HasPrimaryPKA() const;
  virtual SEScalar& GetPrimaryPKA();
  virtual double GetPrimaryPKA() const;

  virtual bool HasSecondaryPKA() const;
  virtual SEScalar& GetSecondaryPKA();
  virtual double GetSecondaryPKA() const;

  virtual SESubstanceBindingProtein GetBindingProtein() const;
  virtual void SetBindingProtein(SESubstanceBindingProtein state);
  virtual bool HasBindingProtein() const;
  virtual void InvalidateBindingProtein();

  virtual bool HasBloodPlasmaRatio() const;
  virtual SEScalar& GetBloodPlasmaRatio();
  virtual double GetBloodPlasmaRatio() const;

  virtual bool HasFractionUnboundInPlasma() const;
  virtual SEScalarFraction& GetFractionUnboundInPlasma();
  virtual double GetFractionUnboundInPlasma() const;

  virtual SESubstanceIonicState GetIonicState() const;
  virtual void SetIonicState(SESubstanceIonicState state);
  virtual bool HasIonicState() const;
  virtual void InvalidateIonicState();

  virtual bool HasLogP() const;
  virtual SEScalar& GetLogP();
  virtual double GetLogP() const;

  virtual bool HasHydrogenBondCount() const;
  virtual SEScalar& GetHydrogenBondCount();
  virtual double GetHydrogenBondCount() const;

  virtual bool HasPolarSurfaceArea() const;
  virtual SEScalar& GetPolarSurfaceArea();
  virtual double GetPolarSurfaceArea() const;       
  
  bool operator==( const SESubstancePhysicochemical& rhs) const;
  bool operator!=( const SESubstancePhysicochemical& rhs) const;

protected:
  virtual void Unload(CDM::SubstancePhysicochemicalData& data) const;

protected:
  std::vector<SEScalar*> m_AcidDissociationConstants;
  SESubstanceBindingProtein m_BindingProtein;
  SEScalar* m_BloodPlasmaRatio;
  SEScalarFraction* m_FractionUnboundInPlasma;
  SESubstanceIonicState m_IonicState;
  SEScalar* m_LogP;
  SEScalar* m_HydrogenBondCount;
  SEScalar* m_PolarSurfaceArea;
};
}