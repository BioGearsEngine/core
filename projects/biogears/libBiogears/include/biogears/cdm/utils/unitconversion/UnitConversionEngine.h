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
/// @file UnitConversionEngine.h
/// @author Chris Volpe
///
/// This is the central object within which all unit conversion information
/// is stored. Its primary function is to maintain a list of
/// QuantityTypeDescriptors and provide an API for loading unit definitions,
/// parsing unit strings, etc.
//----------------------------------------------------------------------------
#pragma once
//Standard Includes
#include <cassert>
#include <cmath>
#include <unordered_map>
#include <cstddef>
//Project Includes
#include <biogears/cdm/utils/unitconversion/PrefixDescriptor.h>
#include <biogears/cdm/utils/unitconversion/QuantityConversionDescriptor.h>
#include <biogears/cdm/utils/unitconversion/QuantityConversionKey.h>
#include <biogears/cdm/utils/unitconversion/QuantityTypeDescriptor.h>
#include <biogears/cdm/utils/unitconversion/UnitDescriptor.h>
#include <biogears/cdm/utils/unitconversion/UnitDimension.h>

namespace biogears {
class BIOGEARS_API CUnitConversionEngine {
private:
  static CUnitConversionEngine* uce;

public:
  // Define vector types for holding the data structures
  typedef std::vector<CQuantityTypeDescriptor*> QuantityTypeList;
  typedef std::vector<CUnitDescriptor*> UnitList;
  typedef std::vector<CPrefixDescriptor*> PrefixList;
  typedef std::vector<CQuantityConversionDescriptor*> QuantityConversionList;

  // Define hash map types for easy lookup of names and symbols
  typedef std::unordered_map<char, unsigned int> PrefixMap;
  typedef std::unordered_map<std::string, unsigned int> UnitSymbolMap;
  typedef std::unordered_map<std::string, unsigned int> QuantityNameMap;
  typedef std::unordered_map<CQuantityConversionKey, unsigned int> QuantityConversionMap;
  typedef std::unordered_map<CUnitDimension, unsigned int> DimensionToQuantityMap;

  static CUnitConversionEngine& GetEngine(void);

  static void DestroyEngine()
  {
    SAFE_DELETE(uce);
  }

  void SetWorkingDirectory(const std::string& wrkDir)
  {
    m_wrkDir = wrkDir;
  }

  const std::string& GetWorkingDirectory() const
  {
    return m_wrkDir;
  }

  // Do conversion, using either string or CCompoundUnit specifications, and
  // either with or without (quick) dimension checking.
  double ConvertValue(const double& value, const CCompoundUnit& fromUnit, const CCompoundUnit& toUnit) const;
  double ConvertValue(const double& value, const std::string& fromUnit, const std::string& toUnit) const;
  double QuickConvertValue(const double& value, const CCompoundUnit& fromUnit, const CCompoundUnit& toUnit) const;
  double QuickConvertValue(const double& value, const std::string& fromUnit, const std::string& toUnit) const;
  // ConvertValueInterval basically ignores the biases in order to convert intervals of a
  // particular unit. Useful for *adding* quantities together
  double ConvertValueInterval(const double& value, const CCompoundUnit& fromUnit, const CCompoundUnit& toUnit) const;
  double ConvertValueInterval(const double& value, const std::string& fromUnit, const std::string& toUnit) const;
  double ConvertQuantityType(const double& value, const CCompoundUnit& fromUnit, const CCompoundUnit& toUnit) const;

  // Simple direct lookup of a unit symbol.
  int GetUnitID(const std::string& unitSym) const;

  // Lookup a quantity type
  int GetQuantityTypeID(const std::string& qtName) const;
  int GetQuantityTypeID(const CUnitDimension& ud) const;

  // Lookup a conversion descriptor
  int GetQuantityConversionID(const CUnitDimension* fromDim, const CUnitDimension* toDim) const;

