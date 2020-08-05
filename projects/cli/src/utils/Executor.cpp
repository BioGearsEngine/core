#include "Executor.h"

#include <algorithm>
#include <string>

namespace biogears {
//-----------------------------------------------------------------------------
Executor::Executor()
  : Executor("Undefined", EDriver::Undefined)
{
}
//-----------------------------------------------------------------------------
Executor::Executor(std::string n, EDriver d)
  : name(n)
  , driver(d)
  , plot_style(EPlotStyle::FastPlot)
  , no_compare(false)
  , group("Other")
  , patient("")
  , state("")
  , scenario("Scenarios/BasicStandard.xml")
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
std::string Executor::Group() const { return group; }
//-----------------------------------------------------------------------------
std::string Executor::Patient() const { return patient; }
//-----------------------------------------------------------------------------
std::string Executor::State() const { return state; }
//-----------------------------------------------------------------------------
std::string Executor::Scenario() const { return scenario; }
//-----------------------------------------------------------------------------
std::vector<std::string> Executor::Results() const { return results; }

//-----------------------------------------------------------------------------
void Executor::Name(const std::string& n)
{
  name = n;
}
//-----------------------------------------------------------------------------
void Executor::Driver(EDriver d)
{
  driver = d;
}
//-----------------------------------------------------------------------------
void Executor::PlotStyle(EPlotStyle p)
{
  plot_style = p;
}
//-----------------------------------------------------------------------------
void Executor::NoCompare(bool nc)
{
  no_compare = nc;
}
//-----------------------------------------------------------------------------
void Executor::Baselines(const std::string& b)
{
  baselines = b;
}
//-----------------------------------------------------------------------------
void Executor::Computed(const std::string& c)
{
  computed = c;
}
//-----------------------------------------------------------------------------
void Executor::Group(const std::string& g)
{
  group = g;
}
//-----------------------------------------------------------------------------
void Executor::Patient(const std::string& p)
{
  patient = p;
  state.clear();
}
//-----------------------------------------------------------------------------
void Executor::Scenario(const std::string& s)
{
  scenario = s;
}
//-----------------------------------------------------------------------------
void Executor::State(const std::string& s)
{
  state = s;
  patient.clear();
}
//-----------------------------------------------------------------------------
void Executor::Results(const std::vector<std::string>& r)
{
  results = r;
}

//-----------------------------------------------------------------------------
void Executor::push_back_results(const std::string& r) { results.push_back(r); }
//-----------------------------------------------------------------------------
void Executor::clear_results() { results.clear(); }
//-----------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& ostr, const Executor& executor)
{
  std::string results;
  const auto& rv = executor.Results();
  std::for_each(rv.begin(), rv.end(), [&](const std::string& piece) { results += piece + ", "; });
  if (!results.empty()) {
    results.substr(results.size() - 2);
  }
  ostr << "Executor " << executor.Name();
  ostr << ",\n\tDriver=" << executor.Driver();
  ostr << ",\n\tPlotStyle" << executor.PlotStyle();
  ostr << ",\n\tNoCompare=" << (executor.NoCompare() ? "True" : "False");
  if (!executor.Baselines().empty())
    ostr << ",\n\tBaselines=" << executor.Baselines();
  if (!executor.Computed().empty())
    ostr << ",\n\tComputed=" << executor.Computed();
  ostr << ",\n\tGroup=" << executor.Group();
  if (!executor.Patient().empty()) {
    ostr << ",\n\tPatient=" << executor.Patient();
  }
  if (!executor.State().empty()) {
    ostr << ",\n\tState=" << executor.State();
  }
  ostr << ",\n\tScenario=" << executor.Scenario();
  if (!executor.Results().empty()) {
    ostr << ",\n\tResults=" << results;
  }
  return ostr;
}
//-----------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& ostr, const EDriver& driver)
{
  switch (driver) {
  case EDriver::Undefined:
    ostr << "EDriver::Undefined";
    break;
  case EDriver::BGEUnitTestDriver:
    ostr << "EDriver::BGEUnitTestDriver";
    break;
  case EDriver::CDMUnitTestDriver:
    ostr << "EDriver::CDMUnitTestDriver";
    break;
  case EDriver::ScenarioTestDriver:
    ostr << "EDriver::ScenarioTestDriver";
    break;
  default:
    ostr << "EDriver::Unsupported_Value(" << static_cast<size_t>(driver) << ")";
    break;
  }
  return ostr;
}
//-----------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& ostr, const EPlotter& plotter)
{
  switch (plotter) {
  case EPlotter::Undefined:
    ostr << "EPlotter::Undefined";
    break;
  case EPlotter::ActionEventPlotter:
    ostr << "EPlotter::BGEUnitTestDriver";
    break;
  case EPlotter::MultiPlotter:
    ostr << "EPlotter::CDMUnitTestDriver";
    break;
  default:
    ostr << "EPlotter::Unsupported_Value(" << static_cast<size_t>(plotter) << ")";
    break;
  }
  return ostr;
}
//-----------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& ostr, const EPlotStyle& plot_style)
{
  switch (plot_style) {
  case EPlotStyle::FastPlot:
    ostr << "EPlotStyle::FastPlot";
    break;
  case EPlotStyle::FullPlot:
    ostr << "EPlotStyle::FullPlot";
    break;
  case EPlotStyle::FastPlotErrors:
    ostr << "EPlotStyle::FastPlotErrors";
    break;
  case EPlotStyle::FullPlotErrors:
    ostr << "EPlotStyle::FullPlotErrors";
    break;
  case EPlotStyle::MemoryFastPlot:
    ostr << "EPlotStyle::MemoryFastPlot";
    break;
  default:
    ostr << "EPlotStyle::Unsupported_Value(" << static_cast<size_t>(plot_style) << ")";
    break;
  }
  return ostr;
}
//-----------------------------------------------------------------------------
}
