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

package mil.tatrc.physiology.datamodel.system.equipment.anesthesia;

import java.util.HashMap;
import java.util.Map;

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.AnesthesiaMachineData;
import mil.tatrc.physiology.datamodel.bind.EnumAnesthesiaMachineConnection;
import mil.tatrc.physiology.datamodel.bind.EnumAnesthesiaMachineEvent;
import mil.tatrc.physiology.datamodel.bind.EnumAnesthesiaMachineOxygenSource;
import mil.tatrc.physiology.datamodel.bind.EnumAnesthesiaMachinePrimaryGas;
import mil.tatrc.physiology.datamodel.bind.EnumOnOff;
import mil.tatrc.physiology.datamodel.properties.SEScalar;
import mil.tatrc.physiology.datamodel.properties.SEScalarFraction;
import mil.tatrc.physiology.datamodel.properties.SEScalarFrequency;
import mil.tatrc.physiology.datamodel.properties.SEScalarPressure;
import mil.tatrc.physiology.datamodel.properties.SEScalarVolumePerTime;
import mil.tatrc.physiology.datamodel.substance.SESubstanceManager;
import mil.tatrc.physiology.datamodel.system.SESystem;

public class SEAnesthesiaMachine implements SESystem
{
	protected EnumAnesthesiaMachineConnection		connection;
	protected SEScalarVolumePerTime				      inletFlow;
	protected SEScalar 							            inspiratoryExpiratoryRatio;
	protected SEScalarFraction 					        oxygenFraction;
	protected EnumAnesthesiaMachineOxygenSource	oxygenSource;
	protected SEScalarPressure					        positiveEndExpiredPressure;
	protected EnumAnesthesiaMachinePrimaryGas	  primaryGas;
	protected SEScalarFrequency					        respiratoryRate;
	protected SEScalarPressure                  reliefValvePressure;

	protected SEScalarPressure					        ventilatorPressure;

	protected SEAnesthesiaMachineChamber		    leftChamber;
	protected SEAnesthesiaMachineChamber		    rightChamber;

	protected SEAnesthesiaMachineOxygenBottle   oxygenBottleOne;
	protected SEAnesthesiaMachineOxygenBottle   oxygenBottleTwo;


	protected Map<EnumAnesthesiaMachineEvent,Boolean> events = new HashMap<EnumAnesthesiaMachineEvent,Boolean>();

	public SEAnesthesiaMachine()
	{
		connection = null;
		inletFlow = null;
		inspiratoryExpiratoryRatio = null;
		oxygenFraction = null;
		oxygenSource = null;
		positiveEndExpiredPressure = null;
		primaryGas = null;
		respiratoryRate = null;
		reliefValvePressure = null;

		ventilatorPressure = null;

		leftChamber = null;
		rightChamber = null;
		
		oxygenBottleOne = null;
		oxygenBottleTwo = null;
	}

	public void reset()
	{
		connection = null;
		if (inletFlow != null)
			inletFlow.invalidate();
		if (inspiratoryExpiratoryRatio != null)
			inspiratoryExpiratoryRatio.invalidate();
		if (oxygenFraction != null)
			oxygenFraction.invalidate();
		oxygenSource = null;
		if (positiveEndExpiredPressure != null)
			positiveEndExpiredPressure.invalidate();
		primaryGas = null;
		if (respiratoryRate != null)
			respiratoryRate.invalidate();
		if (reliefValvePressure != null)
			reliefValvePressure.invalidate();
		if (ventilatorPressure != null)
			ventilatorPressure.invalidate();

		if (hasLeftChamber())
			leftChamber.reset();
		if (hasRightChamber())
			rightChamber.reset();
		if (hasOxygenBottleOne())
			oxygenBottleOne.reset();
		if (hasOxygenBottleTwo())
			oxygenBottleTwo.reset();

		events.clear();
	}

