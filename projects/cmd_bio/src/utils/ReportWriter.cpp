#include "ReportWriter.h"

namespace biogears
{
  ReportWriter::ReportWriter()
  {
    
  }

  ReportWriter::~ReportWriter()
  {
    
  }

  std::string ReportWriter::to_html()
  {
    return std::string("html");
  }

  std::string ReportWriter::to_markdown()
  {
    return std::string("markdown");
  }

  std::string ReportWriter::to_xml()
  {
    return std::string("xml");
  }
}