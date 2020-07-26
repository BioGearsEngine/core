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

#include "Driver.h"

#include <functional>
#include <iostream>

#include <boost/asio.hpp>
#include <boost/process.hpp>

#include <biogears/cdm/engine/PhysiologyEngineTrack.h>
#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/cdm/utils/DataTrack.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/engine/Controller/Scenario/BioGearsScenario.h>
#include <biogears/engine/Controller/Scenario/BioGearsScenarioExec.h>
#ifdef CMD_BIO_SUPPORT_CIRCUIT_TEST
#include <biogears/cdm/test/CommonDataModelTest.h>
#include <biogears/engine/test/BioGearsEngineTest.h>
#endif

#include <biogears/schema/cdm/Scenario.hxx>
#include <biogears/string/manipulation.h>
#include <xsd/cxx/tree/exceptions.hxx>

#include "../utils/Config.h"
#include "../utils/Executor.h"

void cout_loop(boost::process::async_pipe& p, boost::asio::mutable_buffer buf)
{
  p.async_read_some(buf, [&p, buf](std::error_code ec, size_t n) {
    std::cout.write(boost::asio::buffer_cast<char const*>(buf), n);
    if (!ec)
      cout_loop(p, buf);
  });
}

void cerr_loop(boost::process::async_pipe& p, boost::asio::mutable_buffer buf)
{
  p.async_read_some(buf, [&p, buf](std::error_code ec, size_t n) {
    std::cerr.write(boost::asio::buffer_cast<char const*>(buf), n);
    if (!ec)
      cerr_loop(p, buf);
  });
}

