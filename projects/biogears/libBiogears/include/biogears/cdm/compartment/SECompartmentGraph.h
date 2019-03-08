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
#include <string>
#include <vector>

namespace biogears {

class Logger;

#define COMPARTMENT_GRAPH_TEMPLATE typename CompartmentType, typename CompartmentLinkType
#define COMPARTMENT_GRAPH_TYPES CompartmentType, CompartmentLinkType

template <COMPARTMENT_GRAPH_TEMPLATE>
class SECompartmentGraph : public Loggable {
protected:
  SECompartmentGraph(const std::string& name, Logger* logger);
  SECompartmentGraph(const char* name, Logger* logger);

public:
  virtual ~SECompartmentGraph();

  virtual void Clear();

public:
  virtual std::string GetName() const;
  virtual const char* GetName_cStr() const;

  virtual void AddCompartment(CompartmentType& cmpt);
  virtual CompartmentType* GetCompartment(const char* name);
  virtual CompartmentType* GetCompartment(const std::string& name);
  virtual const CompartmentType* GetCompartment(const char* name) const;
  virtual const CompartmentType* GetCompartment(const std::string& name) const;
  virtual const std::vector<CompartmentType*>& GetCompartments() const;
  virtual void RemoveCompartment(const CompartmentType& cmpt);
  virtual void RemoveCompartment(const char* name);
  virtual void RemoveCompartment(const std::string& name);

  virtual CompartmentLinkType* GetLink(const char* name);
  virtual void AddLink(CompartmentLinkType& link);
  virtual CompartmentLinkType* GetLink(const std::string& name);
  virtual const CompartmentLinkType* GetLink(const char* name) const;
  virtual const CompartmentLinkType* GetLink(const std::string& name) const;
  virtual const std::vector<CompartmentLinkType*>& GetLinks() const;
  virtual void RemoveLink(const CompartmentLinkType& link);
  virtual void RemoveLink(const char* name);
  virtual void RemoveLink(const std::string& name);

protected:
  std::string m_Name;
  std::vector<CompartmentType*> m_Compartments;
  std::vector<CompartmentLinkType*> m_CompartmentLinks;
};
}