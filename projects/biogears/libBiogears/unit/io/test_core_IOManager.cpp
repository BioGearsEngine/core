#include <cmath>
#include <stdlib.h>
#include <thread>

#include <gtest/gtest.h>

#include <biogears/filesystem/path.h>
#include <biogears/io/io-manager.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <ShlObj.h>
#include <shellapi.h>
#else
#include <dirent.h>
#include <unistd.h>
#endif
#ifdef DISABLE_BIOGEARS_IOManager_TEST
#define TEST_FIXTURE_NAME DISABLED_IOManager_Fixture
#else
#define TEST_FIXTURE_NAME IOManager_Fixture
#endif

// The fixture for testing class Foo.
class TEST_FIXTURE_NAME : public ::testing::Test {
protected:
  // You can do set-up work for each test here.
  TEST_FIXTURE_NAME() = default;

  // You can do clean-up work that doesn't throw exceptions here.
  virtual ~TEST_FIXTURE_NAME() = default;

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  // Code here will be called immediately after the constructor (right
  // before each test).
  virtual void SetUp() override;

  // Code here will be called immediately after each test (right
  // before the destructor).
  virtual void TearDown() override;

  char const* m_prefix = "test_core_IOManager\0\0";
  char const* m_data_prefix = "test_core_IOManager/data";
  char const* m_schema_prefix = "test_core_IOManager/schema";
  char const* m_install_prefix = "test_core_IOManager/install";
  char const* m_cwd_prefix = "test_core_IOManager/cwd";
};

void TEST_FIXTURE_NAME::SetUp()
{
  biogears::filesystem::create_directory(m_prefix);
}

void TEST_FIXTURE_NAME::TearDown()
{
#if defined(_WIN32)
  SHFILEOPSTRUCT shfo = {
    NULL,
    FO_DELETE,
    m_prefix,
    NULL,
    FOF_SILENT | FOF_NOERRORUI | FOF_NOCONFIRMATION,
    FALSE,
    NULL,
    NULL
  };

  SHFileOperation(&shfo);
#else
  auto rmrf = [](const char* dirname) {
    DIR* dir;
    struct dirent* entry;
    char path[PATH_MAX];

    if (path == NULL) {
      fprintf(stderr, "Out of memory error\n");
      return 0;
    }
    dir = opendir(dirname);
    if (dir == NULL) {
      perror("Error opendir()");
      return 0;
    }

    while ((entry = readdir(dir)) != NULL) {
      if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
        snprintf(path, (size_t)PATH_MAX, "%s/%s", dirname, entry->d_name);
        if (entry->d_type == DT_DIR) {
          removedirectoryrecursively(path);
        }
        printf("(not really) Deleting: %s\n", path);
      }
    }
    closedir(dir);
    printf("(not really) Deleting: %s\n", dirname);
    return 1;
  };
  rmrf(prefix);

#endif
}

#if defined(_WIN32)
void setenv(char const* name, char const* value)
{
  _putenv_s(name, value);
}

#endif
TEST_F(TEST_FIXTURE_NAME, FallbackOverride)
{

  setenv("BIOGEARS_DATA_ROOT", m_data_prefix);
  setenv("BIOGEARS_SCHEMA_ROOT", m_schema_prefix);

  biogears::IOManager iom { m_cwd_prefix };

  EXPECT_EQ(iom.GetBioGearsWorkingDirectory(), m_cwd_prefix);
  EXPECT_EQ(iom.GetBioGearsSchemaRootDirectory(), m_schema_prefix);
  EXPECT_EQ(iom.GetBioGearsDataRootDirectory(), m_data_prefix);
}

