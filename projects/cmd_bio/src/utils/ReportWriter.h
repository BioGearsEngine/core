#include <vector>
#include <string>
#include <map>

namespace biogears
{
class TableRow 
{
public:
  bool passed;
  std::string field_name; //field_name = value_name+unit_name
  std::string expected_value; //expected_value = reference_value+'@cite'+reference
  std::string percent_error; //
  std::string notes; //
  std::string table_name;
  double engine_value; //
};

class Report 
{
public:
  std::string table_name;
  std::vector<std::pair<std::string,std::vector<TableRow>>> tables;
};

class ReferenceValue 
{
public:
  bool is_range;
  std::string value_name;
  std::string unit_name;
  std::string table_name;
  std::string reference;
  std::string notes;
  std::pair<double, double> reference_range;
  double reference_value;
  double error_threshold;
};

class ReportWriter
{
public:
  ReportWriter();
  ~ReportWriter();
  std::string to_markdown();
  std::string to_html();
  std::string to_xml();
  void ParseCSV(std::string& filename);
  void CalculateAverages(std::string& filename);
  void ExtractValues();
  void Validate();
  void PopulateTables();
private:
  std::map<std::string,std::vector<std::vector<std::string>>> tables;
  std::map<std::string,std::vector<biogears::TableRow>> tables_;
  std::map<std::string, TableRow> table_row_map;
  std::vector<biogears::ReferenceValue> reference_values;
  std::vector<std::vector<std::string>> results_table;
  std::vector<std::vector<std::string>> validation_data;
  std::vector<std::vector<std::string>> biogears_results;
  std::string report;
};  

} // end namespace biogears