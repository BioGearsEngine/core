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
/// @file CompoundUnit.cpp
/// @author Chris Volpe
//----------------------------------------------------------------------------
#include <biogears/cdm/utils/Logger.h>
#include <biogears/cdm/utils/unitconversion/CompoundUnit.h>
#include <biogears/cdm/utils/unitconversion/UnitConversionEngine.h>
#include <biogears/cdm/utils/unitconversion/UnitStringLexer.h>
//----------------------------------------------------------------------------
namespace biogears {
// Default ctor
CCompoundUnit::CCompoundUnit()
  : m_CUD(nullptr)
  , m_dBigness(1.0)
  , m_bStaleBigness(true)
  , m_bStaleDimension(true)
  , m_bDBFlag(false)
  , m_bExplicitDBFlag(false)
  , m_bExplicitNonDBFlag(false)
  , m_strUnit (""){};
//----------------------------------------------------------------------------
// Construct directly from a unit string specification
CCompoundUnit::CCompoundUnit(const char* unitString)
  : CCompoundUnit(std::string{ unitString })
{
}
//----------------------------------------------------------------------------
// Construct directly from a unit string specification
CCompoundUnit::CCompoundUnit(const std::string& unitString)
  : m_CUD(nullptr)
  , m_dBigness(1.0)
  , m_bStaleBigness(true)
  , m_bStaleDimension(true)
  , m_bDBFlag(false)
  , m_bExplicitDBFlag(false)
  , m_bExplicitNonDBFlag(false)
  , m_strUnit("")
{
  ParseString(unitString);
}
//----------------------------------------------------------------------------
// Copy ctor
CCompoundUnit::CCompoundUnit(const CCompoundUnit& src) // Copy constructor
  : m_CUEVec(src.m_CUEVec)
  , m_dBigness(src.m_dBigness)
  , m_bStaleBigness(src.m_bStaleBigness)
  , m_strUnit(src.m_strUnit)
  , m_bStaleDimension(src.m_bStaleDimension)
  , m_bDBFlag(src.m_bDBFlag)
  , m_bExplicitDBFlag(src.m_bExplicitDBFlag)
  , m_bExplicitNonDBFlag(src.m_bExplicitNonDBFlag)
{
  // In the initializer list, I'm assuming that initializing one vector with another
  // copies the vector correctly. For the CUnitDimension object, we can't just
  // initialize our pointer with the src's pointer, because each CCompoundUnit owns its
  // CUnitDimension object, so we need to invoke the copy constructor on the
  // CUnitDimension. Unfortunately, since the pointer might be nullptr, we can't simply
  // do this in the initializer list.
  if (src.m_CUD == nullptr) {
    m_CUD = nullptr;
  } else {
    m_CUD = new CUnitDimension(*src.m_CUD);
  }
}
//----------------------------------------------------------------------------
void CCompoundUnit::ParseString(const char* unitString)
{
  ParseString(std::string{ unitString });
}
//----------------------------------------------------------------------------
void CCompoundUnit::ParseString(const std::string& unitString)
{
  // Start with a clean slate
  clear();

  CUnitConversionEngine& uce = CUnitConversionEngine::GetEngine();

  CUnitStringLexer parser(unitString);
  SUnitStringToken token, tmpToken;
  bool inDenominator = false;
  bool gotSomethingInNumerator = false;
  bool gotOneInNumerator = false;
  bool dBIsAcceptable = true;
  bool error = false;
  CCompoundUnitElement::ExponentType exponent, expDivisor;
  int prefixID, unitID;
  DEBUGOUT(std::cerr << "Parsing [" << unitString << "]\n";)
  while ((parser.GetNextToken(token), token.tokenID != SUnitStringToken::TT_EOF)) {
    switch (token.tokenID) {
    case SUnitStringToken::TT_DB:
      // Make sure the dB token is acceptable in this context. It's not acceptable
      // if we've already encountered some unit symbols
      if (dBIsAcceptable) {
        m_bExplicitDBFlag = true;
        m_bDBFlag = true;
        dBIsAcceptable = false;
      } else {
        std::cerr << "Parse error: dB token is not acceptable in this context" << std::endl;
        error = true;
      }
      break;

    case SUnitStringToken::TT_SLASH:
      inDenominator = true;
      DEBUGOUT(std::cerr << "Got token slash \n";)
      break;

    case SUnitStringToken::TT_FLOAT: {

      if (inDenominator) {
        std::cerr << "Parse error: Unexpected TT_FLOAT in denominator" << std::endl;
        break;
      }

      if (token.fltval != 1.0) {
        std::cerr << "Parse error: Only numeric value allowed as a unit symbol in numerator is '1'" << std::endl;
        break;
      }

      if (gotOneInNumerator) {
        std::cerr << "Parse error: Numeric value '1' allowed only once in numerator" << std::endl;
        break;
      }

      if (gotSomethingInNumerator) {
        std::cerr << "Parse error: Numeric value '1' must be sole unit in numerator" << std::endl;
      }

      gotOneInNumerator = true;

      break;
    }

    case SUnitStringToken::TT_IDENTIFIER:
      // Ugh, need extra level of braces to shut up compiler about
      // variable initialization here being skipped by subsequent case labels.
      {
        if (!inDenominator) {
          gotSomethingInNumerator = true;

          if (gotOneInNumerator) {
            std::cerr << "Parse error: Numeric value '1' must be sole unit in numerator" << std::endl;
            error = true;
            break;
          }
        }

        DEBUGOUT(std::cerr << "Got identifier token [" << token.symbol << "]\n";)
        // Before figuring out what to do with this identifier, peek
        // ahead and see if we have an exponent to parse. Use a different
        // token structure so we don't corrupt the identifier
        parser.PeekNextToken(tmpToken);
        DEBUGOUT(std::cerr << "Peeking at token " << tmpToken.toString() << std::endl;)
        exponent = 1.0;
        expDivisor = 1.0;
        if (tmpToken.tokenID == SUnitStringToken::TT_EXPONENT) {
          // We may or may not have parentheses around the exponent. Parens are
          // needed if there is a division expression. (Hopefully this won't get
          // any more complicated, or I may have to use Flex and Bison.)
          bool matchParens = false;

          // We've got an exponent
          // Skip over the token we just peeked at
          parser.GetNextToken(tmpToken);
          DEBUGOUT(std::cerr << "Got token " << tmpToken.toString() << std::endl;)

          // Do we have a parenthesized expression?
          parser.PeekNextToken(tmpToken);
          if (tmpToken.tokenID == SUnitStringToken::TT_PARENOPEN) {
            DEBUGOUT(std::cerr << "Got token " << tmpToken.toString() << std::endl;)
            matchParens = true;
            // Skip over the paren
            parser.GetNextToken(tmpToken);
          }

          // Now make sure the next token is a float
          parser.GetNextToken(tmpToken);
          if (tmpToken.tokenID != SUnitStringToken::TT_FLOAT) {
            std::cerr << "Parse error. Expecting TT_FLOAT, got " << tmpToken.toString();
            if (tmpToken.tokenID == SUnitStringToken::TT_IDENTIFIER) {
              DEBUGOUT(cerr << "(" << tmpToken.symbol << ")";)
            }
            DEBUGOUT(std::cerr << std::endl;)
            error = true;
            continue;
          }

          // Ok, get the floating point value. This is our exponent unless there's a fraction
          exponent = tmpToken.fltval;

          if (matchParens) {
            // If we're parsing a parenthesized expression,
            // peek at the next token to see if there's
            // a slash indicating a fractional exponent;
            parser.PeekNextToken(tmpToken);
            if (tmpToken.tokenID == SUnitStringToken::TT_SLASH) {
              // We've got a rational exponent of the form "a/b", so
              // grab "b" and do the division.

              // Skip over the token we just peeked at
              parser.GetNextToken(tmpToken);

              // Now make sure the next token is a float
              parser.GetNextToken(tmpToken);
              if (tmpToken.tokenID != SUnitStringToken::TT_FLOAT) {
                DEBUGOUT(std::cerr << "Parse error. Expecting TT_FLOAT, got " << tmpToken.toString() << std::endl;)
                error = true;
                continue;
              }

              // Ok, get the floating point value. This is the divisor of our exponent;
              expDivisor = tmpToken.fltval;
              exponent /= expDivisor;
            } // exponent has a divisor

            // Need a close paren here
            parser.GetNextToken(tmpToken);
            if (tmpToken.tokenID != SUnitStringToken::TT_PARENCLOSE) {
              DEBUGOUT(std::cerr << "Parse error. Expecting TT_PARENCLOSE, got " << tmpToken.toString() << std::endl;)
              error = true;
              continue;
            }
          } // if we're matching parens
        } // symbol has an explicit exponent

        // If we're past a slash in the compound unit, we're parsing unit
        // elements in the denominator, so negate the exponent
        if (inDenominator) {
          exponent *= -1.0;
        }

        // Now, we've got our exponent, so go back to examining the unit symbol
        bool rc;
        rc = uce.LookupFullUnit(token.symbol, unitID, prefixID);
        if (!rc) {
          std::cerr << "\"" << token.symbol << "\" is not a valid unit symbol\n";
          error = true;
          continue;
        }
        AddElement(CCompoundUnitElement(unitID, exponent, prefixID));

        break;
      }

    case SUnitStringToken::TT_ERR:
      DEBUGOUT(std::cerr << "Invalid token in unit string\n";)
      error = true;
      break;

    default:
      DEBUGOUT(std::cerr << "Unexpected token ID=" << token.toString() << " at this point in parse\n";)
      error = true;
      break;
    } // switch tokenID
  } // while more tokens
  if (!error) {
    m_strUnit = unitString;
    ComputeBigness();
    BuildDimension();
  } else {
    std::stringstream ss;
    ss << "CompoundUnt unable to parse unit string : " << unitString;
    throw std::runtime_error(ss.str().c_str());
  }

} // ParseString

//----------------------------------------------------------------------------
const CUnitDimension* CCompoundUnit::GetDimension() const
{
  if (m_bStaleDimension) {
    if (m_CUD != nullptr) {
      delete m_CUD;
      m_CUD = nullptr;
    }
    BuildDimension();
  }
  return m_CUD;
}

//----------------------------------------------------------------------------
const double& CCompoundUnit::GetBigness() const
{
  if (m_bStaleBigness) {
    ComputeBigness();
  }
  return m_dBigness;
}

//----------------------------------------------------------------------------
bool CCompoundUnit::IsDecibel() const
{
  // We update the decibel flag along with the bigness
  if (m_bStaleBigness) {
    ComputeBigness();
  }
  return m_bDBFlag;
}

//----------------------------------------------------------------------------
double CCompoundUnit::GetDecibelLogScaleFactor() const
{
  CUnitConversionEngine& uce = CUnitConversionEngine::GetEngine();
  double scalefac = 10;
  // See if there's a standard quantity type associated with this unit's dimension
  int qtId = uce.GetQuantityTypeID(*GetDimension());
  if (qtId != -1) {
    const CQuantityTypeDescriptor& qtd = uce.GetQuantityTypeDescriptor(qtId);
    if (qtd.Is20LogRuleQuantity()) {
      scalefac = 20.0;
    }
  }
  return scalefac;
}

//----------------------------------------------------------------------------
double CCompoundUnit::GetBias() const
{
  // Bias only relevant when dealing with single unit to power of 1. Note that
  // there is a certain amount of robustness that we are sacrificing for the sake
  // of performance here. Specifically, it's possible that a compound unit may have
  // dimensions of temperature with an exponent of 1, but still comprise other
  // CompoundUnitElement objects raised to the power of zero due to previous
  // CompoundUnit multiplications or divisions, or even have different units of the
  // same quantity type but with exponents that cancel each other out. Handling
  // this properly would require examining the result of GetDimension, ensuring that
  // each element (fundamental quantity index) had the value zero with the exception
  // of a single element (corresponding to a particular fundamental quantity) having
  // the value 1, and then searching for the CompoundUnitElement in our CompoundUnit
  // having that particular fundamental quantity index and retrieving that unit's
  // bias. And even if we do that, we still have the potential problem of not
  // finding such a compound unit element because we're actually composing the very units
  // that have the biases themselves. (E.g. what's the bias associated with a kelvin times
  // a degree Fahrenheit divided by a degree Celsius?). So, for now, we punt, and insist
  // that biases are only taken into consideration if the CompoundUnit contains only
  // a single biased unit all by its lonesome. Deal with it, baby.
  if (m_CUEVec.size() != 1) {
    return 0.0;
  }

  if (m_CUEVec[0].GetExponent() != 1.0) {
    return 0.0;
  }

  // Ok, return the bias associated with this element.
  // Note that we are not currently enforcing the rule that only
  // fundamental quantity type units can have biases, even though we
  // are assuming it.
  return m_CUEVec[0].GetBias();
}

//----------------------------------------------------------------------------
void CCompoundUnit::BuildDimension() const
{
  m_CUD = new CUnitDimension();
  int uID;

  int qtID; // quantity type index
  int qtFID; // quantity type fundamental-quantity index
  CCompoundUnit const* qtExpansion;
  CUnitConversionEngine& uce = CUnitConversionEngine::GetEngine();

  for (auto CUERef = m_CUEVec.begin(); CUERef != m_CUEVec.end(); CUERef++) {
    CCompoundUnitElement::ExponentType const& exponent = CUERef->GetExponent();

    // Get the quantity type information associated with this element
    uID = CUERef->GetUnitID();
    CUnitDescriptor const& uDesc = uce.GetUnitDescriptor(uID);
    qtID = uDesc.GetQuantityTypeID();
    CQuantityTypeDescriptor const& qtDesc = uce.GetQuantityTypeDescriptor(qtID);

    if ((qtFID = qtDesc.GetFundIdx()) == -1) {
      // It's a derived quantity. Our exponent applies to the expansion of
      // the quantity type
      qtExpansion = qtDesc.GetExpansion();

      // We raise the expansion to our exponent by multiplying each exponent
      // in its dimension by our exponent. That value gets added to our dimension
      CUnitDimension const* expDim = qtExpansion->GetDimension();
      size_t expDimSize = expDim->size();
      for (size_t fqtIdx = 0; fqtIdx < expDimSize; ++fqtIdx) {
        (*m_CUD)[fqtIdx] += exponent * (*expDim)[fqtIdx];
      }
    } else {
      // It's a fundamental quantity
      // The fundamental index is the one to which our current exponent applies, so
      // add the current exponent to that element
      (*m_CUD)[qtFID] += exponent;
    }
  }
  m_bStaleDimension = false;
}

//----------------------------------------------------------------------------
void CCompoundUnit::ComputeBigness() const
{
  m_dBigness = 1.0;
  bool computeDBFlag = false;
  // Only compute the decibel flag if we need to
  if (m_bExplicitDBFlag) {
    m_bDBFlag = true;
  } else if (m_bExplicitNonDBFlag) {
    m_bDBFlag = false;
  } else {
    computeDBFlag = true;
  }

  if (computeDBFlag) {
    m_bDBFlag = false;
    for (auto CUERef = m_CUEVec.begin(); CUERef != m_CUEVec.end(); CUERef++) {
      m_dBigness *= CUERef->GetBigness();

      // I have no idea WTF I'm doing here. Should the decibel flag be on
      // if *any* of the individual Compound Unit Elements has the flag set?
      // Or only if *all* of them have the flag set? What does it mean if
      // some of them have it set and some don't? Should that signify an error
      // condition? If I have a unit called "dba" that I've defined as "1 dB A"
      // (an ampere, stored as a decibel) and then I create a compound unit "dba s"
      // (dimensionally, charge), what is that? Is it meaningful? What if I create
      // another unit "dbs" and I define it as "1 dB s". And then what if I create
      // another compound unit "dba dbs"? Is that different from a "dba s"? And is
      // either of them the same as a "dB C" (decibel Coulombs) I have no
      // f$@%ing clue. And I haven't even *considered* the whole "20 log rule" can
      // of worms. This code used to be clean and elegant, a textbook example of a
      // well-thought-out C++ design process. Now it's a giant clusterf&@k due to
      // this damn decibel bull$#!t. Anyway, I've decided to logical-or the individual
      // decibel flags together. Why? I don't know. Should it matter? Probably not.
      // Any arithmetic done on compound units should be done by converting to
      // non-decibels first anyway (and I'm going to enforce that in PScalar), so the
      // only issue should be with contrived compound units like "dba s", and
      // whoever writes that deserves what (s)he gets.
      m_bDBFlag |= CUERef->IsDecibel();
    }
  } else {
    for (auto CUERef = m_CUEVec.begin(); CUERef != m_CUEVec.end(); CUERef++) {
      m_dBigness *= CUERef->GetBigness();
    }
  }
  m_bStaleBigness = false;
}

//----------------------------------------------------------------------------
CCompoundUnitElement& CCompoundUnit::AddElement(const CCompoundUnitElement& elem)
{
  m_CUEVec.push_back(elem);
  m_bStaleBigness = true;
  m_bStaleDimension = true;
  return m_CUEVec[m_CUEVec.size() - 1];
}

//----------------------------------------------------------------------------
CCompoundUnit& CCompoundUnit::operator*=(const CCompoundUnit& rhs)
{
  // Let's try to do this semi-intelligently. If we encounter
  // a compound unit element on the right hand side that denotes
  // the SAME UNIT (including prefix) as one on the left hand side, then add the
  // exponents rather than appending the CUE vector. This won't prevent having
  // more than one compound unit element with a unit of the same quantity
  // type (e.g. hours and seconds, which could only be consolidated by
  // introducing an additional fudge factor), but will help keep the
  // CUE vector from growing unnecessarily, though possibly at the expense
  // of some CPU cycles to check for matches. Note, however, that we're going
  // to be a bit lazy and not worry about removing elements that cancel
  // each other out completely. That is, we may have elements with a
  // zero exponent. This won't affect dimensional analysis, however.
  int rhsUnitID;
  int rhsPrefixID;

  // We can gain some efficiency by computing the new bigness and dimension
  // from the old ones if they're both valid, rather than recomputing them from
  // scratch the next time they're needed. Just do an upfront test, compute
  // the values, and store them back when we're done
  bool preserveBigness = (!m_bStaleBigness && !rhs.m_bStaleBigness);
  bool preserveDimension = (!m_bStaleDimension && !rhs.m_bStaleDimension);
  if (preserveBigness) {
    m_dBigness *= rhs.m_dBigness;
  }
  if (preserveDimension) {
    *m_CUD += *rhs.m_CUD;
  }

  // None of the composing operations below should actually modify either
  // the bigness or the dimension, so if we're preserving the composition of the
  // argument values, we can just clear the staleness flags at the end, despite the
  // fact that we set them in this loop.
  for (auto rightRef = rhs.m_CUEVec.begin(); rightRef != rhs.m_CUEVec.end(); rightRef++) {
    rhsUnitID = rightRef->GetUnitID();
    rhsPrefixID = rightRef->GetPrefixID();
    // Check for same unit in our side;
    bool foundMatch = false;
    for (auto leftRef = m_CUEVec.begin(); leftRef != m_CUEVec.end(); leftRef++) {
      if ((leftRef->GetUnitID() == rhsUnitID) && (leftRef->GetPrefixID() == rhsPrefixID)) {
        foundMatch = true;
        leftRef->AddExponent(rightRef->GetExponent());
        m_bStaleDimension = true;
        m_bStaleBigness = true;
        break;
      }
    }
    if (!foundMatch) {
      // No duplicate was found, so we need to add a new element
      AddElement(*rightRef); // this method sets the stale flags
    }
  }

  if (preserveBigness) {
    m_bStaleBigness = false;
  }
  if (preserveDimension) {
    m_bStaleDimension = false;
  }
  return *this;
};

//----------------------------------------------------------------------------
CCompoundUnit& CCompoundUnit::operator/=(const CCompoundUnit& rhs)
{
  // Let's try to do this semi-intelligently. If we encounter
  // a compound unit element on the right hand side that denotes
  // the SAME UNIT as one on the left hand side, then add the exponents
  // rather than appending the CUE vector. This won't prevent having
  // more than one compound unit element with a unit of the same quantity
  // type (e.g. hours and seconds, which could only be consolidated by
  // introducing an additional fudge factor), but will help keep the
  // CUE vector from growing unnecessarily, though possibly at the expense
  // of some CPU cycles to check for matches. Note, however, that we're going
  // to be a bit lazy and not worry about removing elements that cancel
  // each other out completely. That is, we may have elements with a
  // zero exponent. This won't affect dimensional analysis, however.
  int rhsUnitID;
  int rhsPrefixID;

  // We can gain some efficiency by computing the new bigness and dimension
  // from the old ones if they're both valid, rather than recomputing them from
  // scratch the next time they're needed. Just do an upfront test, compute
  // the values, and store them back when we're done
  bool preserveBigness = (!m_bStaleBigness && !rhs.m_bStaleBigness);
  bool preserveDimension = (!m_bStaleDimension && !rhs.m_bStaleDimension);
  if (preserveBigness) {
    m_dBigness /= rhs.m_dBigness;
  }
  if (preserveDimension) {
    *m_CUD -= *rhs.m_CUD;
  }

  // None of the composing operations below should actually modify either
  // the bigness or the dimension, so if we're preserving the composition of the
  // argument values, we can just clear the stalenessflags at the end, despite the
  // fact that we set them in this loop.
  for (auto rightRef = rhs.m_CUEVec.begin(); rightRef != rhs.m_CUEVec.end(); rightRef++) {
    rhsUnitID = rightRef->GetUnitID();
    rhsPrefixID = rightRef->GetPrefixID();
    // Check for same unit in our side;
    bool foundMatch = false;

    for (auto leftRef = m_CUEVec.begin(); leftRef != m_CUEVec.end(); leftRef++) {
      if ((leftRef->GetUnitID() == rhsUnitID) && (leftRef->GetPrefixID() == rhsPrefixID)) {
        foundMatch = true;
        leftRef->SubtractExponent(rightRef->GetExponent());
        m_bStaleDimension = true;
        m_bStaleBigness = true;
        break;
      }
    }
    if (!foundMatch) {
      // No duplicate was found, so we need to add a new element
      AddElement(*rightRef).Invert();
      // Add element sets the stale flags, but we need to be careful about calling
      // Invert on the element in any other context, because we would need to
      // set the stale flags explicitly. The only reason we don't need to do so here
      // is because there's no way the UnitDimension or Bigness can be calculated
      // and cached between the AddElement call and the Invert call on the added
      // element.
    }
  }

  if (preserveBigness) {
    m_bStaleBigness = false;
  }
  if (preserveDimension) {
    m_bStaleDimension = false;
  }
  return *this;
};

//----------------------------------------------------------------------------
CCompoundUnit& CCompoundUnit::Raise(CCompoundUnitElement::ExponentType exp)
{
  for (auto CUERef = m_CUEVec.begin(); CUERef != m_CUEVec.end(); CUERef++) {
    CUERef->MultExponent(exp);
  }

  if (!m_bStaleBigness) {
    m_dBigness = std::pow(m_dBigness, exp);
  }

  if (!m_bStaleDimension) {
    *m_CUD *= exp;
  }
  return (*this);
}

//----------------------------------------------------------------------------
bool CCompoundUnit::IsOfType(int quantityTypeID)
{
  CUnitConversionEngine& uce = CUnitConversionEngine::GetEngine();
  const CQuantityTypeDescriptor& qtDesc = uce.GetQuantityTypeDescriptor(quantityTypeID);
  int qtFID = qtDesc.GetFundIdx();
  if (qtFID == -1) {
    // It's a derived quantity, so all we need to do is check if our dimension is equal to
    // the dimension of its expansion
    return GetDimension() == qtDesc.GetExpansion()->GetDimension();
  } else {
    // It's a fundamental quantity. Ask our dimension object if it
    // represents this single fundamental quantity
    return GetDimension()->IsFundamentalQuantity(static_cast<size_t>(qtFID));
  }
}
//----------------------------------------------------------------------------
bool CCompoundUnit::IsOfType(const char* quantityName)
{
  return IsOfType(std::string{ quantityName });
}
//----------------------------------------------------------------------------
bool CCompoundUnit::IsOfType(const std::string& quantityName)
{
  CUnitConversionEngine& uce = CUnitConversionEngine::GetEngine();
  int qtID = uce.GetQuantityTypeID(quantityName);
  if (qtID == -1) {
    return false;
  } else {
    return IsOfType(qtID);
  }
}
//----------------------------------------------------------------------------
const char* CCompoundUnit::GetString() const
{
  return (m_strUnit.empty()) ? "N/A" : m_strUnit.c_str();
}
//----------------------------------------------------------------------------
std::ostream& CCompoundUnit::PrintSelf(std::ostream& output) const
{
  // Let's try to make this a "natural" representation. Let's try to take all the
  // negative exponent terms and place them after the "/" symbol with positive
  // exponents, and let's omit exponents equal to 1. That way, instead of
  // outputting "s^-2 kg^1 m^2", we'll output "kg m^2 / s^2".
  // We're going to be extra slick and print out negative exponents and
  // skip the "/" character if there are only divisor factors, and no dividend factors.

  CUnitConversionEngine& uce = CUnitConversionEngine::GetEngine();
  bool divisorFlag = false;
  bool dividendFlag = false;
  int uID, pID;

  // I was printing out a space *after* each element, but I was getting sick of
  // seeing the superfluous space that the end, so I'm going to use a flag to
  // print out a space *before* each element, except for the first element
  bool printedFirstElement = false;

  // Before examining any of the elements, check if we're in decibel mode
  if (IsDecibel()) {
    output << "dB ";
  }

  // First pass through the elements: Output numerator factors and remember if we
  // come across anything that needs to go in the denominator later
  for (auto CUERef = m_CUEVec.begin(); CUERef != m_CUEVec.end(); CUERef++) {
    CCompoundUnitElement::ExponentType const& exponent = CUERef->GetExponent();
    if (exponent < 0.0) {
      divisorFlag = true;
    } else if (exponent == 0.0) {
      // We managed to build a compound unit where unit elements canceled out,
      // so skip it
      continue;
    } else {
      if (printedFirstElement) {
        output << ' ';
      }

      // ok, we have a positive exponent, so print out this element.
      dividendFlag = true;
      uID = CUERef->GetUnitID();
      CUnitDescriptor const& uDesc = uce.GetUnitDescriptor(uID);
      pID = CUERef->GetPrefixID();
      if (pID >= 0) {
        CPrefixDescriptor const& pDesc = uce.GetPrefixDescriptor(pID);
        output << pDesc.GetSymbol();
      }
      output << uDesc.GetSymbol();

      if (static_cast<double>(exponent) != 1.0) {
        output << "^" << static_cast<double>(exponent);
      }
      printedFirstElement = true;
    } // if positive exponent
  } // for each element

  // If there's no divisor, bail.
  if (!divisorFlag) {
    return output;
  }

  if (dividendFlag) {
    output << "/";
    // Refrain from printing out a space right after a slash
    printedFirstElement = false;
  }

  // Second pass through the elements: output denominator factors
  for (auto CUERef = m_CUEVec.begin(); CUERef != m_CUEVec.end(); CUERef++) {
    if (printedFirstElement) {
      output << ' ';
    }

    CCompoundUnitElement::ExponentType const& exponent = CUERef->GetExponent();
    if (exponent < 0.0) {
      // ok, we have a negative exponent, so negate exponent and print out this element.
      uID = CUERef->GetUnitID();
      CUnitDescriptor const& uDesc = uce.GetUnitDescriptor(uID);
      pID = CUERef->GetPrefixID();
      if (pID >= 0) {
        CPrefixDescriptor const& pDesc = uce.GetPrefixDescriptor(pID);
        output << pDesc.GetSymbol();
      }
      output << uDesc.GetSymbol();

      if (dividendFlag) {
        if (static_cast<double>(exponent) != -1.0) {
          output << "^" << -(static_cast<double>(exponent));
        }
      } else {
        output << "^" << (static_cast<double>(exponent));
      }

      printedFirstElement = true;
    } // it's a denominator factor
  } // for each element
  return output;
}
//----------------------------------------------------------------------------
double Convert(double d, const CCompoundUnit& from, const CCompoundUnit& to)
{
  if (from == to)
    return d;
  // I am assuming we are not going to do Quantity A to Quantity B Conversions
  return CUnitConversionEngine::GetEngine().QuickConvertValue(d, from, to);
}
//----------------------------------------------------------------------------
bool CompatibleUnits(const CCompoundUnit& from, const CCompoundUnit& to)
{
  if (from == to)
    return true;
  if (from.GetDimension() == to.GetDimension())
    return true;
  // See if the quantity types (Dimensions) are convertable
  double fromExp;
  CCompoundUnit mappingUnit;
  CUnitConversionEngine& uce = CUnitConversionEngine::GetEngine();
  if (uce.GetQuantityConversionParams(from.GetDimension(), to.GetDimension(), fromExp, mappingUnit))
    return true;
  return false;
}
}