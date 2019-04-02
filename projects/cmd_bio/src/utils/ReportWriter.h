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
  void CalculateAverages(std::string& filename);
private:
  std::vector<std::pair<std::string,double>> average_values;
  std::vector<std::vector<double>> values;
  std::vector<std::vector<std::string>> cells;
  std::string report;
};  


} // end namespace biogears