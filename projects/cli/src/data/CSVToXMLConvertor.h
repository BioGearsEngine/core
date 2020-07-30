#ifndef BIOGEARS_CSV_TO_XML_CONVERTOR_H
#define BIOGEARS_CSV_TO_XML_CONVERTOR_H

//**********************************************************************************
//Copyright 2015 Applied Research Associates, Inc.
//Licensed under the Apache License, Version 2.0 (the "License"); you may not use
//this file except in compliance with the License.You may obtain a copy of the License
//at :
//http://www.apache.org/licenses/LICENSE-2.0
//Unless required by applicable law or agreed to in writing, software distributed under
//the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
//CONDITIONS OF ANY KIND, either express or implied.See the License for the
//specific language governing permissions and limitations under the License.
//**************************************************************************************

#include <string>
#include <vector>
#include <map>

namespace biogears
{
  class CSVToXMLConvertor
  {
  public:
    using CSV_Row = std::pair<std::string, std::vector<std::string>>;
    using CSV_RowItr = std::vector<CSV_Row>::iterator;

    CSVToXMLConvertor( std::string path, std::string filename);

    virtual ~CSVToXMLConvertor() = default;

    virtual bool parse() = 0;
    virtual bool save() const = 0;
    virtual void print() const = 0;

    inline std::string Path() const { return _path; }
    inline std::string Filename() const { return _filename;  }

  protected:
    void Path(const std::string& path) { _path = path; }
    void FileName(const std::string& filename) { _filename = filename; }
    bool read_csv();

  private:
    using csv = std::vector<std::pair<std::string, std::vector<std::string>>>;
    using csv_row = std::pair < std::string, std::vector<std::string>>;

    bool parse_line(std::string&, csv_row&, std::ifstream&);

  private:
    std::string _path;
    std::string _filename;

    std::vector<CSV_Row> _data;

  protected:
    inline auto begin() -> decltype(_data.begin()) { return _data.begin(); }
    inline auto end() -> decltype(_data.end()) { return _data.end(); };
  };

 
} //namespace biogears

#endif //BIOGEARS_CSV_TO_XML_CONVERTOR_H