TEST_F(TEST_FIXTURE_NAME, DirecotryOverride)
{
  biogears::IOManager iom { m_cwd_prefix };

  iom.SetConfigDirectory("unittest/configs/");
  iom.SetEcgDirectory("unittest/ecg/");
  iom.SetEnvironmentsDirectory("unittest/environments/");
  iom.SetNutritionDirectory("unittest/nutrition/");
  iom.SetOverrideDirectory("unittest/override/");
  iom.SetPatientsDirectory("unittest/patients/");
  iom.SetStatesDirectory("unittest/states/");
  iom.SetSubstancesDirectory("unittest/substances/");
  iom.SetScenariosDirectory("unittest/scenarios/");
  iom.SetLogDirectory("unittest/logs/");
  iom.SetResultsDirectory("unittest/results/");

  EXPECT_EQ("unittest/configs/", iom.GetConfigDirectory());
  EXPECT_EQ("unittest/ecg/", iom.GetEcgDirectory());
  EXPECT_EQ("unittest/environments/", iom.GetEnvironmentsDirectory());
  EXPECT_EQ("unittest/nutrition/", iom.GetNutritionDirectory());
  EXPECT_EQ("unittest/override/", iom.GetOverrideDirectory());
  EXPECT_EQ("unittest/patients/", iom.GetPatientsDirectory());
  EXPECT_EQ("unittest/states/", iom.GetStatesDirectory());
  EXPECT_EQ("unittest/substances/", iom.GetSubstancesDirectory());
  EXPECT_EQ("unittest/scenarios/", iom.GetScenariosDirectory());
  EXPECT_EQ("unittest/logs/", iom.GetLogDirectory());
  EXPECT_EQ("unittest/results/", iom.GetResultsDirectory());
}

#if defined(BIOGEARS_IO_PRESENT)
TEST_F(TEST_FIXTURE_NAME, CreateRuntimeDirectory)
{
  using namespace biogears::filesystem;

  //NOTE: These test some code in path.cpp
  std::string null_t_test;
  null_t_test += '\0';
  assert(null_t_test[0] == '\0');
  assert(null_t_test[1] == '\0');
  assert(null_t_test.c_str()[0] == '\0');
  assert(null_t_test.c_str()[1] == '\0');

  biogears::IOManager iom { m_cwd_prefix };
  EXPECT_TRUE(iom.generate_runtime_directory(m_cwd_prefix));
  auto bgc = iom.find_resource_file("BioGearsConfiguration.xml");
  auto expected = biogears::filesystem::path(m_cwd_prefix) / "BioGearsConfiguration.xml";
  EXPECT_EQ(bgc, expected.ToString());
}
#endif

#if defined(BIOGEARS_IO_PRESENT)
#include <biogears/io/directories/config.h>
TEST_F(TEST_FIXTURE_NAME, DoesEmbeddedFileExistConfig)
{
  using namespace biogears;

  biogears::IOManager iom { m_cwd_prefix };
  auto file_list = io::list_config_files();
  for (auto ii = io::config_file_count(); ii != 0; --ii) {
    EXPECT_TRUE(iom.does_embedded_file_exist(file_list[ii - 1]));
  }
}
#endif

#if defined(BIOGEARS_IO_PRESENT)
#include <biogears/io/directories/ecg.h>
TEST_F(TEST_FIXTURE_NAME, DoesEmbeddedFileExistECG)
{
  using namespace biogears;

  biogears::IOManager iom { m_cwd_prefix };
  auto file_list = io::list_ecg_files();
  for (auto ii = io::ecg_file_count(); ii != 0; --ii) {
    EXPECT_TRUE(iom.does_embedded_file_exist(file_list[ii - 1]));
  }
}
#endif

#if defined(BIOGEARS_IO_PRESENT)
#include <biogears/io/directories/environments.h>
TEST_F(TEST_FIXTURE_NAME, DoesEmbeddedFileExistEnvironments)
{
  using namespace biogears;

  biogears::IOManager iom { m_cwd_prefix };
  auto file_list = io::list_environments_files();
  for (auto ii = io::environments_file_count(); ii != 0; --ii) {
    EXPECT_TRUE(iom.does_embedded_file_exist(file_list[ii - 1]));
  }
}
#endif

