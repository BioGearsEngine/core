#include "ReportWriter.h"
#include "Tokenizer.h"
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <regex>
#include <sstream>

#include "mz.h"
#include "mz_os.h"
#include "mz_strm.h"
#include "mz_strm_buf.h"
#include "mz_strm_split.h"
#include "mz_zip.h"
#include "mz_zip_rw.h"
#include <biogears/filesystem/path.h>
#include <biogears/string/manipulation.h>

#include "mz_strm_mem.h"

#include "KnownPaths.h"
#include "dir.h"

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

//-------------------------------------------------------------------------------
/// \brief Takes in a single sheet, unlike gen_tables which takes in a list of validation and baselines files
/// It is essentially one iteration of the for loop in gen_tables
/// \param reference_file: std::string name of file containing validation data
/// \param results_file: std::string name of file containing baseline data
/// \param table_type: char denoting what type of results file should be produced (html, md, or xml)
//-------------------------------------------------------------------------------

void ReportWriter::gen_tables_single_sheet(std::string reference_file, std::string results_file, char table_type)

{
  logger->Info("Generating table: " + split(reference_file, '.')[0]);
  //TODO: This is a great example on how SetConsoleSetConversionPattern should be pushConsoleSetCOnversionPattern and popConsoleSetConversionPattern
  //TODO: We should not expect people to know what conversion pattern it was before they changed it.

  logger->SetConsolesetConversionPattern("\t%m%n");

  ParseReferenceCSV(reference_file);
  ParseResultsCSV(results_file);
  CalculateAverages();
  logger->Debug("Successfully calculated averages of file: " + results_file);
  ExtractValues();
  logger->Debug("Successfully populated data structures with validation data");
  Validate();
  logger->Debug("Successfully ran validation");
  PopulateTables();
  logger->Debug("Successfully populated tables vector");
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
  logger->Debug("Successfully generated table: " + split(reference_file, '.')[0]);
  clear();
  logger->SetConsolesetConversionPattern("%d [%p] %m%n");
}

