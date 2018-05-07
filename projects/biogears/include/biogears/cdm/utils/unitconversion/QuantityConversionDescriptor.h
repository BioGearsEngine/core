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
/// @file QuantityConversionDescriptor.h
/// @author Chris Volpe
/// 
/// This class contains a representation of the formula for converting values
/// of one quantity type to values of another quantity type. 
//----------------------------------------------------------------------------
#pragma once

class CQuantityConversionDescriptor
{
public:
	CQuantityConversionDescriptor(double fromExp, const std::string & mappingUnit);

	~CQuantityConversionDescriptor();

	void Validate(int fromQTID, int toQTID);
	
	const double &GetFromExponent() const
	{
		return m_dFromExponent;
	}

	const CCompoundUnit *GetMappingUnit() const
	{
		return m_CUMappingUnit;
	}

private:
	// What we raise the original quantity to
	double m_dFromExponent;

	// Mapping Unit Identifier
	CCompoundUnit *m_CUMappingUnit;
};