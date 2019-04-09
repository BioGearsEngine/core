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
  report.append(std::string("<html><body>\n"));
  //...Do work
  for(auto table_itr = tables_.begin();table_itr != tables_.end();++table_itr) {
    report.append(std::string("<table border=\"1\">"));

    for(int i = 0;i < table_itr->second.size();i++) {
      std::string line("<tr");
      if(table_itr->second[i].passed) {
        line += "bgcolor=#32CD32>";
      } else {
        line += "bgcolor=#FF0000>";
      }
      line += "<td>";
      line += table_itr->second[i].field_name;
      line += "</td>";
      line += "<td>";
      line += table_itr->second[i].expected_value;
      line += "</td>";
      line += "<td>";
      line += std::to_string(table_itr->second[i].engine_value);
      line += "</td>";
      line += table_itr->second[i].percent_error;
      line += "</td>";
      line += "<td>";
      line += table_itr->second[i].notes;
      line += "</td>";
      line += "</tr>";
      report.append(line);
    }
    report.append(std::string("</table>"));
  }

  //...Finish work
  report.append(std::string("</body></html>\n"));
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
  report.append(std::string("<xml><body>\n"));
  //...Do work
  for (auto table_itr = tables_.begin(); table_itr != tables_.end(); ++table_itr) {
    report.append(std::string("<table border=\"1\">"));

    for (int i = 0; i < table_itr->second.size(); i++) {
      std::string line("<tr");
      if (table_itr->second[i].passed) {
        line += "bgcolor=#32CD32>";
      } else {
        line += "bgcolor=#FF0000>";
      }
      line += "<td>";
      line += table_itr->second[i].field_name;
      line += "</td>";
      line += "<td>";
      line += table_itr->second[i].expected_value;
      line += "</td>";
      line += "<td>";
      line += std::to_string(table_itr->second[i].engine_value);
      line += "</td>";
      line += table_itr->second[i].percent_error;
      line += "</td>";
      line += "<td>";
      line += table_itr->second[i].notes;
      line += "</td>";
      line += "</tr>";
      report.append(line);
    }
    report.append(std::string("</table>"));
  }

  //...Finish work
  report.append(std::string("</body></xml>\n"));
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
    table_row.table_name = ref.table_name;
  }
}

