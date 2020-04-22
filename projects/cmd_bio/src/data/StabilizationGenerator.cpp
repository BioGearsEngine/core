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

#include "StabilizationGenerator.h"
#include "../utils/string-helpers.h"
#include <biogears/schema/cdm/EngineConfiguration.hxx>
#include <fstream>
#include <iostream>

#include <biogears/string/manipulation.h>
namespace biogears {
//-----------------------------------------------------------------------------
StabilizationGenerator::StabilizationGenerator(std::string path)
  : CSVToXMLConvertor(path, "Stabilization.csv")
{
  namespace CDM = mil::tatrc::physiology::datamodel;

  _timed.TrackingStabilization(CDM::enumOnOff::On);
  _dynamic.TrackingStabilization(CDM::enumOnOff::On);
}
//-----------------------------------------------------------------------------
StabilizationGenerator::~StabilizationGenerator()
{
}
//-----------------------------------------------------------------------------
//!
//! \return success : true if parsed correctly else false
//!
bool StabilizationGenerator::parse()
{
  namespace CDM = mil::tatrc::physiology::datamodel;
  bool rValue = true;
  read_csv();
  for (auto lineItr = begin(); lineItr != end(); ++lineItr) {
    if ("Resting" == lineItr->first) {
      rValue &= process_RestingStabilizationCriteria(lineItr + 1);
      lineItr += 13;
    } else if ("Feedback" == lineItr->first) {
      rValue &= process_FeedbackStabilizationCriteria(lineItr + 1);
      lineItr += 15;
    } else {
      rValue &= process_ConditionStabilization(lineItr);
      lineItr += 15;
    }
  }
  return rValue;
}
//-----------------------------------------------------------------------------
//!
//! \brief Saves xml for timed and dynamic stabilization object
//! \return 
//! 
bool StabilizationGenerator::save() const
{
  namespace CDM = mil::tatrc::physiology::datamodel;
  xml_schema::namespace_infomap info;
  info[""].name = "uri:/mil/tatrc/physiology/datamodel";
  info[""].schema = "BioGears.xsd";

  try {
    std::ofstream file;
    file.open("config/DynamicStabilization.xml");
    CDM::PhysiologyEngineDynamicStabilization(file, _dynamic, info);
    file.close();
    std::cout << "Saved config/DynamicStabilization.xml" << "\n";

  } catch (std::exception e) {
    std::cout << e.what() << std::endl;
  }

  try {
    std::ofstream file;
    file.open("config/TimedStabilization.xml");
    CDM::PhysiologyEngineTimedStabilization(file, _timed, info);
    file.close();
    std::cout << "Saved config/TimedStabilization.xml" << "\n";
  } catch (std::exception e) {
    std::cout << e.what() << std::endl;
  }
  return false;
}
//-----------------------------------------------------------------------------
void StabilizationGenerator::print() const
{

  std::cout << _dynamic << "\n";
  std::cout << _timed;
}

//-----------------------------------------------------------------------------
//!
//! \brief Reads in resting resting stabilization data from Stabilization.csv
//! \param itr 
//! \return 
//! 
bool StabilizationGenerator::process_RestingStabilizationCriteria(CSV_RowItr itr)
{
  namespace CDM = mil::tatrc::physiology::datamodel;
  size_t pos;
  bool rValue = true;

  CDM::PhysiologyEngineDynamicStabilizationData::RestingStabilizationCriteria_type data;
  auto value = itr->first;
  try {
    for (size_t count = 0; count < 14; ++count, ++itr) {
      if ("Criteria" == itr->first) {
        CDM::PhysiologyEngineDynamicStabilizationData::RestingStabilizationCriteria_type::PropertyConvergence_type property_data;
        value = itr->second[0];
        property_data.PercentDifference(std::stod(value, &pos));
        value = itr->second[1];
        property_data.Name(value);
        data.PropertyConvergence().push_back(property_data);
      } else if ("ConvergenceTime" == itr->first) {
        value = itr->second[0];

        data.ConvergenceTime(std::stod(value, &pos));
        data.ConvergenceTime().unit(trim(value.substr(pos)));
      } else if ("MinimumReactionTime" == itr->first) {
        value = itr->second[0];
        data.MinimumReactionTime(std::stod(value, &pos));
        data.MinimumReactionTime().unit(trim(value.substr(pos)));
      } else if ("MaxAllowedStabilizationTime" == itr->first) {
        value = itr->second[0];
        data.MaximumAllowedStabilizationTime(std::stod(value, &pos));
        data.MaximumAllowedStabilizationTime().unit(trim(value.substr(pos)));
      } else if ("TimedStabilizationLength" == itr->first) {
        value = itr->second[0];
        _timed.RestingStabilizationTime(std::stod(value, &pos));
        _timed.RestingStabilizationTime().unit(trim(value.substr(pos)));
      }
    }
  } catch (std::exception e) {
    rValue = false;
  }
  _dynamic.RestingStabilizationCriteria(data);

  return rValue;
}
//-----------------------------------------------------------------------------
//!
//! \brief Reads in feedback stabilization data from Stabilization.csv
//! \param itr 
//! \return 
//! 
bool StabilizationGenerator::process_FeedbackStabilizationCriteria(CSV_RowItr itr)
{
  namespace CDM = mil::tatrc::physiology::datamodel;
  size_t pos;
  bool rValue = true;

  CDM::PhysiologyEngineDynamicStabilizationData::FeedbackStabilizationCriteria_type data;
  auto value = itr->first;
  try {
    for (size_t count = 0; count < 15; ++count, ++itr) {
      if ("Criteria" == itr->first) {
        CDM::PhysiologyEngineDynamicStabilizationData::FeedbackStabilizationCriteria_type::PropertyConvergence_type property_data;
        value = itr->second[0];
        property_data.PercentDifference(std::stod(value, &pos));
        value = itr->second[1];
        property_data.Name(value);
        data.PropertyConvergence().push_back(property_data);
      } else if ("ConvergenceTime" == itr->first) {
        value = itr->second[0];
        data.ConvergenceTime(std::stod(value, &pos));
        data.ConvergenceTime().unit(trim(value.substr(pos)));
      } else if ("MinimumReactionTime" == itr->first) {
        value = itr->second[0];
        data.MinimumReactionTime(std::stod(value, &pos));
        data.MinimumReactionTime().unit(trim(value.substr(pos)));
      } else if ("MaxAllowedStabilizationTime" == itr->first) {
        value = itr->second[0];
        data.MaximumAllowedStabilizationTime(std::stod(value, &pos));
        data.MaximumAllowedStabilizationTime().unit(trim(value.substr(pos)));
      } else if ("TimedStabilizationLength" == itr->first) {
        value = itr->second[0];
        _timed.FeedbackStabilizationTime(std::stod(value, &pos));
        _timed.FeedbackStabilizationTime().get().unit(trim(value.substr(pos)));
      }
    }
  } catch (std::exception e) {
    rValue = false;
  }
  _dynamic.FeedbackStabilizationCriteria(data);

  return rValue;
}
//-----------------------------------------------------------------------------
//!
//! \brief Reads in condition stabilization data from Stabilization.csv
//! \param itr 
//! \return 
//! 
bool StabilizationGenerator::process_ConditionStabilization(CSV_RowItr itr)
{
  namespace CDM = mil::tatrc::physiology::datamodel;
  size_t pos;
  bool rValue = true;

  auto condition_name = itr++->first;
  CDM::PhysiologyEngineDynamicStabilizationData::ConditionStabilization_type data;
  CDM::PhysiologyEngineDynamicStabilizationData::ConditionStabilization_type::Criteria_type criteria;
  auto value = itr->first;
  try {
    for (size_t count = 0; count < 15; ++count, ++itr) {
      if ("Criteria" == itr->first) {
        CDM::PhysiologyEngineDynamicStabilizationData::FeedbackStabilizationCriteria_type::PropertyConvergence_type property_data;
        value = itr->second[0];
        property_data.PercentDifference(std::stod(value, &pos));
        value = itr->second[1];
        property_data.Name(value);
        criteria.PropertyConvergence().push_back(property_data);
      } else if ("ConvergenceTime" == itr->first) {
        value = itr->second[0];
        criteria.ConvergenceTime(std::stod(value, &pos));
        criteria.ConvergenceTime().unit(trim(value.substr(pos)));
      } else if ("MinimumReactionTime" == itr->first) {
        value = itr->second[0];
        criteria.MinimumReactionTime(std::stod(value, &pos));
        criteria.MinimumReactionTime().unit(trim(value.substr(pos)));
      } else if ("MaxAllowedStabilizationTime" == itr->first) {
        value = itr->second[0];
        criteria.MaximumAllowedStabilizationTime(std::stod(value, &pos));
        criteria.MaximumAllowedStabilizationTime().unit(trim(value.substr(pos)));
      } else if ("TimedStabilizationLength" == itr->first) {
        value = itr->second[0];
        CDM::PhysiologyEngineTimedStabilizationData::ConditionStabilization_type cs_data;
        cs_data.Name(condition_name);
        cs_data.Time(std::stod(value, &pos));
        cs_data.Time().unit(trim(value.substr(pos)));
        _timed.ConditionStabilization().push_back(cs_data);
      }
    }
  } catch (std::exception e) {
    rValue = false;
  }

  data.Name(condition_name);
  data.Criteria(criteria);
  _dynamic.ConditionStabilization().push_back(data);
  return rValue;
}
} //Namespace biogears