//--------------------------------------------------------------------------------
//  \brief True when the filename provided is readable using fopen else false
//
inline bool file_exists(const std::string& name)
{
  if (FILE* file = fopen(name.c_str(), "r")) {
    fclose(file);
    return true;
  } else {
    return false;
  }
}
//--------------------------------------------------------------------------------
//
//  \brief Returns a string representing the path.join(name) or the fallback location
//         returns path_s.join(name) if it exist else
//         returns path_s/baselines/name+Results.zip if it exists else
//         returns baseline_dir/path_s/baselines/name+Results.zip else
//         returns name + -NOTFOUND
//
//   The value of baseline_dir is determined by CMAKE durring code configuration
//   from the cmake variable Biogears_Baseline_DIRot
//
std::string locateBaseline(biogears::filesystem::path path_s, std::string name)
{
  using namespace biogears;

  static const filesystem::path baseline_dir { Baseline_Directory };
  filesystem::path baseline = path_s / (name + "Results.csv");
  if (baseline.exists()) {
    return baseline.string();
  }
  baseline = path_s / "baselines" / (name + "Results.zip");
  if (baseline.exists()) {
    return baseline.string();
  }
  baseline = baseline_dir / path_s / "baselines" / (name + "Results.zip");
  if (baseline.exists()) {
    return baseline.string();
  }
  return name + "-NOTFOUND";
}
//--------------------------------------------------------------------------------
std::string resolveTestLocation(biogears::filesystem::path baseline, std::string test)
{
  using namespace biogears;

  static const filesystem::path baseline_dir { Baseline_Directory };

  auto ext = baseline.extension();
  if (ext == "zip") {
    return baseline.str();
  } else {
    std::regex re { R"((.*Results)(.*))" };
    std::smatch m;
    auto bl = baseline.string();
    if (std::regex_match(bl, m, re)) {
      baseline = m[1].str() + test;

      auto dir_list = biogears::filesystem::dirlist(baseline.parent_path().string(), baseline.filename().string() + ".*[.]xml");
      if (dir_list.size()) {
        return dir_list[0];
      }
    }
  }
  return test + "-NOTFOUND";
}
//-------------------------------------------------------------------------------
/// \brief Takes a list of validation files and results files from biogears and produces
/// validation tables for our documentation.
/// \param table_type: char denoting what type of results file should be produced (html, md, or xml)
//-------------------------------------------------------------------------------
void ReportWriter::gen_tables(TYPE table_type)
{
  std::vector<std::pair<std::string, std::string>> SystemTables {
    { "BloodChemistryValidation", "Scenarios/Validation" },
    { "CardiovascularValidation", "Scenarios/Validation" },
    { "EnergyValidation", "Scenarios/Validation" },
    { "EndocrineValidation", "Scenarios/Validation" },
    { "RenalValidation", "Scenarios/Validation" },
    { "RespiratoryValidation", "Scenarios/Validation" },
    { "TissueValidation", "Scenarios/Validation" }
  };

  //Map of SystemTables to required test. Second paramater i
  std::map<std::string, std::vector<std::string>> test_files {
    { "BloodChemistryValidation", { "CMP" } }
  };

  bool success = true;
  for (int i = 0; i < SystemTables.size(); i++) {
    try {
      bool no_faults = false;
      const std::string reference_value_file = SystemTables[i].first + ".csv";
      const std::string library_baseline_file = locateBaseline(SystemTables[i].second, SystemTables[i].first);
      clear();
      logger->SetConsolesetConversionPattern(SystemTables[i].first + ".md %n");
      logger->Info("");
      logger->SetConsolesetConversionPattern("\t[%p]%m%n");
      ParseReferenceCSV(reference_value_file);
      ParseResultsCSV(library_baseline_file);
      if (test_files[SystemTables[i].first].size()) {
        for (auto test : test_files[SystemTables[i].first]) {
          try {
            ParseXML(resolveTestLocation(library_baseline_file, test), test);
          } catch (std::runtime_error e) {
            logger->Error(biogears::asprintf("Unable to parse %s for %s \n\t %s", test.c_str(), SystemTables[i].first.c_str(), e.what()));
            success = false;
            continue;
          }
        }
      }
      CalculateAverages();
      ExtractValues();
      Validate();
      PopulateTables();
      if (table_type == HTML) {
        set_html();
      } else if (table_type == MD) {
        set_md();
      } else if (table_type == XML) {
        set_xml();
      } else {
        set_web();
      }
      to_table();
    } catch (std::runtime_error e) {
      logger->Error("Failure generating " + SystemTables[i].first + "\n\t " + e.what());
      success &= false;
    }
    clear();
  }
}
//-------------------------------------------------------------------------------
/// \brief Takes the data stored in tables (a map pairing table names (std::string) to a list of TableRow objects (std::vector<biogears::TableRow>)
/// and prints out a full validation table for each item in tables.
//-------------------------------------------------------------------------------
void ReportWriter::to_table()
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
    mkdir("doc", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    mkdir("doc/validation", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif
    auto filename = "doc/validation/" + table_name + "ValidationTable" + _file_extension;
    file.open(filename);
    if (!file) {
      throw std::runtime_error(biogears::asprintf("Unable to open file %s", filename.c_str()));
    }
    file << (std::string(_body_begin) + table + std::string(_body_end));
    file.close();
    table.clear();
  }
}

void ReportWriter::ParseReferenceCSV(const std::string filename)
{
  return ParseCSV(filename, this->validation_data);
}

