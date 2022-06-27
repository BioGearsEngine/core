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
/// @file UnitConversionEngine.cpp
/// @author Chris Volpe
//----------------------------------------------------------------------------
#include <biogears/cdm/utils/unitconversion/UnitConversionEngine.h>

//Standard includes
#include <sstream>
#include <algorithm>
#include <cctype>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <limits>
//Project Includes
#include <biogears/cdm/utils/unitconversion/CompoundUnit.h>
#include <biogears/cdm/utils/Logger.h>
//Private Includes
#include "UCEDefs.h"
namespace biogears {
CUnitConversionEngine* CUnitConversionEngine::uce = nullptr;

class InitializeUnitConversionEngine {
public:
  InitializeUnitConversionEngine()
  {
    CUnitConversionEngine::GetEngine();
  }
  InitializeUnitConversionEngine(std::string wrkDir)
  {
    CUnitConversionEngine::GetEngine().SetWorkingDirectory(wrkDir);
  }
};
// Commented out so one can set the working directory if needed
// Other clients of this library may want to do this in their own code
//static InitializeUnitConversionEngine init;

//----------------------------------------------------------------------------
CUnitConversionEngine::CUnitConversionEngine()
{
  m_wrkDir = ".";
  m_iNumFundamentalQuantities = 0;
  m_iNumQuantities = 0;
  m_QTList = new QuantityTypeList();
  m_UList = new UnitList();
  m_PList = new PrefixList();
  m_QCList = new QuantityConversionList();
  m_PMap = new PrefixMap();
  m_USMap = new UnitSymbolMap();
  m_QNMap = new QuantityNameMap();
  m_QCMap = new QuantityConversionMap();
  m_D2QMap = new DimensionToQuantityMap();
}
//----------------------------------------------------------------------------
CUnitConversionEngine::CUnitConversionEngine(const CUnitConversionEngine& obj)
  :  m_wrkDir(obj.m_wrkDir)
  ,  m_iNumFundamentalQuantities(obj.m_iNumFundamentalQuantities)
  ,  m_iNumQuantities (obj.m_iNumQuantities)
  ,  m_QTList ( new QuantityTypeList(*obj.m_QTList))
  ,  m_UList ( new UnitList(*obj.m_UList))
  ,  m_PList ( new PrefixList(*obj.m_PList))
  ,  m_QCList ( new QuantityConversionList(*obj.m_QCList))
  ,  m_PMap ( new PrefixMap(*obj.m_PMap))
  ,  m_USMap ( new UnitSymbolMap(*obj.m_USMap))
  ,  m_QNMap ( new QuantityNameMap(*obj.m_QNMap))
  ,  m_QCMap ( new QuantityConversionMap(*obj.m_QCMap))
  ,  m_D2QMap ( new DimensionToQuantityMap(*obj.m_D2QMap))

{

}
//----------------------------------------------------------------------------
CUnitConversionEngine::~CUnitConversionEngine()
{
  // Free everything that we allocate

  for (auto it = m_PList->begin(); it != m_PList->end(); it++) {
    delete *it;
  }
  delete m_PList;

  for (auto it = m_UList->begin(); it != m_UList->end(); it++) {
    delete *it;
  }
  delete m_UList;

  for (auto it = m_QTList->begin(); it != m_QTList->end(); it++) {
    delete *it;
  }
  delete m_QTList;

  for (auto it = m_QCList->begin(); it != m_QCList->end(); it++) {
    delete *it;
  }
  delete m_QCList;

  delete m_PMap;
  delete m_USMap;
  delete m_QNMap;
  delete m_QCMap;
  delete m_D2QMap;
}

CUnitConversionEngine& CUnitConversionEngine::GetEngine(void)
{
  if (uce == nullptr) {
    uce = new CUnitConversionEngine();
    uce->LoadDefinitionsFlatFile();
  }
  return *uce;
}
//----------------------------------------------------------------------------
// Convenience method to tell if a string instance and c-string pointer are
// equivalent in a Case-Insensitive manner
inline bool CIEQUAL(std::string strA, std::string strB)
{
  if (strA == strB) {
    return true;
  }

  std::transform(begin(strA), end(strA), begin(strA), (int (*)(int))std::tolower);
  std::transform(begin(strB), end(strB), begin(strB), (int (*)(int))std::tolower);

  return strA == strB;
}

//----------------------------------------------------------------------------
void CUnitConversionEngine::LoadDefinitionsFlatFile()
{
  std::stringstream defs;
  defs << biogears::UCEDEFS_TEXT;

  DEBUGOUT(cerr << "Loading definitions...\n";)
  
  std::string curLine;
  CQuantityTypeDescriptor* qtd = nullptr;

  while (!defs.eof()) {
    getline(defs, curLine);
    DEBUGOUT(cerr << "Got [" << curLine << "]\n";)

    if (curLine.empty()) {
      continue;
    }

    if (curLine[0] == '#') {
      continue;
    }

    std::istringstream curLineStream(curLine);

    std::string fieldType;
    curLineStream >> fieldType;

    // You would think that the STL string class would contain a built-in method
    // for doing case-insensitive comparisons, but you'd be wrong.
    if (CIEQUAL(fieldType, std::string("QUANTITY"))) {
      // We've got a quantity type definition
      DEBUGOUT(std::cerr << "Found Quantity Definition" << std::endl;)

      // See if it's a fundamental, derived, or unity quantity.
      std::string FundDer;
      curLineStream >> FundDer;
      DEBUGOUT(std::cerr << "Fundamental/Derived: " << FundDer << std::endl;)
      bool fundFlag = CIEQUAL(FundDer, std::string("FUNDAMENTAL"));
      bool derFlag = CIEQUAL(FundDer, std::string("DERIVED"));
      // Note: UNITY type should be deprecated
      //bool unityFlag = CIEQUAL(FundDer, std::string("UNITY"));

      // Get the name of the quantity
      std::string QuantName;
      curLineStream >> QuantName;
      // Check for optional "20LOG" specifier before name
      bool twentyLog = CIEQUAL(QuantName, std::string("20LOG"));
      if (twentyLog) {
        // It's there, so name is in the next field
        curLineStream >> QuantName;
        DEBUGOUT(std::cerr << QuantName << " is a 20-Log-Rule quantity." << std::endl;);
      }
      DEBUGOUT(std::cerr << "Quantity name: " << QuantName << std::endl;)

      std::string ExpansionStr;
      if (derFlag) {
        // Pull out the remainder of this line into its own string. I'm
        // not sure if there's a more "direct" way to do this, given that
        // the curLineStream knows where we left off in curLine, but I'll
        // do it the straightforward way for now. Since nothing else
        // comes after this on the line, I can pull out the remainder of the line,
        // as opposed to building an expansion string piece by piece like
        // we need to do for Unit definitions.
        getline(curLineStream, ExpansionStr);
        DEBUGOUT(std::cerr << "Expansion is " << ExpansionStr << "\n";)
        qtd = NewQuantityType(QuantName, ExpansionStr, twentyLog);
      } else if (fundFlag) {
        qtd = NewQuantityType(QuantName, "", twentyLog);
      } else {
        std::cerr << "Invalid quantity type category: " << FundDer << std::endl;
      }

    } // QUANTITY

    else if (CIEQUAL(fieldType, std::string("UNIT"))) {
      // We've got a new unit definition
      std::string UnitName;
      std::string UnitSymbol;
      std::string QuantName;
      double convFac;
      double bias = 0.0;
      std::string TargetCompoundUnit;
      std::string TCUSubstr;
      std::string PrefixModeStr;
      CUnitDescriptor::PrefixModeType PrefixMode;
      std::string Prefixes;

      curLineStream >> QuantName;
      curLineStream >> UnitName;
      curLineStream >> UnitSymbol;
      curLineStream >> convFac;
      curLineStream >> TCUSubstr;
      bool firstIteration = true;
      //Append pieces of the Target Compound Unit until we reach a piece that isn't
      while (!CIEQUAL(TCUSubstr, std::string("NOPREFIXES")) && !CIEQUAL(TCUSubstr, std::string("ALLPREFIXES")) && !CIEQUAL(TCUSubstr, std::string("GOODPREFIXES")) && !CIEQUAL(TCUSubstr, std::string("BADPREFIXES")) && !curLineStream.eof()) {
        //cerr << "Appending [" << TCUSubstr << "]...";
        if (!firstIteration) {
          TargetCompoundUnit += " ";
        }
        firstIteration = false;
        TargetCompoundUnit += TCUSubstr;
        curLineStream >> TCUSubstr;
      }
      //cerr << endl;
      PrefixModeStr = TCUSubstr;

      unsigned int qtID = GetQuantityTypeID(QuantName);

      //cerr << "Found unit name:" << UnitName << " symbol:" << UnitSymbol <<
      //  " convfac:" << convFac << " target:" << TargetCompoundUnit << " prefix mode:" <<
      //  PrefixModeStr << endl;

      if (CIEQUAL(PrefixModeStr, std::string("NOPREFIXES"))) {
        PrefixMode = CUnitDescriptor::NONE;
      } else if (CIEQUAL(PrefixModeStr, std::string("ALLPREFIXES"))) {
        PrefixMode = CUnitDescriptor::ALL;
      } else if (CIEQUAL(PrefixModeStr, std::string("GOODPREFIXES"))) {
        PrefixMode = CUnitDescriptor::SOME;
        curLineStream >> Prefixes;
      } else if (CIEQUAL(PrefixModeStr, std::string("BADPREFIXES"))) {
        PrefixMode = CUnitDescriptor::ALLBUT;
        curLineStream >> Prefixes;
      } else {
        std::cerr << "Invalid prefix mode on " << UnitName << " definition";
      }

      curLineStream >> bias;

      NewUnit(UnitName, UnitSymbol, qtID, TargetCompoundUnit, convFac, bias, PrefixMode, Prefixes);
    } // UNIT

    else if (CIEQUAL(fieldType, std::string("PREFIX"))) {
      std::string PrefixName;
      std::string PrefixSymbol;
      double scaleFac;
      curLineStream >> PrefixName >> PrefixSymbol >> scaleFac;

      NewPrefix(PrefixName, PrefixSymbol, scaleFac);
    } // PREFIX

    else if (CIEQUAL(fieldType, std::string("TYPEMAP"))) {
      std::string FromType;
      std::string ToType;
      double fromExponent;
      std::string MappingUnit;

      curLineStream >> ToType >> FromType >> fromExponent;
      // Remainder of line is the mapping unit.
      getline(curLineStream, MappingUnit);

      NewQuantityConversion(FromType, fromExponent, ToType, MappingUnit);
    }
  } // while not EOF
}

//----------------------------------------------------------------------------
CQuantityTypeDescriptor* CUnitConversionEngine::NewQuantityType(const std::string& name, const std::string& expansion, bool twentyLog)
{
  CQuantityTypeDescriptor* qtd;

  // expansion has a default value of "". If it's not specified, it's a fundamental quantity
  if (expansion == "") {
    // Create a fundamental quantity
    // Increment num fundamental quantities first, before creating the
    // descriptor, because creating the descriptor also involves creating its
    // internal unit dimension object, and doing that requires knowing this
    // updated value.
    m_iNumFundamentalQuantities++;
    // Subtract 1 from num fundamental quantities because the largest index
    // is one less than the number of them.
    qtd = new CQuantityTypeDescriptor(name, m_iNumFundamentalQuantities - 1, twentyLog);
  } else {
    // Create a derived quantity
    CCompoundUnit* CUExpansion = GetCompoundUnit(expansion);
    qtd = new CQuantityTypeDescriptor(name, CUExpansion, twentyLog);
  }

  // Add it to the list
  m_QTList->push_back(qtd);

  // Since we just added it to the end of the vector, the size of the vector
  // is one more than the index where we added it.
  (*m_QNMap)[name] = (static_cast<unsigned int>(m_QTList->size()) - 1);
  // Also add to the map from dimension to quantity type so that we can look up an
  // arbitrary dimension and see if it corresponds to an existing Quantity Type that
  // happens to obey that damn 20-Log rule for decibel units. Grrr.
  (*m_D2QMap)[*(qtd->GetDimension())] = (static_cast<unsigned int>(m_QTList->size()) - 1);
  m_iNumQuantities++;
  return qtd;
}

//----------------------------------------------------------------------------
CUnitDescriptor* CUnitConversionEngine::NewUnit(const std::string& name, const std::string& symbol, const unsigned int quantityTypeId, const std::string& targetCompoundUnit,
  const double& convFac, const double& bias, const CUnitDescriptor::PrefixModeType mode, const std::string& prefixSet)
{
  CUnitDescriptor* ud = new CUnitDescriptor(name, symbol, quantityTypeId, targetCompoundUnit, convFac, bias, mode, prefixSet);

  // Add it to the list
  m_UList->push_back(ud);
  auto itr = m_USMap->find(symbol);
  if (itr != m_USMap->end())
    std::cerr << "Symbol '" << symbol << "' is trying to be used as " << name << ", but it has already been defined as " << (*m_UList)[itr->second]->GetName() << std::endl;

  // Since we just added it to the end of the vector, the size of the vector
  // is one more than the index where we added it.
  (*m_USMap)[symbol] = (static_cast<unsigned int>(m_UList->size()) - 1);
  return ud;
}

//----------------------------------------------------------------------------
CPrefixDescriptor* CUnitConversionEngine::NewPrefix(const std::string& name, const std::string& sym, const double& scaleFac)
{
  CPrefixDescriptor* pd = new CPrefixDescriptor(name, sym[0], scaleFac);

  // Add it to the list
  m_PList->push_back(pd);

  // Since we just added it to the end of the vector, the size of the vector
  // is one more than the index where we added it.
  (*m_PMap)[static_cast<char>(sym[0])] = (static_cast<unsigned int>(m_PList->size()) - 1);

  return pd;
}

//----------------------------------------------------------------------------
CQuantityConversionDescriptor* CUnitConversionEngine::NewQuantityConversion(const std::string& fromTypeName, double fromExp,
  const std::string& toTypeName, const std::string& mappingUnit)
{
  // Lookup all the relevant strings
  CUnitConversionEngine& uce = CUnitConversionEngine::GetEngine();
  int fromQTID = uce.GetQuantityTypeID(fromTypeName);
  int toQTID = uce.GetQuantityTypeID(toTypeName);
  CQuantityConversionKey qckey(fromQTID, toQTID);

  CQuantityConversionDescriptor* qcd = new CQuantityConversionDescriptor(fromExp, mappingUnit);
  qcd->Validate(fromQTID, toQTID);

  // Add it to the list
  m_QCList->push_back(qcd);

  // Since we just added it to the end of the vector, the size of the vector
  // is one more than the index where we added it.
  (*m_QCMap)[qckey] = (static_cast<unsigned int>(m_QCList->size()) - 1);

  return qcd;
}

//----------------------------------------------------------------------------
double CUnitConversionEngine::ConvertValue(const double& value, const CCompoundUnit& fromUnit, const CCompoundUnit& toUnit) const
{
  CUnitDimension const *fud, *tud;
  fud = fromUnit.GetDimension();
  tud = toUnit.GetDimension();

  if (*fud != *tud) {
    return ConvertQuantityType(value, fromUnit, toUnit);
  }
  return QuickConvertValue(value, fromUnit, toUnit);
}

//----------------------------------------------------------------------------
double CUnitConversionEngine::QuickConvertValue(const double& value, const CCompoundUnit& fromUnit, const CCompoundUnit& toUnit) const
{
  double rv = 0;

  rv = value;
  if (fromUnit.IsDecibel()) {
    double scalefac = fromUnit.GetDecibelLogScaleFactor();
    rv = std::pow(10.0, rv / scalefac);
  }

  rv -= fromUnit.GetBias();
  rv *= fromUnit.GetBigness() / toUnit.GetBigness();
  // Surely, this bias can't be compatible with decibel notation. You can't take
  // the log of zero, but a zero in degrees Fahrenheit isn't really "zero" as in
  // "absolute zero" like, say, zero watts is. If someone creates a Compound Unit
  // of the form "dB degF", the result will be nonsense.
  rv += toUnit.GetBias();

  if (toUnit.IsDecibel()) {
    double scalefac = toUnit.GetDecibelLogScaleFactor();
    rv = scalefac * log10(rv);
  }

  return rv;
}

//----------------------------------------------------------------------------
double CUnitConversionEngine::ConvertValue(const double& value, const std::string& fromUnit, const std::string& toUnit) const
{
  CCompoundUnit from, to;
  double retval;
  from.ParseString(fromUnit);
  to.ParseString(toUnit);
  retval = ConvertValue(value, from, to);
  return retval;
}

//----------------------------------------------------------------------------
double CUnitConversionEngine::QuickConvertValue(const double& value, const std::string& fromUnit, const std::string& toUnit) const
{
  CCompoundUnit from, to;
  double retval;
  from.ParseString(fromUnit);
  to.ParseString(toUnit);
  retval = QuickConvertValue(value, from, to);
  return retval;
}

//----------------------------------------------------------------------------
double CUnitConversionEngine::ConvertQuantityType(const double& value, const CCompoundUnit& fromUnit, const CCompoundUnit& toUnit) const
{
  // Called by ConvertValue when dimensions don't match.
  const CUnitDimension* fromDim = fromUnit.GetDimension();
  const CUnitDimension* toDim = toUnit.GetDimension();
  double fromExp;
  CCompoundUnit mappingUnit;
  bool rc = GetQuantityConversionParams(fromDim, toDim, fromExp, mappingUnit);
  if (!rc) {
    std::cerr << "Incompatible conversion types: [" << fromUnit << "] and ["
              << toUnit << "]." << std::endl;
    return std::numeric_limits<double>::signaling_NaN();
  }

  // Ok, raise both the original value & unit to the fromExp power, and multiply the raised
  // original unit by the mapping unit. Since the fromUnit is a const ref, we'll work with
  // the mappingUnit and multiply it by the raised fromUnit instead. Remember that the
  // mappingUnit returned has already been raised to the necessary power by the
  // GetQuantityConversionParams method. The result will be a value specified in a
  // funky dimensionally-correct hybrid unit consisting of the original fromUnit and
  // the mapping unit. To get the unit we want, we then have to use
  // QuickConvertValue.
  double mappedVal = value;
  mappedVal -= fromUnit.GetBias();
  mappedVal = std::pow(mappedVal, fromExp);
  mappingUnit *= biogears::pow(fromUnit, fromExp);

  // Sanity check
  if (*(mappingUnit.GetDimension()) != *toDim) {
    std::cerr << "Programming error! Validated quantity conversion fails to produce correct dimension" << std::endl;
  }

  mappedVal = ConvertValueInterval(mappedVal, mappingUnit, toUnit);
  mappedVal += toUnit.GetBias();
  return mappedVal;
}

//----------------------------------------------------------------------------
double CUnitConversionEngine::ConvertValueInterval(const double& value, const CCompoundUnit& fromUnit, const CCompoundUnit& toUnit) const
{
  double rv = 0;

  rv = value * fromUnit.GetBigness() / toUnit.GetBigness();

  return rv;
}

//----------------------------------------------------------------------------
double CUnitConversionEngine::ConvertValueInterval(const double& value, const std::string& fromUnit, const std::string& toUnit) const
{
  CCompoundUnit from, to;
  double retval;
  from.ParseString(fromUnit);
  to.ParseString(toUnit);
  retval = ConvertValueInterval(value, from, to);
  return retval;
}

//----------------------------------------------------------------------------
int CUnitConversionEngine::GetUnitID(const std::string& unitSym) const
{
  UnitSymbolMap::iterator iter;
  iter = m_USMap->find(unitSym);
  if (iter == m_USMap->end()) {
    // Not found
    return -1;
  } else {
    return iter->second;
  }
}

//----------------------------------------------------------------------------
int CUnitConversionEngine::GetQuantityTypeID(const std::string& qtName) const
{
  QuantityNameMap::iterator iter;
  iter = m_QNMap->find(qtName);
  if (iter == m_QNMap->end()) {
    // Not found
    return -1;
  } else {
    return iter->second;
  }
}

//----------------------------------------------------------------------------
int CUnitConversionEngine::GetQuantityTypeID(const CUnitDimension& ud) const
{
  DimensionToQuantityMap::iterator iter;
  iter = m_D2QMap->find(ud);
  if (iter == m_D2QMap->end()) {
    // Not found
    return -1;
  } else {
    return iter->second;
  }
}

//----------------------------------------------------------------------------
int CUnitConversionEngine::GetQuantityConversionID(const CUnitDimension* fromDim, const CUnitDimension* toDim) const
{

  QuantityConversionMap::iterator iter;
  CQuantityConversionKey qckey(fromDim, toDim);
  iter = m_QCMap->find(qckey);
  if (iter == m_QCMap->end()) {
    // Not found
    return -1;
  } else {
    return iter->second;
  }
}

//----------------------------------------------------------------------------
bool CUnitConversionEngine::GetQuantityConversionParams(const CUnitDimension* fromDim, const CUnitDimension* toDim, double& fromExp, CCompoundUnit& mappingUnit) const
{
  bool found = false;
  bool invert = false;
  int QCID;

  // Check if the requested conversion has been defined
  QCID = GetQuantityConversionID(fromDim, toDim);

  // Anything but -1 is a Quantity Conversion ID
  if (QCID != -1) {
    found = true;
  } else {
    // Check for other direction
    QCID = GetQuantityConversionID(toDim, fromDim);
    if (QCID != -1) {
      found = true;
      invert = true;
    }
  }

  // Bail out if neither direction is a valid conversion
  if (!found) {
    return false;
  }

  const CQuantityConversionDescriptor& QCDesc = GetQuantityConversionDescriptor(QCID);
  // Get the params
  mappingUnit = *QCDesc.GetMappingUnit();
  fromExp = QCDesc.GetFromExponent();

  // We're done, unless we have to invert the params
  if (invert) {
    // If:
    //     TO = M_unit * FROM^F_exp
    // then solving for FROM yields:
    //     FROM = M_unit^(-1/F_exp) * TO^(1/F_exp)

    mappingUnit.Raise(-1.0 / fromExp);
    fromExp = 1.0 / fromExp;
  }
  return true;
}

//----------------------------------------------------------------------------
bool CUnitConversionEngine::LookupFullUnit(const std::string& fullUnitSym, int& unitID, int& prefixID) const
{
  // Note: I'm still on the fence about whether to check for a match with or without
  // a prefix first. An argument can be made either way. On the one hand, there's something
  // to be said for the "longest unit name wins" rule, or at least there was, back when we
  // were thinking about not requiring a space delimiter between adjacent unit symbols. But
  // now we have the ability to control on a per-unit-symbol basis whether a given prefix
  // character is permitted, so we should give that (shorter, prefix-removed) symbol first
  // dibs on matching before matching the whole string as a unit symbol. Ok, I think I've
  // decided on that latter approach.

  char prefix = fullUnitSym[0];
  bool checkPrefix = true;
  prefixID = -1;
  unitID = -1;

  // Check if it's long enough to have a prefix
  if (fullUnitSym.size() == 1) {
    // Can't have a prefix if it's only 1 character long
    checkPrefix = false;
  }

  // Check if the prefix exists
  if (checkPrefix) {
    PrefixMap::iterator iter;
    iter = m_PMap->find(prefix);
    if (iter == m_PMap->end()) {
      // Not found
      checkPrefix = false;
    } else {
      prefixID = iter->second;
    }
  }

  // Still going for the prefix? Check symbol that follows.
  if (checkPrefix) {
    std::string postPrefix = fullUnitSym.substr(1, fullUnitSym.size() - 1);

    unitID = GetUnitID(postPrefix);
    if (unitID != -1) {
      // We've got a unit symbol, make sure it's compatible with the
      // prefix we've found
      const CUnitDescriptor& theUnit = GetUnitDescriptor(unitID);
      if (theUnit.IsPrefixAllowed(prefix)) {
        // We've got a prefix, a unit symbol, and they're compatible,
        // so we're done.
        return true;
      } // Prefix is compatible with unit
    } // unit found
  }

  // If we're still here, we haven't successfully resolved the string as a
  // legal prefix-unit combination, so try for a stand-alone unit.
  prefixID = -1;
  unitID = GetUnitID(fullUnitSym);
  if (unitID == -1) {
    return false;
  }

  return true;
}

//----------------------------------------------------------------------------
CCompoundUnit* CUnitConversionEngine::GetCompoundUnit(const std::string& unitString) const
{
  // Caller is responsible for freeing.
  return new CCompoundUnit(unitString);
}
}
