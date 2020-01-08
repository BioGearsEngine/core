#include "ReportWriter.h"
#include "Tokenizer.h"
#include "string-helpers.h"
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <sstream>

#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif

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
/*
 * The functions set_html(), set_md(), set_xml(), and set_web(), change the type of file produced by calling to_table()
 */
//-------------------------------------------------------------------------------
///! \brief Sets the table type to html, meaning to_table() will produce html files
//-------------------------------------------------------------------------------
void ReportWriter::set_html()
{
  _body_begin = "<html><body>\n";
  _table_begin = "<table border=\"1\">\n";
  _table_row_begin = "<tr>";
  _table_row_begin_green = "<tr bgcolor=#32CD32>";
  _table_row_begin_red = "<tr bgcolor=#FF0000>";
  _table_row_begin_yellow = "<tr bgcolor=#FFFF99>";
  _table_second_line = "";
  _table_item_begin = "<td>";
  _table_item_end = "</td>";
  _table_row_end = "</td></tr>\n";
  _table_end = "</table>\n";
  _body_end = "</body></html>\n";
  _file_extension = ".html";
}
//-------------------------------------------------------------------------------
///! \brief Sets the table type to md, meaning to_table() will produce md files
//-------------------------------------------------------------------------------
void ReportWriter::set_md()
{
  _body_begin = "";
  _table_begin = "";
  _table_row_begin = "";
  _table_row_begin_green = "<span class=\"success\">";
  _table_row_begin_red = "<span class=\"danger\">";
  _table_row_begin_yellow = "<span class=\"warning\">";
  _table_second_line = "---|---|---|---|---|\n";
  _table_item_begin = "";
  _table_item_end = "|";
  _table_row_end = "</span>|\n";
  _table_end = "\n";
  _body_end = "\n";
  _file_extension = ".md";
}
//-------------------------------------------------------------------------------
///! \brief Sets the table type to xml, meaning to_table() will produce xml files
//-------------------------------------------------------------------------------
void ReportWriter::set_xml()
{
  _body_begin = "<xml><body>\n";
  _table_begin = "<table border=\"1\">\n";
  _table_row_begin = "<tr>";
  _table_row_begin_green = "<tr bgcolor=#32CD32>";
  _table_row_begin_red = "<tr bgcolor=#FF0000>";
  _table_row_begin_yellow = "<tr bgcolor=#FFFF99>";
  _table_second_line = "";
  _table_item_begin = "<td>";
  _table_item_end = "</td>";
  _table_row_end = "</td></tr>\n";
  _table_end = "</table>\n";
  _body_end = "</body></xml>\n";
  _file_extension = ".xml";
}

//-------------------------------------------------------------------------------
///! \brief Sets the table type to web, meaning to_table() will produce html files with the .md file extension
//-------------------------------------------------------------------------------
void ReportWriter::set_web()
{ // For website generation if the doxygen preprocessing isn't working correctly 
  // with md files you should be able to just generate html files with the .md extension.
  // Doxygen will find the table and put it in the correct page.
  _body_begin = "<html><body>\n";
  _table_begin = "<table border=\"1\">\n";
  _table_row_begin = "<tr>";
  _table_row_begin_green = "<tr bgcolor=#32CD32>";
  _table_row_begin_red = "<tr bgcolor=#FF0000>";
  _table_row_begin_yellow = "<tr bgcolor=#FFFF99>";
  _table_second_line = "";
  _table_item_begin = "<td>";
  _table_item_end = "</td>";
  _table_row_end = "</td></tr>\n";
  _table_end = "</table>\n";
  _body_end = "</body></html>\n";
  _file_extension = ".md";
}

bool ReportWriter::gen_tables_single_sheet(const char* reference_file, const char* results_file, char table_type)
{
  return gen_tables_single_sheet(std::string(reference_file), std::string(results_file), table_type);
}
//-------------------------------------------------------------------------------
/// \brief Takes in a single sheet, unlike gen_tables which takes in a list of validation and baselines files
/// It is essentially one iteration of the for loop in gen_tables
/// \param reference_file: std::string name of file containing validation data
/// \param results_file: std::string name of file containing baseline data
/// \param table_type: char denoting what type of results file should be produced (html, md, or xml)
//-------------------------------------------------------------------------------

bool ReportWriter::gen_tables_single_sheet(std::string reference_file, std::string results_file, char table_type)

