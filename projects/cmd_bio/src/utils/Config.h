#ifndef CMD_BIO_CONFIG_H
#define CMD_BIO_CONFIG_H
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
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <list>

namespace biogears {
class Tokenizer;

enum class EDriver { Undefined, BGEUnitTestDriver, CDMUnitTestDriver, ScenarioTestDriver };
enum class EPlotter{ Undefined, ActionEventPlotter, MultiPlotter};
enum class EPlotStyle {FastPlot //Plot every nth data point (hardcoded)
                      ,FullPlot //Plot all data points
                      ,FastPlotErrors //Only plot failures and only every nth data point (hardcoded)
                      ,FullPlotErrors //Plot all data points only for failing data
                      ,MemoryFastPlot //Do a fast plot, but manage memory better, slower plotting, but can handle larger files
};
class Executor {
public:
  Executor();
  Executor(std::string name, EDriver driver);

  std::string Name() const;
  EDriver Driver() const;
  EPlotStyle PlotStyle() const;
  bool NoCompare() const;
  std::string Baselines() const;
  std::string Computed() const;
  std::string Group() const;
  std::vector<std::string> Results() const;

  Executor& Name(const std::string&) &&;
  Executor& Driver(EDriver) &&;
  Executor& PlotStyle(EPlotStyle) &&;
  Executor& NoCompare(bool) &&;
  Executor& Baselines(const std::string&) &&;
  Executor& Computed(const std::string&) &&;
  Executor& Group(const std::string&) &&;
  Executor& Results(const std::vector<std::string>&) &&;

  std::string Name(const std::string&) &;
  EDriver Driver(EDriver) &;
  EPlotStyle PlotStyle(EPlotStyle) &;
  bool NoCompare(bool) &;
  std::string Baselines(const std::string&) &;
  std::string Computed(const std::string&) &;
  std::string Group(const std::string&) &;
  std::vector<std::string> Results(const std::vector<std::string>&) &;

  Executor& push_back_results(const std::string&) &&;
  void push_back_results(const std::string&) &;
  void clear_results();
private:
  EDriver driver;
  EPlotStyle   plot_style;
  bool no_compare;
  std::string name;
  std::string baselines;
  std::string computed;
  std::string group;
  std::vector<std::string> results;
};

class Config {

public:
  Config();
  Config(const std::string& file);
  ~Config() = default;

  bool load(std::string filepath);
  void clear();
  //TODO: Create our own Iterator Class which acts just like a Executor*
  using executor_vector = std::vector<Executor>;
  using iterator = executor_vector::iterator;
  using const_iterator = executor_vector::const_iterator;

  iterator begin();
  const_iterator begin() const;

  iterator end();
  const_iterator end() const;
  protected:
  bool  process(Tokenizer&&);
  
private:
  bool _send_email;
  std::string _email_smtp_server;
  std::string _email_smtp_user;
  std::string _email_smtp_password;
  std::string _email_subject;
  std::string _email_sender_address;
  std::vector<std::string> _email_receipients;

  int _threads;
  bool _execute_tests;
  bool _plot_results;
  double _percent_difference;
  std::string _current_group;
  std::vector<Executor> _execs;
};
}

#endif //CMD_BIO_CONFIG_H