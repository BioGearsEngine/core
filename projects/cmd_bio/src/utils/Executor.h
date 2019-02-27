#ifndef CMD_BIO_UTILS_EXECUTOR_H
#define CMD_BIO_UTILS_EXECUTOR_H

#include <ostream>
#include <string>
#include <vector>

namespace biogears {
enum class EDriver { Undefined,
                     BGEUnitTestDriver,
                     CDMUnitTestDriver,
                     ScenarioTestDriver };
enum class EPlotter { Undefined,
                      ActionEventPlotter,
                      MultiPlotter };
enum class EPlotStyle { FastPlot //Plot every nth data point (hardcoded)
                        ,
                        FullPlot //Plot all data points
                        ,
                        FastPlotErrors //Only plot failures and only every nth data point (hardcoded)
                        ,
                        FullPlotErrors //Plot all data points only for failing data
                        ,
                        MemoryFastPlot //Do a fast plot, but manage memory better, slower plotting, but can handle larger files
};
class Executor {
public:
  Executor();
  Executor(std::string nam, EDriver driver);
  Executor(const Executor&) = default;
  Executor(Executor&&) = default;

  Executor& operator=(const Executor&) = default;
  Executor& operator=(Executor&&) = default;

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
  EPlotStyle plot_style;
  bool no_compare;
  std::string name;
  std::string baselines;
  std::string computed;
  std::string group;
  std::vector<std::string> results;
};

std::ostream& operator<<(std::ostream&, const EDriver&);
std::ostream& operator<<(std::ostream&, const EPlotter&);
std::ostream& operator<<(std::ostream&, const EPlotStyle&);
} //namespace biogears

#endif //CMD_BIO_UTILS_EXECUTOR_H