{
  logger->Info("Generating table: " + split(reference_file,'.')[0]);
  logger->SetConsolesetConversionPattern("\t%m%n");

  bool success = false;
  success = ParseReferenceCSV(reference_file);
  success &= ParseResultsCSV(results_file);
  if (CalculateAverages()) {
    //logger->Info("Successfully calculated averages of file: " + results_file);
  ExtractValues();
    //logger->Info("Successfully populated data structures with validation data");
  Validate();
    //logger->Info("Successfully ran validation");
  PopulateTables();
    //logger->Info("Successfully populated tables vector");
  if (table_type == 0) {
    set_html();
  } else if (table_type == 1) {
    set_md();
  } else if (table_type == 2) {
    set_xml();
  } else {
    set_web();
  }
  to_table();
  logger->Info("Successfully generated table: " + split(reference_file, '.')[0]);
  clear();
  logger->SetConsolesetConversionPattern("%d [%p] %m%n");
  } else {
    logger->Info("Failed to generate table: " + split(reference_file, '.')[0]);
    return false;
  }
  return true;
}
//-------------------------------------------------------------------------------
 /// \brief Takes a list of validation files and results files from biogears and produces
 /// validation tables for our documentation.
 /// \param table_type: char denoting what type of results file should be produced (html, md, or xml)
//-------------------------------------------------------------------------------
bool ReportWriter::gen_tables(TYPE table_type)
{
  std::vector<std::string> reference_files{ "BloodChemistryValidation.csv",
                                            "CardiovascularValidation.csv",
                                            "EnergyValidation.csv",
                                            "EndocrineValidation.csv",
                                            "RenalValidation.csv",
                                            "RespiratoryValidation.csv",
                                            "TissueValidation.csv" };
  std::vector<std::string> results_files{ "Scenarios/Validation/BloodChemistryValidationResults.csv",
                                          "Scenarios/Validation/CardiovascularValidationResults.csv",
                                          "Scenarios/Validation/EnergyValidationResults.csv",
                                          "Scenarios/Validation/EndocrineValidationResults.csv",
                                          "Scenarios/Validation/RenalValidationResults.csv",
                                          "Scenarios/Validation/RespiratoryValidationResults.csv",
                                          "Scenarios/Validation/TissueValidationResults.csv" };
  std::vector<std::string> xml_files { "HeatStrokeResultsCMP@2610.2s.xml" };


  bool success = true;
  for (int i = 0; i < reference_files.size(); i++) {
    logger->Info("Generating table: " + split(reference_files[i], '.')[0]);
    logger->SetConsolesetConversionPattern("\t%m%n");
    bool no_faults = false;
    if (ParseReferenceCSV(std::string(reference_files[i]))) {
      if (ParseResultsCSV(std::string(results_files[i]))) {
        if (i == 0 && ParseXML(xml_files[0])) {
          success = false;
          continue;
        }
        if (CalculateAverages()) {
          //logger->Info("Successfully calculated averages of file: " + results_files[i]);
          if (ExtractValues()) {
            //logger->Info("Successfully populated data structures with validation data");
    Validate();
            //logger->Info("Successfully ran validation");
    PopulateTables();
            //logger->Info("Successfully populated tables vector");
            if (table_type == HTML) {
      set_html();
            } else if (table_type == MD) {
      set_md();
            } else if (table_type == XML) {
      set_xml();
    } else {
      set_web();
    }

            if (!to_table()) {
              logger->Error("Failed to write table: " + split(reference_files[i], '.')[0]);
            } else {

    clear();
    logger->SetConsolesetConversionPattern("%d [%p] %m%n");
              no_faults = true;
            }
          }
  }
      }
    }
    if (no_faults) {
      logger->Info("Successfully generated table: " + split(reference_files[i], '.')[0]);
    } else {
      logger->Error("Failed generating table: " + split(reference_files[i], '.')[0]);
    }
    success &= no_faults;
  }
  return success;
}
//-------------------------------------------------------------------------------
 /// \brief Takes the data stored in tables (a map pairing table names (std::string) to a list of TableRow objects (std::vector<biogears::TableRow>)
 /// and prints out a full validation table for each item in tables.
