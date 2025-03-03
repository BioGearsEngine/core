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
#include <biogears/cdm/enums/SEPatientActionsEnums.h>
#include <biogears/cdm/properties/SEScalarFlowResistance.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>

#include <random>

namespace biogears {
namespace io {
	class PatientActions;
}
class BIOGEARS_API SEAmputation : public SEPatientAction {
	friend io::PatientActions;


public:
	SEAmputation();
	virtual ~SEAmputation() override;

	static constexpr const char* TypeTag() { return "SEAmputation"; };
	const char* classname() const override { return TypeTag(); }

	virtual void Invalidate() override; // clear memory

	virtual bool IsValid() const override;

	virtual const char* GetCompartment_cStr() const;
	virtual std::string GetCompartment() const;
	virtual bool HasCompartment() const;
	virtual void SetCompartment(const char* name);
	virtual void SetCompartment(const std::string& name);
	virtual void InvalidateCompartment();
 
	virtual bool HasBleedResistance() const;
	virtual SEScalarFlowResistance& GetBleedResistance();
    virtual SEScalarFlowResistance const& GetBleedResistance() const;
	
	virtual const char* GetWeightPercent_cStr() const;
    virtual std::string GetWeightPercent() const;
	//virtual bool HasWeightPercent() const;
    virtual void SetWeightPercent(const char* name);
    virtual void SetWeightPercent(const std::string& name);
    //virtual void InvalidateWeightPercent();
 
	virtual void ToString(std::ostream& str) const override;

	bool operator==(const SEAmputation& rhs) const;
	bool operator!=(const SEAmputation& rhs) const;

protected:
	std::string m_Compartment;
    SEScalarFlowResistance* m_BleedResistance;
	std::string m_WeightPercent;
};
}

