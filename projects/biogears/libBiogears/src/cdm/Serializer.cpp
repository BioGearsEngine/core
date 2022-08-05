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
#include <mutex>
#include <type_traits>
#include <fstream>

//Project Includes
#include <biogears/cdm/Serializer.h>

#include <biogears/filesystem/path.h>
#include <biogears/io/io-manager.h>

#include <biogears/schema/biogears/BioGears.hxx>

#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/framework/Wrapper4InputSource.hpp>

#ifdef BIOGEARS_IO_PRESENT
#include <biogears/io/directories/xsd.h>
#endif

namespace xml = xsd::cxx::xml;

XERCES_CPP_NAMESPACE_BEGIN

struct embedded_resource_resolver : DOMLSResourceResolver {
  xercesc::DOMLSInput* resolveResource(XMLCh const* const resourceType,
                                       XMLCh const* const namespaceUri,
                                       XMLCh const* const publicId,
                                       XMLCh const* const systemId,
                                       XMLCh const* const baseURI)
  {
#ifdef BIOGEARS_IO_PRESENT
    using ::biogears::filesystem::path;
    using namespace ::biogears::io;
    std::string systemId_str = xml::transcode<char>(systemId);
    std::string baseURI_str = xml::transcode<char>(baseURI);

    path normalized_path;
    if (baseURI_str != "uri:/mil/tatrc/physiology/datamodel") {
      path include_path = path(baseURI_str).parent_path() / systemId_str;
      normalized_path = include_path.make_normal();
    } else {
      normalized_path = systemId_str;
    }

    auto xsd_files = list_xsd_files();

    for (auto i = xsd_file_count(); i != 0; --i) {
      auto schema = xsd_files[i - 1];
      std::string n_path_str = normalized_path;
      if (std::strcmp(schema, n_path_str.c_str()) == 0) {
        size_t schema_size = 0;
        char const* schema_content = get_embedded_xsd_file(schema, schema_size);
        auto source = new xercesc::MemBufInputSource(reinterpret_cast<XMLByte const*>(schema_content), schema_size, schema);
        return new xercesc::Wrapper4InputSource(source);
      }
    }
#endif
    return nullptr;
  }
};
embedded_resource_resolver g_embedded_resource_resolver;
XERCES_CPP_NAMESPACE_END

namespace biogears {

std::mutex g_serializer_mutex;

Serializer* Serializer::m_me = nullptr;
bool Serializer::m_Initialized = false;

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
  bool warn(err.getSeverity() == xercesc::DOMError::DOM_SEVERITY_WARNING);

  if (!warn)
    failed_ = true;

  xercesc::DOMLocator* loc(err.getLocation());

  char* uri(xercesc::XMLString::transcode(loc->getURI()));
  char* msg(xercesc::XMLString::transcode(err.getMessage()));

  error_ << uri << ":"
         << loc->getLineNumber() << ":" << loc->getColumnNumber() << " "
         << (warn ? "warning: " : "error: ") << msg << std::ends;
  /// \error Invalid schema file
  std::cerr << std::flush << error_.str() << "\n" << err.getRelatedData() << "\n" << err.getRelatedException() << std::endl;

  xercesc::XMLString::release(&uri);
  xercesc::XMLString::release(&msg);