void ReportWriter::PopulateTables()
{
  // This block of code is to have at least the first row in every table, since we're going to have to print out at least one 
  // line for all of them. 
  std::vector<std::vector<std::string>> bcvec; //BloodChemistry
  bcvec.push_back(std::vector<std::string> {"BloodChemistry","Expected Value","Engine Value","Percent Error","Notes"});
  std::vector<std::vector<std::string>> cmpvec; //CompleteMetabolicPanel
  cmpvec.push_back(std::vector<std::string>{ "CompleteMetabolicPanel", "Expected Value", "Engine Value", "Percent Error", "Notes" });
  std::vector<std::vector<std::string>> cbcvec; //CompleteBloodCount
  cbcvec.push_back(std::vector<std::string>{ "CompleteBloodCount", "Expected Value", "Engine Value", "Percent Error", "Notes" });
  std::vector<std::vector<std::string>> cvec; //Cardiovascular
  cvec.push_back(std::vector<std::string>{ "Cardiovascular", "Expected Value", "Engine Value", "Percent Error", "Notes" });
  std::vector<std::vector<std::string>> ccvec; //CardiovascularComponents
  ccvec.push_back(std::vector<std::string>{ "CardiovascularComponents", "Expected Value", "Engine Value", "Percent Error", "Notes" });
  std::vector<std::vector<std::string>> evec; //Endocrine
  evec.push_back(std::vector<std::string>{ "Endocrine", "Expected Value", "Engine Value", "Percent Error", "Notes" });
  std::vector<std::vector<std::string>> envec; //Energy
  envec.push_back(std::vector<std::string>{ "Energy", "Expected Value", "Engine Value", "Percent Error", "Notes" });
  std::vector<std::vector<std::string>> gvec; //Gastrointestinal
  gvec.push_back(std::vector<std::string>{ "Gastrointestinal", "Expected Value", "Engine Value", "Percent Error", "Notes" });
  std::vector<std::vector<std::string>> nvec; //Nervous
  nvec.push_back(std::vector<std::string>{ "Nervous", "Expected Value", "Engine Value", "Percent Error", "Notes" });
  std::vector<std::vector<std::string>> rcvec; //RenalCompartments
  rcvec.push_back(std::vector<std::string>{ "RenalCompartments", "Expected Value", "Engine Value", "Percent Error", "Notes" });
  std::vector<std::vector<std::string>> uvec; //Urinalysis
  uvec.push_back(std::vector<std::string>{ "Urinalysis", "Expected Value", "Engine Value", "Percent Error", "Notes" });
  std::vector<std::vector<std::string>> rvec; //Renal
  rvec.push_back(std::vector<std::string>{ "Renal", "Expected Value", "Engine Value", "Percent Error", "Notes" });
  std::vector<std::vector<std::string>> rsvec; //RenalSubstances
  rsvec.push_back(std::vector<std::string>{ "RenalSubstances", "Expected Value", "Engine Value", "Percent Error", "Notes" });
  std::vector<std::vector<std::string>> revec; //Respiratory
  revec.push_back(std::vector<std::string>{ "Respiratory", "Expected Value", "Engine Value", "Percent Error", "Notes" });
  std::vector<std::vector<std::string>> pftvec; //PulmonaryFunctionTest
  pftvec.push_back(std::vector<std::string>{ "PulmonaryFunctionTest", "Expected Value", "Engine Value", "Percent Error", "Notes" });
  std::vector<std::vector<std::string>> recvec; //RespiratoryCompartments
  recvec.push_back(std::vector<std::string>{ "RespiratoryCompartments", "Expected Value", "Engine Value", "Percent Error", "Notes" });
  std::vector<std::vector<std::string>> tvec; //Tissue
  tvec.push_back(std::vector<std::string>{ "Tissue", "Expected Value", "Engine Value", "Percent Error", "Notes" });
  std::vector<std::vector<std::string>> tcvec; //TissueCompartments
  tcvec.push_back(std::vector<std::string>{ "TissueComparments", "Expected Value", "Engine Value", "Percent Error", "Notes" });
  std::vector<std::vector<std::string>> tsvec; //TissueSubstances
  tsvec.push_back(std::vector<std::string>{ "TissueSubstances", "Expected Value", "Engine Value", "Percent Error", "Notes" });
  std::vector<std::vector<std::string>> svvec; //SystemValidationData.xlsx
  svvec.push_back(std::vector<std::string>{ "SystemValidationData.xlsx", "Expected Value", "Engine Value", "Percent Error", "Notes" });

  tables.insert(std::pair<std::string, std::vector<std::vector<std::string>>>(std::string("BloodChemistry"), bcvec));
  tables.insert(std::pair<std::string, std::vector<std::vector<std::string>>>(std::string("CompleteMetabolicPanel"), cmpvec));
  tables.insert(std::pair<std::string, std::vector<std::vector<std::string>>>(std::string("CompleteBloodCount"), cbcvec));
  tables.insert(std::pair<std::string, std::vector<std::vector<std::string>>>(std::string("Cardiovascular"), cvec));
  tables.insert(std::pair<std::string, std::vector<std::vector<std::string>>>(std::string("CardiovascularComponents"), ccvec));
  tables.insert(std::pair<std::string, std::vector<std::vector<std::string>>>(std::string("Endocrine"), evec));
  tables.insert(std::pair<std::string, std::vector<std::vector<std::string>>>(std::string("Energy"), envec));
  tables.insert(std::pair<std::string, std::vector<std::vector<std::string>>>(std::string("Gastrointestinal"), gvec));
  tables.insert(std::pair<std::string, std::vector<std::vector<std::string>>>(std::string("Nervous"), nvec));
  tables.insert(std::pair<std::string, std::vector<std::vector<std::string>>>(std::string("RenalCompartments"), rcvec));
  tables.insert(std::pair<std::string, std::vector<std::vector<std::string>>>(std::string("Urinalysis"), uvec));
  tables.insert(std::pair<std::string, std::vector<std::vector<std::string>>>(std::string("Renal"), rvec));
  tables.insert(std::pair<std::string, std::vector<std::vector<std::string>>>(std::string("RenalSubstances"), rsvec));
  tables.insert(std::pair<std::string, std::vector<std::vector<std::string>>>(std::string("Respiratory"), revec));
  tables.insert(std::pair<std::string, std::vector<std::vector<std::string>>>(std::string("PulmonaryFunctionTest"), pftvec));
  tables.insert(std::pair<std::string, std::vector<std::vector<std::string>>>(std::string("RespiratoryCompartments"), recvec));
  tables.insert(std::pair<std::string, std::vector<std::vector<std::string>>>(std::string("Tissue"), tvec));
  tables.insert(std::pair<std::string, std::vector<std::vector<std::string>>>(std::string("TissueCompartments"), tcvec));
  tables.insert(std::pair<std::string, std::vector<std::vector<std::string>>>(std::string("TissueSubstances"), tsvec));
  tables.insert(std::pair<std::string, std::vector<std::vector<std::string>>>(std::string("SystemValidationData.xlsx"), svvec));

  for(auto itr = table_row_map.begin();itr != table_row_map.end();++itr) {
//  So this line pulls the table corresponding to the table_name of the row object, it then pushes a vector with the following layout
//  field_name, expected_value, engine_value, percent_error, notes 
    tables.find(itr->second.table_name)->second.push_back(std::vector<std::string>{ itr->second.field_name, itr->second.expected_value, std::to_string(itr->second.engine_value), itr->second.percent_error, itr->second.notes });
    tables_.find(itr->second.table_name)->second.push_back(itr->second);
  }
}


}