void ReportWriter::ParseResultsCSV(const std::string filename)
{
  return ParseCSV(filename, this->biogears_results);
}
//-------------------------------------------------------------------------------
/// \brief Parses a CSV file and loads it into a 2 dimension vector data, which represents a CSV.
/// data[y] represents an individual row of the CSV file, while data[y][x] represents an individual cell.
/// \param file: The CSV in istream form.  Stream must be open and valid
/// \param data: Two dimensional vector for storing data from CSV file
//-------------------------------------------------------------------------------
void ReportWriter::ParseCSV(std::istream& file, std::vector<std::vector<std::string>>& data)
{
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
        while (true) { //std::getline treats any newline character as the end of a line, this means that
          ++i; //certain cells are split across two calls of std::getline
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
  //logger->Info("Successfully parsed file: " + filename);
}

//-------------------------------------------------------------------------------
/// \brief Searches the filename for CSV data and passes it to ParseCSV(std::istream*, data)
/// \param filename: Name of file to aquire the csv data from.
///                  If the name is a zip file then the name with out an extension + .csv will be pulled form the archive and read
/// \param data: Two dimensional vector for storing data from CSV file
//-------------------------------------------------------------------------------
void ReportWriter::ParseCSV(const std::string& filename, std::vector<std::vector<std::string>>& data)
{
  auto ext = split(filename, '.').back();
  if (ext == "zip") {
    mz_zip_file* file_info = NULL;
    uint32_t ratio = 0;
    int16_t level = 0;
    int32_t err = MZ_OK;
    struct tm tmu_date;
    const char* string_method = NULL;
    char crypt = ' ';
    void* reader = NULL;

    mz_zip_reader_create(&reader);
    err = mz_zip_reader_open_file(reader, filename.c_str());
    if (err != MZ_OK) {
      mz_zip_reader_delete(&reader);
      throw std::runtime_error(biogears::asprintf("minizip error detected errorno=%i", err));
    }

    biogears::filesystem::path entry { filename };
    entry = entry.filename();
    std::string internal_file = split(entry.string(), '.')[0] + ".csv";
    err = mz_zip_reader_locate_entry(reader, internal_file.c_str(), true);

    if (err != MZ_OK) {
      mz_zip_reader_delete(&reader);
      throw std::runtime_error(biogears::asprintf("Error % " PRId32 " Locating % s\n ", err, internal_file.c_str()));
    }

    err = mz_zip_reader_entry_get_info(reader, &file_info);
    if (err != MZ_OK) {
      mz_zip_reader_delete(&reader);
      throw std::runtime_error(biogears::asprintf("Error %" PRId32 " getting entry info in archive\n", err));
    }
    err = mz_zip_reader_entry_open(reader);
    if (err < 0) {
      mz_zip_reader_delete(&reader);
      throw std::runtime_error(biogears::asprintf("Error %" PRId32 " opening entry in archive\n", err));
    }
    auto buffer = std::vector<char>(file_info->uncompressed_size + 1, '\0');
    err = mz_zip_reader_entry_read(reader, &buffer[0], file_info->uncompressed_size);
    if (err < 0) {
      mz_zip_reader_delete(&reader);
      throw std::runtime_error(biogears::asprintf("Error %" PRId32 " reading entry in archive\n", err));
    }
    std::istringstream stream { &buffer[0] };
    ParseCSV(stream, data);

    err = mz_zip_reader_entry_close(reader);
    if (err != MZ_OK) {
      mz_zip_reader_delete(&reader);
      throw std::runtime_error(biogears::asprintf("Error %" PRId32 " closing entry in archive\n", err));
    }
    mz_zip_reader_delete(&reader);
  } else if (ext == "csv") {
    std::ifstream file { filename };
    if (file.is_open()) {
      return ParseCSV(file, data);
    } else {
      throw std::runtime_error("error opening: " + filename);
    }
  } else {
    throw std::runtime_error(biogears::asprintf("error unable to find %s", filename.c_str()));
  }
}
//-------------------------------------------------------------------------------
//! \param stdl::istream : XML Input in stream form.
//!
//! \brief Parses an XML file containing results from biogears, puts the relevant data into a TableRow object,
/// and inserts the TableRow into a map
void ReportWriter::ParseXML(std::istream& stream)
{
  std::string line;
  while (std::getline(stream, line)) { // This loop reads in the XML file line by line, then pulls out the value name, the unit name, and the value
    size_t name_index = line.find("p1:"); // The value name is always preceded by "p1:"
    if (name_index == std::string::npos || line.find("/p1:") != std::string::npos) {
      continue;
    }
    name_index += 3;
    size_t name_end = line.find(" ", name_index);
    size_t unit_index = line.find("unit=\"", name_end); //The unit name is always preceded by "unit="
    if (unit_index == std::string::npos) {
      continue;
    }
    unit_index += 6;
    size_t unit_end = line.find("\"", unit_index);
    size_t value_index = line.find("value=\"", unit_end) + 7; //The value is always preceded by "value="
    size_t value_end = line.find("\"/", value_index);
    biogears::TableRow xmlRow;
    std::string name = trim(line.substr(name_index, name_end - name_index));
    std::string unit = trim(line.substr(unit_index, unit_end - unit_index));
    std::string value = trim(line.substr(value_index, value_end - value_index));
    xmlRow.field_name = name + "(" + unit + ")";
    xmlRow.expected_value = "0.0";
    xmlRow.engine_value = std::stod(value);
    table_row_map.insert(std::pair<std::string, biogears::TableRow>(name, xmlRow));
  }
}
//-------------------------------------------------------------------------------
/// \brief Determines if the XML is part of a baseline archive or a raw file.
///        If it is in a raw file the raw file is read in to a stream and passed to ParseXML(std::istream&)
///        If it is in an archive then the value of test is used as a REGEX search match in the form of ".*test.*"
///        To find the right entry in the archive to read in and send to ParseXML(std::istream&)
///
/// \param filename: Name of XML file to be parsed
/// \param test: The value of test will be used when filename ends in .zip by creating the regex ".*test.*"
//-------------------------------------------------------------------------------
void ReportWriter::ParseXML(const std::string& filename, std::string test)
{
  auto ext = split(filename, '.').back();
  if (ext == "zip") {
    mz_zip_file* file_info = NULL;
    uint32_t ratio = 0;
    int16_t level = 0;
    int32_t err = MZ_OK;
    struct tm tmu_date;
    const char* string_method = NULL;
    char crypt = ' ';
    void* reader = NULL;

    mz_zip_reader_create(&reader);
    err = mz_zip_reader_open_file(reader, filename.c_str());
    if (err != MZ_OK) {
      mz_zip_reader_delete(&reader);
      throw std::runtime_error(biogears::asprintf("Error %" PRId32 " opening  archive %s\n", err, filename.c_str()));
    }

    biogears::filesystem::path entry { filename };
    entry = entry.filename();

    err = mz_zip_reader_goto_first_entry(reader);
    if (err != MZ_OK) {
      mz_zip_reader_delete(&reader);
      throw std::runtime_error(biogears::asprintf("Error % " PRId32 " going to first entry in archive\n", err));
    }

    do {
      err = mz_zip_reader_entry_get_info(reader, &file_info);
      if (err != MZ_OK) {
        mz_zip_reader_delete(&reader);
        throw std::runtime_error(biogears::asprintf("Error %" PRId32 " getting  entry info from archive\n", err));
      }

      std::regex re { ".*" + test + ".*" };
      std::smatch m;
      std::string entry_name = file_info->filename;
      if (std::regex_match(entry_name, m, re)) {
        err = mz_zip_reader_entry_open(reader);
        if (err < 0) {
          mz_zip_reader_delete(&reader);
          throw std::runtime_error(biogears::asprintf("Error %" PRId32 "  opening entry in archive\n", err));
        }
        auto buffer = std::vector<char>(file_info->uncompressed_size + 1, '\0');
        err = mz_zip_reader_entry_read(reader, &buffer[0], file_info->uncompressed_size);
        if (err < 0) {
          mz_zip_reader_delete(&reader);
          throw std::runtime_error(biogears::asprintf("Error %" PRId32 "  reading entry in archive\n", err));
        }

        std::istringstream stream { &buffer[0] };
        ParseXML(stream);

        err = mz_zip_reader_entry_close(reader);
        if (err != MZ_OK) {
          mz_zip_reader_delete(&reader);
          throw std::runtime_error(biogears::asprintf("Error %" PRId32 "  closing entry in archive\n", err));
        }
        break;
      }
      err = mz_zip_reader_goto_next_entry(reader);
      if (err != MZ_OK && err != MZ_END_OF_LIST) {
        printf("Error %" PRId32 " going to next entry in archive\n", err);
        break;
      }

    } while (err == MZ_OK);

    mz_zip_reader_delete(&reader);
    return;
  } else if (ext == "xml") {
    filesystem::path full_path { filename };
    std::ifstream file { full_path.string() };
    if (!file.is_open()) {
      throw std::runtime_error("Error opening: " + filename);
    }
    return ParseXML(file);
  }
  throw std::runtime_error(biogears::asprintf("Unsupported extension %s for file %s", ext.c_str(), filename.c_str()));
}

//-------------------------------------------------------------------------------
/// \brief Takes the information from biogears_results, which one of the results CSV files, averages them, creates
/// a TableRow object for each one, and inserts them into a map of table rows
//-------------------------------------------------------------------------------
void ReportWriter::CalculateAverages()
{
  std::vector<biogears::TableRow> rows;
  std::vector<int> row_items;
  for (int i = 0; i < biogears_results[0].size(); i++) { // This iterates across the top row of the CSV, which is the name of each value
    biogears::TableRow row;
    row.field_name = biogears_results[0][i];
    row.expected_value = "0.0";
    row.engine_value = 0.0;
    row.max_value = 0.0;
    row.min_value = 0.0;
    rows.push_back(row);
    row_items.push_back(0); // Because not every value was measured the same amount of times, we need to track this
  } // in order to take the average of each value

  for (int i = 1; i < biogears_results.size(); i++) { // Because the data is stored in columns, we have to iterate down from each cell
    for (int k = 0; k < biogears_results[i].size(); k++) { // of the second row
      try {
        if (trim(biogears_results[i][k]) != "") { //if trim returns an empty string, it means the cell is empty
          double engineValue = std::stod(biogears_results[i][k]);
          rows[k].engine_value += engineValue;
          rows[k].max_value = engineValue > rows[k].max_value ? engineValue : rows[k].max_value;
          rows[k].min_value = engineValue < rows[k].min_value ? engineValue : rows[k].min_value;
          ++row_items[k]; // Further down we'll calculate the average of these values by dividing engine_value by row_items
        }
      } catch (std::exception& e) { // This usually occurs if one of the cells contains non-numeric characters, std::stod will throw an exception
        throw std::runtime_error(std::string(e.what()) + "\n\t" + "Cell Contents: " + biogears_results[i][k]);
      }
    }
  }
  for (int i = 0; i < rows.size(); i++) {
    rows[i].engine_value /= row_items[i]; // This line calculates the average of a value
    //Inspiratory flow and expiratory flow are validated against their maximum values.  If we have more data requests validated in this way we should probably make a map of these things or something
    if (rows[i].field_name.compare("InspiratoryFlow(L/s)") == 0 || rows[i].field_name.compare("ExpiratoryFlow(L/s)") == 0) {
      rows[i].engine_value = rows[i].max_value;
    }
    std::string field_name_with_units = rows[i].field_name;
    TableRow row = rows[i]; //So field_name_with_units looks like "Name(Unit)", which is why it gets split to just be "Name"
    table_row_map.insert(std::pair<std::string, biogears::TableRow>(trim(split(field_name_with_units, '(')[0]), row));
  }
}
//-------------------------------------------------------------------------------
/// \brief Extracts the values from validation_data, which is one of the validation CSV files, and
/// creates a ReferenceValue object for each column
//-------------------------------------------------------------------------------
void ReportWriter::ExtractValues()
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
        throw std::runtime_error(std::string("Error: ") + e.what() + "\n\tCell Contents: [" + value_range[0] + "," + value_range[1] + "]");
        ;
      }
    } else {
      ref.is_range = false;
      std::vector<std::string> value = split(validation_data[i][2], ',');
      try {
        ref.reference_value = std::stod(value[0]);
      } catch (std::exception& e) {
        throw std::runtime_error(std::string("Error: ") + e.what() + "\n\t" + "Cell Contents: " + value[0]);
      }
    }
    ref.reference = split(validation_data[i][3], ',')[0];
    ref.notes = validation_data[i][4];
    ref.table_name = validation_data[i][5];
    reference_values.push_back(ref);
  }
}

