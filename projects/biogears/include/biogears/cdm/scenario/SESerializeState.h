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
#include <biogears/cdm/scenario/SEAction.h>
#include <biogears/schema/SerializeStateData.hxx>
#include <biogears/schema/enumSerializationType.hxx>

class BIOGEARS_API SESerializeState : public SEAction {
public:
  SESerializeState();
  virtual ~SESerializeState();

  virtual void Clear(); //clear memory

  virtual bool IsValid() const;

  virtual bool Load(const CDM::SerializeStateData& in);
  virtual CDM::SerializeStateData* Unload() const;

protected:
  virtual void Unload(CDM::SerializeStateData& data) const;

public:
  virtual void ToString(std::ostream& str) const;

  virtual CDM::enumSerializationType::value GetType() const;
  virtual void SetType(CDM::enumSerializationType::value t);
  virtual bool HasType() const;
  virtual void InvalidateType();

  virtual bool HasFilename() const;
  virtual std::string GetFilename() const;
  virtual void SetFilename(const std::string& filename);
  virtual void InvalidateFilename();

protected:
  std::string m_Filename;
  CDM::enumSerializationType::value m_Type;
};