//-------------------------------------------------------------------------------
bool ReportWriter::to_table()
{
  report.append(_body_begin);
  for (auto table_itr = tables.begin(); table_itr != tables.end(); ++table_itr) {
    std::string table;
    std::string table_name = table_itr->first;
    table += std::string(_table_begin);
    for (int i = 0; i < table_itr->second.size(); i++) { // This loop iterates through every TableRow inside table_itr
      std::string line;
      if (table_itr->second[i].result == Green) {
        line += (i == 0) ? _table_row_begin : _table_row_begin_green;
      } else if (table_itr->second[i].result == Red) {
        line += (i == 0) ? _table_row_begin : _table_row_begin_red;
      } else if (table_itr->second[i].result == Yellow) { //This line isn't technically necessary, but I'm leaving it in for readability
        line += (i == 0) ? _table_row_begin : _table_row_begin_yellow;
      }
      line += _table_item_begin;
      line += table_itr->second[i].field_name;
      line += _table_item_end;
      line += _table_item_begin;
      line += table_itr->second[i].expected_value;
      line += _table_item_end;
      line += _table_item_begin;
      line += (i == 0) ? "Engine Value" : std::to_string(table_itr->second[i].engine_value);
      line += _table_item_end;
      line += _table_item_begin;
      line += table_itr->second[i].percent_error;
      line += _table_item_end;
      line += _table_item_begin;
      line += table_itr->second[i].notes;
      line += _table_row_end;
      table.append(line);
      if (i == 0) { //So markdown needs this line after the first line to know that it's representing a table
        table.append(_table_second_line);
      }
    }
    table += std::string(_table_end);
    // This block saves out the html tables for website generation
    std::ofstream file;

#if _WIN32
    _mkdir("doc");
    _mkdir("doc/validation");
#else
    mkdir("doc",  S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    mkdir("doc/validation",  S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif

    file.open("doc/validation/" + table_name + "ValidationTable" + _file_extension);
    if (!file) {
      return false;
    }
    file << (std::string(_body_begin) + table + std::string(_body_end));
    file.close();
    table.clear();
  }

  return true;
}

bool ReportWriter::ParseReferenceCSV(const char* filename)
{
  return ParseReferenceCSV(std::string(filename));
}

bool ReportWriter::ParseReferenceCSV(std::string filename)
{
  return ParseCSV(filename, this->validation_data);
}


bool ReportWriter::ParseResultsCSV(const char* filename)
{
  return ParseResultsCSV(std::string(filename));
}

bool ReportWriter::ParseResultsCSV(std::string filename)

{
  return ParseCSV(filename, this->biogears_results);
}
//-------------------------------------------------------------------------------
/// \brief Parses a CSV file and loads it into a 2 dimension vector data, which represents a CSV.
/// data[y] represents an individual row of the CSV file, while data[y][x] represents an individual cell.
/// \param filename: Name of the CSV file to be parsed
/// \param data: Two dimensional vector for storing data from CSV file
//-------------------------------------------------------------------------------
bool ReportWriter::ParseCSV(std::string& filename, std::vector<std::vector<std::string>>& data)
{
  std::ifstream file{ filename };
  if (!file.is_open()) {
    logger->Error("Error opening: " + filename);
    return false;
  }
  std::string line;
  int line_number = 0;
  int index = 0;
  while (std::getline(file, line)) { //This loop reads in the file line by line
    std::string cell;
    std::vector<std::string> vec;
    data.push_back(vec);
    for (int i = 0; i < line.size(); i++) { // This loop iterates through the current line character by character
      if (line[i] == ',') {
        data[line_number].push_back(ltrim(cell));
        cell.clear();
      } else if (line[i] == '"') { //This check is necessary because certain cells may contain newline characters, and
        while (true) {             //std::getline treats any newline character as the end of a line, this means that 
          ++i;                     //certain cells are split across two calls of std::getline
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
  logger->Info("Successfully parsed file: " + filename);
  return true;
}
//-------------------------------------------------------------------------------
/// \brief Parses an XML file containing results from biogears, puts the relevant data into a TableRow object,
/// and inserts the TableRow into a map
/// \param filename: Name of XML file to be parsed
//-------------------------------------------------------------------------------
bool ReportWriter::ParseXML(std::string& filename)
{
  std::ifstream file{ filename };
  if (!file.is_open()) {
    logger->Error("Error opening: " + filename);
    return false;
  }
  std::string line;
  while (std::getline(file, line)) {        // This loop reads in the XML file line by line, then pulls out the value name, the unit name, and the value
    size_t name_index = line.find("p1:");   // The value name is always preceded by "p1:"
    if(name_index == std::string::npos || line.find("/p1:") != std::string::npos) {
      continue;
    }
    name_index += 3;
    size_t name_end = line.find(" ",name_index);
    size_t unit_index = line.find("unit=\"",name_end); //The unit name is always preceded by "unit="
    if (unit_index == std::string::npos) {
      continue;
    }
    unit_index += 6;
    size_t unit_end = line.find("\"",unit_index);
    size_t value_index = line.find("value=\"",unit_end) + 7; //The value is always preceded by "value="
    size_t value_end = line.find("\"/", value_index);
    biogears::TableRow xmlRow;
    std::string name = trim(line.substr(name_index, name_end - name_index));
    std::string unit = trim(line.substr(unit_index, unit_end - unit_index));
    std::string value = trim(line.substr(value_index, value_end - value_index));
    xmlRow.field_name = name+"("+unit+")";
    xmlRow.expected_value = "0.0";
    xmlRow.engine_value = std::stod(value);
    table_row_map.insert(std::pair<std::string, biogears::TableRow>(name,xmlRow));
  }

  return true;
}

//-------------------------------------------------------------------------------
/// \brief Takes the information from biogears_results, which one of the results CSV files, averages them, creates
/// a TableRow object for each one, and inserts them into a map of table rows
//-------------------------------------------------------------------------------
bool ReportWriter::CalculateAverages()
{
  std::vector<biogears::TableRow> rows;
  std::vector<int> row_items;
  for (int i = 0; i < biogears_results[0].size(); i++) { // This iterates across the top row of the CSV, which is the name of each value
    biogears::TableRow row;
    row.field_name = biogears_results[0][i];
    row.expected_value = "0.0";
    row.engine_value = 0.0;
    rows.push_back(row);
    row_items.push_back(0); // Because not every value was measured the same amount of times, we need to track this
  }                         // in order to take the average of each value
  
  for (int i = 1; i < biogears_results.size(); i++) {         // Because the data is stored in columns, we have to iterate down from each cell
    for (int k = 0; k < biogears_results[i].size(); k++) {    // of the second row
      try {
        if(trim(biogears_results[i][k]) != "") { //if trim returns an empty string, it means the cell is empty
          rows[k].engine_value += std::stod(biogears_results[i][k]); 
          ++row_items[k]; // Further down we'll calculate the average of these values by dividing engine_value by row_items
        }
      } catch (std::exception& e) { // This usually occurs if one of the cells contains non-numeric characters, std::stod will throw an exception
        logger->Error(std::string("Error: ") + e.what());
        logger->Error("Cell Contents: " + biogears_results[i][k]);
        return false;
      }
    }
  }
  for (int i = 0; i < rows.size(); i++) {
    rows[i].engine_value /= row_items[i]; // This line calculates the average of a value
    std::string field_name_with_units = rows[i].field_name;
    TableRow row = rows[i]; //So field_name_with_units looks like "Name(Unit)", which is why it gets split to just be "Name"
    table_row_map.insert(std::pair<std::string, biogears::TableRow>(trim(split(field_name_with_units, '(')[0]), row));
  }
  return true;
}
//-------------------------------------------------------------------------------
/// \brief Extracts the values from validation_data, which is one of the validation CSV files, and
/// creates a ReferenceValue object for each column
//-------------------------------------------------------------------------------
bool ReportWriter::ExtractValues()
{
  for (int i = 1; i < validation_data.size(); i++) {
    biogears::ReferenceValue ref;
    ref.value_name = validation_data[i][0];
    ref.unit_name = validation_data[i][1];
    if (validation_data[i][2][0] == '[') {
      ref.is_range = true; // In the case that the validation data looks like [num1,num2],....
      // This line splits [num1,num2],.... into a vector where the first element is num1,num2
      // Then it splits the first vector element into a vector where the first two elements are num1 and num2
      std::vector<std::string> value_range = split(split(validation_data[i][2].substr(1), ']')[0], ',');
      try {
        ref.reference_range = std::pair<double, double>(std::stod(value_range[0]), std::stod(value_range[1]));
      } catch (std::exception& e) {
        logger->Error(std::string("Error: ") + e.what());
        logger->Error("Cell Contents: [" + value_range[0] + "," + value_range[1] + "]");
        return false;
      }
    } else {
      ref.is_range = false;
      std::vector<std::string> value = split(validation_data[i][2], ',');
      try {
        ref.reference_value = std::stod(value[0]);
      } catch (std::exception& e) {
        logger->Error(std::string("Error: ") + e.what());
        logger->Error("Cell Contents: " + value[0]);
        return false;
      }
    }
    ref.reference = split(validation_data[i][3], ',')[0];
    ref.notes = validation_data[i][4];
    ref.table_name = validation_data[i][5];
    reference_values.push_back(ref);
  }
  return true;
}

/*
 * This function is not currently in use, it's part of planned future upgrades to the ReportWriter
 */
bool ReportWriter::ExtractValuesList()
{
  for (int i = 1; i < validation_data.size(); i++) {
    biogears::ReferenceValue ref;
    ref.value_name = validation_data[i][0];
    ref.unit_name = validation_data[i][1];
    std::string values = validation_data[i][2];
    for (size_t j = 0; j < values.size(); j++) {
      if (isdigit(values[j]) || values[j] == '-') {
        size_t eod = j + 1;
        while (eod != validation_data.size() && (isdigit(values[eod]) || values[eod] == '.' || values[eod] == '-' || values[eod] == 'E' || values[eod] == 'e')) {
          eod++;
        }
        try {
          ref.reference_values.push_back(std::stod(values.substr(j, eod - j)));
          j += ((eod - j) + 1);
        } catch (std::exception& e) {
          logger->Error(std::string("Error: ") + e.what());
          logger->Error("Cell Contents: " + values.substr(j ,eod - j));
          return false;
        }
      } else if (values[j] == '[') {
        std::vector<std::string> value_range = split(values.substr(j + 1, values.find_first_of(']', j + 1) - (1 + j)),',');
        try {
          ref.reference_ranges.push_back(std::pair<double, double>(std::stod(value_range[0]), std::stod(value_range[1])));
        } catch (std::exception& e) {
          logger->Error(std::string("Error: ") + e.what());
          logger->Error("Cell Contents: [" + value_range[0] + "," + value_range[1] + "]");
          return false;
        }
        j = values.find_first_of(']', j + 1);
      }
    }
  }
  return true;
}
//-------------------------------------------------------------------------------
/// \brief Takes each item in the vector reference_values and searches table_row_map for the corresponding
/// TableRow, it then checks the biogears result against the validation data and marks it Green, Yellow, or Red
//-------------------------------------------------------------------------------
void ReportWriter::Validate()
{
  for (biogears::ReferenceValue ref : reference_values) {
    //logger->Info(ref.value_name);
    auto table_row_itr = table_row_map.find(trim(ref.value_name));
    if (table_row_itr == table_row_map.end()) { // Certain reference values might not be present in biogears results
      logger->Info(std::string("  Not Found"));
      continue;
    }

    biogears::TableRow table_row = table_row_itr->second; // Validation data either takes the form of a single value, or a range
    if (ref.is_range) {                                   // If it's a range the we first check whether the value is in range, and if not check how far out of range it is
      table_row.expected_value = "[" + std::to_string(ref.reference_range.first) + "," + std::to_string(ref.reference_range.second) + "]" + "@" + ref.reference;
      if (ref.reference_range.first <= table_row.engine_value && ref.reference_range.second >= table_row.engine_value) {
        table_row.result = Green;
        table_row.percent_error = "Within Bounds";
      } else {
        const double median_to_bound = (ref.reference_range.second - ref.reference_range.first)/2.0;
        const double reference_median = ref.reference_range.first + median_to_bound;
        const double warning_range_lower = reference_median * 0.75;
        const double warning_range_upper = reference_median *  1.25;

        if (ref.reference_range.first > table_row.engine_value && warning_range_lower <= table_row.engine_value
            || ref.reference_range.second < table_row.engine_value && warning_range_upper >= table_row.engine_value) {
          table_row.result = Yellow;
        } else {
          table_row.result = Red;
        }
        table_row.percent_error = "Outside Bounds";
      }

    } else { //For a value, we check how closely the biogears data matches the validation data
      table_row.expected_value = std::to_string(ref.reference_value) + "@" + ref.reference;
      if (std::fabs(ref.reference_value - table_row.engine_value) <= 0.000001) {
        table_row.percent_error = "0.0%";
        table_row.result = Green;
      } else {
        double error = (std::fabs(ref.reference_value - table_row.engine_value) / ((ref.reference_value + table_row.engine_value) / 2));
        error = std::fabs(error);
        table_row.percent_error = std::to_string(error * 100) + "%";
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
//-------------------------------------------------------------------------------
/// \brief Populates the map tables, each entry in the map is a vector of TableRows representing an individual table,
/// the key is always the table's name
//-------------------------------------------------------------------------------
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
//-------------------------------------------------------------------------------
/// \brief Clears all of the data structures used in gen_tables, does not reset the value of pointers
//-------------------------------------------------------------------------------
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
