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
/// @file QuantityConversionKey.h
/// @author Chris Volpe
/// 
/// This class contains a representation of a key that can be used in a hash table
/// for looking up Quantity Conversion Descriptors.
//----------------------------------------------------------------------------

#pragma once

class CQuantityConversionKey
{
public:
	// Construct from two Quantity Type IDs
	// This is called by the code that parses the initialization file and
	// creates the hash map entry
	CQuantityConversionKey(int fromId, int toID);

	// Construct from two UnitDimension pointers
	CQuantityConversionKey(const CUnitDimension *fromDim, const CUnitDimension *toDim)
		:m_CUDfromDim(fromDim), m_CUDtoDim(toDim)
	{
		// Do nothing
	}

	// override less-than operator to facilitate default hash comparison function
	bool operator< (const CQuantityConversionKey &rhs) const;

	// For the sake of completeness only, implement the other relational operators
	bool operator>(const CQuantityConversionKey &rhs) const
	{
		return rhs < *this;
	}

	bool operator <=(const CQuantityConversionKey &rhs) const
	{
		return !(*this > rhs);
	}

	bool operator >=(const CQuantityConversionKey &rhs) const
	{
		return !(*this < rhs);
	}

	bool operator ==(const CQuantityConversionKey &rhs) const;

	bool operator !=(const CQuantityConversionKey &rhs) const
	{
		return ! (*this == rhs);
	}


	// Compute a hash value from an array of the two hash values of the From and
	// To Dimension objects
	size_t hash_value() const;

private:
	// Regardless of how we're constructed, these two CUnitDimension pointers
	// are "owned" by other objects. In the case of the one physically stored in
	// the hash table, the pointers are owned by the QuantityTypeDescriptor objects 
	// referenced by name in the initializaiton file. In the case of one constructed
	// in the course of hash table lookup, it's owned by the compound unit objects 
	// involved in the type conversion. 
	const CUnitDimension *m_CUDfromDim;
	const CUnitDimension *m_CUDtoDim;
};

// Overload non_member hash_value on CQuantityConversionKey so that the
// templated hash_compare used by hash_map can call it. 
inline size_t hash_value(const CQuantityConversionKey &ref)
{
	return ref.hash_value();
}

namespace std 
{
	template<>
	struct hash<CQuantityConversionKey>
	{
		size_t operator()(const CQuantityConversionKey& ref) const
		{
			return ref.hash_value();
		}
	};
}


