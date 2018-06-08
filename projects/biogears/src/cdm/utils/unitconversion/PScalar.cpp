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
/// @file PScalar.cpp
/// @author Chris Volpe
///
/// PScalar is short for Physics Scalar. It is an encapsulation of a
/// double precision floating point value and a CompoundUnit object.
/// It allows all the expected arithmetic operations, as well as the
/// functions pow() for raising to a power, and sqrt() for computing the
/// square root.
//----------------------------------------------------------------------------

#include <biogears/cdm/stdafx.h>
#include <biogears/cdm/utils/unitconversion/UCCommon.h>

// We need one translation unit that processes the header file in
// this DLL or else the methods won't be available in the Debug build,
// since inline function optimization isn't done

// Technically, we don't need any more than this, but let's insert some
// dummy code here to exercise some of the functionality at compile time.
// I've run into a few situations where enhancements I've made to the API,
// particularly for PScalars, resulted in overload-resolution issues when
// clients attempt to use them. So let's try to catch those issues here so that
// we don't have a situation where this library builds, but a client won't.

/*
static void TestMyAPI()
{
	CPScalar s1("g"), s2(s1);
	s1.ConvertTo(s2);
}
*/