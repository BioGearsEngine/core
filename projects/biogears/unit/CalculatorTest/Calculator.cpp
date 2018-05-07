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

#include "Calculator.h"

double Calculator::Add(double a, double b)
{
	return Plus(a, b);
}

double Calculator::Plus(double a, double b)
{
	return a + b;
}

double Calculator::Subtract(double a, double b)
{
	return Minus(a, b);
}

double Calculator::Minus(double a, double b)
{
	return a - b;
}

double Calculator::Multiply(double a, double b)
{
	return Star(a, b);
}

double Calculator::Star(double a, double b)
{
	return a * b;
}

double Calculator::Divide(double a, double b)
{
	return Quotient(a, b);
}

double Calculator::Quotient(double a, double b)
{
	return a / b;
}

void Calculator::Save(double a)
{
	m_Saved = a;
}

double Calculator::GetSaved()
{
	return m_Saved;
}