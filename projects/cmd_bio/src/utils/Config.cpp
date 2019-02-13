#include "Config.h"
#include "Tokenizer.h"
#include "string-helpers.h"

#include <fstream>
#include <list>
#include <regex>
#include <sstream>

namespace biogears {

std::string error_context;
Tokenizer::token_list::iterator token_end;
bool handle_assignment(Tokenizer::token_list::iterator&, std::string& rhs);
bool handle_assignment(Tokenizer::token_list::iterator&, bool& rhs);
bool handle_assignment(Tokenizer::token_list::iterator&, int& rhs);
bool handle_assignment(Tokenizer::token_list::iterator&, double& rhs);
bool handle_driver_definition(Tokenizer::token_list::iterator&, Executor& rhs);
bool handle_bool_string(const std::string&, bool&);
bool handle_double_string(const std::string&, double&);
bool handle_integral_string(const std::string&, int&);
bool handle_driver_string(const std::string&, EDriver&);
bool handle_tick_string(Tokenizer::token_list::iterator&, std::string&);
bool handle_quote_string(Tokenizer::token_list::iterator&, std::string&);
bool handle_email_list(const std::string&, std::vector<std::string>&);
bool handle_delimited_list(const std::string&, const std::string, std::vector<std::string>&);
bool validate_email_string(const std::string&);
bool validate_server_address(const std::string&);
bool grab_next_word_or_fail(Tokenizer::token_list::iterator&);
bool grab_next_symbol_or_fail(Tokenizer::token_list::iterator&);
bool goble_until_next_given(Tokenizer::token_list::iterator&, Token&, Tokenizer::token_list&);
std::list<std::string> split_token_on_word_boundry(std::string);

Executor::Executor(std::string n, EDriver d)
  : name(n)
  , driver(d)
  , plot_style(EPlotStyle::FastPlot)
  , no_compare(false)
{
}
//-----------------------------------------------------------------------------
std::string Executor::Name() const { return name; }
//-----------------------------------------------------------------------------
EDriver Executor::Driver() const { return driver; }
//-----------------------------------------------------------------------------
EPlotStyle Executor::PlotStyle() const { return plot_style; }
//-----------------------------------------------------------------------------
bool Executor::NoCompare() const
{
  return no_compare;
}
//-----------------------------------------------------------------------------
std::string Executor::Baselines() const { return baselines; }
//-----------------------------------------------------------------------------
std::string Executor::Computed() const { return computed; }
//-----------------------------------------------------------------------------
std::vector<std::string> Executor::Results() const { return results; }
//-----------------------------------------------------------------------------
Executor& Executor::Name(const std::string& n) &&
{
  name = n;
  return *this;
}
//-----------------------------------------------------------------------------
Executor& Executor::Driver(EDriver d) &&
{
  driver = d;
  return *this;
}
//-----------------------------------------------------------------------------
Executor& Executor::PlotStyle(EPlotStyle p) &&
{
  plot_style = p;
  return *this;
}
//-----------------------------------------------------------------------------
Executor& Executor::NoCompare(bool nc) &&
{
  no_compare = nc;
  return *this;
}
//-----------------------------------------------------------------------------
Executor& Executor::Baselines(const std::string& b) &&
{
  baselines = b;
  return *this;
}
//-----------------------------------------------------------------------------
Executor& Executor::Computed(const std::string& c) &&
{
  computed = c;
  return *this;
}
//-----------------------------------------------------------------------------
Executor& Executor::Results(const std::vector<std::string>& r) &&
{
  results = r;
  return *this;
}
//-----------------------------------------------------------------------------
std::string Executor::Name(const std::string& n) &
{
  return name = n;
}
//-----------------------------------------------------------------------------
EDriver Executor::Driver(EDriver d) &
{
  return driver = d;
}
//-----------------------------------------------------------------------------
EPlotStyle Executor::PlotStyle(EPlotStyle p) &
{
  return plot_style = p;
}
//-----------------------------------------------------------------------------
bool Executor::NoCompare(bool nc) &
{
  return no_compare = nc;
}
//-----------------------------------------------------------------------------
std::string Executor::Baselines(const std::string& b) &
{
  return baselines = b;
}
//-----------------------------------------------------------------------------
std::string Executor::Computed(const std::string& c) &
{
  return computed = c;
}
//-----------------------------------------------------------------------------
std::vector<std::string> Executor::Results(const std::vector<std::string>& r) &
{
  return results = r;
}
//-----------------------------------------------------------------------------
Executor& Executor::push_back_results(const std::string& r) &&
{
  results.push_back(r);
  return *this;
}
//-----------------------------------------------------------------------------
void Executor::push_back_results(const std::string& r) & { results.push_back(r); }
//-----------------------------------------------------------------------------
void Executor::clear_results() { results.clear(); }
//-----------------------------------------------------------------------------
Config::Config()
  : _email_subject("BioGears Notification Email")
  , _current_group("Default")
  , _send_email(true)
  , _execute_tests(true)
  , _plot_results(true)
  , _percent_difference(2.)
  , _threads(-1)
{
}
//-----------------------------------------------------------------------------
Config::Config(const std::string& file)
  : _email_subject("BioGears Notification Email")
  , _current_group("Default")
  , _send_email(true)
  , _execute_tests(true)
  , _plot_results(true)
  , _percent_difference(2.)
  , _threads(-1)
{
  load(file);
}
//-----------------------------------------------------------------------------
bool Config::load(std::string filepath)
{
  Tokenizer tokens;
  std::ifstream ifs(filepath);
  if (ifs.is_open() && tokens.tokenize(ifs)) {
    int count = 1;
    return process(std::move(tokens));
  } else {
    std::cerr << "Unable to Load " << filepath << "!!!\n";
    return false;
  }
  return true;
}
//-----------------------------------------------------------------------------
bool Config::process(Tokenizer&& tokens)
{
  token_end = tokens.end();
  for (auto tokenItr = tokens.begin(); tokenItr != tokens.end(); ++tokenItr) {
    std::string& token = tokenItr->value;
    std::string original_token = token;

    //This should be safe
    std::transform(token.begin(), token.end(), token.begin(), ::tolower);

    std::map<std::string, Executor> symbol_table;

    bool valid_state = true;
    if (tokenItr->type == ETokenClass::Newline || tokenItr->type == ETokenClass::Whitespace) {
      continue;
    }
    if (symbol_table.find("token") != symbol_table.end()) {
      //TODO:Process Symbol
    } else if (token == "subject") {
      error_context = token;
      valid_state &= handle_assignment(tokenItr, _email_subject);
    } else if (token == "recepients") {
      error_context = token;
      std::string recipents;
      valid_state &= handle_assignment(tokenItr, recipents);
      valid_state &= handle_email_list(recipents, _email_receipients);
    } else if (token == "sender") {
      error_context = token;
      valid_state &= handle_assignment(tokenItr, _email_sender_address);
      valid_state &= validate_email_string(_email_sender_address);
    } else if (token == "smtp") {
      error_context = token;
      valid_state &= handle_assignment(tokenItr, _email_smtp_server);
      valid_state &= validate_server_address(_email_smtp_server);
    } else if (token == "smtp_password") {
      error_context = token;
      valid_state &= handle_assignment(tokenItr, _email_smtp_password);
    } else if (token == "smtp_username") {
      error_context = token;
      valid_state &= handle_assignment(tokenItr, _email_smtp_user);
    } else if (token == "sendemail") {
      error_context = token;
      valid_state &= handle_assignment(tokenItr, _send_email);
    } else if (token == "executetests") {
      error_context = token;
      valid_state &= handle_assignment(tokenItr, _execute_tests);
    } else if (token == "plotresults") {
      error_context = token;
      valid_state &= handle_assignment(tokenItr, _plot_results);
    } else if (token == "percentdifference") {
      error_context = token;
      valid_state &= handle_assignment(tokenItr, _percent_difference);
    } else if (token == "threads") {
      error_context = token;
      valid_state &= handle_assignment(tokenItr, _threads);
    } else if (token == "@group") {
      ++tokenItr;
      if (tokenItr != tokens.end()) {
        _current_group = tokenItr->value;
      } else {
        std::cerr << "Expected token after @group but found nothing.\n";
        valid_state &= false;
      }
    } else if (token == "driver") {
      Executor ex{ "Undefined", EDriver::Undefined };
      valid_state &= handle_driver_definition(tokenItr, ex);
    } else {
      std::cerr << "Unable to process " << token << ".\n";
      valid_state &= false;
    }
    if (!valid_state) {
      return valid_state;
    }
  }
  return true;
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
bool handle_assignment(Tokenizer::token_list::iterator& tokenItr, std::string& rhs)
{
  rhs = "";
  auto next = tokenItr;
  ++next;
  if (*next == "=") {
    ++next;
    if (handle_quote_string(next, rhs)) {
      tokenItr = next;
    } else if (handle_tick_string(next, rhs)) {
      tokenItr = next;
    } else {
      while (next->type != ETokenClass::Newline) {
        if (next == token_end) {
          std::cerr << "Error: Expected closing \", but none was found after " << error_context << ".\n";
          tokenItr = next;
          return false;
        }
        rhs += next->value;
        ++next;
      }
      tokenItr = next;
    }
  } else {
    std::cerr << "Error: Expected = after " << *tokenItr << "but " << *next << "was found instead.\n";
  }
  return true;
}
//-----------------------------------------------------------------------------
bool handle_assignment(Tokenizer::token_list::iterator& tokenItr, bool& rhs)
{
  std::string token;
  if (handle_assignment(tokenItr, token)) {
    return handle_bool_string(token, rhs);
  } else {
    return false;
  }
}
//-----------------------------------------------------------------------------
bool handle_assignment(Tokenizer::token_list::iterator& tokenItr, int& rhs)
{
  std::string token;
  if (handle_assignment(tokenItr, token)) {
    return handle_integral_string(token, rhs);
  } else {
    return false;
  }
}
//-----------------------------------------------------------------------------
bool handle_assignment(Tokenizer::token_list::iterator& tokenItr, double& rhs)
{
  std::string token;
  if (handle_assignment(tokenItr, token)) {
    return handle_double_string(token, rhs);
  } else {
    return false;
  }
}
//-----------------------------------------------------------------------------
bool handle_driver_definition(Tokenizer::token_list::iterator& tokenItr, Executor& executor)
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
    if (next == token_end) {
      std::cerr << "Error: Unexpected EOF after " << error_context << "\n";
      return false;
    }
    if (current_mode == ParseMode::ID) {
      if (grab_next_word_or_fail(next)) {
        driver_name = next->value;
        current_mode = ParseMode::PARENT;
      } else {
        std::cerr << "Error: Keyword Driver must be followed by a vaklid ID \n";
        return false;
      }
    } else if (current_mode == ParseMode::PARENT) {
      if (grab_next_symbol_or_fail(next)) {
        if (next->value == ":") {
          if (grab_next_word_or_fail(next)) {
            if (!handle_driver_string(next->value, parent_driver)) {
              std::cerr << "Error: Invalid Driver Name " << next->value << " found after : for Driver " << driver_name << "\n";
            }
            if (grab_next_symbol_or_fail(next)) {
              if (next->value == "{") {
                current_mode = ParseMode::OPTIONS;
              } else {
                std::cerr << "Error: Driver  " << driver_name << ": " << next->value << "must be followed by valid option block"
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
      auto definition_tokens = Tokenizer::token_list{};
      auto token = Token{ ETokenClass::Symbol, '}' };
      if (goble_until_next_given(next, token, definition_tokens)) {
        executor = Executor{ driver_name, parent_driver };
        done_prcessing_definition = true;
      } else {
        std::cerr << "Error: Unexpected EOF after { for Driver  " << driver_name
                  << "\n";
      }
    }
  }
  tokenItr = next;

  for (auto itr = paramaters.begin(); itr != paramaters.end(); ++itr) {
    std::string token = itr->value;
    std::transform(token.begin(), token.end(), token.begin(), ::tolower);

    if (itr->value == "option") {
      continue;
    }
    if (token == "fastplot") {
      executor.PlotStyle(EPlotStyle::FastPlot);
    } else if (token == "fullplot") {
      executor.PlotStyle(EPlotStyle::FullPlot);
    } else if (token == "fastploterrors") {
      executor.PlotStyle(EPlotStyle::FastPlotErrors);
    } else if (token == "fullploterrors") {
      executor.PlotStyle(EPlotStyle::FullPlotErrors);
    } else if (token == "memoryfastplot") {
      executor.PlotStyle(EPlotStyle::MemoryFastPlot);
    } else if (token == "baselines") {
      std::string baselines;
      handle_assignment(itr,baselines);
      executor.Baselines(baselines);
    } else if (token == "computed") {
      std::string computed;
      handle_assignment(itr,computed);
      executor.Baselines();
    } else if (token == "results") {
      std::string results;
      std::vector<std::string> results_list;
      handle_assignment(itr,results);
      handle_delimited_list(results,",", results_list);
      executor.Results(results_list);
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
  std::cerr << "Error: Expected boolean <true|false|on|off|1|0> but found " << token << "near " << error_context << "\n";
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
bool handle_tick_string(Tokenizer::token_list::iterator& itr, std::string& value)
{
  auto next = itr;
  value.clear();
  if (*itr == "'") {
    ++next;
    while ((*next)[0] != '\'') {
      value += next->value;
      ++next;
      if (next == token_end) {
        std::cerr << "Error: Expected closing ', but none was found after " << error_context << ".\n";
        return false;
      }
    }
  } else {
    return false;
  }
  itr = next;
  return true;
}

//-----------------------------------------------------------------------------
bool handle_quote_string(Tokenizer::token_list::iterator& itr, std::string& value)
{
  auto next = itr;
  value.clear();
  if (*itr == "\"") {
    ++next;
    while ((*next)[0] != '"') {
      value += next->value;
      ++next;
      if (next == token_end) {
        std::cerr << "Error: Expected closing \", but none was found after " << error_context << ".\n";
        return false;
      }
    }
  } else {
    return false;
  }
  itr = next;
  return true;
}
//-----------------------------------------------------------------------------
bool handle_email_list(const std::string& token, std::vector<std::string>& result)
{

  constexpr char delimiter = ';';
  size_t start = 0;
  size_t end = token.find(delimiter, start);

  std::string address;
  std::stringstream stream(token);
  while (std::getline(stream, address, ';')) {
    if (!address.empty() && validate_email_string(address)) {

      result.push_back(address);
    }
  }
  return true;
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
bool grab_next_word_or_fail(Tokenizer::token_list::iterator& itr)
{
  auto next = itr;
  do {
    ++next;
    if (next == token_end) {
      std::cerr << "Error: Unexpected EOF after " << error_context << "\n";

    } else if (next->type == ETokenClass::Whitespace) {
      continue;
    } else if (next->type == ETokenClass::Newline) {
      continue;
    } else if (next->type == ETokenClass::Word) {
      itr = next;
      return true;
    } else {
      return false;
    }
  } while (next != token_end);
  return false;
}
//-----------------------------------------------------------------------------
bool grab_next_symbol_or_fail(Tokenizer::token_list::iterator& itr)
{
  auto next = itr;
  do {
    ++next;
    if (next == token_end) {
      std::cerr << "Error: Unexpected EOF after " << error_context << "\n";
    } else if (next->type == ETokenClass::Whitespace) {
      continue;
    } else if (next->type == ETokenClass::Newline) {
      continue;
    } else if (next->type == ETokenClass::Symbol) {
      itr = next;
      return true;
    } else {
      return false;
    }
  } while (next != token_end);
  return false;
}
//-----------------------------------------------------------------------------
bool goble_until_next_given(Tokenizer::token_list::iterator& itr, Token& token, Tokenizer::token_list& list)
{
  auto next = itr;
  do {
    ++next;
    if (next == token_end) {
      std::cerr << "Error: Unexpected EOF after " << error_context << "\n";
    } else if (next->type == token.type && next->value == token.value) {
      itr = next;
      return true;
    } else {
      list.emplace_back(*next);
    }
  } while (next != token_end);
  return false;
}
//-------------------------------------------------------------------------------
bool handle_delimited_list(const std::string&, const std::string, std::vector<std::string>&)
{
  
}
}
