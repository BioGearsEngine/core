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

/**
 * 
 */
package mil.tatrc.physiology.datamodel.properties;

import mil.tatrc.physiology.utilities.UnitConverter;

public class CommonUnits
{
	
	/**
   * Enumeration of commonly used units for this type
   * Units are not limited to this enum.
   * Any method that accepts this enum will have another method
   * that takes any valid standard unit abbreviation string 
   * in place of method that take this enum.
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public enum AmountUnit
  {
    ct("ct"),
    mol("mol");
    private String unit;
    private AmountUnit(String unit)
    {
      this.unit=unit;
    }
    public String toString(){return this.unit;}
    public static boolean contains(String unit)
    {
      for(AmountUnit v : AmountUnit.values())
      {
        if(v.toString().equals(unit))
          return true;
      }
      return false;
    }
    /**
     * Test if unit string provided is a
     * valid unit abbreviation for this quantity
     * Does not need to be an enumerated value.
     * Can be any string of standard unit 
     * abbreviations that represent this quantity
     * Unit Abbreviation Standards: http://www.bipm.org/en/si/
     */
    public static boolean validUnit(String unit)
    {
      if(unit == null || unit.isEmpty())
        return false;
      if(AmountUnit.contains(unit))
        return true;
      return UnitConverter.isOfQuantityType("ct", unit);
    }
  }
  
  /**
   * Enumeration of commonly used units for this type
   * Units are not limited to this enum.
   * Any method that accepts this enum will have another method
   * that takes any valid standard unit abbreviation string 
   * in place of method that take this enum.
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public enum AmountPerMassUnit
  {
    ct_Per_g("ct/g"),
    ct_Per_ug("ct/ug");
    private String unit;
    private AmountPerMassUnit(String unit)
    {
      this.unit=unit;
    }
    public String toString(){return this.unit;}
    public static boolean contains(String unit)
    {
      for(AmountPerMassUnit v : AmountPerMassUnit.values())
      {
        if(v.toString().equals(unit))
          return true;
      }
      return false;
    }
    /**
     * Test if unit string provided is a
     * valid unit abbreviation for this quantity
     * Does not need to be an enumerated value.
     * Can be any string of standard unit 
     * abbreviations that represent this quantity
     * Unit Abbreviation Standards: http://www.bipm.org/en/si/
     */
    public static boolean validUnit(String unit)
    {
      if(unit == null || unit.isEmpty())
        return false;
      if(AmountPerMassUnit.contains(unit))
        return true;
      return UnitConverter.isOfQuantityType("ct/g", unit);
    }
  }
  
  /**
   * Enumeration of commonly used units for this type
   * Units are not limited to this enum.
   * Any method that accepts this enum will have another method
   * that takes any valid standard unit abbreviation string 
   * in place of method that take this enum.
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public enum AmountPerTimeUnit 
  {
  mol_Per_day("mol/day"),
  mol_Per_s("mol/s"),
  pmol_Per_min("pmol/min");
    
    private String unit;
    private AmountPerTimeUnit(String unit)
    {
      this.unit=unit;
    }
    public String toString(){return this.unit;}
    public static boolean contains(String unit)
    {
      for(AmountPerTimeUnit v : AmountPerTimeUnit.values())
      {
        if(v.toString().equals(unit))
          return true;
      }
      return false;
    }
    /**
     * Test if unit string provided is a
     * valid unit abbreviation for this quantity
     * Does not need to be an enumerated value.
     * Can be any string of standard unit 
     * abbreviations that represent this quantity
     * Unit Abbreviation Standards: http://www.bipm.org/en/si/
     */
    public static boolean validUnit(String unit)
    {
      if(unit == null || unit.isEmpty())
        return false;
      if(AmountPerTimeUnit.contains(unit))
        return true;
      return UnitConverter.isOfQuantityType("mol/day", unit);
    }
  }
  
  /**
   * Enumeration of commonly used units for this type
   * Units are not limited to this enum.
   * Any method that accepts this enum will have another method
   * that takes any valid standard unit abbreviation string 
   * in place of method that take this enum.
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public enum AmountPerVolumeUnit
  {
    ct_Per_L("ct/L"),
    mol_Per_L("mol/L"),
    mol_Per_mL("mol/mL");
    private String unit;
    private AmountPerVolumeUnit(String unit)
    {
      this.unit=unit;
    }
    public String toString(){return this.unit;}
    public static boolean contains(String unit)
    {
      for(AmountPerVolumeUnit v : AmountPerVolumeUnit.values())
      {
        if(v.toString().equals(unit))
          return true;
      }
      return false;
    }
    /**
     * Test if unit string provided is a
     * valid unit abbreviation for this quantity
     * Does not need to be an enumerated value.
     * Can be any string of standard unit 
     * abbreviations that represent this quantity
     * Unit Abbreviation Standards: http://www.bipm.org/en/si/
     */
    public static boolean validUnit(String unit)
    {
      if(unit == null || unit.isEmpty())
        return false;
      if(AmountPerVolumeUnit.contains(unit))
        return true;
      return UnitConverter.isOfQuantityType("ct/L", unit);
    }
  }
  
  /**
   * Enumeration of commonly used units for this type
   * Units are not limited to this enum.
   * Any method that accepts this enum will have another method
   * that takes any valid standard unit abbreviation string 
   * in place of method that take this enum.
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public enum AreaUnit 
  {
    cm2("cm^2"),
    m2("m^2");
    
    private String unit;
    private AreaUnit(String unit)
    {
      this.unit=unit;
    }
    public String toString(){return this.unit;}
    public static boolean contains(String unit)
    {
      for(AreaUnit v : AreaUnit.values())
      {
        if(v.toString().equals(unit))
          return true;
      }
      return false;
    }
    /**
     * Test if unit string provided is a
     * valid unit abbreviation for this quantity
     * Does not need to be an enumerated value.
     * Can be any string of standard unit 
     * abbreviations that represent this quantity
     * Unit Abbreviation Standards: http://www.bipm.org/en/si/
     */
    public static boolean validUnit(String unit)
    {
      if(unit == null || unit.isEmpty())
        return false;
      if(AreaUnit.contains(unit))
        return true;
      return UnitConverter.isOfQuantityType("cm^2", unit);
    }
  }
    
  /**
   * Enumeration of commonly used units for this type
   * Units are not limited to this enum.
   * Any method that accepts this enum will have another method
   * that takes any valid standard unit abbreviation string 
   * in place of method that take this enum.
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public enum ElectricResistanceUnit 
  {
  	Ohm("ohm");
    private String unit;
    private ElectricResistanceUnit(String unit)
    {
      this.unit=unit;
    }
    public String toString(){return this.unit;}
    public static boolean contains(String unit)
    {
      for(ElectricResistanceUnit v : ElectricResistanceUnit.values())
      {
        if(v.toString().equals(unit))
          return true;
      }
      return false;
    }
    /**
     * Test if unit string provided is a
     * valid unit abbreviation for this quantity
     * Does not need to be an enumerated value.
     * Can be any string of standard unit 
     * abbreviations that represent this quantity
     * Unit Abbreviation Standards: http://www.bipm.org/en/si/
     */
    public static boolean validUnit(String unit)
    {
      if(unit == null || unit.isEmpty())
        return false;
      if(ElectricResistanceUnit.contains(unit))
        return true;
      return UnitConverter.isOfQuantityType("ohm", unit);
    }
  }
  
  
  /**
   * Enumeration of commonly used units for this type
   * Units are not limited to this enum.
   * Any method that accepts this enum will have another method
   * that takes any valid standard unit abbreviation string 
   * in place of method that take this enum.
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public enum ElectricPotentialUnit 
  {
  	V("V"),
  	mV("mV");
    private String unit;
    private ElectricPotentialUnit(String unit)
    {
      this.unit=unit;
    }
    public String toString(){return this.unit;}
    public static boolean contains(String unit)
    {
      for(ElectricPotentialUnit v : ElectricPotentialUnit.values())
      {
        if(v.toString().equals(unit))
          return true;
      }
      return false;
    }
    /**
     * Test if unit string provided is a
     * valid unit abbreviation for this quantity
     * Does not need to be an enumerated value.
     * Can be any string of standard unit 
     * abbreviations that represent this quantity
     * Unit Abbreviation Standards: http://www.bipm.org/en/si/
     */
    public static boolean validUnit(String unit)
    {
      if(unit == null || unit.isEmpty())
        return false;
      if(ElectricPotentialUnit.contains(unit))
        return true;
      return UnitConverter.isOfQuantityType("V", unit);
    }
  }
  
  
  /**
   * Enumeration of commonly used units for this type
   * Units are not limited to this enum.
   * Any method that accepts this enum will have another method
   * that takes any valid standard unit abbreviation string 
   * in place of method that take this enum.
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public enum EnergyUnit 
  {
  	Inverse_Pa("J"),
  	Inverse_mmHg("mJ");
    private String unit;
    private EnergyUnit(String unit)
    {
      this.unit=unit;
    }
    public String toString(){return this.unit;}
    public static boolean contains(String unit)
    {
      for(EnergyUnit v : EnergyUnit.values())
      {
        if(v.toString().equals(unit))
          return true;
      }
      return false;
    }
    /**
     * Test if unit string provided is a
     * valid unit abbreviation for this quantity
     * Does not need to be an enumerated value.
     * Can be any string of standard unit 
     * abbreviations that represent this quantity
     * Unit Abbreviation Standards: http://www.bipm.org/en/si/
     */
    public static boolean validUnit(String unit)
    {
      if(unit == null || unit.isEmpty())
        return false;
      if(EnergyUnit.contains(unit))
        return true;
      return UnitConverter.isOfQuantityType("J", unit);
    }
  }
  
    
  /**
   * Enumeration of commonly used units for this type
   * Units are not limited to this enum.
   * Any method that accepts this enum will have another method
   * that takes any valid standard unit abbreviation string 
   * in place of method that take this enum.
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public enum FlowComplianceUnit 
  {
  	L_Per_cmH2O("L/cmH2O"),
  	mL_Per_mmHg("mL/mmHg"),
  	m3_Per_Pa("m^3/Pa");
    private String unit;
    private FlowComplianceUnit(String unit)
    {
      this.unit=unit;
    }
    public String toString(){return this.unit;}
    public static boolean contains(String unit)
    {
      for(FlowComplianceUnit v : FlowComplianceUnit.values())
      {
        if(v.toString().equals(unit))
          return true;
      }
      return false;
    }
    /**
     * Test if unit string provided is a
     * valid unit abbreviation for this quantity
     * Does not need to be an enumerated value.
     * Can be any string of standard unit 
     * abbreviations that represent this quantity
     * Unit Abbreviation Standards: http://www.bipm.org/en/si/
     */
    public static boolean validUnit(String unit)
    {
      if(unit == null || unit.isEmpty())
        return false;
      if(FlowComplianceUnit.contains(unit))
        return true;
      return UnitConverter.isOfQuantityType("L/cmH2O", unit);
    }
  }
  
  /**
   * Enumeration of commonly used units for this type
   * Units are not limited to this enum.
   * Any method that accepts this enum will have another method
   * that takes any valid standard unit abbreviation string 
   * in place of method that take this enum.
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public enum FlowElastanceUnit 
  {
    cmH2O_Per_L("cmH2O/L"),
    mmHg_Per_mL("mmHg/mL"),
    Pa_Per_m3("Pa/m^3");
    private String unit;
    private FlowElastanceUnit(String unit)
    {
      this.unit=unit;
    }
    public String toString(){return this.unit;}
    public static boolean contains(String unit)
    {
      for(FlowElastanceUnit v : FlowElastanceUnit.values())
      {
        if(v.toString().equals(unit))
          return true;
      }
      return false;
    }
    /**
     * Test if unit string provided is a
     * valid unit abbreviation for this quantity
     * Does not need to be an enumerated value.
     * Can be any string of standard unit 
     * abbreviations that represent this quantity
     * Unit Abbreviation Standards: http://www.bipm.org/en/si/
     */
    public static boolean validUnit(String unit)
    {
      if(unit == null || unit.isEmpty())
        return false;
      if(FlowElastanceUnit.contains(unit))
        return true;
      return UnitConverter.isOfQuantityType("cmH2O/L", unit);
    }
  }
  
  /**
   * Enumeration of commonly used units for this type
   * Units are not limited to this enum.
   * Any method that accepts this enum will have another method
   * that takes any valid standard unit abbreviation string 
   * in place of method that take this enum.
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public enum FlowInertanceUnit
  {
  	mmHg_s_Per_mL_Per_s("mmHg s^2/mL"),
    Pa_s2_Per_m3("Pa s^2/m^3");
    private String unit;
    private FlowInertanceUnit(String unit)
    {
      this.unit=unit;
    }
    public String toString(){return this.unit;}
    public static boolean contains(String unit)
    {
      for(FlowInertanceUnit v : FlowInertanceUnit.values())
      {
        if(v.toString().equals(unit))
          return true;
      }
      return false;
    }
    /**
     * Test if unit string provided is a
     * valid unit abbreviation for this quantity
     * Does not need to be an enumerated value.
     * Can be any string of standard unit 
     * abbreviations that represent this quantity
     * Unit Abbreviation Standards: http://www.bipm.org/en/si/
     */
    public static boolean validUnit(String unit)
    {
      if(unit == null || unit.isEmpty())
        return false;
      if(FlowInertanceUnit.contains(unit))
        return true;
      return UnitConverter.isOfQuantityType("mmHg s^2/mL", unit);
    }
  }
  
  /**
   * Enumeration of commonly used units for this type
   * Units are not limited to this enum.
   * Any method that accepts this enum will have another method
   * that takes any valid standard unit abbreviation string 
   * in place of method that take this enum.
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public enum FlowResistanceUnit 
  {
  	cmH2O_Per_L_s("cmH2O s/L"),
  	mmHg_Per_mL_Per_s("mmHg s/mL"),
  	mmHg_Per_mL_Per_min("mmHg min/mL"),
  	Pa_s_Per_m3("Pa s/m^3");
    private String unit;
    private FlowResistanceUnit(String unit)
    {
      this.unit=unit;
    }
    public String toString(){return this.unit;}
    public static boolean contains(String unit)
    {
      for(FlowResistanceUnit v : FlowResistanceUnit.values())
      {
        if(v.toString().equals(unit))
          return true;
      }
      return false;
    }
    /**
     * Test if unit string provided is a
     * valid unit abbreviation for this quantity
     * Does not need to be an enumerated value.
     * Can be any string of standard unit 
     * abbreviations that represent this quantity
     * Unit Abbreviation Standards: http://www.bipm.org/en/si/
     */
    public static boolean validUnit(String unit)
    {
      if(unit == null || unit.isEmpty())
        return false;
      if(FlowResistanceUnit.contains(unit))
        return true;
      return UnitConverter.isOfQuantityType("cmH2O s/L", unit);
    }
  }
  
  /**
   * Enumeration of commonly used units for this type
   * Units are not limited to this enum.
   * Any method that accepts this enum will have another method
   * that takes any valid standard unit abbreviation string 
   * in place of method that take this enum.
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public enum ForceUnit 
  {
  	N("N"),
  	lbf("lbf"),
  	dyn("dyn");
    private String unit;
    private ForceUnit(String unit)
    {
      this.unit=unit;
    }
    public String toString(){return this.unit;}
    public static boolean contains(String unit)
    {
      for(ForceUnit v : ForceUnit.values())
      {
        if(v.toString().equals(unit))
          return true;
      }
      return false;
    }
    /**
     * Test if unit string provided is a
     * valid unit abbreviation for this quantity
     * Does not need to be an enumerated value.
     * Can be any string of standard unit 
     * abbreviations that represent this quantity
     * Unit Abbreviation Standards: http://www.bipm.org/en/si/
     */
    public static boolean validUnit(String unit)
    {
      if(unit == null || unit.isEmpty())
        return false;
      if(ForceUnit.contains(unit))
        return true;
      return UnitConverter.isOfQuantityType("N", unit);
    }
  }
 
  /**
   * Enumeration of commonly used units for this type
   * Units are not limited to this enum.
   * Any method that accepts this enum will have another method
   * that takes any valid standard unit abbreviation string 
   * in place of method that take this enum.
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public enum FrequencyUnit
  {
    Hz("Hz"),
    Per_s("1/s"),
    Per_min("1/min");
    private String unit;
    private FrequencyUnit(String unit)
    {
      this.unit=unit;
    }
    public String toString(){return this.unit;}
    public static boolean contains(String unit)
    {
      for(FrequencyUnit v : FrequencyUnit.values())
      {
        if(v.toString().equals(unit))
          return true;
      }
      return false;
    }
    /**
     * Test if unit string provided is a
     * valid unit abbreviation for this quantity
     * Does not need to be an enumerated value.
     * Can be any string of standard unit 
     * abbreviations that represent this quantity
     * Unit Abbreviation Standards: http://www.bipm.org/en/si/
     */
    public static boolean validUnit(String unit)
    {
      if(unit == null || unit.isEmpty())
        return false;
      if(FrequencyUnit.contains(unit))
        return true;
      return UnitConverter.isOfQuantityType("Hz", unit);
    }
  }
  
  /**
   * Enumeration of commonly used units for this type
   * Units are not limited to this enum.
   * Any method that accepts this enum will have another method
   * that takes any valid standard unit abbreviation string 
   * in place of method that take this enum.
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public enum HeatCapacitancePerMass
  {
	J_Per_K_kg("J/K kg"),
	kJ_Per_K_kg("kJ/K kg"),
	kcal_Per_K_kg("kcal/K kg"),
	kcal_Per_C_kg("kcal/degC kg");
    private String unit;
    private HeatCapacitancePerMass(String unit)
    {
      this.unit=unit;
    }
    public String toString(){return this.unit;}
    public static boolean contains(String unit)
    {
      for(HeatCapacitancePerMass v : HeatCapacitancePerMass.values())
      {
        if(v.toString().equals(unit))
          return true;
      }
      return false;
    }
    /**
     * Test if unit string provided is a
     * valid unit abbreviation for this quantity
     * Does not need to be an enumerated value.
     * Can be any string of standard unit 
     * abbreviations that represent this quantity
     * Unit Abbreviation Standards: http://www.bipm.org/en/si/
     */
    public static boolean validUnit(String unit)
    {
      if(unit == null || unit.isEmpty())
        return false;
      if(HeatCapacitancePerMass.contains(unit))
        return true;
      return UnitConverter.isOfQuantityType("J/K kg", unit);
    }
  }
  
  /**
   * Enumeration of commonly used units for this type
   * Units are not limited to this enum.
   * Any method that accepts this enum will have another method
   * that takes any valid standard unit abbreviation string 
   * in place of method that take this enum.
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public enum HeatConductancePerAreaUnit
  {
	W_Per_m2_K("W/m^2 K"),
	W_Per_m2_C("W/m^2 degC"),
	BTU_Per_hr_ft2_C("BTU/hr ft^2 degC");
    private String unit;
    private HeatConductancePerAreaUnit(String unit)
    {
      this.unit=unit;
    }
    public String toString(){return this.unit;}
    public static boolean contains(String unit)
    {
      for(HeatConductancePerAreaUnit v : HeatConductancePerAreaUnit.values())
      {
        if(v.toString().equals(unit))
          return true;
      }
      return false;
    }
    /**
     * Test if unit string provided is a
     * valid unit abbreviation for this quantity
     * Does not need to be an enumerated value.
     * Can be any string of standard unit 
     * abbreviations that represent this quantity
     * Unit Abbreviation Standards: http://www.bipm.org/en/si/
     */
    public static boolean validUnit(String unit)
    {
      if(unit == null || unit.isEmpty())
        return false;
      if(HeatConductancePerAreaUnit.contains(unit))
        return true;
      return UnitConverter.isOfQuantityType("W/m^2 K", unit);
    }
  }
  
  /**
   * Enumeration of commonly used units for this type
   * Units are not limited to this enum.
   * Any method that accepts this enum will have another method
   * that takes any valid standard unit abbreviation string 
   * in place of method that take this enum.
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public enum HeatResistanceAreaUnit
  {
    rsi("rsi"),
    clo("clo"),
    rValue("rValue"),
    tog("tog");
    private String unit;
    private HeatResistanceAreaUnit(String unit)
    {
      this.unit=unit;
    }
    public String toString(){return this.unit;}
    public static boolean contains(String unit)
    {
      for(HeatResistanceAreaUnit v : HeatResistanceAreaUnit.values())
      {
        if(v.toString().equals(unit))
          return true;
      }
      return false;
    }
    /**
     * Test if unit string provided is a
     * valid unit abbreviation for this quantity
     * Does not need to be an enumerated value.
     * Can be any string of standard unit 
     * abbreviations that represent this quantity
     * Unit Abbreviation Standards: http://www.bipm.org/en/si/
     */
    public static boolean validUnit(String unit)
    {
      if(unit == null || unit.isEmpty())
        return false;
      if(HeatResistanceAreaUnit.contains(unit))
        return true;
      return UnitConverter.isOfQuantityType("Hz", unit);
    }
  }
  
  /**
   * Enumeration of commonly used units for this type
   * Units are not limited to this enum.
   * Any method that accepts this enum will have another method
   * that takes any valid standard unit abbreviation string 
   * in place of method that take this enum.
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public enum InversePressureUnit 
  {
	Inverse_atm("1/atm"),
	Inverse_Pa("1/Pa"),
  	Inverse_mmHg("1/mmHg"),
  	Inverse_cmH2O("1/cmH2O");
    private String unit;
    private InversePressureUnit(String unit)
    {
      this.unit=unit;
    }
    public String toString(){return this.unit;}
    public static boolean contains(String unit)
    {
      for(InversePressureUnit v : InversePressureUnit.values())
      {
        if(v.toString().equals(unit))
          return true;
      }
      return false;
    }
    /**
     * Test if unit string provided is a
     * valid unit abbreviation for this quantity
     * Does not need to be an enumerated value.
     * Can be any string of standard unit 
     * abbreviations that represent this quantity
     * Unit Abbreviation Standards: http://www.bipm.org/en/si/
     */
    public static boolean validUnit(String unit)
    {
      if(unit == null || unit.isEmpty())
        return false;
      if(InversePressureUnit.contains(unit))
        return true;
      return UnitConverter.isOfQuantityType("1/Pa", unit);
    }
  }
    
  /**
	 * Enumeration of commonly used units for this type
	 * Units are not limited to this enum.
	 * Any method that accepts this enum will have another method
	 * that takes any valid standard unit abbreviation string 
	 * in place of method that take this enum.
	 * Unit Abbreviation Standards: http://www.bipm.org/en/si/
	 */
	public enum LengthUnit 
	{
	  m("m"),cm("cm"),in("in"),
	  km("km"),mm("mm"),um("um");
	  private String unit;
	  private LengthUnit(String unit)
	  {
	    this.unit=unit;
	  }
	  public String toString(){return this.unit;}
	  public static boolean contains(String unit)
	  {
	    for(LengthUnit v : LengthUnit.values())
	    {
	      if(v.toString().equals(unit))
	        return true;
	    }
	    return false;
	  }
	  /**
	   * Test if unit string provided is a
	   * valid unit abbreviation for this quantity
	   * Does not need to be an enumerated value.
	   * Can be any string of standard unit 
	   * abbreviations that represent this quantity
	   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
	   */
	  public static boolean validUnit(String unit)
	  {
	    if(unit == null || unit.isEmpty())
	      return false;
	    if(LengthUnit.contains(unit))
	      return true;
	    return UnitConverter.isOfQuantityType("m", unit);
	  }
	}
	
	/**
	 * Enumeration of commonly used units for this type
	 * Units are not limited to this enum.
	 * Any method that accepts this enum will have another method
	 * that takes any valid standard unit abbreviation string 
	 * in place of method that take this enum.
	 * Unit Abbreviation Standards: http://www.bipm.org/en/si/
	 */
	public enum LengthPerTimeUnit 
	{
	  m_Per_s("m/s"),ft_Per_s("ft/s");
	  private String unit;
	  private LengthPerTimeUnit(String unit)
	  {
	    this.unit=unit;
	  }
	  public String toString(){return this.unit;}
	  public static boolean contains(String unit)
	  {
	    for(LengthPerTimeUnit v : LengthPerTimeUnit.values())
	    {
	      if(v.toString().equals(unit))
	        return true;
	    }
	    return false;
	  }
	  /**
	   * Test if unit string provided is a
	   * valid unit abbreviation for this quantity
	   * Does not need to be an enumerated value.
	   * Can be any string of standard unit 
	   * abbreviations that represent this quantity
	   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
	   */
	  public static boolean validUnit(String unit)
	  {
	    if(unit == null || unit.isEmpty())
	      return false;
	    if(LengthPerTimeUnit.contains(unit))
	      return true;
	    return UnitConverter.isOfQuantityType("m/s", unit);
	  }
	}

	/**
   * Enumeration of commonly used units for this type
   * Units are not limited to this enum.
   * Any method that accepts this enum will have another method
   * that takes any valid standard unit abbreviation string 
   * in place of method that take this enum.
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public enum MassUnit 
  {
    lb("lb"),kg("kg"),
    ug("ug"),mg("mg"),
    g("g"),oz("oz"),ATOMIC("u");
    private String unit;
    private MassUnit(String unit)
    {
      this.unit=unit;
    }
    public String toString(){return this.unit;}
    public static boolean contains(String unit)
    {
      for(MassUnit v : MassUnit.values())
      {
        if(v.toString().equals(unit))
          return true;
      }
      return false;
    }
    /**
     * Test if unit string provided is a
     * valid unit abbreviation for this quantity
     * Does not need to be an enumerated value.
     * Can be any string of standard unit 
     * abbreviations that represent this quantity
     * Unit Abbreviation Standards: http://www.bipm.org/en/si/
     */
    public static boolean validUnit(String unit)
    {
      if(unit == null || unit.isEmpty())
        return false;
      if(MassUnit.contains(unit))
        return true;
      return UnitConverter.isOfQuantityType("kg", unit);
    }
  }
  
  /**
   * Enumeration of commonly used units for this type
   * Units are not limited to this enum.
   * Any method that accepts this enum will have another method
   * that takes any valid standard unit abbreviation string 
   * in place of method that take this enum.
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public enum MassPerAmountUnit 
  {    
    g_Per_mol("g/mol"),
    ug_Per_mol("ug/mol");
    private String unit;
    private MassPerAmountUnit(String unit)
    {
      this.unit=unit;
    }
    public String toString(){return this.unit;}
    public static boolean contains(String unit)
    {
      for(MassPerAmountUnit v : MassPerAmountUnit.values())
      {
        if(v.toString().equals(unit))
          return true;
      }
      return false;
    }
    /**
     * Test if unit string provided is a
     * valid unit abbreviation for this quantity
     * Does not need to be an enumerated value.
     * Can be any string of standard unit 
     * abbreviations that represent this quantity
     * Unit Abbreviation Standards: http://www.bipm.org/en/si/
     */
    public static boolean validUnit(String unit)
    {
      if(unit == null || unit.isEmpty())
        return false;
      if(MassPerAmountUnit.contains(unit))
        return true;
      return UnitConverter.isOfQuantityType("g/mol", unit);
    }
  }
  
  /**
   * Enumeration of commonly used units for this type
   * Units are not limited to this enum.
   * Any method that accepts this enum will have another method
   * that takes any valid standard unit abbreviation string 
   * in place of method that take this enum.
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public enum MassPerAreaTimeUnit 
  {    
    g_Per_cm2_s("g/cm^2 s");
    private String unit;
    private MassPerAreaTimeUnit(String unit)
    {
      this.unit=unit;
    }
    public String toString(){return this.unit;}
    public static boolean contains(String unit)
    {
      for(MassPerAreaTimeUnit v : MassPerAreaTimeUnit.values())
      {
        if(v.toString().equals(unit))
          return true;
      }
      return false;
    }
    /**
     * Test if unit string provided is a
     * valid unit abbreviation for this quantity
     * Does not need to be an enumerated value.
     * Can be any string of standard unit 
     * abbreviations that represent this quantity
     * Unit Abbreviation Standards: http://www.bipm.org/en/si/
     */
    public static boolean validUnit(String unit)
    {
      if(unit == null || unit.isEmpty())
        return false;
      if(MassPerAreaTimeUnit.contains(unit))
        return true;
      return UnitConverter.isOfQuantityType("g/cm^2 s", unit);
    }
  }
  
  /**
   * Enumeration of commonly used units for this type
   * Units are not limited to this enum.
   * Any method that accepts this enum will have another method
   * that takes any valid standard unit abbreviation string 
   * in place of method that take this enum.
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public enum MassPerMassUnit 
  {
  	ug_Per_kg("ug/kg");
    private String unit;
    private MassPerMassUnit(String unit)
    {
      this.unit=unit;
    }
    public String toString(){return this.unit;}
    public static boolean contains(String unit)
    {
      for(MassPerMassUnit v : MassPerMassUnit.values())
      {
        if(v.toString().equals(unit))
          return true;
      }
      return false;
    }
    /**
     * Test if unit string provided is a
     * valid unit abbreviation for this quantity
     * Does not need to be an enumerated value.
     * Can be any string of standard unit 
     * abbreviations that represent this quantity
     * Unit Abbreviation Standards: http://www.bipm.org/en/si/
     */
    public static boolean validUnit(String unit)
    {
      if(unit == null || unit.isEmpty())
        return false;
      if(MassPerMassUnit.contains(unit))
        return true;
      return UnitConverter.isOfQuantityType("ug/kg", unit);
    }
  }
  
  /**
   * Enumeration of commonly used units for this type
   * Units are not limited to this enum.
   * Any method that accepts this enum will have another method
   * that takes any valid standard unit abbreviation string 
   * in place of method that take this enum.
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public enum MassPerTimeUnit 
  {
  	ug_Per_s("ug/s"),
  	kg_Per_s("kg/s"),
  	g_Per_min("g/min"),
  	mg_Per_min("mg/min"),
  	ug_Per_min("ug/min");
    private String unit;
    private MassPerTimeUnit(String unit)
    {
      this.unit=unit;
    }
    public String toString(){return this.unit;}
    public static boolean contains(String unit)
    {
      for(MassPerTimeUnit v : MassPerTimeUnit.values())
      {
        if(v.toString().equals(unit))
          return true;
      }
      return false;
    }
    /**
     * Test if unit string provided is a
     * valid unit abbreviation for this quantity
     * Does not need to be an enumerated value.
     * Can be any string of standard unit 
     * abbreviations that represent this quantity
     * Unit Abbreviation Standards: http://www.bipm.org/en/si/
     */
    public static boolean validUnit(String unit)
    {
      if(unit == null || unit.isEmpty())
        return false;
      if(MassPerTimeUnit.contains(unit))
        return true;
      return UnitConverter.isOfQuantityType("ug/s", unit);
    }
  }
  
  /**
   * Enumeration of commonly used units for this type
   * Units are not limited to this enum.
   * Any method that accepts this enum will have another method
   * that takes any valid standard unit abbreviation string 
   * in place of method that take this enum.
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public enum MassPerVolumeUnit 
  {
	  ug_Per_mL("ug/mL"),
	  mg_Per_mL("mg/mL"),
	  ug_Per_L("ug/L"),
    g_Per_mL("g/mL"),
    g_Per_dL("g/dL"),
  	g_Per_m3("g/m^3"),
  	mg_Per_m3("mg/m^3"),
  	kg_Per_m3("kg/m^3");
    private String unit;
    private MassPerVolumeUnit(String unit)
    {
      this.unit=unit;
    }
    public String toString(){return this.unit;}
    public static boolean contains(String unit)
    {
      for(MassPerVolumeUnit v : MassPerVolumeUnit.values())
      {
        if(v.toString().equals(unit))
          return true;
      }
      return false;
    }
    /**
     * Test if unit string provided is a
     * valid unit abbreviation for this quantity
     * Does not need to be an enumerated value.
     * Can be any string of standard unit 
     * abbreviations that represent this quantity
     * Unit Abbreviation Standards: http://www.bipm.org/en/si/
     */
    public static boolean validUnit(String unit)
    {
      if(unit == null || unit.isEmpty())
        return false;
      if(MassPerVolumeUnit.contains(unit))
        return true;
      return UnitConverter.isOfQuantityType("g/dL", unit);
    }
  }
  
  /**
   * Enumeration of commonly used units for this type
   * Units are not limited to this enum.
   * Any method that accepts this enum will have another method
   * that takes any valid standard unit abbreviation string 
   * in place of method that take this enum.
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public enum OsmolalityUnit 
  {
    Osm_Per_kg("Osm/kg"),
    mOsm_Per_kg("mOsm/kg");
    
    private String unit;
    private OsmolalityUnit(String unit)
    {
      this.unit=unit;
    }
    public String toString(){return this.unit;}
    public static boolean contains(String unit)
    {
      for(OsmolalityUnit v : OsmolalityUnit.values())
      {
        if(v.toString().equals(unit))
          return true;
      }
      return false;
    }
    /**
     * Test if unit string provided is a
     * valid unit abbreviation for this quantity
     * Does not need to be an enumerated value.
     * Can be any string of standard unit 
     * abbreviations that represent this quantity
     * Unit Abbreviation Standards: http://www.bipm.org/en/si/
     */
    public static boolean validUnit(String unit)
    {
      if(unit == null || unit.isEmpty())
        return false;
      if(OsmolalityUnit.contains(unit))
        return true;
      return UnitConverter.isOfQuantityType("Osm/kg", unit);
    }
  }
  
  /**
   * Enumeration of commonly used units for this type
   * Units are not limited to this enum.
   * Any method that accepts this enum will have another method
   * that takes any valid standard unit abbreviation string 
   * in place of method that take this enum.
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public enum OsmolarityUnit 
  {
    Osm_Per_L("Osm/L"),
    mOsm_Per_L("mOsm/L");
    
    private String unit;
    private OsmolarityUnit(String unit)
    {
      this.unit=unit;
    }
    public String toString(){return this.unit;}
    public static boolean contains(String unit)
    {
      for(OsmolarityUnit v : OsmolarityUnit.values())
      {
        if(v.toString().equals(unit))
          return true;
      }
      return false;
    }
    /**
     * Test if unit string provided is a
     * valid unit abbreviation for this quantity
     * Does not need to be an enumerated value.
     * Can be any string of standard unit 
     * abbreviations that represent this quantity
     * Unit Abbreviation Standards: http://www.bipm.org/en/si/
     */
    public static boolean validUnit(String unit)
    {
      if(unit == null || unit.isEmpty())
        return false;
      if(OsmolarityUnit.contains(unit))
        return true;
      return UnitConverter.isOfQuantityType("Osm/L", unit);
    }
  }
  
  /**
   * Enumeration of commonly used units for this type
   * Units are not limited to this enum.
   * Any method that accepts this enum will have another method
   * that takes any valid standard unit abbreviation string 
   * in place of method that take this enum.
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public enum PowerUnit 
  {
  	W("W"),J_Per_s("J/s"),BTU_Per_hr("BTU/hr"),
  	kcal_Per_day("kcal/day");
    private String unit;
    private PowerUnit(String unit)
    {
      this.unit=unit;
    }
    public String toString(){return this.unit;}
    public static boolean contains(String unit)
    {
      for(PowerUnit v : PowerUnit.values())
      {
        if(v.toString().equals(unit))
          return true;
      }
      return false;
    }
    /**
     * Test if unit string provided is a
     * valid unit abbreviation for this quantity
     * Does not need to be an enumerated value.
     * Can be any string of standard unit 
     * abbreviations that represent this quantity
     * Unit Abbreviation Standards: http://www.bipm.org/en/si/
     */
    public static boolean validUnit(String unit)
    {
      if(unit == null || unit.isEmpty())
        return false;
      if(PowerUnit.contains(unit))
        return true;
      return UnitConverter.isOfQuantityType("Pa", unit);
    }
  }
  
  /**
   * Enumeration of commonly used units for this type
   * Units are not limited to this enum.
   * Any method that accepts this enum will have another method
   * that takes any valid standard unit abbreviation string 
   * in place of method that take this enum.
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public enum PressureUnit 
  {
	mmHg("mmHg"),Pa("Pa"),cmH2O("cmH2O");
    private String unit;
    private PressureUnit(String unit)
    {
      this.unit=unit;
    }
    public String toString(){return this.unit;}
    public static boolean contains(String unit)
    {
      for(PressureUnit v : PressureUnit.values())
      {
        if(v.toString().equals(unit))
          return true;
      }
      return false;
    }
    /**
     * Test if unit string provided is a
     * valid unit abbreviation for this quantity
     * Does not need to be an enumerated value.
     * Can be any string of standard unit 
     * abbreviations that represent this quantity
     * Unit Abbreviation Standards: http://www.bipm.org/en/si/
     */
    public static boolean validUnit(String unit)
    {
      if(unit == null || unit.isEmpty())
        return false;
      if(PressureUnit.contains(unit))
        return true;
      return UnitConverter.isOfQuantityType("Pa", unit);
    }
  }
  
  /**
   * Enumeration of commonly used units for this type
   * Units are not limited to this enum.
   * Any method that accepts this enum will have another method
   * that takes any valid standard unit abbreviation string 
   * in place of method that take this enum.
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public enum PressurePerVolumeUnit 
  {
    mmHg_Per_mL("mmHg/mL"),cmH2O_Per_mL("cmH2O/mL");
    private String unit;
    private PressurePerVolumeUnit(String unit)
    {
      this.unit=unit;
    }
    public String toString(){return this.unit;}
    public static boolean contains(String unit)
    {
      for(PressurePerVolumeUnit v : PressurePerVolumeUnit.values())
      {
        if(v.toString().equals(unit))
          return true;
      }
      return false;
    }
    /**
     * Test if unit string provided is a
     * valid unit abbreviation for this quantity
     * Does not need to be an enumerated value.
     * Can be any string of standard unit 
     * abbreviations that represent this quantity
     * Unit Abbreviation Standards: http://www.bipm.org/en/si/
     */
    public static boolean validUnit(String unit)
    {
      if(unit == null || unit.isEmpty())
        return false;
      if(PressurePerVolumeUnit.contains(unit))
        return true;
      return UnitConverter.isOfQuantityType("mmHg/mL", unit);
    }
  }
  
  /**
   * Enumeration of commonly used units for this type
   * Units are not limited to this enum.
   * Any method that accepts this enum will have another method
   * that takes any valid standard unit abbreviation string 
   * in place of method that take this enum.
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public enum PressureTimePerVolumeAreaUnit 
  {
  	mmHg_min_Per_mL_m2("mmHg min/mL m^2"),mmHg_s_Per_mL_m2("mmHg s/mL m^2"),dyn_s_Per_cm5_m2("dyn s /cm^5 m^2");
    private String unit;
    private PressureTimePerVolumeAreaUnit(String unit)
    {
      this.unit=unit;
    }
    public String toString(){return this.unit;}
    public static boolean contains(String unit)
    {
      for(PressureTimePerVolumeAreaUnit v : PressureTimePerVolumeAreaUnit.values())
      {
        if(v.toString().equals(unit))
          return true;
      }
      return false;
    }
    /**
     * Test if unit string provided is a
     * valid unit abbreviation for this quantity
     * Does not need to be an enumerated value.
     * Can be any string of standard unit 
     * abbreviations that represent this quantity
     * Unit Abbreviation Standards: http://www.bipm.org/en/si/
     */
    public static boolean validUnit(String unit)
    {
      if(unit == null || unit.isEmpty())
        return false;
      if(PressureTimePerVolumeAreaUnit.contains(unit))
        return true;
      return UnitConverter.isOfQuantityType("mmHg min/mL m^2", unit);
    }
  }
  
  /**
   * Enumeration of commonly used units for this type
   * Units are not limited to this enum.
   * Any method that accepts this enum will have another method
   * that takes any valid standard unit abbreviation string 
   * in place of method that take this enum.
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public enum PressureTimePerAreaUnit 
  {
    mmHg_Per_mL_m2("mmHg/mL m^2"),cmH2O_Per_mL_m2("cmH2O/mL m^2");
    private String unit;
    private PressureTimePerAreaUnit(String unit)
    {
      this.unit=unit;
    }
    public String toString(){return this.unit;}
    public static boolean contains(String unit)
    {
      for(PressureTimePerAreaUnit v : PressureTimePerAreaUnit.values())
      {
        if(v.toString().equals(unit))
          return true;
      }
      return false;
    }
    /**
     * Test if unit string provided is a
     * valid unit abbreviation for this quantity
     * Does not need to be an enumerated value.
     * Can be any string of standard unit 
     * abbreviations that represent this quantity
     * Unit Abbreviation Standards: http://www.bipm.org/en/si/
     */
    public static boolean validUnit(String unit)
    {
      if(unit == null || unit.isEmpty())
        return false;
      if(PressureTimePerAreaUnit.contains(unit))
        return true;
      return UnitConverter.isOfQuantityType("mmHg/mL m^2", unit);
    }
  }
  
  /**
   * Enumeration of commonly used units for this type
   * Units are not limited to this enum.
   * Any method that accepts this enum will have another method
   * that takes any valid standard unit abbreviation string 
   * in place of method that take this enum.
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public enum TemperatureUnit 
  {
    C("degC"),
    F("degF"),
    K("K"),
    R("degR");
    private String unit;
    private TemperatureUnit(String unit)
    {
      this.unit=unit;
    }
    public String toString(){return this.unit;}
    public static boolean contains(String unit)
    {
      for(TemperatureUnit v : TemperatureUnit.values())
      {
        if(v.toString().equals(unit))
          return true;
      }
      return false;
    }
    /**
     * Test if unit string provided is a
     * valid unit abbreviation for this quantity
     * Does not need to be an enumerated value.
     * Can be any string of standard unit 
     * abbreviations that represent this quantity
     * Unit Abbreviation Standards: http://www.bipm.org/en/si/
     */
    public static boolean validUnit(String unit)
    {
      if(unit == null || unit.isEmpty())
        return false;
      if(TemperatureUnit.contains(unit))
        return true;
      return UnitConverter.isOfQuantityType("degF", unit);
    }
  }
  
  /**
   * Enumeration of commonly used units for this type
   * Units are not limited to this enum.
   * Any method that accepts this enum will have another method
   * that takes any valid standard unit abbreviation string 
   * in place of method that take this enum.
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public enum TimeUnit 
  {
    s("s"),min("min"),hr("hr"),
    day("day"),yr("yr");
    private String unit;
    private TimeUnit(String unit)
    {
      this.unit=unit;
    }
    public String toString(){return this.unit;}
    public static boolean contains(String unit)
    {
      for(TimeUnit v : TimeUnit.values())
      {
        if(v.toString().equals(unit))
          return true;
      }
      return false;
    }
    /**
     * Test if unit string provided is a
     * valid unit abbreviation for this quantity
     * Does not need to be an enumerated value.
     * Can be any string of standard unit 
     * abbreviations that represent this quantity
     * Unit Abbreviation Standards: http://www.bipm.org/en/si/
     */
    public static boolean validUnit(String unit)
    {
      if(unit == null || unit.isEmpty())
        return false;
      if(TimeUnit.contains(unit))
        return true;
      return UnitConverter.isOfQuantityType("s", unit);
    }
  }
  
  /**
   * Enumeration of commonly used units for this type
   * Units are not limited to this enum.
   * Any method that accepts this enum will have another method
   * that takes any valid standard unit abbreviation string 
   * in place of method that take this enum.
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public enum VolumeUnit 
  {
    L("L"), mL("mL"), m3("m3");
    private String unit;
    private VolumeUnit(String unit)
    {
      this.unit=unit;
    }
    public String toString(){return this.unit;}
    public static boolean contains(String unit)
    {
      for(VolumeUnit v : VolumeUnit.values())
      {
        if(v.toString().equals(unit))
          return true;
      }
      return false;
    }
    /**
     * Test if unit string provided is a
     * valid unit abbreviation for this quantity
     * Does not need to be an enumerated value.
     * Can be any string of standard unit 
     * abbreviations that represent this quantity
     * Unit Abbreviation Standards: http://www.bipm.org/en/si/
     */
    public static boolean validUnit(String unit)
    {
      if(unit == null || unit.isEmpty())
        return false;
      if(VolumeUnit.contains(unit))
        return true;
      return UnitConverter.isOfQuantityType("L", unit);
    }
  }
  
  /**
   * Enumeration of commonly used units for this type
   * Units are not limited to this enum.
   * Any method that accepts this enum will have another method
   * that takes any valid standard unit abbreviation string 
   * in place of method that take this enum.
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public enum VolumePerPressureUnit 
  {
    L_Per_Pa("L/Pa");
    private String unit;
    private VolumePerPressureUnit(String unit)
    {
      this.unit=unit;
    }
    public String toString(){return this.unit;}
    public static boolean contains(String unit)
    {
      for(VolumePerPressureUnit v : VolumePerPressureUnit.values())
      {
        if(v.toString().equals(unit))
          return true;
      }
      return false;
    }
    /**
     * Test if unit string provided is a
     * valid unit abbreviation for this quantity
     * Does not need to be an enumerated value.
     * Can be any string of standard unit 
     * abbreviations that represent this quantity
     * Unit Abbreviation Standards: http://www.bipm.org/en/si/
     */
    public static boolean validUnit(String unit)
    {
      if(unit == null || unit.isEmpty())
        return false;
      if(VolumePerPressureUnit.contains(unit))
        return true;
      return UnitConverter.isOfQuantityType("L/Pa", unit);
    }
  }
  
  /**
   * Enumeration of commonly used units for this type
   * Units are not limited to this enum.
   * Any method that accepts this enum will have another method
   * that takes any valid standard unit abbreviation string 
   * in place of method that take this enum.
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public enum VolumePerTimeUnit 
  {
    L_Per_s("L/s"), mL_Per_s("mL/s"), L_Per_min("L/min"),
    m3_Per_s("m^3/s"), mL_Per_min("mL/min"), mL_Per_day("mL/day");
    private String unit;
    private VolumePerTimeUnit(String unit)
    {
      this.unit=unit;
    }
    public String toString(){return this.unit;}
    public static boolean contains(String unit)
    {
      for(VolumePerTimeUnit v : VolumePerTimeUnit.values())
      {
        if(v.toString().equals(unit))
          return true;
      }
      return false;
    }
    /**
     * Test if unit string provided is a
     * valid unit abbreviation for this quantity
     * Does not need to be an enumerated value.
     * Can be any string of standard unit 
     * abbreviations that represent this quantity
     * Unit Abbreviation Standards: http://www.bipm.org/en/si/
     */
    public static boolean validUnit(String unit)
    {
      if(unit == null || unit.isEmpty())
        return false;
      if(VolumePerTimeUnit.contains(unit))
        return true;
      return UnitConverter.isOfQuantityType("L/s", unit);
    }
  }
  
  /**
   * Enumeration of commonly used units for this type
   * Units are not limited to this enum.
   * Any method that accepts this enum will have another method
   * that takes any valid standard unit abbreviation string 
   * in place of method that take this enum.
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public enum VolumePerTimeAreaUnit 
  {
  	mL_Per_min_Per_m2("mL/min m^2"), mL_Per_s_Per_m2("mL/s m^2"),L_Per_min_m2("L/min m^2");
    private String unit;
    private VolumePerTimeAreaUnit(String unit)
    {
      this.unit=unit;
    }
    public String toString(){return this.unit;}
    public static boolean contains(String unit)
    {
      for(VolumePerTimeAreaUnit v : VolumePerTimeAreaUnit.values())
      {
        if(v.toString().equals(unit))
          return true;
      }
      return false;
    }
    /**
     * Test if unit string provided is a
     * valid unit abbreviation for this quantity
     * Does not need to be an enumerated value.
     * Can be any string of standard unit 
     * abbreviations that represent this quantity
     * Unit Abbreviation Standards: http://www.bipm.org/en/si/
     */
    public static boolean validUnit(String unit)
    {
      if(unit == null || unit.isEmpty())
        return false;
      if(VolumePerTimeAreaUnit.contains(unit))
        return true;
      return UnitConverter.isOfQuantityType("mL/min m^2", unit);
    }
  }
  
   /**
   * Enumeration of commonly used units for this type
   * Units are not limited to this enum.
   * Any method that accepts this enum will have another method
   * that takes any valid standard unit abbreviation string 
   * in place of method that take this enum.
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public enum VolumePerTimePressureAreaUnit 
  {
  	mL_Per_min_Per_mmHg_Per_m2("mL/min mmHg m^2"), mL_Per_s_Per_mmHg_Per_m2("mL/s mmHg m^2");
    private String unit;
    private VolumePerTimePressureAreaUnit(String unit)
    {
      this.unit=unit;
    }
    public String toString(){return this.unit;}
    public static boolean contains(String unit)
    {
      for(VolumePerTimePressureAreaUnit v : VolumePerTimePressureAreaUnit.values())
      {
        if(v.toString().equals(unit))
          return true;
      }
      return false;
    }
    /**
     * Test if unit string provided is a
     * valid unit abbreviation for this quantity
     * Does not need to be an enumerated value.
     * Can be any string of standard unit 
     * abbreviations that represent this quantity
     * Unit Abbreviation Standards: http://www.bipm.org/en/si/
     */
    public static boolean validUnit(String unit)
    {
      if(unit == null || unit.isEmpty())
        return false;
      if(VolumePerTimePressureAreaUnit.contains(unit))
        return true;
      return UnitConverter.isOfQuantityType("mL/min mmHg m^2", unit);
    }
  }

  /**
   * Enumeration of commonly used units for this type
   * Units are not limited to this enum.
   * Any method that accepts this enum will have another method
   * that takes any valid standard unit abbreviation string 
   * in place of method that take this enum.
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public enum VolumePerTimeMassUnit 
  {
  	mL_Per_s_g("mL/s g"),
  	mL_Per_min_kg("mL/min kg");
    private String unit;
    private VolumePerTimeMassUnit(String unit)
    {
      this.unit=unit;
    }
    public String toString(){return this.unit;}
    public static boolean contains(String unit)
    {
      for(VolumePerTimeMassUnit v : VolumePerTimeMassUnit.values())
      {
        if(v.toString().equals(unit))
          return true;
      }
      return false;
    }
    /**
     * Test if unit string provided is a
     * valid unit abbreviation for this quantity
     * Does not need to be an enumerated value.
     * Can be any string of standard unit 
     * abbreviations that represent this quantity
     * Unit Abbreviation Standards: http://www.bipm.org/en/si/
     */
    public static boolean validUnit(String unit)
    {
      if(unit == null || unit.isEmpty())
        return false;
      if(VolumePerTimeMassUnit.contains(unit))
        return true;
      return UnitConverter.isOfQuantityType("mL/s g", unit);
    }
  }
  
  /**
   * Enumeration of commonly used units for this type
   * Units are not limited to this enum.
   * Any method that accepts this enum will have another method
   * that takes any valid standard unit abbreviation string 
   * in place of method that take this enum.
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public enum VolumePerTimePressureUnit 
  {
    L_Per_s_Per_mmHg("L/s mmHg"), mL_Per_s_Per_mmHg("mL/s mmHg"), 
    L_Per_min_Per_mmHg("L/min mmHg"), mL_Per_min_Per_mmHg("mL/min mmHg");
    private String unit;
    private VolumePerTimePressureUnit(String unit)
    {
      this.unit=unit;
    }
    public String toString(){return this.unit;}
    public static boolean contains(String unit)
    {
      for(VolumePerTimePressureUnit v : VolumePerTimePressureUnit.values())
      {
        if(v.toString().equals(unit))
          return true;
      }
      return false;
    }
    /**
     * Test if unit string provided is a
     * valid unit abbreviation for this quantity
     * Does not need to be an enumerated value.
     * Can be any string of standard unit 
     * abbreviations that represent this quantity
     * Unit Abbreviation Standards: http://www.bipm.org/en/si/
     */
    public static boolean validUnit(String unit)
    {
      if(unit == null || unit.isEmpty())
        return false;
      if(VolumePerTimePressureUnit.contains(unit))
        return true;
      return UnitConverter.isOfQuantityType("L/s mmHg", unit);
    }
  }
}
