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

#include"Calculator.h"
#include <iostream>
#include <sstream>
#include <gtest/gtest.h>

TEST(CalculatorTest, AdditionTest)
{
	double _A = 5;
	double _B = 4;

	double _Expected = 9;

	Calculator *Calc = new Calculator;

	double _Actual = Calc->Add(_A, _B);

	EXPECT_DOUBLE_EQ(_Actual, _Expected);

}

TEST(CalculatorTest, SubtractionTest)
{
	double _A = 5;
	double _B = 4;

	double _Expected = 1;

	Calculator *Calc = new Calculator;

	double _Actual = Calc->Subtract(_A, _B);

	EXPECT_DOUBLE_EQ(_Actual, _Expected);

}

TEST(CalculatorTest, MultiTest)
{
	double _A = 5;
	double _B = 4;

	double _Expected = 20;

	Calculator *Calc = new Calculator;

	double _Actual = Calc->Multiply(_A, _B);

	EXPECT_DOUBLE_EQ(_Actual, _Expected);

}

TEST(CalculatorTest, DivisionTest)
{
	double _A = 5;
	double _B = 4;

	double _Expected = _A/_B;

	Calculator *Calc = new Calculator;

	double _Actual = Calc->Divide(_A, _B);

	EXPECT_DOUBLE_EQ(_Actual, _Expected);

}


class TestableCalculator : Calculator
{
	public:
		using Calculator::Plus;
		using Calculator::Minus;
		using Calculator::Quotient;
		using Calculator::Star;
};

TEST(CalculatorTest, PlusTest)
{
	double _A = 5;
	double _B = 4;

	double _Expected = 9;

	TestableCalculator *Calc = new TestableCalculator;

	double _Actual = Calc->Plus(_A, _B);
	
	ASSERT_EQ(_Expected, _Actual);
}

TEST(CalculatorTest, MinusTest)
{
	double _A = 5;
	double _B = 4;

	double _Expected= 1;

	TestableCalculator *Calc = new TestableCalculator;

	double _Actual = Calc->Minus(_A, _B);

	ASSERT_EQ(_Expected, _Actual);
}

TEST(CalculatorTest, QuotientTest)
{
	double _A = 5;
	double _B = 4;

	double _Expected = 5./4.;

	TestableCalculator *Calc = new TestableCalculator;

	double _Actual = Calc->Quotient(_A, _B);

	ASSERT_EQ(_Expected, _Actual);
}

TEST(CalculatorTest, StarTest)
{
	double _A = 5;
	double _B = 4;

	double _Expected = 20;

	TestableCalculator *Calc = new TestableCalculator;

	double _Actual = Calc->Star(_A, _B);

	ASSERT_EQ(_Expected, _Actual);
}