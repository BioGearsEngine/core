#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/engine/Controller/BioGearsEngine.h>


#include <memory>

namespace std {
template class std::unique_ptr<biogears::PhysiologyEngine>;
template class std::shared_ptr<biogears::PhysiologyEngine>;
}

namespace biogears {
std::unique_ptr<PhysiologyEngine> CreateBioGearsEngine(Logger* logger)
{
  return std::make_unique<BioGearsEngine>(logger);
}
//-------------------------------------------------------------------------------
std::unique_ptr<PhysiologyEngine> CreateBioGearsEngine(const std::string logfile)
{
  return std::make_unique<BioGearsEngine>(logfile);
}
//-------------------------------------------------------------------------------
std::unique_ptr<PhysiologyEngine> CreateBioGearsEngine(const char* logfile)
{
  return std::make_unique<BioGearsEngine>(logfile);
}
//-------------------------------------------------------------------------------
std::unique_ptr<biogears::PhysiologyEngine> CreateBioGearsEngine(const std::string working_dir, const std::string logfile)
{
  return std::make_unique<BioGearsEngine>(logfile, working_dir);
}
//-------------------------------------------------------------------------------
std::unique_ptr<biogears::PhysiologyEngine> CreateBioGearsEngine(const char* working_dir, const char* logfile)
{
  return std::make_unique<BioGearsEngine>(logfile, working_dir);
}
//-------------------------------------------------------------------------------
std::unique_ptr<biogears::PhysiologyEngine> CreateBioGearsEngine(const std::string working_dir, biogears::Logger* logger)
{
  return std::make_unique<BioGearsEngine>(logger, working_dir);
}
//-------------------------------------------------------------------------------
std::unique_ptr<biogears::PhysiologyEngine> CreateBioGearsEngine(const char* working_dir, biogears::Logger* logger)
{
  return std::make_unique<BioGearsEngine>(logger, working_dir);
}
}
