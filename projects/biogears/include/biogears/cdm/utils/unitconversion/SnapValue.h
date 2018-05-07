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
/// @file SnapValue.h
/// @author Chris Volpe
/// 
// This class acts like an ordinary double precision floating point value, except
// that it "snaps" to the nearest integer whenever it is close enough to an
// integer that we can presume that the difference is due to rounding error. 
// The purpose of having this is that we want to be able to represent fractional
// exponents of a unit symbol in a CompoundUnitElement, but we want to make sure
// that when doing conversions to other units, we don't spuriously find a mismatch
// in the corresponding UnitDimension objects of the two CompoundUnit objects, simply
// because, for example, one has a meters exponent of 1.0 and the other has a
// meters exponent of 0.9999998 due to some roundabout computation in which we
// took the square root of an acre. 
//----------------------------------------------------------------------------
#pragma once
#include <float.h>

// The use of FLT_EPSILON instead of
// DBL_EPSILON is just in case our values are the result of some
// funky compiler intrinsic that does stuff in single precision. This should
// be fine because the only thing we're trying to do, really, is represent
// rational numbers of the form [m / n] where n is small, e.g. 1/2, 1/3, etc.
// the scale factor (16) is to account for the fact that the rounding error
// will be larger for values with a larger integer portion (due to the finite
// number of mantissa bits). Ideally, we should scale-up FLT_EPSILON by the 
// size of the value being snapped, and compare that to the error, but then we
// have to do that extra multiplication even though most values will be within 
// small range anyway, and we'd have to consider when the original value is close 
// to zero, the multiplication by FLT_EPSILON might underflow. I don't think it
// will, but I don't trust my intuition on IEEE 754.
#define SNAP_TOLERANCE (FLT_EPSILON * 16.0)

class CSnapValue
{
private:
	// A negating constructor for use by unary-minus only. Add in a dummy arg
	// so that the compiler can resolve to the correct constructor. This
	// skips the snapping because we're negating the value from an existing
	// CSnapValue
	CSnapValue(const CSnapValue &src, bool dummyflag)
		:m_dVal(-src.m_dVal)
	{
		// nothing
	}

public:
	// Make it look, smell, and act like a double

	// Constructors
	CSnapValue()
	{
		m_dVal = 0.0;
	}

	CSnapValue(const CSnapValue &src)
		:m_dVal(src.m_dVal)
	{
		// nothing
	}

	CSnapValue(double val)
		:m_dVal(val)
	{
		Snap();
	}

	CSnapValue(float val)
		:m_dVal(val)
	{
		Snap();
	}

	CSnapValue(int val)
		:m_dVal(val)
	{
		// No need to snap if we're initializing with an int
	}

	operator double() const
	{
		return m_dVal;
	}

	// Assignment and arithmetic operators
	// See Myers, "More Effective C++", Addison-Wesley
	CSnapValue & operator=(const CSnapValue &rhs)
	{
		m_dVal = rhs.m_dVal;
		return *this;
	}

	CSnapValue & operator*=(const CSnapValue &rhs)
	{
		m_dVal *= rhs.m_dVal;
		Snap();
		return *this;
	}

	const CSnapValue operator*(const CSnapValue &rhs) const
	{
		return CSnapValue(*this) *= rhs;
	}

	CSnapValue & operator/=(const CSnapValue &rhs)
	{
		m_dVal /= rhs.m_dVal;
		Snap();
		return *this;
	}

	const CSnapValue operator/(const CSnapValue &rhs) const
	{
		return CSnapValue(*this) /= rhs;
	}

	CSnapValue & operator+=(const CSnapValue &rhs)
	{
		m_dVal += rhs.m_dVal;
		Snap();
		return *this;
	}

	const CSnapValue operator+(const CSnapValue &rhs) const
	{
		return CSnapValue(*this) += rhs;
	}

	const CSnapValue operator+() const
	{
		return CSnapValue(*this);
	}

