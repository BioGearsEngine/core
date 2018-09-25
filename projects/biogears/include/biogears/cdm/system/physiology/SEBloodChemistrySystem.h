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

#include <biogears/cdm/system/SESystem.h>
#include <biogears/schema/biogears/BioGearsPhysiology.hxx>

namespace biogears {
class SEScalarMass;
class MassUnit;
class SEScalarMassPerVolume;
class MassPerVolumeUnit;
class SEScalarHeatCapacitancePerMass;
class HeatCapacitancePerMassUnit;
class SEScalarFraction;
class SEScalarVolume;
class VolumeUnit;
class SEScalarAmountPerVolume;
class AmountPerVolumeUnit;
class SEScalarPressure;
class PressureUnit;


/** @copydoc Physiology_BloodChemistrySystemData
  @nosubgrouping */
class BIOGEARS_API SEBloodChemistrySystem : public SESystem {
public:
  SEBloodChemistrySystem(Logger* logger);
  virtual ~SEBloodChemistrySystem();

  virtual void Clear(); /**< @copydoc DOXY_CDM_CLEAR */

  virtual const SEScalar* GetScalar(const std::string& name); /**< @copydoc DOXY_CDM_GET_SCALAR */

  /**  @name Serialization */ //@{
  virtual bool Load(const CDM::BloodChemistrySystemData& in); /**< @copydoc DOXY_CDM_LOAD */
  virtual CDM::BloodChemistrySystemData* Unload() const; /**< @copydoc DOXY_CDM_UNLOAD */
protected:
  virtual void Unload(CDM::BloodChemistrySystemData& data) const; /**< @copydoc DOXY_CDM_UNLOAD_TO */ //@}
public:
  /**  @name BloodDensity */ //@{ @copybrief Physiology_BloodChemistrySystemData_BloodDensity
  virtual bool HasBloodDensity() const; /**< @copydoc DOXY_CDM_HAS */
  virtual SEScalarMassPerVolume& GetBloodDensity(); /**< @copydoc DOXY_CDM_GET */
  virtual double GetBloodDensity(const MassPerVolumeUnit& unit) const; /**< @copydoc DOXY_CDM_GET_VALUE */ //@}

 
  /** @name ArterialBloodPH
  *  @brief @copybrief Physiology_BloodChemistrySystemData_ArterialBloodPH
  *  @{*/
  /// %Test if member has been allocated
  virtual bool HasArterialBloodPH() const;
  /// Get member class, allocate if nullptr
  virtual SEScalar& GetArterialBloodPH();
  virtual double GetArterialBloodPH() const;
  //@}

  /** @name VenousBloodPH
  *  @brief @copybrief Physiology_BloodChemistrySystemData_VenousBloodPH
  *  @{*/
  /// %Test if member has been allocated
  virtual bool HasVenousBloodPH() const;
  /// Get member class, allocate if nullptr
  virtual SEScalar& GetVenousBloodPH();
  virtual double GetVenousBloodPH() const;
  //@}

  /** @name BloodSpecificHeat
  *  @brief @copybrief Physiology_BloodChemistrySystemData_BloodSpecificHeat
  *  @{*/
  /// %Test if member has been allocated
  virtual bool HasBloodSpecificHeat() const;
  /// Get member class, allocate if nullptr
  virtual SEScalarHeatCapacitancePerMass& GetBloodSpecificHeat();
  virtual double GetBloodSpecificHeat(const HeatCapacitancePerMassUnit& unit) const;
  //@}

  /** @name BloodUreaNitrogenConcentration
  *  @brief @copybrief Physiology_BloodChemistrySystemData_BloodUreaNitrogenConcentration
  *  @{*/
  /// %Test if member has been allocated
  virtual bool HasBloodUreaNitrogenConcentration() const;
  /// Get member class, allocate if nullptr
  virtual SEScalarMassPerVolume& GetBloodUreaNitrogenConcentration();
  virtual double GetBloodUreaNitrogenConcentration(const MassPerVolumeUnit& unit) const;
  //@}

  /** @name CarbonDioxideSaturation
  *  @brief @copybrief Physiology_BloodChemistrySystemData_CarbonDioxideSaturation
  *  @{*/
  /// %Test if member has been allocated
  virtual bool HasCarbonDioxideSaturation() const;
  /// Get member class, allocate if nullptr
  virtual SEScalarFraction& GetCarbonDioxideSaturation();
  virtual double GetCarbonDioxideSaturation() const;
  //@}

