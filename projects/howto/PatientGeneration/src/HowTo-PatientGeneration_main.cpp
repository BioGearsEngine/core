#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <thread>

#include "Arguments.h"
#include "HowTo-PatientGeneration.h"
#include "HowToTracker.h"

#include <biogears/string/manipulation.h>
#include <biogears/threading/thread_pool.h>

void print_help()
{

  std::cout << "Usage howto-patientgeneration\n"
               "[threads N] [duration N] [patient FILE] trials INPUTS [INPUTS...]"
               "[config [FILE]]\n\n";

  std::cout << "Flags: \n";
  std::cout << "j : Thread control -j N\n";
  std::cout << "d : Duration control N number of hours total simulation will take\n";
  std::cout << "p : Patient file to be used in trials\n";
  std::cout << "h : Print this message\n";
  std::cout << "\n\n";
  std::cout << "INPUTS = [severity, mic_g_Per_l,  apply_at_m,  application_interval_m]\n";
  std::cout << "severity = {0=Low, 1=Moderate, 2=Severe}\n";
  std::cout << "mic_g_Per_l = Minimal Inhibitory Concentration for the infections substance\n";
  std::cout << "apply_at_m = n number of minutes till the first application of antibiotics\n";
  std::cout << "application_interval_m = N Number of minutes following an antibiotic application until it is reapplied\n";

  std::cout << std::endl;
  exit(0);
}

using csv = std::vector<std::vector<std::string>>;
using csv_row = std::vector<std::string>;

std::string printVector(std::vector<std::string> input)
{
  std::string rValue;
  for (auto& v : input) {
    rValue = rValue + v + ';';
  }
  if (rValue.back() == ';') {
    rValue.pop_back();
  }
  return rValue;
};

//-------------------------------------------------------------------------------
//!
//! \brief tokenizes a line of a csv document
//! \param line : csv line to be tokenized
//! \param row : a data structure representation of a csv row
//! \param input : the csv filestream (this is necessary for an edge case in which a newline character appears inbetween quotation marks)
//! \return bool rValue
//!
bool parse_line(std::string& line, csv_row& row, std::ifstream& input)
{
  bool rValue = true;

  auto token_start = line.begin();
  auto token_end = line.begin();
  std::string delimiters = R"(")";
  //Grab The Header
  while (token_end != line.end()) {
    switch (*token_end) {
    case ',':
      row.emplace_back(token_start, token_end);
      biogears::trim(row.back());
      token_start = token_end + 1;
      break;
    case '"':
      token_end = std::find_first_of(token_end + 1, line.end(), delimiters.begin(), delimiters.end());
      //Handle Returnlines in " which is insane but in the files
      if (token_end == line.end()) {
        do {
          std::string next_line;
          std::getline(input, next_line);
          size_t start = std::distance(line.begin(), token_start);
          size_t end = std::distance(line.begin(), token_end);

          line += next_line;
          token_start = line.begin() + start;
          token_end = line.begin() + end;
          token_end = std::find_first_of(token_end + 1, line.end(), delimiters.begin(), delimiters.end());
        } while (token_end == line.end() && input.good());
      }
      break;
    default:
      break;
    }
    ++token_end;
  }

  if (token_start < token_end) {
    row.emplace_back(biogears::trim(std::string(token_start, token_end)));
  } else if (token_start == token_end) {
  }
  return rValue;
}

//-------------------------------------------------------------------------------
//!
//! \brief  reads in the csv file line by line, then passes each line to parse_line
//! \return bool rValue true if no issues were encountered while parsing, false otherwise
//!
auto read_csv(std::string path, std::string filename) -> csv
{

  auto filePath = (path.empty()) ? filename : path + "/" + filename;
  std::ifstream input{ filePath };
  std::string line = "";
  std::string token = "";

  if (input.is_open()) {
    csv result;
    csv_row current_row;
    while (std::getline(input, line)) {
      if (parse_line(line, current_row, input)) {
        result.push_back(current_row);
      }
      current_row.clear();
    }
    return result;
  } else {
    throw std::runtime_error{ "Error opening given file " + filePath };
  }
}

