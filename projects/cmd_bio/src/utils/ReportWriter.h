#include <vector>
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
  void generate();
  void ParseCSV(std::string& filename);
private:
  std::vector<std::vector<std::string>> cells;
  std::string report;
};  


} // end namespace biogears