  /** @name CarbonMonoxideSaturation
  *  @brief @copybrief Physiology_BloodChemistrySystemData_CarbonMonoxideSaturation
  *  @{*/
  /// %Test if member has been allocated
  virtual bool HasCarbonMonoxideSaturation() const;
  /// Get member class, allocate if nullptr
  virtual SEScalarFraction& GetCarbonMonoxideSaturation();
  virtual double GetCarbonMonoxideSaturation() const;
  //@}

  /** @name Hematocrit
  *  @brief @copybrief Physiology_BloodChemistrySystemData_Hematocrit
  *  @{*/
  /// %Test if member has been allocated
  virtual bool HasHematocrit() const;
  /// Get member class, allocate if nullptr
  virtual SEScalarFraction& GetHematocrit();
  virtual double GetHematocrit() const;
  //@}

  /** @name HemoglobinContent
  *  @brief @copybrief Physiology_BloodChemistrySystemData_HemoglobinContent
  *  @{*/
  /// %Test if member has been allocated
  virtual bool HasHemoglobinContent() const;
  /// Get member class, allocate if nullptr
  virtual SEScalarMass& GetHemoglobinContent();
  virtual double GetHemoglobinContent(const MassUnit& unit) const;
  //@}

  /** @name OxygenSaturation
   *  @brief @copybrief Physiology_BloodChemistrySystemData_OxygenSaturation
   *  @{*/
  /// %Test if member has been allocated
  virtual bool HasOxygenSaturation() const;
  /// Get member class, allocate if nullptr
  virtual SEScalarFraction& GetOxygenSaturation();
  virtual double GetOxygenSaturation() const;
  //@}

  /** @name Phosphate
  *  @brief @copybrief Physiology_BloodChemistrySystemData_Phosphate
  *  @{*/
  /// %Test if member has been allocated
  virtual bool HasPhosphate() const;
  /// Get member class, allocate if nullptr
  virtual SEScalarAmountPerVolume& GetPhosphate();
  virtual double GetPhosphate(const AmountPerVolumeUnit& unit) const;
  //@}

  /** @name PlasmaVolume
  *  @brief @copybrief Physiology_BloodChemistrySystemData_PlasmaVolume
  *  @{*/
  /// %Test if member has been allocated
  virtual bool HasPlasmaVolume() const;
  /// Get member class, allocate if nullptr
  virtual SEScalarVolume& GetPlasmaVolume();
  virtual double GetPlasmaVolume(const VolumeUnit& unit) const;
  //@}

  /** @name PulseOximetry
  *  @brief @copybrief Physiology_BloodChemistrySystemData_PulseOximetry
  *  @{*/
  /// %Test if member has been allocated
  virtual bool HasPulseOximetry() const;
  /// Get member class, allocate if nullptr
  virtual SEScalarFraction& GetPulseOximetry();
  virtual double GetPulseOximetry() const;
  //@}

  /** @name RedBloodCellAcetylcholinesteraseConcentration
  *  @brief @copybrief Physiology_BloodChemistrySystemData_RedBloodCellAcetylcholinesteraseConcentration
  *  @{*/
  /// %Test if member has been allocated
  virtual bool HasRedBloodCellAcetylcholinesterase() const;
  /// Get member class, allocate if nullptr
  virtual SEScalarAmountPerVolume& GetRedBloodCellAcetylcholinesterase();
  virtual double GetRedBloodCellAcetylcholinesterase(const AmountPerVolumeUnit& unit) const;
  //@}

  /** @name RedBloodCellCount
   *  @brief @copybrief Physiology_BloodChemistrySystemData_RedBloodCellCount
   *  @{*/
  /// %Test if member has been allocated
  virtual bool HasRedBloodCellCount() const;
  /// Get member class, allocate if nullptr
  virtual SEScalarAmountPerVolume& GetRedBloodCellCount();
  virtual double GetRedBloodCellCount(const AmountPerVolumeUnit& unit) const;
  //@}

  /** @name StrongIonDifference
  *  @brief @copybrief Physiology_BloodChemistrySystemData_StrongIonDifference
  *  @{*/
  /// %Test if member has been allocated
  virtual bool HasStrongIonDifference() const;
  /// Get member class, allocate if nullptr
  virtual SEScalarAmountPerVolume& GetStrongIonDifference();
  virtual double GetStrongIonDifference(const AmountPerVolumeUnit& unit) const;
  //@}