#if defined(BIOGEARS_IO_PRESENT)
#include <biogears/io/directories/nutrition.h>
TEST_F(TEST_FIXTURE_NAME, DoesEmbeddedFileExistNutrition)
{
  using namespace biogears;

  biogears::IOManager iom { m_cwd_prefix };
  auto file_list = io::list_nutrition_files();
  for (auto ii = io::nutrition_file_count(); ii != 0; --ii) {
    EXPECT_TRUE(iom.does_embedded_file_exist(file_list[ii - 1]));
  }
}
#endif

#if defined(BIOGEARS_IO_PRESENT)
#include <biogears/io/directories/override.h>
TEST_F(TEST_FIXTURE_NAME, DoesEmbeddedFileExistOverride)
{
  using namespace biogears;

  biogears::IOManager iom { m_cwd_prefix };
  auto file_list = io::list_override_files();
  for (auto ii = io::override_file_count(); ii != 0; --ii) {
    EXPECT_TRUE(iom.does_embedded_file_exist(file_list[ii - 1]));
  }
}
#endif

#if defined(BIOGEARS_IO_PRESENT)
#include <biogears/io/directories/patients.h>
TEST_F(TEST_FIXTURE_NAME, DoesEmbeddedFileExistPatients)
{
  using namespace biogears;

  biogears::IOManager iom { m_cwd_prefix };
  auto file_list = io::list_patients_files();
  for (auto ii = io::patients_file_count(); ii != 0; --ii) {
    EXPECT_TRUE(iom.does_embedded_file_exist(file_list[ii - 1]));
  }
}
#endif

#if defined(BIOGEARS_IO_PRESENT)
#include <biogears/io/directories/substances.h>
TEST_F(TEST_FIXTURE_NAME, DoesEmbeddedFileExistSubstances)
{
  using namespace biogears;

  biogears::IOManager iom { m_cwd_prefix };
  auto file_list = io::list_substances_files();
  for (auto ii = io::substances_file_count(); ii != 0; --ii) {
    EXPECT_TRUE(iom.does_embedded_file_exist(file_list[ii - 1]));
  }
}
#endif

#if defined(BIOGEARS_IO_PRESENT)
#include <biogears/io/directories/xsd.h>
TEST_F(TEST_FIXTURE_NAME, DoesEmbeddedFileExistSchema)
{
  using namespace biogears;

  biogears::IOManager iom { m_cwd_prefix };
  auto file_list = io::list_xsd_files();
  for (auto ii = io::xsd_file_count(); ii != 0; --ii) {
    EXPECT_TRUE(iom.does_embedded_file_exist(file_list[ii - 1]));
  }
}
#endif

