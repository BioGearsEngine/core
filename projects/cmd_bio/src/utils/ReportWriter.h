#include <memory>
#include <map>
#include <string>
#include <vector>

#include <biogears/cdm/utils/Logger.h>

namespace biogears {

enum color
{
  Green = 0,
  Red = 1,
  Yellow = 2
};

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
  color result;
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
};

class ReportWriter {
public:
  ReportWriter();
  ~ReportWriter();
  int to_table();
  void set_html();
  void set_md();
  void set_xml();
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
  void clear(); // This does not reset the value of the pointers

private:
  void ParseCSV(std::string& filename, std::vector<std::vector<std::string>>& vec);
  void ParseXML(std::string& filename);
  std::map<std::string, std::vector<biogears::TableRow>> tables;
  std::map<std::string, biogears::TableRow> table_row_map;
  std::vector<biogears::ReferenceValue> reference_values;
  std::vector<std::vector<std::string>> validation_data;
  std::vector<std::vector<std::string>> biogears_results;
  std::string report;

  std::unique_ptr<biogears::Logger> logger;
  char* body_begin;
  char* table_begin;
  char* table_row_begin;
  char* table_row_begin_green;
  char* table_row_begin_red;
  char* table_row_begin_yellow;
  char* table_second_line;
  char* table_item_begin;
  char* table_item_end;
  char* table_row_end;
  char* table_end;
  char* body_end;
  char* file_extension;
};

} // end namespace biogears