  /** @name ShuntFraction
   *  @brief @copybrief Physiology_BloodChemistrySystemData_ShuntFraction
   *  @{*/
  /// %Test if member has been allocated
  virtual bool HasShuntFraction() const;
  /// Get member class, allocate if nullptr
  virtual SEScalarFraction& GetShuntFraction();
  virtual double GetShuntFraction() const;
  //@}

  virtual bool HasTotalBilirubin() const;
  virtual SEScalarMassPerVolume& GetTotalBilirubin();
  virtual double GetTotalBilirubin(const MassPerVolumeUnit& unit) const;

  /** @name TotalProteinConcentration
  *  @brief @copybrief Physiology_BloodChemistrySystemData_TotalProteinConcentration
  *  @{*/
  /// %Test if member has been allocated
  virtual bool HasTotalProteinConcentration() const;
  /// Get member class, allocate if nullptr
  virtual SEScalarMassPerVolume& GetTotalProteinConcentration();
  virtual double GetTotalProteinConcentration(const MassPerVolumeUnit& unit) const;
  //@}

  /** @name VolumeFractionNeutralPhospholipidInPlasma
  *  @brief @copybrief Physiology_BloodChemistrySystemData_VolumeFractionNeutralPhospholipidInPlasma
  *  @{*/
  /// %Test if member has been allocated
  virtual bool HasVolumeFractionNeutralPhospholipidInPlasma() const;
  /// Get member class, allocate if nullptr
  virtual SEScalarFraction& GetVolumeFractionNeutralPhospholipidInPlasma();
  virtual double GetVolumeFractionNeutralPhospholipidInPlasma() const;
  //@}

  /** @name VolumeFractionNeutralLipidInPlasma
  *  @brief @copybrief Physiology_BloodChemistrySystemData_VolumeFractionNeutralLipidInPlasma
  *  @{*/
  /// %Test if member has been allocated
  virtual bool HasVolumeFractionNeutralLipidInPlasma() const;
  /// Get member class, allocate if nullptr
  virtual SEScalarFraction& GetVolumeFractionNeutralLipidInPlasma();
  virtual double GetVolumeFractionNeutralLipidInPlasma() const;
  //@}

  /** @name WhiteBloodCellCount
  *  @brief @copybrief Physiology_BloodChemistrySystemData_WhiteBloodCellCount
  *  @{*/
  /// %Test if member has been allocated
  virtual bool HasWhiteBloodCellCount() const;
  /// Get member class, allocate if nullptr
  virtual SEScalarAmountPerVolume& GetWhiteBloodCellCount();
  virtual double GetWhiteBloodCellCount(const AmountPerVolumeUnit& unit) const;
  //@}

  /** @name ArterialCarbonDioxidePressure
  *  @brief @copybrief Physiology_BloodChemistrySystemData_ArterialCarbonDioxidePressure
  *  @{*/
  /// %Test if member has been allocated
  virtual bool HasArterialCarbonDioxidePressure() const;
  /// Get member class, allocate if nullptr
  virtual SEScalarPressure& GetArterialCarbonDioxidePressure();
  virtual double GetArterialCarbonDioxidePressure(const PressureUnit& unit) const;
  //@}

  /** @name ArterialOxygenPressure
  *  @brief @copybrief Physiology_BloodChemistrySystemData_ArterialOxygenPressure
  *  @{*/
  /// %Test if member has been allocated
  virtual bool HasArterialOxygenPressure() const;
  /// Get member class, allocate if nullptr
  virtual SEScalarPressure& GetArterialOxygenPressure();
  virtual double GetArterialOxygenPressure(const PressureUnit& unit) const;
  //@}

  /** @name PulmonaryArterialCarbonDioxidePressure
  *  @brief @copybrief Physiology_BloodChemistrySystemData_PulmonaryArterialCarbonDioxidePressure
  *  @{*/
  /// %Test if member has been allocated
  virtual bool HasPulmonaryArterialCarbonDioxidePressure() const;
  /// Get member class, allocate if nullptr
  virtual SEScalarPressure& GetPulmonaryArterialCarbonDioxidePressure();
  virtual double GetPulmonaryArterialCarbonDioxidePressure(const PressureUnit& unit) const;
  //@}