  // Lookup quantity conversion parameters. Will automatically compute inverted parameters
  // if looking up conversion in opposite direction from that which was defined. The two reference parameters
  // are modified by this function to return the exponent that the original value (including
  // its associated compound unit) must be raised to, and the conversion factor as a CCompoundUnit
  // object that must be factored in as well. The CCompoundUnit object that serves as the
  // conversion factor contains the mapping unit and has already been raised to the necessary power
  // as specified by the QuantityConversionDescriptor. In the event that the requested
  // quantity conversion is the reverse of a defined quantity conversion, the defined
  // version's conversion parameters are transmogrified into those of the requested
  // conversion automatically.
  bool GetQuantityConversionParams(const CUnitDimension* fromDim, const CUnitDimension* toDim, double& fromExp, CCompoundUnit& mappingUnit) const;

  // Returns a CompoundUnit corresponding to the string representation.
  // Dynamically allocated, and caller must delete.
  CCompoundUnit* GetCompoundUnit(const std::string& unitString) const;

  // More robust symbol lookup, allows symbols with prefixes attached, and returns
  // (via references) both the unit ID and the prefix scale factor.
  bool LookupFullUnit(const std::string& fullUnitSym, int& unitID, double& prefixScaleFac) const
  {
    int prefixID;
    bool rc = LookupFullUnit(fullUnitSym, unitID, prefixID);
    if (rc) {
      if (prefixID != -1) {
        CPrefixDescriptor* pd = (*m_PList)[prefixID];
        prefixScaleFac = pd->GetScaleFactor();
      } else {
        prefixScaleFac = 1.0;
      }
      return true;
    } else {
      return false;
    }
  }

  bool LookupFullUnit(const std::string& fullUnitSym, int& unitID, int& prefixID) const;

  // Accessor convenience methods
  const CUnitDescriptor& GetUnitDescriptor(int unitID) const
  {
    assert((unitID >= 0) && (unitID < static_cast<int>(m_UList->size())));
    return *(*m_UList)[unitID];
  };

  const CQuantityTypeDescriptor& GetQuantityTypeDescriptor(int qtID) const
  {
    assert((qtID >= 0) && (qtID < static_cast<int>(m_QTList->size())));
    return *(*m_QTList)[qtID];
  };

  const CPrefixDescriptor& GetPrefixDescriptor(int pfxID) const
  {
    assert((pfxID >= 0) && (pfxID < static_cast<int>(m_PList->size())));
    return *(*m_PList)[pfxID];
  };

  const CQuantityConversionDescriptor& GetQuantityConversionDescriptor(int qcID) const
  {
    assert((qcID >= 0) && (qcID < static_cast<int>(m_QCList->size())));
    return *(*m_QCList)[qcID];
  }

  int GetNumQuantities()
  {
    return m_iNumQuantities;
  }

  int GetNumFundamentalQuantities()
  {
    return m_iNumFundamentalQuantities;
  }

  void LoadDefinitionsFlatFile();

  // These are the methods that the LoadDefinitions methods call to build up the
  // internal database of units information
  CUnitDescriptor* NewUnit(const std::string& name, const std::string& symbol, unsigned int quantityTypeId, const std::string& targetUnit,
    const double& convFac, const double& bias, CUnitDescriptor::PrefixModeType mode, const std::string& prefixSet);
  CQuantityTypeDescriptor* NewQuantityType(const std::string& name, const std::string& expansion = "", bool twentyLog = false);

  CPrefixDescriptor* NewPrefix(const std::string& name, const std::string& sym, const double& scaleFac);
  CQuantityConversionDescriptor* NewQuantityConversion(const std::string& fromTypeName, double fromExp, const std::string& toTypeName,
    const std::string& mappingUnit);

protected:
private:
  // Only want one unit conversion engine, so declare constructors private
  CUnitConversionEngine();
  CUnitConversionEngine(const CUnitConversionEngine& obj);
  ~CUnitConversionEngine();

  // Make these pointers rather than the actual vectors to avoid compiler warnings
  // about exporting STL classes
  QuantityTypeList* m_QTList;
  UnitList* m_UList;
  PrefixList* m_PList;
  QuantityConversionList* m_QCList;
  PrefixMap* m_PMap;
  UnitSymbolMap* m_USMap;
  QuantityNameMap* m_QNMap;
  QuantityConversionMap* m_QCMap;
  DimensionToQuantityMap* m_D2QMap;

  int m_iNumFundamentalQuantities;
  int m_iNumQuantities;

  std::string m_wrkDir; // I made this static and accessable for another project, so this is legacy and could be removed if causing issues
};
}