	CSnapValue & operator-=(const CSnapValue &rhs)
	{
		m_dVal -= rhs.m_dVal;
		Snap();
		return *this;
	}

	const CSnapValue operator-(const CSnapValue &rhs) const
	{
		return CSnapValue(*this) -= rhs;
	}

	const CSnapValue operator-() const
	{
		// Invoke the special negating constructor!!!
		return CSnapValue(*this,true);
	}

	// Increment and decrement
	// prefix
	CSnapValue & operator++()
	{
		*this += CSnapValue(1.0);
		return *this;
	}

	CSnapValue & operator--()
	{
		*this -= CSnapValue(1.0);
		return *this;
	}

	// postfix
	const CSnapValue operator++(int)
	{
		CSnapValue oldValue = *this;
		++(*this);
		return oldValue;
	}

	const CSnapValue operator--(int)
	{
		CSnapValue oldValue = *this;
		--(*this);
		return oldValue;
	}

	const double &GetValue() const
	{
		return m_dVal;
	};


	// Relational operators

	// http://support.microsoft.com/kb/168958 says we need to define operators < and ==
	// for this if we want to export the vector of these contained in UnitDimension. 
	bool operator< (const CSnapValue &rhs) const
	{
		return (m_dVal < rhs.m_dVal);
	}

	bool operator== (const CSnapValue &rhs) const
	{
		return  (m_dVal == rhs.m_dVal);
	}
	
	bool operator!=(const CSnapValue &rhs) const
	{
		return (m_dVal != rhs.m_dVal);
	}

	bool operator>(const CSnapValue &rhs) const
	{
		return (m_dVal > rhs.m_dVal);
	}

	bool operator<=(const CSnapValue &rhs) const
	{
		return (m_dVal <= rhs.m_dVal);
	}

	bool operator>=(const CSnapValue &rhs) const
	{
		return (m_dVal >= rhs.m_dVal);
	}

	// Provide versions of relational operators that take other types to avoid 
	// method resolution ambiguity 
	bool operator< (const double &rhs) const
	{
		return (m_dVal < rhs);
	}

	bool operator== (const double &rhs) const
	{
		return  (m_dVal == rhs);
	}

	bool operator!=(const double &rhs) const
	{
		return (m_dVal != rhs);
	}

	bool operator>(const double &rhs) const
	{
		return (m_dVal > rhs);
	}

	bool operator<=(const double &rhs) const
	{
		return (m_dVal <= rhs);
	}

	bool operator>=(const double &rhs) const
	{
		return (m_dVal >= rhs);
	}

	bool operator< (const int &rhs) const
	{
		return (m_dVal < rhs);
	}

	bool operator== (const int &rhs) const
	{
		return  (m_dVal == rhs);
	}

	bool operator!=(const int &rhs) const
	{
		return (m_dVal != rhs);
	}

	bool operator>(const int &rhs) const
	{
		return (m_dVal > rhs);
	}

	bool operator<=(const int &rhs) const
	{
		return (m_dVal <= rhs);
	}

	bool operator>=(const int &rhs) const
	{
		return (m_dVal >= rhs);
	}

protected:
	void Snap()
	{

		// Need to be careful here, because the int and frac portions
		// that modf returns are THE SAME SIGN, and we need to snap
		// when the fractional value is close to 0 or to +/- 1. 
		double intval;
		double bump = 1.0;
		double frac = modf(m_dVal,&intval);
		if (frac < 0)
		{
			frac = -frac;
			bump = -1.0;
		}

		// frac is positive now. We need to snap differently
		// depending on whether it's close to zero or close to 1
		if (frac < 0.5)
		{
			// See if it's close to zero
			if (frac < SNAP_TOLERANCE)
			{
				m_dVal = intval;
			}
		}
		else
		{
			// See if it's close to one
			if ((1.0 - frac) < SNAP_TOLERANCE)
			{
				m_dVal = intval + bump;
			};
		}

	}

private:
	double m_dVal;
};

inline double pow(double x, const CSnapValue &y)
{
	return pow(x,y.GetValue());
}
