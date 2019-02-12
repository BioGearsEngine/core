#include "Config.h"
#include "Tokenizer.h"
#include "string-helpers.h"

#include <fstream>
#include <list>
#include <regex>

namespace biogears {

std::string error_context;
Tokenizer::token_list::iterator token_end;
std::string handle_assignment(Tokenizer::token_list::iterator&);
Executor handle_driver_definition(Tokenizer::token_list::iterator& tokenItr);
bool handle_bool_string(const std::string&, bool&);
bool handle_double_string(const std::string&, double&);
bool handle_integral_string(const std::string&, int&);
bool handle_driver_string(const std::string&, EDriver&);
bool handle_tick_string(Tokenizer::token_list::iterator&, std::string& string);
bool handle_quote_string(Tokenizer::token_list::iterator&, std::string& string);
std::list<std::string> split_token_on_word_boundry(std::string word);

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

    if (symbol_table.find("token") != symbol_table.end()) {
      //TODO:Process Symbol
    } else if (token == "subject") {
      error_context = token;
      auto value = handle_assignment(tokenItr);
      if (value.empty()) {
        return false;
      } else {
        _email_subject = value;
      }
    } else if (token == "sendemail") {
      error_context = token;
      auto value = handle_assignment(tokenItr);
      if (value.empty()) {
        return false;
      } else {
        if (!handle_bool_string(value, _send_email)) {
          return false;
        }
      }
    } else if (token == "executetests") {
      error_context = token;
      auto value = handle_assignment(tokenItr);
      if (value.empty()) {
        return false;
      } else {
        if (!handle_bool_string(value, _execute_tests)) {
          return false;
        }
      }
    } else if (token == "plotresults") {
      error_context = token;
      auto value = handle_assignment(tokenItr);
      if (value.empty()) {
        return false;
      } else {
        if (!handle_bool_string(value, _plot_results)) {
          return false;
        }
      }
    } else if (token == "percentdifference") {
      error_context = token;
      auto value = handle_assignment(tokenItr);
      if (value.empty()) {
        return false;
      } else {
        if (!handle_double_string(value, _percent_difference)) {
          return false;
        }
      }
    } else if (token == "threads") {
      error_context = token;
      auto value = handle_assignment(tokenItr);
      if (value.empty()) {
        return false;
      } else {
        if (!handle_integral_string(value, _threads)) {
          return false;
        }
      }
    } else if (token == "@group") {
      ++tokenItr;
      if (tokenItr != tokens.end()) {
        _current_group = tokenItr->value;
      } else {
        std::cerr << "Expected token after @group but found nothing.\n";
        return false;
      }
    } else if (token == "Driver") {
      Executor ex = handle_driver_definition(tokenItr);
      if (EDriver::Undefined == ex.Driver()) {
        return false;
      }
    } else {
      std::cerr << "Unable to process " << token << ".\n";
      return false;
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
std::string handle_assignment(Tokenizer::token_list::iterator& tokenItr)
{
  std::string value;
  auto next = tokenItr;
  ++next;
  if (*next == "=") {
    ++next;
    if ( handle_quote_string(next, value) ) {
      tokenItr = next;
    } else if ( handle_tick_string(next, value)) {
      tokenItr = next;
    } else {
      value += next->value;
      tokenItr = next;
    }
  } else {
    std::cerr << "Error: Expected = after " << *tokenItr << "but " << *next << "was found instead.\n";
  }
  return value;
}
//-----------------------------------------------------------------------------
Executor handle_driver_definition(Tokenizer::token_list::iterator& tokenItr)
{

  auto current = tokenItr;
  auto next = tokenItr;
  std::advance(current, 1);
  std::advance(next, 2);

  std::string token = current->value;
  if ((*next)[0] == '{') {
    token += next->value;
    std::advance(next, 1);
  }
  const std::regex driver_def_rx(R"((\w+)\s*{)");
  std::smatch matches;
  if (std::regex_match(next->value, matches, driver_def_rx)) {
    std::string name = matches[0];
    EDriver driver = EDriver::Undefined;
    current = next;
    token = current->value;
    std::transform(token.begin(), token.end(), token.begin(), ::tolower);

    if (token == "driver") {
      std::advance(next, 1);
      token = next->value;
      std::advance(next, 1);
      std::transform(token.begin(), token.end(), token.begin(), ::tolower);
      if (!handle_driver_string(token, driver)) {
        return { "Error", EDriver::Undefined };
      }
      Executor result{ name, driver };
      std::list<std::string> token_buffer;
      current = next;
      std::advance(next, 1);
      token_buffer = split_token_on_word_boundry(current->value);
      while (token_buffer.size()) {
        token = token_buffer.front();
        std::transform(token.begin(), token.end(), token.begin(), ::tolower);
        if (token == "option") {
          if (token_buffer.size() == 1) {
            auto v = split_token_on_word_boundry(next->value);
            token_buffer.insert(token_buffer.end(), v.begin(), v.end());
            std::advance(next, 1);
          }
          token_buffer.pop_front();
          token = token_buffer.front();
          std::transform(token.begin(), token.end(), token.begin(), ::tolower);
          if (token == "fastplot") {
            result.PlotStyle(EPlotStyle::FastPlot);
          } else if (token == "fullplot") {
            result.PlotStyle(EPlotStyle::FullPlot);
          } else if (token == "fastploterrors") {
            result.PlotStyle(EPlotStyle::FastPlotErrors);
          } else if (token == "fullploterrors") {
            result.PlotStyle(EPlotStyle::FullPlotErrors);
          } else if (token == "memoryfastplot") {
            result.PlotStyle(EPlotStyle::MemoryFastPlot);
          } else if (token == "baselines") {
            result.Baselines();
          } else if (token == "computed") {
            result.Baselines();
          } else if (token == "results") {
            result.Results({});
          }

        } else if (token == "}") {
          return result;
        } else {
          std::cerr << "Error: Expected Option or } but " << *current << "found.\n";
        }

        auto v = split_token_on_word_boundry(next->value);
        token_buffer.insert(token_buffer.end(), v.begin(), v.end());
        std::advance(next, 1);
      }
    }
  } else {
    std::cout << "Error: Malformed Driver definition. Expected <id> {, but found " << token << ".\n";
    return { "Error", EDriver::Undefined };
  }
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
}
