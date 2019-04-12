#include <map>
#include <string>
#include <vector>

namespace biogears {
class TableRow {
public:
  TableRow();
  TableRow(std::string field_name, std::string expected_value, double engine_value, std::string percent_error, std::string notes);
  ~TableRow();
  bool passed;
  std::string field_name; //field_name = value_name+unit_name
  std::string expected_value; //expected_value = reference_value+'@cite'+reference
  std::string percent_error; //
  std::string notes; //
  std::string table_name;
  double engine_value; //
};

class ReferenceValue {
public:
  ReferenceValue();
  ~ReferenceValue();
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

class ReportWriter {
public:
  ReportWriter();
  ~ReportWriter();
  std::string to_markdown();
  std::string to_html();
  std::string to_xml();
  void gen_tables_single_sheet(const char* validation_file, const char* baseline_file);
  void gen_tables_single_sheet(std::string validation_file, std::string baseline_file);
  void gen_tables();
  void ParseReferenceCSV(const char* filename);
  void ParseReferenceCSV(std::string filename);
  void ParseBaselineCSV(const char* filename);
  void ParseBaselineCSV(std::string filename);
  void CalculateAverages();
  void ExtractValues();
  void Validate();
  void PopulateTables();
  void clear();

private:
  void ParseCSV(std::string& filename, std::vector<std::vector<std::string>>& vec);
  std::map<std::string, std::vector<biogears::TableRow>> tables;
  std::map<std::string, biogears::TableRow> table_row_map;
  std::vector<biogears::ReferenceValue> reference_values;
  std::vector<std::vector<std::string>> validation_data;
  std::vector<std::vector<std::string>> biogears_results;
  std::string report;
};

} // end namespace biogears