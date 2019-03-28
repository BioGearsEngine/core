#include <string>

namespace biogears
{
class ReportWriter
{
public:
  ReportWriter();
  ~ReportWriter();
  std::string to_markdown();
  std::string to_html();
  std::string to_xml();
};  

}