#include "ReportWriter.h"
#include "Tokenizer.h"
#include <algorithm>
#include <fstream>
#include <numeric>
#include "string-helpers.h"

namespace biogears {
ReportWriter::ReportWriter() {}
ReportWriter::~ReportWriter() {}

std::string ReportWriter::to_html()
{
  report.append(std::string("<html><body><table border=\"1\">\n"));
  //...Do work
  for (std::vector<std::string> cell : validation_data) {
    std::string line("<tr>");
    for (std::string item : cell) {
      line.append("<td>");
      line.append(item);
      line.append("</td>");
    }
    line.append("</tr>\n");
    report.append(line);
  }
  //...Finish work
  report.append(std::string("</table></body></html>\n"));
  return report;
}

std::string ReportWriter::to_markdown()
{
  //...Do work
  // The reason for this block of code is that markdown is structured like
  // | Table Header 1 | Table Header 2 | Table Header 3 |
  // | -------------- | -------------- | -------------- |
  // So it's simpler to just generate the first two lines before the rest,
  // since line 2 is its own thing
  std::string first_line("|");
  std::string second_line("|");
  for (std::string item : validation_data[0]) {
    first_line.append(item + "|");
    second_line.append("---|");
  }
  first_line.append("\n");
  second_line.append("\n");
  report.append(first_line);
  report.append(second_line);

  for (int i = 1; i < validation_data.size(); i++) {
    std::vector<std::string> cell = validation_data[i];
    std::string line("|");
    for (std::string item : cell) {
      line.append(item);
      line.append("|");
    }
    line.append("\n");
    report.append(line);
  }
  //...Finish work
  return report;
}

std::string ReportWriter::to_xml()
{
  report.append(std::string("<xml><body><table border=\"1\">\n"));
  //...Do work
  for (std::vector<std::string> cell : validation_data) {
    std::string line("<tr>");
    for (std::string item : cell) {
      line.append("<td>");
      line.append(item);
      line.append("</td>");
    }
    line.append("</tr>\n");
    report.append(line);
  }
  //...Finish work
  report.append(std::string("</table></body></xml>\n"));
  return report;
}

void ReportWriter::ParseCSV(std::string& filename)
{
  std::ifstream file{ filename };
  if (!file.is_open()) {
    return;
  }
  std::string line;
  int line_number = 0;
  int index = 0;
  while (std::getline(file, line)) {
    std::string cell;
    std::vector<std::string> vec;
    validation_data.push_back(vec);
    for (int i = 0;i < line.size();i++) {
      if(line[i] == ',') {
        validation_data[line_number].push_back(cell);
        cell.clear();
      } else if (line[i] == '"') {
        while(true) {
          ++i;
          if (i == line.size()) {
            std::string next_line;
            std::getline(file,next_line);
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
    validation_data[line_number].push_back(cell);
    cell.clear();
    ++line_number;
  }
}

void ReportWriter::CalculateAverages(std::string& filename)
{
  std::ifstream file{ filename };
  if (!file.is_open()) {
    return;
  }
  std::vector<biogears::TableRow> rows;
  for(int i = 0;i < biogears_results[0].size();i++) {
    biogears::TableRow row;
    row.field_name = biogears_results[0][i];
    row.expected_value = 0.0;
    rows.push_back(row);
  }
  for(int i = 1;i < biogears_results.size();i++) {
    for(int k = 0;k < biogears_results[i].size();k++) {
      rows[k].engine_value += std::stod(biogears_results[i][k]);
    }
  }

  for(int i = 0;i < rows.size();i++) { 
    std::string field_name_with_units = rows[i].field_name;
    TableRow row = rows[i]; //So field_name_with_units looks like "Name(Unit)", which is why it gets split to just be "Name"
    table_row_map.insert(std::pair<std::string,biogears::TableRow>(split(field_name_with_units,'(')[0],row));
  }
}
// This method assumes a particular layout for the validation data csvs, if that layout is changed, this
// method will break
void ReportWriter::ExtractValues()
{
  for(int i = 1;i < validation_data.size();i++) {
    biogears::ReferenceValue ref;
    ref.value_name = validation_data[i][0];
    ref.unit_name = validation_data[i][1];
    if(validation_data[i][2][0] == '[') {
      ref.is_range = true; // In the case that the validation data looks like [num1,num2],....
      // This line splits [num1,num2],.... into a vector where the first element is num1,num2
      // Then it splits the first vector element into a vector where the first two elements are num1 and num2
      std::vector<std::string> value_range = split(split(validation_data[i][2].substr(1),']')[0],',');
      ref.reference_range = std::pair<double, double>(std::stod(value_range[0]),std::stod(value_range[1]));
    } else {
      ref.is_range = false;
      std::vector<std::string> value = split(validation_data[i][2],',');
      ref.reference_value = std::stod(value[0]);
    }
    ref.reference = split(validation_data[i][3],',')[0];
    ref.notes = validation_data[i][4];
    ref.table_name = validation_data[i][5];
    ref.error_threshold = 0.5; // This is a placeholder value
    reference_values.push_back(ref);
  }
}

void ReportWriter::Validate()
{
  for(biogears::ReferenceValue ref : reference_values) {
    biogears::TableRow table_row = table_row_map.find(ref.value_name)->second;
    if(ref.is_range) { 
      table_row.expected_value = "["+std::to_string(ref.reference_range.first)+","+std::to_string(ref.reference_range.second)+"]" + "@" + ref.reference;
      table_row.expected_value += ref.reference;
      if(ref.reference_range.first <= table_row.engine_value && ref.reference_range.second >= table_row.engine_value) {
        table_row.passed = true;
      } else {
        table_row.passed = false;
      }
    } else {
      table_row.expected_value = std::to_string(ref.reference_value)+"@"+ref.reference;

      double error = std::abs(1.0 - (ref.reference_value/table_row.engine_value));
      table_row.percent_error = error;
      if(ref.error_threshold >= error) {
        table_row.passed = true;
      } else {
        table_row.passed = false;
      }
    }
    if(ref.unit_name != "") {
      table_row.field_name += "(" + ref.unit_name + ")";
    }
    table_row.notes = ref.notes;
  }
}


}
