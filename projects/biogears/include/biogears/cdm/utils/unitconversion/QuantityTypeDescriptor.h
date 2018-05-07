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

//----------------------------------------------------------------------------
/// @file QuantityTypeDescriptor.h
/// @author Chris Volpe
/// 
/// This class contains a representation of a quantity type, which can be either
/// a fundamental quantity or a derived quantity
//----------------------------------------------------------------------------
#pragma once

class CCompoundUnit;
class CUnitDimension;

class CQuantityTypeDescriptor
{
public:
	// Ctor for a fundamental quantity
	CQuantityTypeDescriptor(const std::string &name, int fundIdx, bool twentyLog);

	// Ctor for a derived quantity
	CQuantityTypeDescriptor(const std::string &name, CCompoundUnit *expansion, bool twentyLog);

	// Need dtor to free the expansion
	~CQuantityTypeDescriptor();

	// Return the name of this quantity type
	const std::string & GetName() const
	{
		return m_sName;
	}

	// Return the "fundamental index". This value is a unique monotonically-increasing
	// numeric identifier given to each quantity type that designates a fundamental
	// (such as time or mass), rather than derived (such as force or energy), quantity.
	// For derived quantities, the value is -1. 
	int GetFundIdx() const
	{
		return m_iFundIdx;
	}

	// Return the expansion of a derived quantity type as a compound unit. The expansion
	// defines the "base unit" (whether or not it has an explicit name) of this quantity
	// in terms of a composition of units of other quantities. 
	const CCompoundUnit *GetExpansion() const
	{
		return m_CUExpansion;
	};

	const CUnitDimension *GetDimension() const
	{
		return m_CUD;
	}

	double GetBigness() const
	{
		return m_dBigness;
	};

	bool Is20LogRuleQuantity() const
	{
		return m_bTwentyLogRule;
	}



	// disable the copy constructor so that I don't accidentally copy a descriptor
	// when I really intended to initialize a reference to one. I made this mistake
	// leaving off the '&' on a ref that was initialized with the result of
	// UnitConversionEngine::GetQuantityTypeDescriptor(), thereby invoking the
	// copy constructor with bitwise copies on our internal pointer objects, and later
	// called out dtor when my would-be ref went out of scope, which freed objects that
	// were still in use. Ugh.
private:
	CQuantityTypeDescriptor(const CQuantityTypeDescriptor&);

private:
	double m_dBigness;
	std::string m_sName;
	int m_iFundIdx;
	CCompoundUnit *m_CUExpansion;
	CUnitDimension *m_CUD;
	bool m_bTwentyLogRule;
};
