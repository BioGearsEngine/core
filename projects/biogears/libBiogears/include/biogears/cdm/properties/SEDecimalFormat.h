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

#pragma once
#include <biogears/cdm/CommonDataModel.h>
#include <biogears/exports.h>

#include <string>
#include <map>

CDM_BIND_DECL(DecimalFormatData)

namespace biogears {
namespace io {
  class Property;
}

enum class DecimalNotation { Default,
                             Fixed,
                             Scientific };

class BIOGEARS_API SEDecimalFormat {
public:
  friend io::Property;
  SEDecimalFormat(const SEDecimalFormat* dfault = nullptr);
  virtual ~SEDecimalFormat();

  virtual void Reset();
  virtual void Set(const SEDecimalFormat& f);

  virtual bool Load(const CDM::DecimalFormatData& in);
  virtual CDM::DecimalFormatData* Unload();

protected:
  virtual void Unload(CDM::DecimalFormatData& to) const;

public:
  void SetPrecision(std::streamsize p);
  std::streamsize GetPrecision();

  void SetNotation(DecimalNotation n);
  DecimalNotation GetNotation();

  void SetStream(std::ofstream& s);

  bool operator==(const SEDecimalFormat&) const ;
  bool operator!=(const SEDecimalFormat&) const;

protected:
  std::streamsize m_Precision;
  DecimalNotation m_Notation;
};
  }//namespace biogears

namespace std{
BG_EXT template class BIOGEARS_API map<std::string, biogears::SEDecimalFormat>;
}