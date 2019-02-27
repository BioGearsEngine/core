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

#include <string>
#include <vector>

#include "Executor.h"

namespace biogears {
class Tokenizer;

class Config {

public:
  Config();
  Config(const std::string& file);
  Config(const Config&) = default;
  Config(Config&&) = default;
  ~Config() = default;

  Config& operator=(const Config&) = default;
  Config& operator=(Config&&) = default;

  bool load(std::string filepath);
  void clear();
  //TODO: Create our own Iterator Class which acts just like a Executor*
  using executor_vector = std::vector<Executor>;
  using iterator = executor_vector::iterator;
  using const_iterator = executor_vector::const_iterator;
  using reference = executor_vector::reference;
  using const_reference = executor_vector::const_reference;
  iterator begin();
  const_iterator begin() const;

  iterator end();
  const_iterator end() const;

  reference front();
  const_reference front() const;

  reference back();
  const_reference back() const;

  void push_back(const Executor&);
  void push_back(Executor&&);
  void merge(Config&&);

protected:
  bool process(Tokenizer&&);

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