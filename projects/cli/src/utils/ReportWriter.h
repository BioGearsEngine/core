#ifndef CMD_BIO_REPORTWRITER_H
#define CMD_BIO_REPORTWRITER_H

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <biogears/cdm/utils/Logger.h>

namespace biogears {

enum color {
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
  double max_value;   //used to track max value of data request--not shown in final table because not set in to_table function
  double min_value;   //used to track min value of data request--not shown in final table because not set in to_table function
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
  std::vector<double> reference_values;
  std::vector<std::pair<double, double>> reference_ranges;
};

class ReportWriter {
public:
  enum TYPE { HTML,
              MD,
              XML,
              WEB };
  ReportWriter();
  ~ReportWriter();
  void to_table();
  void set_html();
  void set_md();
  void set_xml();
  void set_web();

  void gen_tables_single_sheet(std::string reference_file, std::string results_file, char table_type);
  void gen_tables(TYPE table_type);
  void ParseReferenceCSV(const std::string filename);
  void ParseResultsCSV(const std::string filename);
  void CalculateAverages();
  void ExtractValues();
  void ExtractValuesList();

  void Validate();
  void PopulateTables();
  void clear(); // This does not reset the value of the pointers

private:
  void ParseCSV(std::istream& stream, std::vector<std::vector<std::string>>& vec);
  void ParseCSV(const std::string& filename, std::vector<std::vector<std::string>>& vec);
  void ParseXML(std::istream& stream);
  void ParseXML(const std::string& filename, const std::string test);
  std::map<std::string, std::vector<biogears::TableRow>> tables;
  std::map<std::string, biogears::TableRow> table_row_map;
  std::vector<biogears::ReferenceValue> reference_values;
  std::vector<std::vector<std::string>> validation_data;
  std::vector<std::vector<std::string>> biogears_results;
  std::string report;

  std::unique_ptr<biogears::Logger> logger;
  char const* _body_begin;
  char const* _table_begin;
  char const* _table_row_begin;
  char const* _table_row_begin_green;
  char const* _table_row_begin_red;
  char const* _table_row_begin_yellow;
  char const* _table_second_line;
  char const* _table_item_begin;
  char const* _table_item_end;
  char const* _table_row_end;
  char const* _table_end;
  char const* _body_end;
  char const* _file_extension;
};

} // end namespace biogears
#endif //CMD_BIO_REPORTWRITER_H