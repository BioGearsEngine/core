#include "ReportWriter.h"
#include "Tokenizer.h"
#include "string-helpers.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <numeric>
#include <iomanip>

namespace biogears {

TableRow::TableRow() {}
TableRow::TableRow(std::string field_n, std::string expected_v, double engine_v, std::string percent_e, std::string n)
{
  field_name = field_n;
  expected_value = expected_v;
  engine_value = engine_v;
  percent_error = percent_e;
  notes = n;
  result = Green;
}
TableRow::~TableRow() {}

ReferenceValue::ReferenceValue() {}
ReferenceValue::~ReferenceValue() {}

ReportWriter::ReportWriter()
{
  logger = std::make_unique<biogears::Logger>("gen-tables.log");
}
ReportWriter::~ReportWriter()
{
  logger = nullptr;
}

void ReportWriter::set_html()
{
  body_begin = "<html><body>\n";
  table_begin = "<table border=\"1\">\n";
  table_row_begin = "<tr>";
  table_row_begin_green = "<tr bgcolor=#32CD32>";
  table_row_begin_red = "<tr bgcolor=#FF0000>";
  table_row_begin_yellow = "<tr bgcolor=#FFFF99>";
  table_second_line = "";
  table_item_begin = "<td>";
  table_item_end = "</td>";
  table_row_end = "</td></tr>\n";
  table_end = "</table>\n";
  body_end = "</body></html>\n";
}

void ReportWriter::set_md()
{
  body_begin = "";
  table_begin = "";
  table_row_begin = "";
  table_row_begin_green = "<span class=\"success\">";
  table_row_begin_red = "<span class=\"failure\">";
  table_row_begin_yellow = "<span class=\"warning\">";
  table_second_line = "|---|---|---|---|---|\n";
  table_item_begin = "";
  table_item_end = "|";
  table_row_end = "</span>|\n";
  table_end = "\n";
  body_end = "\n";
}

void ReportWriter::set_xml()
{
  body_begin = "<xml><body>\n";
  table_begin = "<table border=\"1\">\n";
  table_row_begin = "<tr>";
  table_row_begin_green = "<tr bgcolor=#32CD32>";
  table_row_begin_red = "<tr bgcolor=#FF0000>";
  table_row_begin_yellow = "<tr bgcolor=#FFFF99>";
  table_second_line = "";
  table_item_begin = "<td>";
  table_item_end = "</td>";
  table_row_end = "</td></tr>\n";
  table_end = "</table>\n";
  body_end = "</body></xml>\n";
}


void ReportWriter::gen_tables_single_sheet(const char* validation_file, const char* baseline_file)
{
  gen_tables_single_sheet(std::string(validation_file), std::string(baseline_file));
}

void ReportWriter::gen_tables_single_sheet(std::string validation_file, std::string baseline_file)
{
  ParseReferenceCSV(validation_file);
  ParseBaselineCSV(baseline_file);
  CalculateAverages();
  ExtractValues();
  Validate();
  PopulateTables();
  to_table();
  clear();
}

void ReportWriter::gen_tables()
{
  std::vector<std::string> validation_files{ "BloodChemistryValidation.csv",
                                             "CardiovascularValidation.csv",
                                             "EnergyValidation.csv",
                                             "EndocrineValidation.csv",
                                             "RenalValidation.csv",
                                             "TissueValidation.csv" };
  std::vector<std::string> baseline_files{ "BloodChemistryValidationResults.csv",
                                           "CardiovascularValidationResults.csv",
                                           "EnergyValidationResults.csv",
                                           "EndocrineValidationResults.csv",
                                           "RenalValidationResults.csv",
                                           "TissueValidationResults.csv" };

  for (int i = 0; i < validation_files.size(); i++) {
    logger->Info("Generating table: " + split(validation_files[i],'.')[0]);
    logger->SetConsolesetConversionPattern("\t%m%n");
    ParseReferenceCSV(std::string(validation_files[i]));
    ParseBaselineCSV(std::string(baseline_files[i]));
    CalculateAverages();
    logger->Info("Successfully calculated averages of file: " + baseline_files[i]);
    ExtractValues();
    logger->Info("Successfully populated data structures with validation data");
    Validate();
    logger->Info("Successfully ran validation");
    PopulateTables();
    logger->Info("Successfully populated tables vector");
    set_md();
    to_table();
    logger->Info("Successfully generated table: " + split(validation_files[i],'.')[0]);
    clear();
    logger->SetConsolesetConversionPattern("%d [%p] %m%n");
  }
  return;
}

int ReportWriter::to_table()
{
  report.append(body_begin);
  //...Do work
  for (auto table_itr = tables.begin(); table_itr != tables.end(); ++table_itr) {
    std::string table;
    std::string table_name = table_itr->first;
    table += std::string(table_begin);
    for (int i = 0; i < table_itr->second.size(); i++) {
      std::string line;
      if (table_itr->second[i].result == Green) {
        line += (i == 0) ? table_row_begin : table_row_begin_green;
      } else if (table_itr->second[i].result == Red) {
        line += (i == 0) ? table_row_begin : table_row_begin_red;
      } else if (table_itr->second[i].result == Yellow) { //This line isn't technically necessary, but I'm leaving it in for readability
        line += (i == 0) ? table_row_begin : table_row_begin_yellow;
      }
      line += table_item_begin;
      line += table_itr->second[i].field_name;
      line += table_item_end;
      line += table_item_begin;
      line += table_itr->second[i].expected_value;
      line += table_item_end;
      line += table_item_begin;
      line += (i == 0) ? "Engine Value" : std::to_string(table_itr->second[i].engine_value);
      line += table_item_end;
      line += table_item_begin;
      line += table_itr->second[i].percent_error;
      line += table_item_end;
      line += table_item_begin;
      line += table_itr->second[i].notes;
      line += table_row_end;
      table.append(line);
      if (i == 0) { //So markdown needs this line after the first line to know that it's representing a table
        table.append(table_second_line);
      }
    }
    table += std::string(table_end);
    // This block saves out the html tables for website generation
    std::ofstream file;
    file.open("validation/tables/" + table_name + "ValidationTable.html");
    if (!file) {
      return 1;
    }
    file << (std::string(body_begin) + table + std::string(body_end));
    file.close();
    table.clear();
  }
  
  return 0;
}

void ReportWriter::ParseReferenceCSV(const char* filename)
{
  ParseReferenceCSV(std::string(filename));
}

void ReportWriter::ParseReferenceCSV(std::string filename)
{
  ParseCSV(filename, this->validation_data);
}

void ReportWriter::ParseBaselineCSV(const char* filename)
{
  ParseBaselineCSV(std::string(filename));
}

void ReportWriter::ParseBaselineCSV(std::string filename)
{
  ParseCSV(filename, this->biogears_results);
}

void ReportWriter::ParseCSV(std::string& filename, std::vector<std::vector<std::string>>& data)
{
  std::ifstream file{ filename };
  if (!file.is_open()) {
    logger->Error("Error opening: " + filename);
    return;
  }
  std::string line;
  int line_number = 0;
  int index = 0;
  while (std::getline(file, line)) {
    std::string cell;
    std::vector<std::string> vec;
    data.push_back(vec);
    for (int i = 0; i < line.size(); i++) {
      if (line[i] == ',') {
        data[line_number].push_back(ltrim(cell));
        cell.clear();
      } else if (line[i] == '"') {
        while (true) {
          ++i;
          if (i == line.size()) {
            std::string next_line;
            std::getline(file, next_line);
            line += next_line;
          }
          if (line[i] == '"') {
            break;
          } else {
            cell += line[i];
          }
        }
      } else {
        cell += line[i];
      }
    }
    data[line_number].push_back(ltrim(cell));
    cell.clear();
    ++line_number;
  }
  for(int i = 0;i < data[0].size();++i)
  {
    logger->Info(data[0][i]);
  }
  logger->Info("Successfully parsed file: " + filename);
}

void ReportWriter::CalculateAverages()
{
  std::vector<biogears::TableRow> rows;
  for (int i = 0; i < biogears_results[0].size(); i++) {
    biogears::TableRow row;
    row.field_name = trim(biogears_results[0][i]);
    row.expected_value = "0.0";
    rows.push_back(row);
  }
  for (int i = 1; i < biogears_results.size(); i++) {
    for (int k = 0; k < biogears_results[i].size(); k++) {
      try {
        rows[k].engine_value += std::stod(biogears_results[i][k]);
      } catch (std::exception& e) {
        logger->Error(std::string("Error: ") + e.what());
        logger->Error("Cell Contents: " + biogears_results[i][k]);
        throw(e);
      }
    }
  }
  for (int i = 0; i < rows.size(); i++) {
    rows[i].engine_value /= biogears_results.size() - 1;
    std::string field_name_with_units = rows[i].field_name;
    TableRow row = rows[i]; //So field_name_with_units looks like "Name(Unit)", which is why it gets split to just be "Name"
    table_row_map.insert(std::pair<std::string, biogears::TableRow>(trim(split(field_name_with_units, '(')[0]), row));
  }
}

void ReportWriter::ExtractValues()
{
  for (int i = 1; i < validation_data.size(); i++) {
    biogears::ReferenceValue ref;
    ref.value_name = trim(validation_data[i][0]);
    ref.unit_name = validation_data[i][1];
    if (validation_data[i][2][0] == '[') {
      ref.is_range = true; // In the case that the validation data looks like [num1,num2],....
      // This line splits [num1,num2],.... into a vector where the first element is num1,num2
      // Then it splits the first vector element into a vector where the first two elements are num1 and num2
      std::vector<std::string> value_range = split(split(validation_data[i][2].substr(1), ']')[0], ',');
      try {
        ref.reference_range = std::pair<double, double>(std::stod(value_range[0]), std::stod(value_range[1]));
      } catch(std::exception& e) {
        logger->Error(std::string("Error: ") + e.what());
        logger->Error("Cell Contents: [" + value_range[0] + "," + value_range[1] + "]");
        throw(e);
      }
    } else {
      ref.is_range = false;
      std::vector<std::string> value = split(validation_data[i][2], ',');
      try {
        ref.reference_value = std::stod(value[0]);
      } catch (std::exception& e) {
        logger->Error(std::string("Error: ") + e.what());
        logger->Error("Cell Contents: " + value[0]);
        throw(e);
      }
    }
    ref.reference = split(validation_data[i][3], ',')[0];
    ref.notes = validation_data[i][4];
    ref.table_name = validation_data[i][5];
    reference_values.push_back(ref);
  }
}

void ReportWriter::Validate()
{
  for (biogears::ReferenceValue ref : reference_values) {
    logger->Info(ref.value_name);
    auto table_row_itr = table_row_map.find(trim(ref.value_name));
    if (table_row_itr == table_row_map.end()) {
      logger->Info(std::string("  Not Found"));
      continue;
    }
    biogears::TableRow table_row = table_row_itr->second;
    if (ref.is_range) {
      table_row.expected_value = "[" + std::to_string(ref.reference_range.first) + "," + std::to_string(ref.reference_range.second) + "]" + "@" + ref.reference;
      if (ref.reference_range.first <= table_row.engine_value && ref.reference_range.second >= table_row.engine_value) {
        table_row.result = Green;
        table_row.percent_error = "Within Bounds";
      } else {
        table_row.result = Red;
        table_row.percent_error = "Outside Bounds";
      }
    } else {
      table_row.expected_value = std::to_string(ref.reference_value) + "@" + ref.reference;
      if(std::fabs(ref.reference_value - table_row.engine_value) <= 0.000001) {
        table_row.percent_error = "0.0%";
        table_row.result = Green;
      } else {
        double error = (std::fabs(ref.reference_value - table_row.engine_value)/((ref.reference_value + table_row.engine_value)/2));
        error = std::fabs(error);
        table_row.percent_error = std::to_string(error*100) + "%";
        if (error < 0.10) {
          table_row.result = Green;
        } else if (0.10 <= error && error <= 0.25) {
          table_row.result = Yellow;
        } else {
          table_row.result = Red;
        }
      }
    }
    table_row.notes = ref.notes;
    table_row.table_name = ref.table_name;
    table_row_itr->second = table_row;
  }
}

void ReportWriter::PopulateTables()
{
  for (auto itr = table_row_map.begin(); itr != table_row_map.end(); ++itr) {
    auto table_itr = tables.find(itr->second.table_name);
    if (table_itr != tables.end()) {
      table_itr->second.push_back(itr->second);
    } else {
      biogears::TableRow tr(itr->second.table_name, "Expected Value", 0.0, "Percent Error", "Notes");
      std::vector<biogears::TableRow> tr_vec;
      tr_vec.push_back(tr);
      tables.insert(std::pair<std::string, std::vector<biogears::TableRow>>(itr->second.table_name, tr_vec));
      table_itr = tables.find(itr->second.table_name);
      table_itr->second.push_back(itr->second);
    }
  }
}

void ReportWriter::clear()
{
  tables.clear();
  table_row_map.clear();
  reference_values.clear();
  validation_data.clear();
  biogears_results.clear();
  report.clear();
}
}
