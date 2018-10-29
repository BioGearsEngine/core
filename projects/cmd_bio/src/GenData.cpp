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

#include <biogears/exports.h>
#include "GenStates.h"

#include <iostream>
#include <fstream>
#include <string>
namespace biogears
{
  CSVToXMLConvertor::CSVToXMLConvertor(std::string path, std::string filename)
    :_path(path)
  ,_filename(filename)
  {
    
  }
  //-----------------------------------------------------------------------------
  SubstanceGenerator::SubstanceGenerator(std::string path, std::string filename)
    :CSVToXMLConvertor(path,filename)
  {
    
  }
  //-----------------------------------------------------------------------------
  SubstanceGenerator::~SubstanceGenerator()
  {
    
  }
  //-----------------------------------------------------------------------------
  bool SubstanceGenerator::parse()
  {
    return false;
  }
  //-----------------------------------------------------------------------------
  bool SubstanceGenerator::save() const
  {
    return false;
  }
  //-----------------------------------------------------------------------------
  void SubstanceGenerator::print() const
  {
    std::cout << "\n" << Path() << Filename() << "\n";
  }
  //-----------------------------------------------------------------------------
}