/*
 * This function is not currently in use, it's part of planned future upgrades to the ReportWriter
 */
void ReportWriter::ExtractValuesList()
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
          throw std::runtime_error(std::string("Error: ") + e.what() + "\n\t" + "Cell Contents: " + values.substr(j, eod - j));
        }
      } else if (values[j] == '[') {
        std::vector<std::string> value_range = split(values.substr(j + 1, values.find_first_of(']', j + 1) - (1 + j)), ',');
        try {
          ref.reference_ranges.push_back(std::pair<double, double>(std::stod(value_range[0]), std::stod(value_range[1])));
        } catch (std::exception& e) {
          throw std::runtime_error(std::string("Error: ") + e.what() + "\n\t" + "Cell Contents: [" + value_range[0] + "," + value_range[1] + "]");
        }
        j = values.find_first_of(']', j + 1);
      }
    }
  }
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
      logger->Warning(trim(ref.value_name) + "  Not Found");
      continue;
    }

    biogears::TableRow table_row = table_row_itr->second; // Validation data either takes the form of a single value, or a range
    std::regex valid_citation("[a-zA-Z0-9]+");
    if (ref.is_range) { // If it's a range the we first check whether the value is in range, and if not check how far out of range it is
      if (std::regex_match(ref.reference, valid_citation)) {
        table_row.expected_value = "[" + std::to_string(ref.reference_range.first) + "," + std::to_string(ref.reference_range.second) + "]" + "@cite " + ref.reference;
      } else {
        table_row.expected_value = "[" + std::to_string(ref.reference_range.first) + "," + std::to_string(ref.reference_range.second) + "]" + ref.reference;
      }
      if (ref.reference_range.first <= table_row.engine_value && ref.reference_range.second >= table_row.engine_value) {
        table_row.result = Green;
        table_row.percent_error = "Within Bounds";
      } else {
        const double median_to_bound = (ref.reference_range.second - ref.reference_range.first) / 2.0;
        const double reference_median = ref.reference_range.first + median_to_bound;
        const double warning_range_lower = reference_median * 0.75;
        const double warning_range_upper = reference_median * 1.25;

        if (ref.reference_range.first > table_row.engine_value && warning_range_lower <= table_row.engine_value
            || ref.reference_range.second < table_row.engine_value && warning_range_upper >= table_row.engine_value) {
          table_row.result = Yellow;
        } else {
          table_row.result = Red;
        }
        table_row.percent_error = "Outside Bounds";
      }

    } else { //For a value, we check how closely the biogears data matches the validation data
      if (std::regex_match(ref.reference, valid_citation)) {
        table_row.expected_value = std::to_string(ref.reference_value) + "@cite " + ref.reference;
      } else {
        table_row.expected_value = std::to_string(ref.reference_value) + ref.reference;
      }
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
