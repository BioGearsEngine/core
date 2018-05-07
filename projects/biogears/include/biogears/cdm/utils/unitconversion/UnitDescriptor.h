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
/// @file UnitDescriptor.h
/// @author Chris Volpe
///
/// This class encapsulates the definition of a particular unit symbol
//----------------------------------------------------------------------------
#pragma once

class CUnitDescriptor
{
public:
	// Defines what kind of constraint we impose on prefixes for this unit.
	typedef enum {NONE, SOME, ALLBUT, ALL} PrefixModeType;

	// ctor
	CUnitDescriptor(const std::string &name, const std::string &symbol, unsigned int quantityTypeId, const std::string &targetUnit,
		const double &convFac, const double &bias, PrefixModeType mode, const std::string &prefixSet);
	
	bool IsPrefixAllowed(char prefix) const;

	const double & GetConvFac() const
	{
		return m_dConvFac;
	}

	const double & GetBias() const
	{
		return m_dBias;
	}

	const std::string &GetName() const
	{
		return m_strName;
	}

	const std::string &GetSymbol() const
	{
		return m_strSymbol;
	}

	unsigned int GetQuantityTypeID() const
	{
		return m_uiQuantityTypeId;
	}

	bool IsDecibel() const
	{
		return m_bDBFlag;
	}

private:
	double m_dConvFac;
	double m_dBias;
	std::string m_strName;
	std::string m_strSymbol;
	unsigned int m_uiQuantityTypeId;
	PrefixModeType m_uiPrefixMode;
	std::string m_strPrefixSet;
	bool m_bDBFlag;

};