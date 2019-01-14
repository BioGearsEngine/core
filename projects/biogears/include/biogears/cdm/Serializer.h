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

#include <cstddef> // std::size_t
#include <iostream>
#include <memory> // std::auto_ptr
#include <string>
#include <memory>

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLUni.hpp>

#include <xercesc/dom/DOM.hpp>

#include <xercesc/framework/XMLGrammarPoolImpl.hpp>
#include <xercesc/validators/common/Grammar.hpp>

#include <biogears/cdm/CommonDataModel.h>
#include <biogears/exports.h>

CDM_BIND_DECL(ObjectData)
namespace biogears {
class BIOGEARS_API Serializer {
public:
  static void Destroy() { SAFE_DELETE(m_me); }
  static std::unique_ptr<CDM::ObjectData> ReadFile(const char* xmlFile, Logger* logger);
  static std::unique_ptr<CDM::ObjectData> ReadFile(const std::string& xmlFile, Logger* logger);
private:
  Serializer();
  virtual ~Serializer();

  bool Initialize(Logger* logger);
  xercesc::DOMLSParser* CreateParser(Logger* logger) const;

  static Serializer* m_me;

  static bool m_Initialized;
  static std::unique_ptr<xercesc::XMLGrammarPool> m_GrammerPool;
};

class ErrorHandler : public xercesc::DOMErrorHandler {
public:
  ErrorHandler()
    : failed_(false)
  {
  }

  bool failed() const;
  const char* getError();

  virtual bool handleError(const xercesc::DOMError& err);

private:
  bool failed_;
  std::stringstream error_;
};
}