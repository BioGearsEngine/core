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
/// @file UnitStringLexer.h
/// @author Chris Volpe
///
/// This class implements a lexical analyzer for the parser inside CompoundUnit
/// that parses a complex unit string
//----------------------------------------------------------------------------
#pragma once

// Note: See top of CompoundUnit.h for a discussion of the lines below which export STL
// container instantiations. In this particular case, the compiler complains about the member
// variables "end" and "cursor". Note that exporting the iterator base must be done before
// exporting the iterator template itself.
#include <string>

namespace biogears {
#pragma warning(disable : 4231)
struct SUnitStringToken {
  SUnitStringToken()
    : tokenID(TT_ERR)
    , symbol()
    , fltval(0.0)
  {
  }
  SUnitStringToken(SUnitStringToken& src)
    : tokenID(src.tokenID)
    , symbol(src.symbol)
    , fltval(src.fltval)
  {
  }
  std::string toString() const
  {
    switch (tokenID) {
    case TT_IDENTIFIER:
      return "TT_IDENTIFIER";
      break;

    case TT_EXPONENT:
      return "TT_EXPONENT";
      break;

    case TT_FLOAT:
      return "TT_FLOAT";
      break;

    case TT_SLASH:
      return "TT_SLASH";
      break;

    case TT_PARENOPEN:
      return "TT_PARENOPEN";
      break;

    case TT_PARENCLOSE:
      return "TT_PARENCLOSE";
      break;

    case TT_ERR:
      return "TT_ERR";
      break;

    case TT_EOF:
      return "TT_EOF";
      break;

    default:
      return "UNDEFINED";

    } // switch
  } // toString

  // Enumerate the various Token Types
  enum { TT_IDENTIFIER,
    TT_EXPONENT,
    TT_FLOAT,
    TT_SLASH,
    TT_PARENOPEN,
    TT_PARENCLOSE,
    TT_DB,
    TT_ERR,
    TT_EOF } tokenID;
  std::string symbol;
  double fltval;
};

class CUnitStringLexer {
public:
  CUnitStringLexer(const std::string& src)
    : source(src)
    , scanNext(true)
    , end(source.end())
    , cursor(source.begin())
  {
    // Nothing else to do
  }

  void GetNextToken(SUnitStringToken& token)
  {
    if (scanNext) {
      ScanNextToken();
    }
    token = curToken;
    scanNext = true;
  }

  void PeekNextToken(SUnitStringToken& token)
  {
    if (scanNext) {
      ScanNextToken();
    }
    token = curToken;
    scanNext = false;
  }

protected:
  void ScanNextToken();

private:
  CUnitStringLexer();

  std::string source;
  SUnitStringToken curToken;
  bool scanNext; // Determines whether a call to PeekNextToken or GetNextToken needs to scan
  std::string::iterator end;
  std::string::iterator cursor; // current position in source string
};}