	public void copy(SEAnesthesiaMachine from)
	{
		reset();
		this.connection=from.connection;			
		if(from.hasInletFlow())
			this.getInletFlow().set(from.getInletFlow());
		if(from.hasInspiratoryExpiratoryRatio())
			this.getInspiratoryExpiratoryRatio().set(from.getInspiratoryExpiratoryRatio());
		if(from.hasOxygenFraction())
			this.getOxygenFraction().set(from.getOxygenFraction());
		this.setOxygenSource(from.getOxygenSource());
		if(from.hasPositiveEndExpiredPressure())
			this.getPositiveEndExpiredPressure().set(from.getPositiveEndExpiredPressure());
		this.setPrimaryGas(from.getPrimaryGas());
		if(from.hasReliefValvePressure())
			this.getReliefValvePressure().set(from.getReliefValvePressure());
		if(from.hasRespiratoryRate())
			this.getRespiratoryRate().set(from.getRespiratoryRate());
		if(from.hasVentilatorPressure())
			this.getVentilatorPressure().set(from.getVentilatorPressure());	  

		if(from.hasLeftChamber())	  
			getLeftChamber().copy(from.getLeftChamber());
		if(from.hasRightChamber())
			getRightChamber().copy(from.getRightChamber());
		if(from.hasOxygenBottleOne())
			getOxygenBottleOne().copy(from.getOxygenBottleOne());
		if(from.hasOxygenBottleTwo())
			getOxygenBottleTwo().copy(from.getOxygenBottleTwo());

		events.putAll(from.events);
	}

	public boolean load(AnesthesiaMachineData in, SESubstanceManager subMgr)
	{
		reset();
		if (in.getConnection() != null)
			setConnection(in.getConnection());
		if (in.getInletFlow() != null)
			getInletFlow().load(in.getInletFlow());
		if (in.getInspiratoryExpiratoryRatio() != null)
			getInspiratoryExpiratoryRatio().load(in.getInspiratoryExpiratoryRatio());
		if (in.getOxygenFraction() != null)
			getOxygenFraction().load(in.getOxygenFraction());
		if (in.getOxygenSource() != null)
			setOxygenSource(in.getOxygenSource());
		if (in.getPositiveEndExpiredPressure() != null)
			getPositiveEndExpiredPressure().load(in.getPositiveEndExpiredPressure());
		if (in.getPrimaryGas() != null)
			setPrimaryGas(in.getPrimaryGas());
		if (in.getRespiratoryRate() != null)
			getRespiratoryRate().load(in.getRespiratoryRate());
		if (in.getReliefValvePressure() != null)
			getReliefValvePressure().load(in.getReliefValvePressure());
		if (in.getVentilatorPressure() != null)
			getVentilatorPressure().load(in.getVentilatorPressure());
		if (in.getLeftChamber() != null)
			getLeftChamber().load(in.getLeftChamber(),subMgr);
		if (in.getRightChamber() != null)
			getRightChamber().load(in.getRightChamber(),subMgr);
		if (in.getOxygenBottleOne() != null)
			getOxygenBottleOne().load(in.getOxygenBottleOne());
		if (in.getOxygenBottleTwo() != null)
			getOxygenBottleTwo().load(in.getOxygenBottleTwo());

		return true;
	}

	public AnesthesiaMachineData unload()
	{
		AnesthesiaMachineData data = CDMSerializer.objFactory.createAnesthesiaMachineData();
		unload(data);
		return data;
	}

	protected void unload(AnesthesiaMachineData data)
	{
		if (hasConnection())
			data.setConnection(connection);
		if (inletFlow != null)
			data.setInletFlow(inletFlow.unload());
		if (inspiratoryExpiratoryRatio != null)
			data.setInspiratoryExpiratoryRatio(inspiratoryExpiratoryRatio.unload());
		if (oxygenFraction != null)
			data.setOxygenFraction(oxygenFraction.unload());
		if (hasOxygenSource())
			data.setOxygenSource(oxygenSource);
		if (positiveEndExpiredPressure != null)
			data.setPositiveEndExpiredPressure(positiveEndExpiredPressure.unload());
		if (hasPrimaryGas())
			data.setPrimaryGas(primaryGas);
		if (respiratoryRate != null)
			data.setRespiratoryRate(respiratoryRate.unload());
		if (reliefValvePressure != null)
			data.setReliefValvePressure(reliefValvePressure.unload());
		if (hasLeftChamber())
			data.setLeftChamber(leftChamber.unload());
		if (hasRightChamber())
			data.setRightChamber(rightChamber.unload());
		if (ventilatorPressure != null)
			data.setVentilatorPressure(ventilatorPressure.unload());
		if (hasOxygenBottleOne())
			data.setOxygenBottleOne(oxygenBottleOne.unload());
		if (hasOxygenBottleTwo())
			data.setOxygenBottleTwo(oxygenBottleTwo.unload());
	}

