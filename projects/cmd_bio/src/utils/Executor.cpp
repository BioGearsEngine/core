#include "Executor.h"

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
  , patient("patients/StandardMale.xml")
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
std::string Executor::Scenario() const { return scenario; }
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
Executor& Executor::Group(const std::string& g) &&
{
  group = g;
  return *this;
}
//-----------------------------------------------------------------------------
Executor& Executor::Patient(const std::string& p) &&
{
  patient = p;
  return *this;
}
//-----------------------------------------------------------------------------
Executor& Executor::Scenario(const std::string& s) &&
{
  scenario = s;
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
std::string Executor::Group(const std::string& g) &
{
  return group = g;
}
//-----------------------------------------------------------------------------
std::string Executor::Patient(const std::string& p) &
{
  return patient = p;
}
//-----------------------------------------------------------------------------
std::string Executor::Scenario(const std::string& s) &
{
  return scenario = s;
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