  /** @name PulmonaryArterialOxygenPressure
  *  @brief @copybrief Physiology_BloodChemistrySystemData_PulmonaryArterialOxygenPressure
  *  @{*/
  /// %Test if member has been allocated
  virtual bool HasPulmonaryArterialOxygenPressure() const;
  /// Get member class, allocate if nullptr
  virtual SEScalarPressure& GetPulmonaryArterialOxygenPressure();
  virtual double GetPulmonaryArterialOxygenPressure(const PressureUnit& unit) const;
  //@}

  /** @name PulmonaryVenousCarbonDioxidePressure
  *  @brief @copybrief Physiology_BloodChemistrySystemData_PulmonaryVenousCarbonDioxidePressure
  *  @{*/
  /// %Test if member has been allocated
  virtual bool HasPulmonaryVenousCarbonDioxidePressure() const;
  /// Get member class, allocate if nullptr
  virtual SEScalarPressure& GetPulmonaryVenousCarbonDioxidePressure();
  virtual double GetPulmonaryVenousCarbonDioxidePressure(const PressureUnit& unit) const;
  //@}

  /** @name PulmonaryVenousOxygenPressure
  *  @brief @copybrief Physiology_BloodChemistrySystemData_PulmonaryVenousOxygenPressure
  *  @{*/
  /// %Test if member has been allocated
  virtual bool HasPulmonaryVenousOxygenPressure() const;
  /// Get member class, allocate if nullptr
  virtual SEScalarPressure& GetPulmonaryVenousOxygenPressure();
  virtual double GetPulmonaryVenousOxygenPressure(const PressureUnit& unit) const;
  //@}

  /** @name VenousCarbonDioxidePressure
  *  @brief @copybrief Physiology_BloodChemistrySystemData_VenousCarbonDioxidePressure
  *  @{*/
  /// %Test if member has been allocated
  virtual bool HasVenousCarbonDioxidePressure() const;
  /// Get member class, allocate if nullptr
  virtual SEScalarPressure& GetVenousCarbonDioxidePressure();
  virtual double GetVenousCarbonDioxidePressure(const PressureUnit& unit) const;
  //@}
  /** @name VenousOxygenPressure
  *  @brief @copybrief Physiology_BloodChemistrySystemData_VenousOxygenPressure
  *  @{*/
  /// %Test if member has been allocated
  virtual bool HasVenousOxygenPressure() const;
  /// Get member class, allocate if nullptr
  virtual SEScalarPressure& GetVenousOxygenPressure();
  virtual double GetVenousOxygenPressure(const PressureUnit& unit) const;
  //@}

protected:
  SEScalarMassPerVolume* m_BloodDensity;
  SEScalar* m_ArterialBloodPH;
  SEScalar* m_VenousBloodPH;
  SEScalarHeatCapacitancePerMass* m_BloodSpecificHeat;
  SEScalarMassPerVolume* m_BloodUreaNitrogenConcentration;
  SEScalarFraction* m_CarbonDioxideSaturation;
  SEScalarFraction* m_CarbonMonoxideSaturation;
  SEScalarFraction* m_Hematocrit;
  SEScalarMass* m_HemoglobinContent;
  SEScalarFraction* m_OxygenSaturation;
  SEScalarAmountPerVolume* m_Phosphate;
  SEScalarVolume* m_PlasmaVolume;
  SEScalarFraction* m_PulseOximetry;
  SEScalarAmountPerVolume* m_RedBloodCellAcetylcholinesterase;
  SEScalarAmountPerVolume* m_RedBloodCellCount;
  SEScalarFraction* m_ShuntFraction;
  SEScalarAmountPerVolume* m_StrongIonDifference;
  SEScalarMassPerVolume* m_TotalBilirubin;
  SEScalarMassPerVolume* m_TotalProteinConcentration;
  SEScalarFraction* m_VolumeFractionNeutralPhospholipidInPlasma;
  SEScalarFraction* m_VolumeFractionNeutralLipidInPlasma;
  SEScalarAmountPerVolume* m_WhiteBloodCellCount;

  SEScalarPressure* m_ArterialCarbonDioxidePressure;
  SEScalarPressure* m_ArterialOxygenPressure;
  SEScalarPressure* m_PulmonaryArterialCarbonDioxidePressure;
  SEScalarPressure* m_PulmonaryArterialOxygenPressure;
  SEScalarPressure* m_PulmonaryVenousCarbonDioxidePressure;
  SEScalarPressure* m_PulmonaryVenousOxygenPressure;
  SEScalarPressure* m_VenousCarbonDioxidePressure;
  SEScalarPressure* m_VenousOxygenPressure;
};
}