//!
//! \brief Reads command line argument and executes corresponding operation
//! \param argc : Number of command line arguments
//! \param argv : Pointers to command line arguments
//! \return int 0 on success, other numbers on failure
//!
int main(int argc, char** argv)
{
  biogears::Arguments args(
    { "H", "HELP" } //Options
    ,
    { "J", "THREADS", "D", "DURATION", "P", "PATIENT", "CONFIG" } //Keywords
    ,
    { "TRIALS" } //MultiWords
  );

  unsigned int thread_count = std::thread::hardware_concurrency() - 1;
  unsigned int trial_duration = 6;
  std::string trial_patient = "states/StandardMale@0s.xml";
  if (!args.parse(argc, argv) || args.Option("HELP") || args.Option("H") || args.empty()) {
    print_help();
  }

  if (args.KeywordFound("THREADS")) {
    try {
      thread_count = std::stoi(args.Keyword("THREADS"));
    } catch (std::exception) {
      std::cerr << "Error: THREADS given but " << args.Keyword("THREADS") << " is not a valid Integer.\n";
      exit(1);
    }
  }
  if (args.KeywordFound("J")) {
    try {
      thread_count = std::stoi(args.Keyword("J"));
    } catch (std::exception) {
      std::cerr << "Error: J given but " << args.Keyword("J") << " is not a valid Integer.\n";
      exit(1);
    }
  }

  if (args.KeywordFound("D")) {
    try {
      trial_duration = std::stoi(args.Keyword("D"));
    } catch (std::exception) {
      std::cerr << "Error: D given but " << args.Keyword("D") << " is not a valid Integer.\n";
      exit(1);
    }
  }

  if (args.KeywordFound("DURATION")) {
    try {
      trial_duration = std::stoi(args.Keyword("DURATION"));
    } catch (std::exception) {
      std::cerr << "Error: DURATION given but " << args.Keyword("DURATION") << " is not a valid Integer.\n";
      exit(1);
    }
  }

  if (args.KeywordFound("P")) {
    try {
      trial_patient = args.Keyword("P");
    } catch (std::exception) {
      std::cerr << "Error: P given but no Patient file found\n";
      exit(1);
    }
  }

  if (args.KeywordFound("PATIENT")) {
    try {
      trial_patient = args.Keyword("PATIENT");
    } catch (std::exception) {
      std::cerr << "Error: Patient given but no Patient file found\n";
      exit(1);
    }
  }

  if (args.KeywordFound("CONFIG")) {
    std::string filename = args.Keyword("CONFIG");

    auto trials = read_csv("", filename);
    if (trials.size() > 2 && trials[0].size() > 0) {
      try {
        trial_duration = std::stoi(trials[0][0]);
      } catch (std::exception e) {
        std::cerr << "Error: trial_duration " << trials[0][0] << " is not a valid Integer.\n";
        exit(1);
      }

      if (trials[0].size() > 1) {
        try {
          thread_count = std::stoi(trials[0][1]);
        } catch (std::exception e) {
          std::cerr << "Error: thread_count " << trials[0][1] << " is not a valid Integer.\n";
          exit(1);
        }
      }

      if (trials[1].size() > 0) {
          trial_patient = trials[1][0];
      }

      trials.erase(trials.begin(), trials.begin() + 1);

      biogears::ThreadPool pool{ thread_count };
      auto channel = pool.get_source();

      auto count = 1; //Danger 1 Indexed
      std::stringstream ss;
      for (auto& params : trials) {
        if (params.size() >= 4) {
          try {
            int severity = std::stoi(params[0]);
            double mic = std::stod(params[1]);
            double apply_at = std::stod(params[2]);
            double application_interval = std::stod(params[3]);

            double duration = (params.size() > 4) ? std::stod(params[4])
                                                  : trial_duration;
            auto patient = (params.size() > 5) ? params[5]
                                               : trial_patient;

            ss.str("");
            ss << "Trial " << count;
            std::string name = ss.str();
            channel->insert([=]() { return HowToPatientGeneration(name, severity, mic, apply_at, application_interval, patient, duration); });
          } catch (std::exception) {
            std::cerr << "Error: Malformed input skipping trial " << count << " with paramaters " << printVector(params) << "invalid double conversion.\n";
            exit(1);
          }
        } else {
          std::cerr << "Warn: Skipping Malformed tria1 " << count << " with paramaters " << printVector(params) << "\n";
          exit(1);
        }
        ++count;
      }
      pool.start();
      while (!pool.stop_if_empty()) {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(16ms);
      }
      pool.join();
      exit(1);
    } else {
      std::cerr << "Error: Malformed config file given.\n";
      exit(1);
    }
  } else if (args.MultiWordFound("TRIALS")) {
    auto trials = args.MultiWord("TRIALS");
    biogears::ThreadPool pool{ thread_count };
    auto channel = pool.get_source();

    auto count = 1; //Danger 1 Indexed
    std::stringstream ss;
    for (auto& trial : trials) {
      auto params = biogears::string_split(trial, ",");
      if (params.size() >= 4) {
        try {
          int severity = std::stoi(params[0]);
          double mic = std::stod(params[1]);
          double apply_at = std::stod(params[2]);
          double application_interval = std::stod(params[3]);
          double duration = (params.size() > 4) ? std::stod(params[4])
                                                : trial_duration;
          auto patient = (params.size() > 5) ? params[5]
                                             : trial_patient;
          ss.str("");
          ss << "Trial " << count;
          std::string name = ss.str();
          channel->insert([=]() { return HowToPatientGeneration(name, severity, mic, apply_at, application_interval, patient, duration); });
        } catch (std::exception) {
          std::cerr << "Error: Malformed input skipping trial " << count << " with paramaters " << trial << "invalid double conversion.\n";
          exit(1);
        }
      } else {
        std::cerr << "Warn: Skipping malformed trial set " << trial;
      }
      ++count;
    }
    pool.start();
    while (!pool.stop_if_empty()) {
      using namespace std::chrono_literals;
      std::this_thread::sleep_for(16ms);
    }
    pool.join();
    exit(1);
  } else {
    print_help();
  }
}