#if defined(BIOGEARS_IO_PRESENT)
TEST_F(TEST_FIXTURE_NAME, FindResourceFile)
{
  using namespace biogears;

  filesystem::path working_dir { m_cwd_prefix };

  biogears::IOManager iom { m_cwd_prefix };
  EXPECT_TRUE(iom.generate_runtime_directory(m_cwd_prefix));

  auto config = io::list_config_files();
  auto ecg = io::list_ecg_files();
  auto environments = io::list_environments_files();
  auto nutrition = io::list_nutrition_files();
  auto override = io::list_override_files();
  auto patients = io::list_patients_files();
  auto substances = io::list_substances_files();
  auto schemas = io::list_xsd_files();

  filesystem::path test_path = working_dir / iom.GetConfigDirectory() / config[0];
  std::string test_path_str = iom.GetConfigDirectory() + "/" + config[0];
  EXPECT_EQ(test_path, iom.FindConfigFile(config[0]));
  EXPECT_EQ(test_path, iom.find_resource_file(test_path_str.c_str()));

  test_path = working_dir / iom.GetEcgDirectory() / ecg[0];
  test_path_str = iom.GetEcgDirectory() + "/" + ecg[0];
  EXPECT_EQ(test_path, iom.FindEcgFile(ecg[0]));
  EXPECT_EQ(test_path, iom.find_resource_file(test_path_str.c_str()));

  test_path = working_dir / iom.GetEnvironmentsDirectory() / environments[0];
  test_path_str = iom.GetEnvironmentsDirectory() + "/" + environments[0];
  EXPECT_EQ(test_path, iom.FindEnvironmentFile(environments[0]));
  EXPECT_EQ(test_path, iom.find_resource_file(test_path_str.c_str()));

  test_path = working_dir / iom.GetNutritionDirectory() / nutrition[0];
  test_path_str = iom.GetNutritionDirectory() + "/" + nutrition[0];
  EXPECT_EQ(test_path, iom.FindNutritionFile(nutrition[0]));
  EXPECT_EQ(test_path, iom.find_resource_file(test_path_str.c_str()));

  test_path = working_dir / iom.GetOverrideDirectory() / override[0];
  test_path_str = iom.GetOverrideDirectory() + "/" + override[0];
  EXPECT_EQ(test_path, iom.FindOverrideFile(override[0]));
  EXPECT_EQ(test_path, iom.find_resource_file(test_path_str.c_str()));

  test_path = working_dir / iom.GetPatientsDirectory() / patients[0];
  test_path_str = iom.GetPatientsDirectory() + "/" + patients[0];
  EXPECT_EQ(test_path, iom.FindPatientFile(patients[0]));
  EXPECT_EQ(test_path, iom.find_resource_file(test_path_str.c_str()));

  test_path = working_dir / iom.GetSubstancesDirectory() / substances[0];
  test_path_str = iom.GetSubstancesDirectory() + "/" + substances[0];
  EXPECT_EQ(test_path, iom.FindSubstanceFile(substances[0]));
  EXPECT_EQ(test_path, iom.find_resource_file(test_path_str.c_str()));

  test_path = working_dir / iom.GetSchemasDirectory() / schemas[0];
  test_path_str = iom.GetSchemasDirectory() + "/" + schemas[0];
  EXPECT_EQ(test_path, iom.find_resource_file(test_path_str.c_str()));
}
#endif

