#include "Config.h"
#include "Tokenizer.h"

#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <regex>
#include <sstream>
#include <functional>

#include <biogears/string/manipulation.h>

namespace biogears {

std::string error_context;

bool handle_assignment(Tokenizer::token_list::iterator& tokenItr, Tokenizer::token_list::iterator end, std::string& rhs);
bool handle_assignment(Tokenizer::token_list::iterator& tokenItr, Tokenizer::token_list::iterator end, bool& rhs);
bool handle_assignment(Tokenizer::token_list::iterator& tokenItr, Tokenizer::token_list::iterator end, int& rhs);
bool handle_assignment(Tokenizer::token_list::iterator& tokenItr, Tokenizer::token_list::iterator end, double& rhs);
bool handle_driver_definition(Tokenizer::token_list::iterator& tokenItr, Tokenizer::token_list::iterator end, Executor& rhs);
bool handle_driver_paramaters(Tokenizer::token_list::iterator& tokenItr, Tokenizer::token_list::iterator end, Executor& rhs);
bool handle_bool_string(const std::string&, bool&);
bool handle_double_string(const std::string&, double&);
bool handle_integral_string(const std::string&, int&);
bool handle_driver_string(const std::string&, EDriver&);
bool handle_tick_string(Tokenizer::token_list::iterator& tokenItr, Tokenizer::token_list::iterator end, std::string& rhs);
bool handle_quote_string(Tokenizer::token_list::iterator& tokenItr, Tokenizer::token_list::iterator end, std::string& rhs);
bool handle_email_list(const std::string& input, std::vector<std::string>& result);
bool handle_delimited_list(const std::string& input, const char delimiter, std::vector<std::string>& result, std::function<bool(std::string)> validator = [](std::string input) { return true; });
bool validate_email_string(const std::string&);
bool validate_server_address(const std::string&);
bool grab_next_word_or_fail(Tokenizer::token_list::iterator& tokenItr, Tokenizer::token_list::iterator end);
bool grab_next_symbol_or_fail(Tokenizer::token_list::iterator& tokenItr, Tokenizer::token_list::iterator end);
bool gobble_until_next_given(Tokenizer::token_list::iterator& tokenItr, Tokenizer::token_list::iterator end, Token&, Tokenizer::token_list&);

std::list<std::string> split_token_on_word_boundry(std::string);

//-----------------------------------------------------------------------------
Config::Config()
  : _email_subject("BioGears Notification Email")
  , _current_group("Other")
  , _send_email(true)
  , _execute_tests(true)
  , _plot_results(true)
  , _percent_difference(2.)
  , _threads(-1)
{
}
//-----------------------------------------------------------------------------
Config::Config(const std::string& file, bool silent)
  : _email_subject("BioGears Notification Email")
  , _current_group("Other")
  , _send_email(true)
  , _execute_tests(true)
  , _plot_results(true)
  , _percent_difference(2.)
  , _threads(-1)
{
  load(file, silent);
}
//-----------------------------------------------------------------------------
bool Config::send_email() const
{
  return _send_email;
}
//-----------------------------------------------------------------------------
void Config::send_email(const bool send_email) &
{
  _send_email = send_email;
}
//-----------------------------------------------------------------------------
Config& Config::send_email(const bool send_email) &&
{
  _send_email = send_email;
  return *this;
}
//-----------------------------------------------------------------------------
std::string Config::email_smtp_server() const
{
  return _email_smtp_server;
}
//-----------------------------------------------------------------------------
void Config::email_smtp_server(std::string cs) &
{
  _email_smtp_server = std::move(cs);
}
//-----------------------------------------------------------------------------
Config& Config::email_smtp_server(std::string cs) &&
{
  _email_smtp_server = std::move(cs);
  return *this;
}
//-----------------------------------------------------------------------------
std::string Config::email_smtp_user() const
{
  return _email_smtp_user;
}
//-----------------------------------------------------------------------------
void Config::email_smtp_user(std::string cs) &
{
  _email_smtp_user = std::move(cs);
}
//-----------------------------------------------------------------------------
Config& Config::email_smtp_user(std::string cs) &&
{
  _email_smtp_user = std::move(cs);
  return *this;
}
//-----------------------------------------------------------------------------
std::string Config::email_smtp_password() const
{
  return _email_smtp_password;
}
//-----------------------------------------------------------------------------
void Config::email_smtp_password(std::string cs) &
{
  _email_smtp_password = std::move(cs);
}

Config& Config::email_smtp_password(std::string cs) &&
{
  _email_smtp_password = std::move(cs);
  return *this;
}
//-----------------------------------------------------------------------------
std::string Config::email_subject() const
{
  return _email_subject;
}
//-----------------------------------------------------------------------------
void Config::email_subject(std::string cs) &
{
  _email_subject = std::move(cs);
}
//-----------------------------------------------------------------------------
Config& Config::email_subject(std::string cs) &&
{
  _email_subject = std::move(cs);
  return *this;
}
//-----------------------------------------------------------------------------
std::string Config::email_sender_address() const
{
  return _email_sender_address;
}
//-----------------------------------------------------------------------------
void Config::email_sender_address(std::string cs) &
{
  _email_sender_address = std::move(cs);
}
//-----------------------------------------------------------------------------
Config& Config::email_sender_address(std::string cs) &&
{
  _email_sender_address = std::move(cs);
  return *this;
}
//-----------------------------------------------------------------------------
std::vector<std::string> Config::email_receipients() const
{
  return _email_receipients;
}
//-----------------------------------------------------------------------------
void Config::email_receipients(std::vector<std::string> basic_strings) &
{
  _email_receipients = std::move(basic_strings);
}
//-----------------------------------------------------------------------------
Config& Config::email_receipients(std::vector<std::string> basic_strings) &&
{
  _email_receipients = std::move(basic_strings);
  return *this;
}
//-----------------------------------------------------------------------------
int Config::threads() const
{
  return _threads;
}
//-----------------------------------------------------------------------------
void Config::threads(const int threads) &
{
  _threads = threads;
}
//-----------------------------------------------------------------------------
Config& Config::threads(const int threads) &&
{
  _threads = threads;
  return *this;
}
//-----------------------------------------------------------------------------
bool Config::execute_tests() const
{
  return _execute_tests;
}
//-----------------------------------------------------------------------------
void Config::execute_tests(const bool execute_tests) &
{
  _execute_tests = execute_tests;
}
//-----------------------------------------------------------------------------
Config& Config::execute_tests(const bool execute_tests) &&
{
  _execute_tests = execute_tests;
  return *this;
}
//-----------------------------------------------------------------------------
bool Config::plot_results() const
{
  return _plot_results;
}
//-----------------------------------------------------------------------------
void Config::plot_results(const bool plot_results) &
{
  _plot_results = plot_results;
}
//-----------------------------------------------------------------------------
Config& Config::plot_results(const bool plot_results) &&
{
  _plot_results = plot_results;
  return *this;
}
//-----------------------------------------------------------------------------
double Config::percent_difference() const
{
  return _percent_difference;
}
//-----------------------------------------------------------------------------
void Config::percent_difference(const double percent_difference) &
{
  _percent_difference = percent_difference;
}
//-----------------------------------------------------------------------------
Config& Config::percent_difference(const double percent_difference) &&
{
  _percent_difference = percent_difference;
  return *this;
}
//-----------------------------------------------------------------------------
std::string Config::current_group() const
{
  return _current_group;
}
//-----------------------------------------------------------------------------
void Config::current_group(std::string cs) &
{
  _current_group = std::move(cs);
}
//-----------------------------------------------------------------------------
Config& Config::current_group(std::string cs) &&
{
  _current_group = std::move(cs);
  return *this;
}
//-----------------------------------------------------------------------------
void Config::copy_globals(const Config& conf)
{
  send_email(conf.send_email());
  email_smtp_server(conf.email_smtp_server());
  email_smtp_user(conf.email_smtp_user());
  email_smtp_password(conf.email_smtp_password());
  email_subject(conf.email_subject());
  email_sender_address(conf.email_sender_address());
  email_receipients(conf.email_receipients());
  threads(conf.threads());
  execute_tests(conf.execute_tests());
  plot_results(conf.plot_results());
  percent_difference(conf.percent_difference());
  current_group(conf.current_group());
}
//-----------------------------------------------------------------------------
void Config::clear()
{
  _execs.clear();
}
//-----------------------------------------------------------------------------
auto Config::begin() -> iterator
{
  return _execs.begin();
}
//-----------------------------------------------------------------------------
auto Config::begin() const -> const_iterator
{
  return _execs.begin();
}
//-----------------------------------------------------------------------------
auto Config::end() -> iterator
{
  return _execs.end();
}
//-----------------------------------------------------------------------------
auto Config::end() const -> const_iterator
{
  return _execs.end();
}
//-----------------------------------------------------------------------------
auto Config::front() -> reference
{
  return _execs.front();
}
//-----------------------------------------------------------------------------
auto Config::front() const -> const_reference
{
  return _execs.front();
}
//-----------------------------------------------------------------------------
auto Config::back() -> reference
{
  return _execs.back();
}
//-----------------------------------------------------------------------------
auto Config::back() const -> const_reference
{
  return _execs.back();
}
//-----------------------------------------------------------------------------
bool Config::load(std::string filepath, bool silent)
{
  Tokenizer tokens;
  std::ifstream ifs(filepath);
  if (ifs.is_open() && tokens.tokenize(ifs)) {
    int count = 1;
    return process(std::move(tokens));
  } else {
    if (!silent) {
      std::cerr << "Unable to Load " << filepath << "!!!\n";
    }
    return false;
  }
  return true;
}
//-----------------------------------------------------------------------------
bool Config::process(Tokenizer&& tokens)
{
  std::map<std::string, Executor> symbol_table;
  for (auto tokenItr = tokens.begin(); tokenItr != tokens.end(); ++tokenItr) {
    std::string& token = tokenItr->value;
    std::string original_token = token;

    //This should be safe
    std::transform(token.begin(), token.end(), token.begin(), ::tolower);

    bool valid_state = true;
    if (tokenItr->type == ETokenClass::Newline || tokenItr->type == ETokenClass::Whitespace) {
      continue;
    }
    if (symbol_table.find(original_token) != symbol_table.end()) {
      error_context = original_token;
      if (grab_next_symbol_or_fail(tokenItr, tokens.end()) && tokenItr->value == "(") {
        Tokenizer::token_list paramaters;
        Token close_scope_token{ ETokenClass::Symbol, ")" };
        if (gobble_until_next_given(tokenItr, tokens.end(), close_scope_token, paramaters)) {
          Executor run = symbol_table[original_token];
          run.Group(_current_group);
          if (!paramaters.empty()) {
            auto itr = paramaters.begin();
            if (itr->type == ETokenClass::Word || grab_next_word_or_fail(itr, paramaters.end())) {
              const std::string& name = itr->value;
              run.Name(name);
              ++itr;
              if (handle_driver_paramaters(itr, paramaters.end(), run)) {
                _execs.push_back(std::move(run));
              } else {
                std::cerr << "Error: Unable to process paramaters for " << original_token << "(" << name << ").\n";
              }
            } else {
              std::cerr << "Error: Call to " << original_token << " requires a test name with in the paramater list but non was found.\n";
            }
          }
        } else {
          return false;
        }
      } else {
        std::cerr << "Error: Expected ( after DriverID " << original_token << "but found tokentItr.\n";
      }
    } else if (token == "subject") {
      error_context = token;
      valid_state &= handle_assignment(tokenItr, tokens.end(), _email_subject);
    } else if (token == "recipients") {
      error_context = token;
      std::string recipents;
      valid_state &= handle_assignment(tokenItr, tokens.end(), recipents);
      valid_state &= handle_email_list(recipents, _email_receipients);
    } else if (token == "sender") {
      error_context = token;
      valid_state &= handle_assignment(tokenItr, tokens.end(), _email_sender_address);
      valid_state &= validate_email_string(_email_sender_address);
    } else if (token == "smtp") {
      error_context = token;
      valid_state &= handle_assignment(tokenItr, tokens.end(), _email_smtp_server);
      valid_state &= validate_server_address(_email_smtp_server);
    } else if (token == "smtp_password") {
      error_context = token;
      valid_state &= handle_assignment(tokenItr, tokens.end(), _email_smtp_password);
    } else if (token == "smtp_username") {
      error_context = token;
      valid_state &= handle_assignment(tokenItr, tokens.end(), _email_smtp_user);
    } else if (token == "sendemail") {
      error_context = token;
      valid_state &= handle_assignment(tokenItr, tokens.end(), _send_email);
    } else if (token == "executetests") {
      error_context = token;
      valid_state &= handle_assignment(tokenItr, tokens.end(), _execute_tests);
    } else if (token == "plotresults") {
      error_context = token;
      valid_state &= handle_assignment(tokenItr, tokens.end(), _plot_results);
    } else if (token == "percentdifference") {
      error_context = token;
      valid_state &= handle_assignment(tokenItr, tokens.end(), _percent_difference);
    } else if (token == "threads") {
      error_context = token;
      valid_state &= handle_assignment(tokenItr, tokens.end(), _threads);
    } else if (token == "@") {
      if (grab_next_word_or_fail(tokenItr, tokens.end()) && tokenItr->value == "group") {
        if (grab_next_word_or_fail(tokenItr, tokens.end())) {
          _current_group = tokenItr->value;
          Tokenizer::token_list group_name;
          auto delimiter = Token{ ETokenClass::Newline, "\n"};
          if (gobble_until_next_given(tokenItr, tokens.end(), delimiter, group_name)) {
            for (auto& component : group_name) {
              _current_group += component.value;
            }
            trim(_current_group);
          } else {
            valid_state &= false;
          }
        } else {
          std::cerr << "Error: Expected valid GroupID after @group but found " << ((tokenItr != tokens.end() && tokenItr->type != ETokenClass::Whitespace && tokenItr->type != ETokenClass::Newline) ? tokenItr->value : "nothing") << ".\n";
          valid_state &= false;
        }
      } else {
        std::cerr << "Error: @" << tokenItr->value << " not a valid expression only @group is.\n";
        valid_state &= false;
      }
    } else if (token == "driver") {
      Executor ex{ "Undefined", EDriver::Undefined };
      valid_state &= handle_driver_definition(tokenItr, tokens.end(), ex);
      if (valid_state) {
        symbol_table[ex.Name()] = ex;
      }
    } else {
      std::cerr << "Unable to process " << original_token << ".\n";
      valid_state &= false;
    }
    if (!valid_state) {
      return valid_state;
    }
  }
  return true;
}
//-----------------------------------------------------------------------------
void Config::push_back(const Executor& ex)
{
  _execs.push_back(ex);
}
//-----------------------------------------------------------------------------
void Config::push_back(Executor&& ex)
{
  _execs.push_back(std::move(ex));
}
//-----------------------------------------------------------------------------
void Config::merge(Config&& conf)
{
  for (auto& ex : conf) {
    _execs.push_back(std::move(ex));
  }
}
//-----------------------------------------------------------------------------
std::list<std::string> split_token_on_word_boundry(std::string token)
{

  std::list<std::string> result;
  std::regex word_boundry(R"(.*)(\b)(.*))");
  std::smatch re_match;
  if (std::regex_match(token, re_match, word_boundry)) {
    if (!re_match[0].str().empty()) {
      result.emplace_back(re_match[0].str());
    }
    if (!re_match[1].str().empty()) {
      result.emplace_back(re_match[1].str());
    }
    if (!re_match[2].str().empty()) {
      result.emplace_back(re_match[2].str());
    }
  } else {
    result.emplace_back(token);
  }
  return result;
}
//-----------------------------------------------------------------------------
bool handle_assignment(Tokenizer::token_list::iterator& tokenItr, Tokenizer::token_list::iterator end, std::string& rhs)
{
  rhs = "";
  auto next = tokenItr;
  ++next;
  if (*next == "=") {
    ++next;
    if (handle_quote_string(next, end, rhs)) {
      tokenItr = next;
    } else if (handle_tick_string(next, end, rhs)) {
      tokenItr = next;
    } else {
      while (next != end) {
        if (next->type == ETokenClass::Newline) {
          break;
        }
        if (next->type == ETokenClass::Whitespace) {
          break;
        }
        rhs += next->value;
        ++next;
      }
      tokenItr = next;
      return true;
    }
  } else {
    std::cerr << "Error: Expected = after " << *tokenItr << "but " << *next << " was found instead.\n";
  }
  return true;
}
//-----------------------------------------------------------------------------
bool handle_assignment(Tokenizer::token_list::iterator& tokenItr, Tokenizer::token_list::iterator end, bool& rhs)
{
  std::string token;
  if (handle_assignment(tokenItr, end, token)) {
    return handle_bool_string(token, rhs);
  } else {
    return false;
  }
}
//-----------------------------------------------------------------------------
bool handle_assignment(Tokenizer::token_list::iterator& tokenItr, Tokenizer::token_list::iterator end, int& rhs)
{
  std::string token;
  if (handle_assignment(tokenItr, end, token)) {
    return handle_integral_string(token, rhs);
  } else {
    return false;
  }
}
//-----------------------------------------------------------------------------
bool handle_assignment(Tokenizer::token_list::iterator& tokenItr, Tokenizer::token_list::iterator end, double& rhs)
{
  std::string token;
  if (handle_assignment(tokenItr, end, token)) {
    return handle_double_string(token, rhs);
  } else {
    return false;
  }
}
//-----------------------------------------------------------------------------
bool handle_driver_definition(Tokenizer::token_list::iterator& tokenItr, Tokenizer::token_list::iterator end, Executor& rhs)
{
  std::string driver_name;
  EDriver parent_driver = EDriver::Undefined;
  auto next = tokenItr;
  bool done_prcessing_definition = false;
  bool done_grabing_paramaters = false;
  Tokenizer::token_list paramaters;

  enum class ParseMode { ID,
                         PARENT,
                         OPTIONS } current_mode
    = ParseMode::ID;
  while (!done_prcessing_definition) {
    ++next;
    if (next == end) {
      std::cerr << "Error: Unexpected EOF after " << error_context << "\n";
      return false;
    }
    if (current_mode == ParseMode::ID) {
      if (grab_next_word_or_fail(next, end)) {
        driver_name = next->value;
        current_mode = ParseMode::PARENT;
      } else {
        std::cerr << "Error: Keyword Driver must be followed by a valid ID \n";
        return false;
      }
    } else if (current_mode == ParseMode::PARENT) {
      if (grab_next_symbol_or_fail(next, end)) {
        if (next->value == ":") {
          if (grab_next_word_or_fail(next, end)) {
            std::string token = next->value;
            std::string& original_token = next->value;
            std::transform(token.begin(), token.end(), token.begin(), ::tolower);
            if (!handle_driver_string(token, parent_driver)) {
              std::cerr << "Error: Invalid Driver Name " << next->value << " found after : for Driver " << driver_name << "\n";
            }
            if (grab_next_symbol_or_fail(next, end)) {
              if (next->value == "{") {
                current_mode = ParseMode::OPTIONS;
              } else {
                std::cerr << "Error: Driver  " << driver_name << ": " << next->value << " must be followed by valid option block"
                          << next->value << " found instead\n";
                return false;
              }
            } else {
              std::cerr << "Error: Driver  " << driver_name << ": must be followed by a valid parent class"
                        << "\n";
              return false;
            }
          } else {
            std::cerr << "Error: Driver  " << driver_name << ": must be followed by a valid parent class"
                      << "\n";
            return false;
          }
        } else if (next->value == "{") {
          current_mode = ParseMode::OPTIONS;
          parent_driver = EDriver::ScenarioTestDriver;
        } else {
          std::cerr << "Error: DriverID  " << driver_name << " must be followed by a parent class : <Parent> or a definition {}"
                    << "\n";
          return false;
        }
      } else {
        std::cerr << "Error: DriverID  " << driver_name << " must be followed by a parent class : <Parent> or a definition {}"
                  << "\n";
        return false;
      }
    } else if (current_mode == ParseMode::OPTIONS) {
      auto token = Token{ ETokenClass::Symbol, '}' };
      if (gobble_until_next_given(next, end, token, paramaters)) {
        rhs.Name(driver_name);
        rhs.Driver(parent_driver);
        done_prcessing_definition = true;
      } else {
        std::cerr << "Error: Unexpected EOF after { for Driver  " << driver_name
                  << "\n";
      }
    }
  }
  tokenItr = next;
  auto p_begin = paramaters.begin();
  auto p_end = paramaters.end();
  if (!handle_driver_paramaters(p_begin, p_end, rhs)) {
    return false;
  }
  return true;
}
//-----------------------------------------------------------------------------
bool handle_driver_paramaters(Tokenizer::token_list::iterator& tokenItr, Tokenizer::token_list::iterator end, Executor& rhs)
{
  auto next = tokenItr;
  for (; next != end; ++next) {
    std::string token = next->value;
    std::string& original_token = next->value;
    std::transform(token.begin(), token.end(), token.begin(), ::tolower);

    if (next->type == ETokenClass::Whitespace || next->type == ETokenClass::Newline) {
      continue;
    } else if (token == "," || token == "option") {
      continue;
    } else if (token == "nocompare") {
      rhs.NoCompare(true);
    } else if (token == "fastplot") {
      rhs.PlotStyle(EPlotStyle::FastPlot);
    } else if (token == "fullplot") {
      rhs.PlotStyle(EPlotStyle::FullPlot);
    } else if (token == "fastploterrors") {
      rhs.PlotStyle(EPlotStyle::FastPlotErrors);
    } else if (token == "fullploterrors") {
      rhs.PlotStyle(EPlotStyle::FullPlotErrors);
    } else if (token == "memoryfastplot") {
      rhs.PlotStyle(EPlotStyle::MemoryFastPlot);
    } else if (token == "baseline" || token == "baselines") {
      std::string baselines;
      handle_assignment(next, end, baselines);
      rhs.Baselines(baselines);
    } else if (token == "computed") {
      std::string computed;
      handle_assignment(next, end, computed);
      rhs.Computed(computed);
    } else if (token == "results") {
      std::string results;
      std::vector<std::string> results_list;
      handle_assignment(next, end, results);
      handle_delimited_list(results, ',', results_list);
      rhs.Results(results_list);
    } else if (token == "scenario") {
      std::string computed;
      handle_assignment(next, end, computed);
      rhs.Scenario(computed);
    } else {
      std::cerr << "Error: Unknown paramater " << original_token << " found after " << tokenItr->value << " .\n";
    }
    if (next == end) {
      break;
    }
  }
  return true;
}
//-----------------------------------------------------------------------------
bool handle_bool_string(const std::string& token, bool& result)
{
  const std::regex bool_true_pattern("true|on|1");
  const std::regex bool_false_pattern("false|off|0");
  if (std::regex_match(token, bool_true_pattern)) {
    result = true;
    return true;
  } else if (std::regex_match(token, bool_false_pattern)) {
    result = false;
    return true;
  }
  std::cerr << "Error: Expected boolean <true|false|on|off|1|0> but found " << token << " near " << error_context << "\n";
  return false;
}
//-----------------------------------------------------------------------------
bool handle_double_string(const std::string& token, double& result)
{
  try {
    result = std::stod(token);
  } catch (const std::invalid_argument& e) {
    std::cerr << "Error: Unable to parse double near " << error_context << e.what() << "\n";
    return false;
  }
  return true;
}
//-----------------------------------------------------------------------------
bool handle_integral_string(const std::string& token, int& result)
{
  try {
    result = std::stoi(token, nullptr, 0);
  } catch (const std::invalid_argument& e) {
    std::cerr << "Error: Unable to parse integer near " << error_context << e.what() << "\n";
    return false;
  }
  return true;
}
//-----------------------------------------------------------------------------
bool handle_driver_string(const std::string& token, EDriver& driver)
{
  if ("undefined" == token) {
    driver = EDriver::Undefined;
  } else if ("bgeunittestdriver" == token) {
    driver = EDriver::BGEUnitTestDriver;
  } else if ("cdmunittestdriver" == token) {
    driver = EDriver::CDMUnitTestDriver;
  } else if ("scenariotestdriver" == token) {
    driver = EDriver::ScenarioTestDriver;
  } else {
    std::cerr << "Error: Expeceted EDriver value [BGEUnitTestDriver|CDMUnitTestDriver|ScenarioTestDriver] but found " << token << ".\n";
    return false;
  }
  return true;
}
//-----------------------------------------------------------------------------
bool handle_tick_string(Tokenizer::token_list::iterator& tokenItr, Tokenizer::token_list::iterator end, std::string& rhs)
{
  auto next = tokenItr;
  rhs.clear();
  if (*tokenItr == "'") {
    ++next;
    while ((*next)[0] != '\'') {
      rhs += next->value;
      ++next;
      if (next == end) {
        std::cerr << "Error: Expected closing ', but none was found after " << error_context << ".\n";
        return false;
      }
    }
  } else {
    return false;
  }
  tokenItr = next;
  return true;
}
//-----------------------------------------------------------------------------
bool handle_quote_string(Tokenizer::token_list::iterator& tokenItr, Tokenizer::token_list::iterator end, std::string& rhs)
{
  auto next = tokenItr;
  rhs.clear();
  if (*tokenItr == "\"") {
    ++next;
    while (next != end && (*next) != "\"") {
      rhs += next->value;
      ++next;
      if (next == end) {
        std::cerr << "Error: Expected closing \", but none was found after " << error_context << ".\n";
        return false;
      }
    }
  } else {
    return false;
  }
  tokenItr = next;
  return true;
}
//-----------------------------------------------------------------------------
bool handle_email_list(const std::string& input, std::vector<std::string>& result)
{
  constexpr char delimiter = ';';
  return handle_delimited_list(input, delimiter, result, validate_email_string);
}
//-----------------------------------------------------------------------------
bool validate_email_string(const std::string& token)
{
  std::regex rx{ R"REGEX(^([a-zA-Z0-9_\-\.]+)@((\[[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.)|(([a-zA-Z0-9\-]+\.)+))([a-zA-Z]{2,4}|[0-9]{1,3})(\]?)$)REGEX" };
  return std::regex_match(token, rx);
}
//-----------------------------------------------------------------------------
bool validate_server_address(const std::string& token)
{
  auto ip_rx = std::regex{ R"REGEX(^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$)REGEX" };
  auto hostname_rx = std::regex{ R"REGEX(^(([a-zA-Z0-9]|[a-zA-Z0-9][a-zA-Z0-9\-]*[a-zA-Z0-9])\.)*([A-Za-z0-9]|[A-Za-z0-9][A-Za-z0-9\-]*[A-Za-z0-9])$)REGEX" };

  return std::regex_match(token, ip_rx) || std::regex_match(token, hostname_rx);
}
//-----------------------------------------------------------------------------
bool grab_next_word_or_fail(Tokenizer::token_list::iterator& tokenItr, Tokenizer::token_list::iterator end)
{
  auto next = tokenItr;
  do {
    ++next;
    if (next == end) {
      std::cerr << "Error: Unexpected EOF after " << error_context << "\n";

    } else if (next->type == ETokenClass::Whitespace) {
      continue;
    } else if (next->type == ETokenClass::Newline) {
      continue;
    } else if (next->type == ETokenClass::Word) {
      tokenItr = next;
      return true;
    } else {
      tokenItr = next;
      return false;
    }
  } while (next != end);
  return false;
}
//-----------------------------------------------------------------------------
bool grab_next_symbol_or_fail(Tokenizer::token_list::iterator& tokenItr, Tokenizer::token_list::iterator end)
{
  auto next = tokenItr;
  do {
    ++next;
    if (next == end) {
      std::cerr << "Error: Unexpected EOF after " << error_context << "\n";
    } else if (next->type == ETokenClass::Whitespace) {
      continue;
    } else if (next->type == ETokenClass::Newline) {
      continue;
    } else if (next->type == ETokenClass::Symbol) {
      tokenItr = next;
      return true;
    } else {
      tokenItr = next;
      return false;
    }
  } while (next != end);
  return false;
}
//-----------------------------------------------------------------------------
bool gobble_until_next_given(Tokenizer::token_list::iterator& tokenItr, Tokenizer::token_list::iterator end, Token& token, Tokenizer::token_list& list)
{
  auto next = tokenItr;
  do {
    ++next;
    if (next == end) {
      std::cerr << "Error: Unexpected EOF after " << error_context << "\n";
    } else if (next->type == token.type && next->value == token.value) {
      tokenItr = next;
      return true;
    } else {
      list.emplace_back(*next);
    }
  } while (next != end);
  return false;
}
//-------------------------------------------------------------------------------
bool handle_delimited_list(const std::string& input, const char delimiter, std::vector<std::string>& result, std::function<bool(std::string)> validator)
{
  std::string token;
  std::stringstream stream(input);
  while (std::getline(stream, token, delimiter)) {
    if (!token.empty() && validator(token)) {

      result.push_back(token);
    }
  }
  return true;
}
}
