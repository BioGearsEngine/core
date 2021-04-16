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
//Standard Includes
#include <iostream>
#include <map>
//Project Includes
#include <biogears/cdm/Serializer.h>

#include <biogears/cdm/utils/FileUtils.h>
#include <biogears/schema/biogears/BioGears.hxx>

using namespace xercesc;
namespace xml = xsd::cxx::xml;

namespace biogears {
Serializer* Serializer::m_me = nullptr;
bool Serializer::m_Initialized = false;
std::unique_ptr<xercesc::XMLGrammarPool> Serializer::m_GrammerPool;

bool ErrorHandler::failed() const
{
  return failed_;
}
//-----------------------------------------------------------------------------
const char* ErrorHandler::getError()
{
  return error_.str().c_str();
}
//-----------------------------------------------------------------------------
bool ErrorHandler::handleError(const xercesc::DOMError& err)
{
  bool warn(err.getSeverity() == DOMError::DOM_SEVERITY_WARNING);

  if (!warn)
    failed_ = true;

  DOMLocator* loc(err.getLocation());

  char* uri(XMLString::transcode(loc->getURI()));
  char* msg(XMLString::transcode(err.getMessage()));

  error_ << uri << ":"
         << loc->getLineNumber() << ":" << loc->getColumnNumber() << " "
         << (warn ? "warning: " : "error: ") << msg << std::ends;
  /// \error Invalid schema file
  std::cerr << error_.str() << std::endl;
  std::cerr << err.getRelatedData() << std::endl;
  std::cerr << err.getRelatedException() << std::endl;

  XMLString::release(&uri);
  XMLString::release(&msg);

  return true;
}
//-----------------------------------------------------------------------------
Serializer::Serializer()
{
}
//-----------------------------------------------------------------------------
Serializer::~Serializer()
{
  m_GrammerPool.release();
  xercesc::XMLPlatformUtils::Terminate();
}
//-----------------------------------------------------------------------------
bool Serializer::Initialize(Logger* logger)
{
  if (m_Initialized)
    return true;

  std::stringstream err;

  xercesc::XMLPlatformUtils::Initialize();
  m_GrammerPool.reset(new XMLGrammarPoolImpl());

  std::string shortDir = ResolvePath("xsd/BioGearsDataModel.xsd");

  ErrorHandler eh;
  DOMLSParser* parser(CreateParser(logger));
  parser->getDomConfig()->setParameter(XMLUni::fgDOMErrorHandler, &eh);
  if (!parser->loadGrammar(shortDir.c_str(), Grammar::SchemaGrammarType, true)) {
    err << "Error: unable to load : " << shortDir << std::ends;
    /// \error Unable to recognize schema grammar
    logger->Error(err.str());
    return false;
  }
  if (eh.failed()) { // TODO append error some how
    err << "Error loading : " << shortDir << std::ends;
    /// \error Unable to load
    logger->Error(err.str());
    return false;
  }
  parser->release();

  // Lock the grammar pool. This is necessary if we plan to use the
  // same grammar pool in multiple threads (this way we can reuse the
  // same grammar in multiple parsers). Locking the pool disallows any
  // modifications to the pool, such as an attempt by one of the threads
  // to cache additional schemas.
  m_GrammerPool->lockPool();
  m_Initialized = true;
  return true;
}
//-----------------------------------------------------------------------------
DOMLSParser* Serializer::CreateParser(Logger* logger) const
{
  const XMLCh ls_id[] = { chLatin_L, chLatin_S, chNull };

  DOMImplementation* impl(DOMImplementationRegistry::getDOMImplementation(ls_id));

  DOMLSParser* parser(
    impl->createLSParser(
      DOMImplementationLS::MODE_SYNCHRONOUS,
      0,
      XMLPlatformUtils::fgMemoryManager,
      m_GrammerPool.get()));

  DOMConfiguration* conf(parser->getDomConfig());

  // Commonly useful configuration.
  //
  conf->setParameter(XMLUni::fgDOMComments, false);
  conf->setParameter(XMLUni::fgDOMDatatypeNormalization, true);
  conf->setParameter(XMLUni::fgDOMEntities, false);
  conf->setParameter(XMLUni::fgDOMNamespaces, true);
  conf->setParameter(XMLUni::fgDOMElementContentWhitespace, false);

  // Enable validation.
  //
  conf->setParameter(XMLUni::fgDOMValidate, true);
  conf->setParameter(XMLUni::fgXercesSchema, true);
  conf->setParameter(XMLUni::fgXercesSchemaFullChecking, false);

  // Use the loaded grammar during parsing.
  //
  conf->setParameter(XMLUni::fgXercesUseCachedGrammarInParse, true);

  // Don't load schemas from any other source (e.g., from XML document's
  // xsi:schemaLocation attributes).
  //
  conf->setParameter(XMLUni::fgXercesLoadSchema, false);

  // Xerces-C++ 3.1.0 is the first version with working multi
  // import support.
  //
#if _XERCES_VERSION >= 30100
  conf->setParameter(XMLUni::fgXercesHandleMultipleImports, true);
#endif

  // We will release the DOM document ourselves.
  //
  conf->setParameter(XMLUni::fgXercesUserAdoptsDOMDocument, true);

  return parser;
}
//-----------------------------------------------------------------------------
std::unique_ptr<CDM::ObjectData> Serializer::ReadFile(const char* xmlFile, Logger* logger)
{
  return ReadFile( std::string{ xmlFile }, logger);
}
//-----------------------------------------------------------------------------
std::unique_ptr<CDM::ObjectData> Serializer::ReadFile(const std::string& xmlFile, Logger* logger)
{
  ScopedFileSystemLock lock;

  if (m_me == nullptr)
    m_me = new Serializer();

  if (!m_me->m_Initialized && !m_me->Initialize(logger)) {
    /// \error Serializer was not able to initialize
    logger->Error("Serializer was not able to initialize");
    return std::unique_ptr<CDM::ObjectData>();
  }
  ErrorHandler eh;
  std::stringstream err;
  std::unique_ptr<DOMLSParser> parser(m_me->CreateParser(logger));
  parser->getDomConfig()->setParameter(XMLUni::fgDOMErrorHandler, &eh);

  const std::string resolved_xmlFile = ResolvePath(xmlFile);
  std::unique_ptr<xercesc::DOMDocument> doc(parser->parseURI( resolved_xmlFile.c_str()));
  if (eh.failed() || doc == nullptr) {
    // TODO Append parse error
    /// \error Error reading xml file
    err << "Error reading xml file " << xmlFile << "\n"
        << eh.getError() << std::ends;
    logger->Error(err.str());
    return std::unique_ptr<CDM::ObjectData>();
  }
  // Let's see what kind of object this is
  DOMElement* root(doc->getDocumentElement());
  std::string ns(xml::transcode<char>(root->getNamespaceURI()));
  std::string name(xml::transcode<char>(root->getLocalName()));

  std::unique_ptr<CDM::ObjectData> obj = std::unique_ptr<CDM::ObjectData>();
  // Check the name and read it into the right object type
  if (name.compare("Substance") == 0)
    return std::unique_ptr<CDM::ObjectData>((CDM::ObjectData*)CDM::Substance(*doc).release());
  if (name.compare("Patient") == 0)
    return std::unique_ptr<CDM::ObjectData>((CDM::ObjectData*)CDM::Patient(*doc).release());
  if (name.compare("SubstanceCompound") == 0)
    return std::unique_ptr<CDM::ObjectData>((CDM::ObjectData*)CDM::SubstanceCompound(*doc).release());
  if (name.compare("Scenario") == 0)
    return std::unique_ptr<CDM::ObjectData>((CDM::ObjectData*)CDM::Scenario(*doc).release());
  if (name.compare("EnvironmentalConditions") == 0)
    return std::unique_ptr<CDM::ObjectData>((CDM::ObjectData*)CDM::EnvironmentalConditions(*doc).release());
  if (name.compare("ElectroCardioGramWaveformInterpolator") == 0)
    return std::unique_ptr<CDM::ObjectData>((CDM::ObjectData*)CDM::ElectroCardioGramWaveformInterpolator(*doc).release());
  if (name.compare("Nutrition") == 0)
    return std::unique_ptr<CDM::ObjectData>((CDM::ObjectData*)CDM::Nutrition(*doc).release());
  if (name.compare("PhysiologyEngineDynamicStabilization") == 0)
    return std::unique_ptr<CDM::ObjectData>((CDM::ObjectData*)CDM::PhysiologyEngineDynamicStabilization(*doc).release());
  if (name.compare("PhysiologyEngineTimedStabilization") == 0)
    return std::unique_ptr<CDM::ObjectData>((CDM::ObjectData*)CDM::PhysiologyEngineTimedStabilization(*doc).release());
  if (name.compare("CircuitManager") == 0)
    return std::unique_ptr<CDM::ObjectData>((CDM::ObjectData*)CDM::CircuitManager(*doc).release());
  if (name.compare("CompartmentManager") == 0)
    return std::unique_ptr<CDM::ObjectData>((CDM::ObjectData*)CDM::CompartmentManager(*doc).release());
  if (name.compare("BioGearsConfiguration") == 0)
    return std::unique_ptr<CDM::ObjectData>((CDM::ObjectData*)CDM::BioGearsConfiguration(*doc).release());
  if (name.compare("BioGearsState") == 0)
    return std::unique_ptr<CDM::ObjectData>((CDM::ObjectData*)CDM::BioGearsState(*doc).release());
  if (name.compare("DataRequests") == 0)
    return std::unique_ptr<CDM::ObjectData>((CDM::ObjectData*)CDM::DataRequests(*doc).release());

  /// \error Unsupported root tag
  err << "Unsupported root tag " << name << " found in xml file " << xmlFile << std::ends;
  logger->Error(err.str());
  return obj;
}
//-----------------------------------------------------------------------------
}