#include "ReportWriter.h"
#include "Tokenizer.h"
#include <fstream>
#include <numeric>

namespace biogears
{
  ReportWriter::ReportWriter(){}
  ReportWriter::~ReportWriter(){}

  std::string ReportWriter::to_html()
  {
    report.append(std::string("<html><body><table>\n"));
    //...Do work
    for(std::vector<std::string> cell : cells) {
      std::string line("<tr>");
      for(std::string item : cell) {
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
    for(std::string item : cells[0]) {
      first_line.append(item+"|");
      second_line.append("---|");
    }
    first_line.append("\n");
    second_line.append("\n");
    report.append(first_line);
    report.append(second_line);

    for (int i = 1; i < cells.size(); i++) {
      std::vector<std::string> cell = cells[i];
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
    report.append(std::string("<xml><body><table>\n"));
    //...Do work
    for (std::vector<std::string> cell : cells) {
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

  void ReportWriter::generate()
  {
    //SystemValidationData.xlsx
    //Maybe have it all spit out into csv files
  }

  void ReportWriter::ParseCSV(std::string& filename)
  {
    std::ifstream file{ filename };
    if(!file.is_open()) {
      return;
    }
    std::string line;
    int line_number = 0;
    while(std::getline(file, line)) {
      std::vector<std::string> vec;
      cells.push_back(vec);
      std::string cell;
      for(int i = 0;i < line.size();i++) {
        if(line[i] == ',' || line[i] == '\n') {
          cells[line_number].push_back(cell);
          cell.clear();
        } else if(line[i] == '"') {
          ++i;
          while(line[i] != '"') {
            if(line[i] != '\n') {
              cell += line[i];
            }
            ++i;
          }
          ++i;
        } else {
          cell += line[i];
        }
      }
      ++line_number;
    }
  }

  void ReportWriter::CalculateAverages(std::string& filename) {
    std::ifstream file{ filename };
    if (!file.is_open()) {
      return;
    }
    std::vector<std::vector<double>> values;
    std::string line;
    std::getline(file,line);
    std::string cell;
    for(int i = 0;i < line.size();i++) {
      if (line[i] == ',') {
        std::vector<double> v;
        values.push_back(v);
        std::pair<std::string,double> p(cell,0.0);
        average_values.push_back(p);
        cell.clear();
      } else {
        cell += line[i];
        if(i == (line.size()-1)) {
          std::vector<double> v;
          values.push_back(v);
          std::pair<std::string,double> p(cell,0.0);
          average_values.push_back(p);
          cell.clear();
        }
      }
    }
    cell.clear();
    int column_number;
    while(std::getline(file,line)) {
      column_number = 0;
      for (int i = 0;i < line.size();i++) {
        if (line[i] == ',') {
          double val = std::stod(cell);
          values[column_number].push_back(val);
          ++column_number;
          cell.clear();
        } else {
          cell += line[i];
          if(i == (line.size()-1)) {
            double val = std::stod(cell);
            values[column_number].push_back(val);
            ++column_number;
            cell.clear();          
          }
        }
      }
    }
    //average values
    std::ofstream myreport;
    myreport.open("averages.txt");
    for(int i = 0;i < values.size();i++) {
      double average = std::accumulate(values[i].begin(), values[i].end(), 0.0)/values[i].size();
      average_values[i].second = average;
      myreport << average_values[i].first << ',' << average_values[i].second << '\n';
    }
    myreport.close();
    return;
  }
}