namespace biogears {
Driver::Driver(size_t thread_count)
  : _pool { thread_count }
{
}
//-----------------------------------------------------------------------------
Driver::~Driver()
{
  _pool.stop();
  _pool.join();
}
//-----------------------------------------------------------------------------
void Driver::configure(const Config& runs)
{
  _globals = runs;
  _globals.clear();
}
//-----------------------------------------------------------------------------
void Driver::queue(const Config& runs)
{
  for (auto& exec : runs) {
    switch (exec.Driver()) {
    case EDriver::Undefined:
      std::cerr << "Unable to queue Undefined Executor for " << exec.Name() << "\n";
      break;
    case EDriver::BGEUnitTestDriver:
      queue_BGEUnitTest(exec);
      break;
    case EDriver::CDMUnitTestDriver:
      queue_CDMUnitTest(exec);
      break;
    case EDriver::ScenarioTestDriver:
      queue_Scenario(exec);
      break;
    default:
      std::cerr << "Unsupported Driver type " << exec.Driver() << " please update your biogears libraries \n";
      break;
    }
  }
}
//-----------------------------------------------------------------------------
void Driver::run()
{
  _pool.start();
}
//-----------------------------------------------------------------------------
void Driver::stop()
{
  _pool.stop();
}
//-----------------------------------------------------------------------------
bool Driver::stop_if_empty()
{
  return _pool.stop_if_empty();
}
//-----------------------------------------------------------------------------
void Driver::stop_when_empty()
{
  while (!_pool.stop_if_empty()) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}
//-----------------------------------------------------------------------------
void Driver::join()
{
  _pool.join();
}
//-----------------------------------------------------------------------------
void Driver::queue_BGEUnitTest(Executor exec)
{
#ifdef CMD_BIO_SUPPORT_CIRCUIT_TEST
  _pool.queue_work(
    [=]() {
      BioGearsEngineTest* executor;
      try {
        executor = new BioGearsEngineTest;
      } catch (const std::exception& e) {
        std::cout << e.what() << "\n";
        return 1;
      }
      executor->RunTest(exec.Name(), exec.Computed());
      delete executor;
      return 0;
    });
#endif
}
//-----------------------------------------------------------------------------
void Driver::queue_CDMUnitTest(Executor exec)
{
#ifdef CMD_BIO_SUPPORT_CIRCUIT_TEST
  _pool.queue_work(
    [&]() {
      CommonDataModelTest* executor;
      try {
        executor = new CommonDataModelTest;
      } catch (const std::exception& e) {
        std::cout << e.what() << "\n";
        return 1;
      }
      executor->RunTest(exec.Name(), exec.Computed());
      delete executor;
      return 0;
    });
#endif
}
//-----------------------------------------------------------------------------
#pragma optimize("", off)

void Driver::queue_Scenario(Executor exec)
{
  enum class PatientType {
    FILE,
    STATE,
    INLINE
  };

  auto scenario_launch = [](Executor& ex, bool multi_patient_run) {
    Logger console_logger;
    try {
      console_logger.SetConsolesetConversionPattern("%d{%H:%M} [%p] %m%n");
      console_logger.FormatMessages(false);
    } catch (std::exception e) {
      std::cout << e.what();
    }
    try {
      using namespace boost::process;

      std::string options;

      options = "--quiet";

      if (ex.Name().size()) {
        auto name = biogears::trim(ex.Name());
        if (name.front() == '\'' && name.back() == '\'') {
          name = biogears::trim(name, "'");
        }
        if (name.front() == '\"' && name.back() == '\"') {
          name = biogears::trim(name, "\"");
        }
        options += biogears::asprintf(" --name \"%s\"", name.c_str());
      }

      if (ex.Driver() != EDriver::ScenarioTestDriver) {
        options += " --driver ";
        switch (ex.Driver()) {
        case EDriver::ScenarioTestDriver:
          options += "'ScenarioTestDriver'";
          break;
        case EDriver::BGEUnitTestDriver:
          options += "'BGEUnitTestDriver'";
          break;
        case EDriver::CDMUnitTestDriver:
          options += "'CDMUnitTestDriver'";
          break;
        }
      }

      if (ex.Patient().size()) {
        auto patient = biogears::trim(ex.Patient());
        if (patient.front() == '\'' && patient.back() == '\'') {
          patient = biogears::trim(patient, "'");
        }
        if (patient.front() == '\"' && patient.back() == '\"') {
          patient = biogears::trim(patient, "\"");
        }
        options += biogears::asprintf(" --patient \"%s\"", patient.c_str());
      }

      if (ex.State().size()) {
        auto state = biogears::trim(ex.State());
        if (state.front() == '\'' && state.back() == '\'') {
          state = biogears::trim(state, "'");
        }
        if (state.front() == '\"' && state.back() == '\"') {
          state = biogears::trim(state, "\"");
        }
        options += biogears::asprintf(" --state \"%s\"", state.c_str());
      }

      if (ex.Scenario().size()) {
        auto scenario = biogears::trim(ex.Scenario());
        if (scenario.front() == '\'' && scenario.back() == '\'') {
          scenario = biogears::trim(scenario, "'");
        }
        if (scenario.front() == '\"' && scenario.back() == '\"') {
          scenario = biogears::trim(scenario, "\"");
        }
        options += biogears::asprintf(" --scenario \"%s\"", scenario.c_str());
      }

      if (ex.Results().size()) {
        auto results = biogears::trim(ex.Results()[0]);
        if (results.front() == '\'' && results.back() == '\'') {
          results = biogears::trim(results, "'");
        }
        if (results.front() == '\"' && results.back() == '\"') {
          results = biogears::trim(results, "\"");
        }
        options += biogears::asprintf(" --results \"%s\"", results.c_str());
      }

      if (ex.Group().size()) {
        auto group = biogears::trim(ex.Group());
        if (group.front() == '\'' && group.back() == '\'') {
          group = biogears::trim(group, "'");
        }
        if (group.front() == '\"' && group.back() == '\"') {
          group = biogears::trim(group, "\"");
        }
        options += biogears::asprintf(" --group \"%s\"", group.c_str());
      }

      boost::asio::io_service svc;
      async_pipe out_stream(svc);
      async_pipe err_stream(svc);
      char out_buffer[128];
      char err_buffer[128];

      //child c("bg-scenario " + options, (std_out & std_err) > out_stream, svc);
      std::cout << "..\\outputs\\RelWithDebInfo\\bin\\bg-scenario " + options << std::endl;
      child c("bg-scenario " + options, std_out > out_stream, std_err > err_stream);

      std::function<void(void)> test = [&]() { test(); };
      cout_loop(out_stream, buffer(out_buffer));
      cerr_loop(err_stream, buffer(err_buffer));

      svc.run();
      c.wait();

      if (c.exit_code() != 0) {
        console_logger.Info(ex.Name() + " failed see log for details.\n");
        return;
      }
      console_logger.Info("Completed " + ex.Name() + "\n");
    } catch (...) {
      console_logger.Error("Failed " + ex.Name() + "\n");
    }
  };

  std::ifstream ifs { exec.Scenario() };
  if (!ifs.is_open()) {
    ifs.open("Scenarios/" + exec.Scenario());
    if (!ifs.is_open()) {
      std::cerr << "Failed to open Scenarios/" << exec.Scenario() << " terminating\n";
      return;
    }
  }

  std::unique_ptr<mil::tatrc::physiology::datamodel::ScenarioData> scenario;
  try {
    xml_schema::flags xml_flags;
    xml_schema::properties xml_properties;

    xml_properties.schema_location("uri:/mil/tatrc/physiology/datamodel", "xsd/BioGearsDataModel.xsd");
    scenario = mil::tatrc::physiology::datamodel::Scenario(ifs, xml_flags, xml_properties);
  } catch (std::runtime_error e) {
    std::cout << e.what() << "\n";
    return;
  } catch (xsd::cxx::tree::parsing<char> e) {
    std::cout << e << "\n";
    return;
  }
  // The Biogears Schema states that ScenarioData supports the following boot strap tags
  // 1. EngineStateFile -- Overrides PatientFile and Patient tag and skips initialization using the state file
  // 2. PatientFile -- External file which will be read in with a patient definition
  // 3. Patient -- Inline Patient definition similar to PatientFile
  // Currently only Patient File is supported by this function
  // TODO: Test for EngineState file and call an appropriate launcher
  // TODO: Test for Patient if PatientFile is not present and call the appropriate launcher
  if (scenario->EngineStateFile().present()) {
    exec.State(scenario->EngineStateFile().get());
    _pool.queue_work(std::bind(scenario_launch, std::move(exec), false));
    return;
  } else if (scenario->InitialParameters().present() && scenario->InitialParameters()->PatientFile().present()) {
    const auto patient_file = scenario->InitialParameters()->PatientFile().get();
    std::string nc_patient_file = patient_file;
    std::transform(nc_patient_file.begin(), nc_patient_file.end(), nc_patient_file.begin(), ::tolower);
    if ("all" == nc_patient_file) {
      auto patient_files = biogears::ListFiles("patients", R"(\.xml)");
      for (const std::string& patient_file : patient_files) {
        Executor patientEx { exec };
        patientEx.Patient(patient_file);
        _pool.queue_work(std::bind(scenario_launch, std::move(patientEx), true));
      }
    } else {
      exec.Patient(patient_file);
      _pool.queue_work(std::bind(scenario_launch, std::move(exec), false));
    }
  } else if (scenario->InitialParameters().present() && scenario->InitialParameters()->Patient().present()) {
    exec.Patient("");
    exec.State("");
    _pool.queue_work(std::bind(scenario_launch, std::move(exec), false));
  } else {
    std::cout << "Skipping " << exec.Name() << " no patient specified.\n";
    return;
  }
}

#pragma optimize("", on)

//-----------------------------------------------------------------------------
} // NAMESPACE