	public void setEvent(EnumAnesthesiaMachineEvent type, boolean active)
	{
		this.events.put(type, active);
	}
	public boolean isEventActive(EnumAnesthesiaMachineEvent type)
	{
		if(!this.events.containsKey(type))
			return false;
		return this.events.get(type);
	}
	
	public EnumAnesthesiaMachineConnection getConnection()
	{
		return connection;
	}
	public void setConnection(EnumAnesthesiaMachineConnection c)
	{
		this.connection = c;
	}
	public boolean hasConnection()
	{
		return connection == null ? false : true;
	}

	/*
	 * Inspiratory/Expiratory Ratio
	 */
	public SEScalar getInspiratoryExpiratoryRatio()
	{
		if (inspiratoryExpiratoryRatio == null)
			inspiratoryExpiratoryRatio = new SEScalar();
		return inspiratoryExpiratoryRatio;
	}
	public boolean hasInspiratoryExpiratoryRatio()
	{
		return inspiratoryExpiratoryRatio == null ? false : inspiratoryExpiratoryRatio.isValid();
	}

	/*
	 * Inlet Flow
	 */
	public SEScalarVolumePerTime getInletFlow()
	{
		if (inletFlow == null)
			inletFlow = new SEScalarVolumePerTime();
		return inletFlow;
	}
	public boolean hasInletFlow()
	{
		return inletFlow == null ? false : inletFlow.isValid();
	}

	/*
	 * Oxygen Fraction
	 */
	public SEScalarFraction getOxygenFraction()
	{
		if (oxygenFraction == null)
			oxygenFraction = new SEScalarFraction();
		return oxygenFraction;
	}
	public boolean hasOxygenFraction()
	{
		return oxygenFraction == null ? false : oxygenFraction.isValid();
	}

	/*
	 * Oxygen Source
	 */
	public EnumAnesthesiaMachineOxygenSource getOxygenSource()
	{
		return oxygenSource;
	}
	public void setOxygenSource(EnumAnesthesiaMachineOxygenSource oxygenSource)
	{
		this.oxygenSource = oxygenSource;
	}
	public boolean hasOxygenSource()
	{
		return oxygenSource == null ? false : true;
	}

	/*
	 * Positive End-Expired Pressure
	 */
	public SEScalarPressure getPositiveEndExpiredPressure()
	{
		if (positiveEndExpiredPressure == null)
			positiveEndExpiredPressure = new SEScalarPressure();
		return positiveEndExpiredPressure;
	}
	public boolean hasPositiveEndExpiredPressure()
	{
		return positiveEndExpiredPressure == null ? false : positiveEndExpiredPressure.isValid();
	}

	/*
	 * Anesthesia Machine Primary Gas
	 */
	public EnumAnesthesiaMachinePrimaryGas getPrimaryGas()
	{
		return primaryGas;
	}
	public void setPrimaryGas(EnumAnesthesiaMachinePrimaryGas primaryGas)
	{
		this.primaryGas = primaryGas;
	}
	public boolean hasPrimaryGas()
	{
		return primaryGas == null ? false : true;
	}

	/*
	 * Respiratory Rate
	 */
	public SEScalarFrequency getRespiratoryRate()
	{
		if (respiratoryRate == null)
			respiratoryRate = new SEScalarFrequency();
		return respiratoryRate;
	}
	public boolean hasRespiratoryRate()
	{
		return respiratoryRate == null ? false : respiratoryRate.isValid();
	}

	/*
	 * Relief Valve Pressure
	 */
	public SEScalarPressure getReliefValvePressure()
	{
		if (reliefValvePressure == null)
			reliefValvePressure = new SEScalarPressure();
		return reliefValvePressure;
	}
	public boolean hasReliefValvePressure()
	{
		return reliefValvePressure == null ? false : reliefValvePressure.isValid();
	}
	
