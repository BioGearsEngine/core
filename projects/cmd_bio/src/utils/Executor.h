#ifndef CMD_BIO_UTILS_EXECUTOR_H
#define CMD_BIO_UTILS_EXECUTOR_H

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

#include <ostream>
#include <string>
#include <vector>

namespace biogears {
enum class EDriver { Undefined,
                     BGEUnitTestDriver,
                     CDMUnitTestDriver,
                     ScenarioTestDriver };
enum class EPlotter { Undefined,
                      ActionEventPlotter,
                      MultiPlotter };
enum class EPlotStyle { FastPlot //Plot every nth data point (hardcoded)
                        ,
                        FullPlot //Plot all data points
                        ,
                        FastPlotErrors //Only plot failures and only every nth data point (hardcoded)
                        ,
                        FullPlotErrors //Plot all data points only for failing data
                        ,
                        MemoryFastPlot //Do a fast plot, but manage memory better, slower plotting, but can handle larger files
};
class Executor {
public:
  Executor();
  Executor(std::string nam, EDriver driver);
  Executor(const Executor&) = default;
  Executor(Executor&&) = default;

  Executor& operator=(const Executor&) = default;
  Executor& operator=(Executor&&) = default;

  std::string Name() const;
  EDriver Driver() const;
  EPlotStyle PlotStyle() const;
  bool NoCompare() const;
  std::string Baselines() const;
  std::string Computed() const;
  std::string Group() const;
  std::string Patient() const;
  std::string Scenario() const;
  std::vector<std::string> Results() const;

  void Name(const std::string&);
  void Driver(EDriver);
  void PlotStyle(EPlotStyle);
  void NoCompare(bool);
  void Baselines(const std::string&);
  void Computed(const std::string&);
  void Group(const std::string&);
  void Patient(const std::string&);
  void Scenario(const std::string&);
  void Results(const std::vector<std::string>&);

  void push_back_results(const std::string&);
  void clear_results();

private:
  EDriver driver;
  EPlotStyle plot_style;
  bool no_compare;
  std::string name;
  std::string baselines;
  std::string computed;
  std::string group;
  std::string patient;
  std::string scenario;
  std::vector<std::string> results;
};

std::ostream& operator<<(std::ostream&, const EDriver&);
std::ostream& operator<<(std::ostream&, const EPlotter&);
std::ostream& operator<<(std::ostream&, const EPlotStyle&);
} //namespace biogears

#endif //CMD_BIO_UTILS_EXECUTOR_H