#include "Arguments.h"

#include <algorithm>
#include <cassert>
#include <iostream>

#include <biogears/filesystem/path.h>
#include <biogears/string/manipulation.h>

namespace biogears {
//!
//! Constructor for Arguments
//! \param Options vector<string>  -- List of Optional Paramaters whose values are either True or False. Keywords default to false unless present
//! \param Keywords vector<string> -- List of Single Value Keywords whose value is an empty string unless present then must be followed by a non keyword or parse error.
//! \param MultiWords vector<string> -- list of Multi Value keywords. These values assume all strings following each keyword is a value for the word until another Multiword,Keyword,or Option occurs.
//!
//! Behavior is undefined if any key is duplicated across the three paramters
Arguments::Arguments(const std::vector<std::string>& options, const std::vector<std::string>& keywords, const std::vector<std::string>& multiwords)
{
  for (auto key : options) {
    std::transform(key.begin(), key.end(), key.begin(), ::tolower);
    _options[key] = false;
  }

  for (auto key : keywords) {
    std::transform(key.begin(), key.end(), key.begin(), ::tolower);
    if (_options.find(key) != _options.end()) {
      std::cerr << "Warrning: Keyword Key" << key << " also found in Options."
                << "\n";
    }
    _keywords[key] = "";
  }

  for (auto key : multiwords) {
    std::transform(key.begin(), key.end(), key.begin(), ::tolower);
    if (_options.find(key) != _options.end()) {
      std::cerr << "Warrning: Multiword Key" << key << " also found in Options."
                << "\n";
    }
    if (_keywords.find(key) != _keywords.end()) {
      std::cerr << "Warrning: Multiword Key " << key << " also found in Keywords."
                << "\n";
    }
    _multiwords[key] = {};
  }
}
//-----------------------------------------------------------------------------
//!
//! \param argc Int -- The number of strings in argv
//! \param argc char** -- List of C-Strings
//!
//! \brief
//! Parses the arguments in ARGV. based on the constructor arguments.
//!  -- For each key in Option present the value of the key is set to true;
//!  -- For each key in Keywords  present the value of key is set to the next c-string unless the next c strng is another key then an error is thrown and false returned
//!  -- For each key in MultiKetwords present all procceding values is pushed to a vector until no more argvs or another key is found. If no Value is found an error is thrown and false is retuend
//!
//! \return bool True if no parse error else false
bool Arguments::parse(int argc, char* argv[])
{
  std::vector<std::string> args;
  std::vector<std::string> flags;
  const std::string prefix = "--";

  _self_name = biogears::filesystem::path(argv[0]).basename().string();

  size_t index = 1;
  if (argc > 1) {
    _empty = false;
    while (1 < argc) {
      args.emplace_back(argv[index++]);
      --argc;
    }
  }
  for (auto& arg : args) {
    if (arg.length() > 2 && arg[0] == '-' && arg[1] == '-') {
      arg = arg.substr(2);
    } else if (arg[0] == '-' && arg.size() > 1) {
      auto sub = arg.substr(2);
      arg = arg[1];
      for (const char ch : sub) {
        flags.push_back(std::string(1, ch));
      }
    }
  }
  args.insert(args.end(), flags.begin(), flags.end());
  return parse(args);
}
//-----------------------------------------------------------------------------
//!
//! \param std::vector<std::string> -- List of arguments to be parsed
//!
//! \brief
//! Parses the arguments in ARGV. based on the constructor arguments.
//!  -- For each key in Option present the value of the key is set to true;
//!  -- For each key in Keywords  present the value of key is set to the next c-string unless the next c strng is another key then an error is thrown and false returned
//!  -- For each key in MultiKetwords present all procceding values is pushed to a vector until no more argvs or another key is found. If no Value is found an error is thrown and false is retuend
//!
//! \return bool True if no parse error else false
bool Arguments::parse(const std::vector<std::string>& args)
{
  size_t index = 0;
  size_t argument_index = 0;
  while (index < args.size()) {
    _empty = false;
    std::string arg = trim(args[index]);
    std::transform(arg.begin(), arg.end(), arg.begin(), ::tolower);
    ++index;
    if (_options.find(arg) != _options.end()) {
      _options[arg] = true;
      continue; // Argument Loop
    }
    if (_keywords.find(arg) != _keywords.end()) {
      if (index < args.size()) {
        const std::string& value = args[index];
        if (isOption(value) || isKeyword(value) || isMultiWord(value)) {
          _error = "Expected value after " + arg + " but keyword " + value + " was found.\n";
          //std::cerr << _error;
          return false;
        }
        ++index;
        _keywords[arg] = value;
        continue; // Argument Loop
      } else {
        _error = "Expected value after " + arg + " but no value was given.\n";
        //std::cerr << _error;
        return false;
      }
    }
    if (_multiwords.find(arg) != _multiwords.end()) {
      if (index < args.size()) {
        bool value_found = false;
        while (index < args.size()) {
          const std::string& value = args[index];
          if (isOption(value) || isKeyword(value) || isMultiWord(value)) {
            if (value_found) {
              break; // Inner Argument Loop
            } else {
              _error = "Expected value after " + arg + " but keyword " + value + " was found.\n";
              //std::cerr << _error;
              return false;
            }
          } else {
            ++index;
            value_found = true;
            _multiwords[arg].emplace_back(value);
          }
        }
        continue; // Argument Loop
      } else {
        _error = "Expected value after " + arg + " but no value was given.\n";
        //std::cerr << _error;
        return false;
      }
    }
    if (_required_keywords.size() && argument_index < _required_keywords.size()) {
      assert(_required_keywords[argument_index].empty());
      _keywords[_required_keywords[argument_index++]] = arg;
      continue;
    }
    if (!_required_multiword.empty()) {
      _multiwords[_required_multiword].emplace_back(arg);
      continue;
    } else {
      if (!_required_multiword.empty() && _required_keywords.size() == 1) {
        _error = _required_keywords.front() + " second occurance as " + arg + "is not allowed.\n";
        //std::cerr << _error;
        return false;
      } else {
        _error = "argument given after final required argument " + _required_keywords.back() + ".\n";
        //std::cerr << _error;
        return false;
      }
    }
  }

  if (!_required_multiword.empty() && _multiwords[_required_multiword].empty()) {
    _error = _required_multiword + " is required.\n";
    //std::cerr << _error;
    return false;
  }
  if (!_required_keywords.empty()) {
    for (auto& key : _required_keywords) {
      if (_keywords[key].empty()) {
        _error = key + " is required.\n";
        //std::cerr << _error;
        return false;
      }
    }
  }
  return true;
}
//-----------------------------------------------------------------------------
bool Arguments::isOption(std::string key) const
{
  std::transform(key.begin(), key.end(), key.begin(), ::tolower);
  return _options.find(key) != _options.end();
}
//-----------------------------------------------------------------------------
bool Arguments::isKeyword(std::string key) const
{
  std::transform(key.begin(), key.end(), key.begin(), ::tolower);
  return _keywords.find(key) != _keywords.end();
}
//-----------------------------------------------------------------------------
bool Arguments::isMultiWord(std::string key) const
{
  std::transform(key.begin(), key.end(), key.begin(), ::tolower);
  return _multiwords.find(key) != _multiwords.end();
}
//-----------------------------------------------------------------------------
void Arguments::set_required_keywords(std::vector<std::string> keys)
{
  for (auto& key : keys) {
    std::transform(key.begin(), key.end(), key.begin(), ::tolower);
  }
  _required_keywords = keys;
}
//-----------------------------------------------------------------------------
auto Arguments::required_keywords() const -> std::vector<std::string>
{
  assert(!_required_keywords.empty());
  return _required_keywords;
}
//-----------------------------------------------------------------------------
void Arguments::set_required_multiword(std::string key)
{
  std::transform(key.begin(), key.end(), key.begin(), ::tolower);
  _required_multiword = key;
}
//-----------------------------------------------------------------------------
std::string Arguments::required_multiword() const
{
  return _required_multiword;
}
//-----------------------------------------------------------------------------
bool Arguments::exists(const std::string& key) const
{
  return isOption(key) || isKeyword(key) || isMultiWord(key);
}
//-----------------------------------------------------------------------------
auto Arguments::Option(std::string key) const -> OptionValue
{
  std::transform(key.begin(), key.end(), key.begin(), ::tolower);
  assert(_options.find(key) != _options.end());
  try {
    return _options.at(key);
  } catch (const std::out_of_range&) {
    return false;
  }
}
//-----------------------------------------------------------------------------
auto Arguments::Keyword(std::string key) const -> KeywordValue
{
  std::transform(key.begin(), key.end(), key.begin(), ::tolower);
  assert(_keywords.find(key) != _keywords.end());
  try {
    return _keywords.at(key);
  } catch (const std::out_of_range&) {
    return "";
  }
}
//-----------------------------------------------------------------------------
auto Arguments::MultiWord(std::string key) const -> MultiwordValue
{
  std::transform(key.begin(), key.end(), key.begin(), ::tolower);
  assert(_multiwords.find(key) != _multiwords.end());
  try {
    return _multiwords.at(key);
  } catch (const std::out_of_range&) {
    return {};
  }
}
//-----------------------------------------------------------------------------
bool Arguments::KeywordFound(std::string key) const
{
  std::transform(key.begin(), key.end(), key.begin(), ::tolower);
  try {
    return !_keywords.at(key).empty();
  } catch (const std::out_of_range&) {
    return false;
  }
}
//-----------------------------------------------------------------------------
bool Arguments::MultiWordFound(std::string key) const
{
  std::transform(key.begin(), key.end(), key.begin(), ::tolower);
  try {
    return !_multiwords.at(key).empty();
  } catch (const std::out_of_range&) {
    return false;
  }
}
//-----------------------------------------------------------------------------
std::string Arguments::error_msg() const
{
  return _error;
}
//-----------------------------------------------------------------------------
std::string Arguments::usuage_string() const
{
  std::stringstream ss;
  bool hasFlags = false;

  ss << "Usage " << _self_name << " [";
  for (auto& option : _options) {
    if (1 < option.first.size()) {
      ss << " " << option.first << ",";
    } else {
      hasFlags = true;
    }
  }
  ss.seekp(-1, std::ios_base::end);
  ss << "]\n\t";

  auto count = 0;
  for (auto& keyword : _keywords) {
    if (keyword.first.size() == 1) {
      //I'm assuming your short flags are proceeding your long flags.
      //In the future we will accept flag names of the form V,VERSION or J,THREADS which will link short flags to full versions
      continue;
    } else {
      ss << "[" << keyword.first << " VAL]" << ((++count % 4 == 0) ? "\n\t" : " ");
    }
  }
  if (count % 4 != 0) {
    ss.seekp(-1, std::ios_base::end);
    ss << "\n\t";
  }

  count = 0;
  for (auto& multiword : _multiwords) {
    ss << "[" << multiword.first << " VAL1 [VAL2]...]" << ((++count % 3 == 0) ? "\n\t" : " ");
  }
  if (count % 3 != 0) {
    ss.seekp(-1, std::ios_base::end);
    ss << "\n";
  }

  ss << "\n\n";
  if (hasFlags) {
    ss << "Flags: \n";
    for (auto& option : _options) {
      if (1 == option.first.size()) {
        ss << "\t-" << option.first << " : Purpose Description Goes Here \n";
        //TODO  Convert Pairs to Struct which fields for help and usuage message aiders
      }
    }
  }
  ss << std::endl;
  return ss.str();
}
}