#if defined(BIOGEARS_IO_PRESENT)
TEST_F(TEST_FIXTURE_NAME, SHA1_Calculations)
{
  using namespace biogears;

  biogears::IOManager iom { m_cwd_prefix };
  filesystem::path cwd_path = m_cwd_prefix;

  EXPECT_TRUE(iom.generate_runtime_directory(m_cwd_prefix));

  auto config = io::list_config_files();
  auto ecg = io::list_ecg_files();
  auto environments = io::list_environments_files();
  auto nutrition = io::list_nutrition_files();
  auto override = io::list_override_files();
  auto patients = io::list_patients_files();
  auto substances = io::list_substances_files();
  auto xsd = io::list_xsd_files();

  const char* embedded_path = config[0];
  filesystem::path physical_file_path = cwd_path / iom.GetConfigDirectory() / embedded_path;
  EXPECT_STREQ(iom.calculate_sha1(physical_file_path.c_str()).c_str(),
               iom.get_expected_sha1(embedded_path));
  size_t embedded_data_size;
  const char* embedded_data = io::get_embedded_config_file(embedded_path, embedded_data_size);
  EXPECT_STREQ(iom.calculate_sha1(embedded_data, embedded_data_size).c_str(),
               iom.get_expected_sha1(embedded_path));

  embedded_path = ecg[0];
  physical_file_path = cwd_path / iom.GetEcgDirectory() / embedded_path;
  EXPECT_STREQ(iom.calculate_sha1(physical_file_path.c_str()).c_str(),
               iom.get_expected_sha1(embedded_path));
  embedded_data = io::get_embedded_ecg_file(embedded_path, embedded_data_size);
  EXPECT_STREQ(iom.calculate_sha1(embedded_data, embedded_data_size).c_str(),
               iom.get_expected_sha1(embedded_path));

  embedded_path = environments[0];
  physical_file_path = cwd_path / iom.GetEnvironmentsDirectory() / embedded_path;
  EXPECT_STREQ(iom.calculate_sha1(physical_file_path.c_str()).c_str(),
               iom.get_expected_sha1(embedded_path));
  embedded_data = io::get_embedded_environments_file(embedded_path, embedded_data_size);
  EXPECT_STREQ(iom.calculate_sha1(embedded_data, embedded_data_size).c_str(),
               iom.get_expected_sha1(embedded_path));

  embedded_path = nutrition[0];
  physical_file_path = cwd_path / iom.GetNutritionDirectory() / embedded_path;
  EXPECT_STREQ(iom.calculate_sha1(physical_file_path.c_str()).c_str(),
               iom.get_expected_sha1(embedded_path));
  embedded_data = io::get_embedded_nutrition_file(embedded_path, embedded_data_size);
  EXPECT_STREQ(iom.calculate_sha1(embedded_data, embedded_data_size).c_str(),
               iom.get_expected_sha1(embedded_path));

  embedded_path = override[0];
  physical_file_path = cwd_path / iom.GetOverrideDirectory() / embedded_path;
  EXPECT_STREQ(iom.calculate_sha1(physical_file_path.c_str()).c_str(),
               iom.get_expected_sha1(embedded_path));
  embedded_data = io::get_embedded_override_file(embedded_path, embedded_data_size);
  EXPECT_STREQ(iom.calculate_sha1(embedded_data, embedded_data_size).c_str(),
               iom.get_expected_sha1(embedded_path));

  embedded_path = patients[0];
  physical_file_path = cwd_path / iom.GetPatientsDirectory() / embedded_path;
  EXPECT_STREQ(iom.calculate_sha1(physical_file_path.c_str()).c_str(),
               iom.get_expected_sha1(embedded_path));
  embedded_data = io::get_embedded_patients_file(embedded_path, embedded_data_size);
  EXPECT_STREQ(iom.calculate_sha1(embedded_data, embedded_data_size).c_str(),
               iom.get_expected_sha1(embedded_path));

  embedded_path = substances[0];
  physical_file_path = cwd_path / iom.GetSubstancesDirectory() / embedded_path;
  EXPECT_STREQ(iom.calculate_sha1(physical_file_path.c_str()).c_str(),
               iom.get_expected_sha1(embedded_path));
  embedded_data = io::get_embedded_substances_file(embedded_path, embedded_data_size);
  EXPECT_STREQ(iom.calculate_sha1(embedded_data, embedded_data_size).c_str(),
               iom.get_expected_sha1(embedded_path));

  embedded_path = xsd[0];
  physical_file_path = cwd_path / iom.GetSchemasDirectory() / embedded_path;
  EXPECT_STREQ(iom.calculate_sha1(physical_file_path.c_str()).c_str(),
               iom.get_expected_sha1(embedded_path));
  embedded_data = io::get_embedded_xsd_file(embedded_path, embedded_data_size);
  EXPECT_STREQ(iom.calculate_sha1(embedded_data, embedded_data_size).c_str(),
               iom.get_expected_sha1(embedded_path));
}
#endif
//TEST_F(TEST_FIXTURE_NAME,GetEmbeddedResourceFileXSD)
//{
//  biogears::IOManager iom { m_cwd_prefix };
//  size_t embeded_size;
//  std::string s(iom.get_embedded_resource_file("xsd/BioGearsDataModel.xsd", embeded_size));
//  EXPECT_EQ(embeded_size, s.size());
//  if (s.size() > 1) {
//    EXPECT_EQ(s[0],'<');
//    EXPECT_LE(100,s.size());
//  }
//}
//
//TEST_F(TEST_FIXTURE_NAME, GetEmbeddedResourceFileConfig)
//{
//
//  biogears::IOManager iom { m_cwd_prefix };
//  size_t embeded_size;
//  std::string s(iom.get_embedded_resource_file("xsd/BioGearsDataModel.xsd", embeded_size));
//  EXPECT_EQ(embeded_size, s.size());
//  if (s.size() > 1) {
//    EXPECT_EQ(s[0], '<');
//    EXPECT_LE(100, s.size());
//  }
//}
//
//TEST_F(TEST_FIXTURE_NAME, GetEmbeddedResourceFileECG)
//{
//
//  biogears::IOManager iom { m_cwd_prefix };
//  size_t embeded_size;
//  std::string s(iom.get_embedded_resource_file("config/DynamicStabilization.xml", embeded_size));
//  EXPECT_EQ(embeded_size, s.size());
//  if (s.size() > 1) {
//    EXPECT_EQ(s[0], '<');
//    EXPECT_LE(100, s.size());
//  }
//}
//
//TEST_F(TEST_FIXTURE_NAME, GetEmbeddedResourceFileEnvironments)
//{
//
//  biogears::IOManager iom { m_cwd_prefix };
//  size_t embeded_size;
//  std::string s(iom.get_embedded_resource_file("environments/AnchorageDecember.xml", embeded_size));
//  EXPECT_EQ(embeded_size, s.size());
//  if (s.size() > 1) {
//    EXPECT_EQ(s[0], '<');
//    EXPECT_LE(100, s.size());
//  }
//}
//
//TEST_F(TEST_FIXTURE_NAME, GetEmbeddedResourceFileNutrition)
//{
//
//  biogears::IOManager iom { m_cwd_prefix };
//  size_t embeded_size;
//  std::string s(iom.get_embedded_resource_file("nutrition/NoMacros.xml", embeded_size));
//  EXPECT_EQ(embeded_size, s.size());
//  if (s.size() > 1) {
//    EXPECT_EQ(s[0], '<');
//    EXPECT_LE(100, s.size());
//  }
//}
//
//TEST_F(TEST_FIXTURE_NAME, GetEmbeddedResourceFileOverride)
//{
//
//  biogears::IOManager iom { m_cwd_prefix };
//  size_t embeded_size;
//  std::string s(iom.get_embedded_resource_file("override/BioGearsOverride.xml", embeded_size));
//  EXPECT_EQ(embeded_size, s.size());
//  if (s.size() > 1) {
//    EXPECT_EQ(s[0], '<');
//    EXPECT_LE(100, s.size());
//  }
//}
//
//TEST_F(TEST_FIXTURE_NAME, GetEmbeddedResourceFilePatients)
//{
//
//  biogears::IOManager iom { m_cwd_prefix };
//  size_t embeded_size;
//  std::string s(iom.get_embedded_resource_file("patients/Bradycardic.xml", embeded_size));
//  EXPECT_EQ(embeded_size, s.size());
//  if (s.size() > 1) {
//    EXPECT_EQ(s[0], '<');
//    EXPECT_LE(100, s.size());
//  }
//}
//
//TEST_F(TEST_FIXTURE_NAME, GetEmbeddedResourceFileSubstances)
//{
//
//  biogears::IOManager iom { m_cwd_prefix };
//  size_t embeded_size;
//  std::string s(iom.get_embedded_resource_file("substances/Acetaminophen.xml",embeded_size));
//  EXPECT_EQ(embeded_size, s.size());
//  if (s.size() > 1) {
//    EXPECT_EQ(s[0], '<');
//    EXPECT_LE(100, s.size());
//  }
//}
//
//TEST_F(TEST_FIXTURE_NAME,GetDirectoryCount)
//{
//  biogears::IOManager iom { m_cwd_prefix };
//  //EXPECT_EQ(150,iom.get_directory_count());
//}
//
//TEST_F(TEST_FIXTURE_NAME, GetDirectoryList)
//{
//  biogears::IOManager iom { m_cwd_prefix };
// // char const** directoryList = iom.get_directory_list();
// // int i = 0;
// // for (;directoryList[i] != nullptr;++i) {
//  //const char* ptr = directoryList[i];
// // }
// // EXPECT_GE(i,1);
//}