	/*
	 * Ventilator Pressure
	 */
	public SEScalarPressure getVentilatorPressure()
	{
		if (ventilatorPressure == null)
			ventilatorPressure = new SEScalarPressure();
		return ventilatorPressure;
	}
	public boolean hasVentilatorPressure()
	{
		return ventilatorPressure == null ? false : ventilatorPressure.isValid();
	}

	/*
	 * Left Chamber
	 */
	public boolean hasLeftChamber()
	{
		return leftChamber == null ? false : true;
	}
	public SEAnesthesiaMachineChamber getLeftChamber()
	{
		if(leftChamber==null)
			this.leftChamber=new SEAnesthesiaMachineChamber();
		return leftChamber;
	}
	public void removeLeftChamber()
	{
		leftChamber = null;
	}

	/*
	 * Right Chamber
	 */
	public boolean hasRightChamber()
	{
		return rightChamber == null ? false : true;
	}
	public SEAnesthesiaMachineChamber getRightChamber()
	{
		if(rightChamber==null)
			this.rightChamber=new SEAnesthesiaMachineChamber();
		return rightChamber;
	}
	public void removeRightChamber()
	{
		rightChamber = null;
	}

	/*
	 * Oxygen Bottle One
	 */
	public SEAnesthesiaMachineOxygenBottle getOxygenBottleOne()
	{
		if (oxygenBottleOne == null)
			oxygenBottleOne = new SEAnesthesiaMachineOxygenBottle();

		return oxygenBottleOne;
	}

	public boolean hasOxygenBottleOne()
	{
		return oxygenBottleOne == null ? false : true;
	}

	/*
	 * Oxygen Bottle Two
	 */
	public SEAnesthesiaMachineOxygenBottle getOxygenBottleTwo()
	{
		if (oxygenBottleTwo == null)
			oxygenBottleTwo = new SEAnesthesiaMachineOxygenBottle();

		return oxygenBottleTwo;
	}

	public boolean hasOxygenBottleTwo()
	{
		return oxygenBottleTwo == null ? false : true;
	}
	
	public String toString()
	{
		String leftChamber = "Left Chamber: NotProvided";
		if(hasLeftChamber())
		{
			leftChamber = getLeftChamber().toString();
			leftChamber = leftChamber.replaceAll("Chamber", "Left Chamber");
		}
		String rightChamber = "Right Chamber: NotProvided";
		if(hasRightChamber())
		{
			rightChamber = getRightChamber().toString();
			rightChamber = rightChamber.replaceAll("Chamber", "Right Chamber");
		}
		String o2BottleOne = "Oxygen Bottle One: NotProvided";
		if(hasOxygenBottleOne())
		{
			o2BottleOne = getOxygenBottleOne().toString();
			o2BottleOne = o2BottleOne.replaceAll("Bottle", "Bottle One");
		}
		String o2BottleTwo = "Oxygen Bottle Two: NotProvided";
		if(hasOxygenBottleTwo())
		{
			o2BottleTwo = getOxygenBottleTwo().toString();
			o2BottleTwo = o2BottleTwo.replaceAll("Bottle", "Bottle Two");
		}
		
			return "Anesthesia Machine"
			+ "\n\tConnection: " + (hasConnection()?getConnection():"NotProvided")
			+ "\n\tInletFlow: " + getInletFlow()
			+ "\n\tInspiratoryExpiratoryRatio: " + getInspiratoryExpiratoryRatio()
			+ "\n\tOxygenFraction: " + getOxygenFraction()
			+ "\n\tOxygenSource: " + (hasOxygenSource()?getOxygenSource():"NotProvided")
			+ "\n\tPositiveEndExpiredPressure: " + getPositiveEndExpiredPressure()
			+ "\n\tPrimaryGas: " + (hasPrimaryGas()?getPrimaryGas():"NotProvided")
			+ "\n\tRespiratoryRate: " + getRespiratoryRate()
			+ "\n\tReliefValvePressure: " + getReliefValvePressure()
			+ "\n\tVentilatorPressure: " + getVentilatorPressure()
			+ "\n\t" + leftChamber
			+ "\n\t" + rightChamber
			+ "\n\t" + o2BottleOne
			+ "\n\t" + o2BottleTwo;
	}
}