  return true;
}
//-----------------------------------------------------------------------------
Serializer::Serializer()
{
  m_buffer.resize(5 * 1024 * 1024, '\0'); //5MBs Average State file is 2MBs
  m_buffer_size = sizeof(XMLByte) * m_buffer.size();
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
  m_GrammerPool.reset(new xercesc::XMLGrammarPoolImpl());

  auto io = logger->GetIoManager().lock();
  std::string xsd_filepath = io->find_resource_file("xsd/BioGearsDataModel.xsd");
  ErrorHandler eh;
  if (!xsd_filepath.empty()) {
    std::unique_ptr<xercesc::DOMLSParser> parser { CreateParser(logger, false) };
    parser->getDomConfig()->setParameter(xercesc::XMLUni::fgDOMErrorHandler, &eh);

    if (!parser->loadGrammar(xsd_filepath.c_str(), xercesc::Grammar::SchemaGrammarType, true)) {
      err << "Error: unable to load : " << xsd_filepath << std::ends;
      /// \error Unable to recognize schema xercesc::Grammar
      logger->Error(err.str());
      return false;
    }
    if (eh.failed()) { // TODO append error some how
      err << "Error loading : " << xsd_filepath << std::ends;
      /// \error Unable to load
      logger->Error(err.str());
      return false;
    }
    //parser->release(); //Careful hear this is to call the DOMLSParser not the unique_ptr release

    // Lock the xercesc::Grammar pool. This is necessary if we plan to use the
    // same xercesc::Grammar pool in multiple threads (this way we can reuse the
    // same xercesc::Grammar in multiple parsers). Locking the pool disallows any
    // modifications to the pool, such as an attempt by one of the threads
    // to cache additional schemas.
    m_GrammerPool->lockPool();
    m_Initialized = true;
    return true;
  } else {
#if BIOGEARS_IO_PRESENT

    std::unique_ptr<xercesc::DOMLSParser> parser { CreateParser(logger, true) };
    parser->getDomConfig()->setParameter(xercesc::XMLUni::fgDOMErrorHandler, &eh);
    size_t schema_size = 0;

    //for (auto i = io::xsd_file_count(); i != 0; --i) {
    schema_size = 0;
    //char const* schema_content = io::get_embedded_xsd_file(io::list_xsd_files()[i - 1], schema_size);
    //xercesc::MemBufInputSource schema_buffer { reinterpret_cast<XMLByte const*>(schema_content), schema_size, io::list_xsd_files()[i - 1] };

    char const* schema_content = io::get_embedded_xsd_file("BioGearsDataModel.xsd", schema_size);
    xercesc::MemBufInputSource schema_buffer { reinterpret_cast<XMLByte const*>(schema_content), schema_size, "uri:/mil/tatrc/physiology/datamodel" };
    //char const* schema_content = io::get_embedded_xsd_file(io::list_xsd_files()[i - 1], schema_size);
    //xercesc::MemBufInputSource schema_buffer { reinterpret_cast<XMLByte const*>(schema_content), schema_size, io::list_xsd_files()[i - 1] };
    xercesc::Wrapper4InputSource embeded_is { &schema_buffer, false };
    auto grammer = parser->loadGrammar(&embeded_is, xercesc::Grammar::SchemaGrammarType, true);
    if (grammer == nullptr) {
      err << "Error: unable to load : " << xsd_filepath << std::ends;
      /// \error Unable to recognize schema xercesc::Grammar
      logger->Error(err.str());
      return false;
    }
    if (eh.failed()) { // TODO append error some how
      err << "Error loading : " << xsd_filepath << std::ends;
      /// \error Unable to load
      logger->Error(err.str());
      return false;
    }
    //}

    m_GrammerPool->lockPool();
    m_Initialized = true;
    return true;
#else
    logger->Error("Unable to find xsd/BioGearsDataModel.xsd");
    return false;
#endif
  }
}
//-----------------------------------------------------------------------------
xercesc::DOMLSParser* Serializer::CreateParser(Logger* logger, bool embeddedXSD) const
{
  using namespace xercesc;
  const XMLCh ls_id[] = { chLatin_L, chLatin_S, chNull };

  DOMImplementation* impl(DOMImplementationRegistry::getDOMImplementation(ls_id));

  xercesc::DOMLSParser* parser(
    impl->createLSParser(
      DOMImplementationLS::MODE_SYNCHRONOUS,
      0,
      XMLPlatformUtils::fgMemoryManager,
      m_GrammerPool.get()));

  DOMConfiguration* conf(parser->getDomConfig());

  // Commonly useful configuration.
  //
  conf->setParameter(xercesc::XMLUni::fgDOMComments, false);
  conf->setParameter(xercesc::XMLUni::fgDOMDatatypeNormalization, true);
  conf->setParameter(xercesc::XMLUni::fgDOMEntities, false);
  conf->setParameter(xercesc::XMLUni::fgDOMNamespaces, true);
  conf->setParameter(xercesc::XMLUni::fgDOMElementContentWhitespace, false);

  // Enable validation.
  //
  conf->setParameter(xercesc::XMLUni::fgDOMValidate, true);
  conf->setParameter(xercesc::XMLUni::fgXercesSchema, true);
  conf->setParameter(xercesc::XMLUni::fgXercesSchemaFullChecking, false);

  // Use the loaded xercesc::Grammar during parsing.
  //
  conf->setParameter(xercesc::XMLUni::fgXercesUseCachedGrammarInParse, true);

  // Don't load schemas from any other source (e.g., from XML document's
  // xsi:schemaLocation attributes).
  //
  conf->setParameter(xercesc::XMLUni::fgXercesLoadSchema, false);

  // Xerces-C++ 3.1.0 is the first version with working multi
  // import support.
  //
#if _XERCES_VERSION >= 30100
  conf->setParameter(xercesc::XMLUni::fgXercesHandleMultipleImports, true);
#endif

  // We will release the DOM document ourselves.
  //
  conf->setParameter(xercesc::XMLUni::fgXercesUserAdoptsDOMDocument, true);

#ifdef BIOGEARS_IO_PRESENT
  if (embeddedXSD) {
    conf->setParameter(xercesc::XMLUni::fgDOMResourceResolver, &g_embedded_resource_resolver);
  }
#endif
  return parser;
}
//-----------------------------------------------------------------------------
std::unique_ptr<CDM::ObjectData> Serializer::ReadFile(const char* xmlFile, Logger* logger)
{
  return ReadFile(std::string { xmlFile }, logger);
}
//-----------------------------------------------------------------------------
std::unique_ptr<CDM::ObjectData> Serializer::ReadFile(const std::string& xmlFile, Logger* logger)
{
  std::lock_guard<std::mutex> thread_lock { g_serializer_mutex };
  if (m_me == nullptr) {
    m_me = new Serializer();
  }

  if (!m_me->m_Initialized && !m_me->Initialize(logger)) {
    /// \error Serializer was not able to initialize
    logger->Error("Serializer was not able to initialize");
    return std::unique_ptr<CDM::ObjectData>();
  }
  ErrorHandler eh;
  std::stringstream err;
  std::unique_ptr<xercesc::DOMLSParser> parser(m_me->CreateParser(logger));
  parser->getDomConfig()->setParameter(xercesc::XMLUni::fgDOMErrorHandler, &eh);

  auto io = logger->GetIoManager().lock();
  size_t content_size = io->read_resource_file(xmlFile.c_str(), reinterpret_cast<char*>(&m_me->m_buffer[0]), m_me->m_buffer.size());
  m_me->m_buffer[content_size] =   std::ifstream::traits_type::eof(); 
  return ReadBuffer(&m_me->m_buffer[0], content_size, logger);
}
//-----------------------------------------------------------------------------
std::unique_ptr<CDM::ObjectData> Serializer::ReadBuffer(XMLByte const* buffer, size_t size, Logger* logger)
{

  if (m_me == nullptr) {
    m_me = new Serializer();
  }

  if (!m_me->m_Initialized && !m_me->Initialize(logger)) {
    /// \error Serializer was not able to initialize
    logger->Error("Serializer was not able to initialize");
    return std::unique_ptr<CDM::ObjectData>();
  }

  ErrorHandler eh;
  std::stringstream err;
  std::unique_ptr<xercesc::DOMLSParser> parser(m_me->CreateParser(logger));
  parser->getDomConfig()->setParameter(xercesc::XMLUni::fgDOMErrorHandler, &eh);

  //Cast: char const* -> unsigned char*
  xercesc::MemBufInputSource content((XMLByte*)buffer, size, "SeralizerBuffer", false);
  xercesc::Wrapper4InputSource content_wraper { &content, false };
  std::unique_ptr<xercesc::DOMDocument> doc(parser->parse(&content_wraper));

  if (eh.failed() || doc == nullptr) {
    // TODO Append parse error
    /// \error Error reading xml file
    err << "Error reading data from buffer\n"
        << eh.getError() << std::endl;
    logger->Error(err.str());
    return std::unique_ptr<CDM::ObjectData>();
  }
  // Let's see what kind of object this is
  xercesc::DOMElement* root(doc->getDocumentElement());
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
  err << "Unsupported root tag " << name << " found in buffer" << std::ends;
  logger->Error(err.str());
  return obj;
}

//-----------------------------------------------------------------------------
}
