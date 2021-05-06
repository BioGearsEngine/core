#include <thread>
#include <cmath>

#include <gtest/gtest.h>

//#include <biogears/io/io-manager.h>
#include <biogears/io/directories/xsd.h>



#ifdef DISABLE_BIOGEARS_BioGearsIO_TEST
#define TEST_FIXTURE_NAME DISABLED_BioGearsIO_Fixture
#else
#define TEST_FIXTURE_NAME BioGearsIO_Fixture
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
};

void TEST_FIXTURE_NAME::SetUp()
{
}

void TEST_FIXTURE_NAME::TearDown()
{
}

//TEST_F(TEST_FIXTURE_NAME,GenerateRuntimeDirectory)
//{
//	biogears::io::IOManager iom;
//	EXPECT_TRUE(iom.generate_runtime_directory("TestTXT.txt"));
//	//EXPECT_TRUE()
//}
//
//TEST_F(TEST_FIXTURE_NAME,DoesEmbeddedFileExistNOFILE)
//{
//	biogears::io::IOManager iom;
//    EXPECT_FALSE(iom.does_embedded_file_exist("XXNOTFILEXX"));
//}
//
//TEST_F(TEST_FIXTURE_NAME,DoesEmbeddedFileExistXSD)
//{
//	biogears::io::IOManager iom;
//	EXPECT_TRUE(iom.does_embedded_file_exist("xsd/BioGearsDataModel.xsd"));
//}
//
//TEST_F(TEST_FIXTURE_NAME, DoesEmbeddedFileExistConfig)
//{
//  biogears::io::IOManager iom;
//  EXPECT_TRUE(iom.does_embedded_file_exist("config/DynamicStabilization.xml"));
//}
//
//TEST_F(TEST_FIXTURE_NAME, DoesEmbeddedFileExistECG)
//{
//  biogears::io::IOManager iom;
//  EXPECT_TRUE(iom.does_embedded_file_exist("ecg/StandardECG.xml"));
//}
//
//TEST_F(TEST_FIXTURE_NAME, DoesEmbeddedFileExistEnvironments)
//{
//  biogears::io::IOManager iom;
//  EXPECT_TRUE(iom.does_embedded_file_exist("environments/AnchorageDecember.xml"));
//}
//
//TEST_F(TEST_FIXTURE_NAME, DoesEmbeddedFileExistNutrition)
//{
//  biogears::io::IOManager iom;
//  EXPECT_TRUE(iom.does_embedded_file_exist("nutrition/NoMacros.xml"));
//}
//
//TEST_F(TEST_FIXTURE_NAME, DoesEmbeddedFileExistOverride)
//{
//  biogears::io::IOManager iom;
//  EXPECT_TRUE(iom.does_embedded_file_exist("override/BioGearsOverride.xml"));
//}
//
//TEST_F(TEST_FIXTURE_NAME, DoesEmbeddedFileExistPatients)
//{
//  biogears::io::IOManager iom;
//  EXPECT_TRUE(iom.does_embedded_file_exist("patients/StandardMale.xml"));
//}
//
//TEST_F(TEST_FIXTURE_NAME, DoesEmbeddedFileExistSubstances)
//{
//  biogears::io::IOManager iom;
//  EXPECT_TRUE(iom.does_embedded_file_exist("substances/Acetaminophen.xml"));
//}
//
//TEST_F(TEST_FIXTURE_NAME,FindResourceFile) 
//{
//  biogears::io::IOManager iom;
//  char buf[10000];  
//  EXPECT_TRUE(0 < iom.find_resource_file("patients/StandardMale.xml",buf,10000));
//}

//TEST_F(TEST_FIXTURE_NAME,GetSha1XSD)
//{
//	biogears::io::IOManager iom; 
//	//38971f4027302e2a2758543a54eb0a21676a7bf4 sha1 of BioGearsDataModel.xsd as of 1/20/21, if the test is failing, maybe the file was changed
//	EXPECT_EQ(biogears::io::generate_file_sha1("xsd/BioGearsDataModel.xsd"),iom.get_expected_sha1("xsd/BioGearsDataModel.xsd"));
//}
//
//TEST_F(TEST_FIXTURE_NAME, GetSha1Config)
//{
//  biogears::io::IOManager iom; // this tests against the sha1 of DynamicStabilization.xml as of 1/22/21
//  EXPECT_EQ(biogears::io::generate_file_sha1("config/DynamicStabilization.xml"), iom.get_expected_sha1("config/DynamicStabilization.xml"));
//}
//
//TEST_F(TEST_FIXTURE_NAME, GetSha1ECG)
//{
//  biogears::io::IOManager iom; // this tests against the sha1 of the file as of 1/22/21
//  EXPECT_EQ(biogears::io::generate_file_sha1("ecg/StandardECG.xml"), iom.get_expected_sha1("ecg/StandardECG.xml"));
//}
//
//TEST_F(TEST_FIXTURE_NAME, GetSha1Environments)
//{
//  biogears::io::IOManager iom; // this tests against the sha1 of the file as of 1/22/21
//  EXPECT_EQ(biogears::io::generate_file_sha1("environments/AnchorageDecember.xml"), iom.get_expected_sha1("environments/AnchorageDecember.xml"));
//}
//
//TEST_F(TEST_FIXTURE_NAME, GetSha1Nutrition)
//{
//  biogears::io::IOManager iom; // this tests against the sha1 of the file as of 1/22/21
//  EXPECT_EQ(biogears::io::generate_file_sha1("nutrition/NoMacros.xml"), iom.get_expected_sha1("nutrition/NoMacros.xml"));
//}
//
//TEST_F(TEST_FIXTURE_NAME, GetSha1Override)
//{
//  biogears::io::IOManager iom; // this tests against the sha1 of the file as of 1/22/21
//  EXPECT_EQ(biogears::io::generate_file_sha1("override/BioGearsOverride.xml"), iom.get_expected_sha1("override/BioGearsOverride.xml"));
//}
//
//TEST_F(TEST_FIXTURE_NAME, GetSha1Patients)
//{
//  biogears::io::IOManager iom; // this tests against the sha1 of the file as of 1/22/21
//  EXPECT_EQ(biogears::io::generate_file_sha1("patients/Bradycardic.xml"), iom.get_expected_sha1("patients/Bradycardic.xml"));
//}
//
//TEST_F(TEST_FIXTURE_NAME, GetSha1Substances)
//{
//  biogears::io::IOManager iom; // this tests against the sha1 of the file as of 1/22/21
//  EXPECT_EQ(biogears::io::generate_file_sha1("substances/Acetaminophen.xml"), iom.get_expected_sha1("substances/Acetaminophen.xml"));
//}


//TEST_F(TEST_FIXTURE_NAME,GetEmbeddedResourceFileXSD)
//{
//	biogears::io::IOManager iom;
//  size_t embeded_size;
//	std::string s(iom.get_embedded_resource_file("xsd/BioGearsDataModel.xsd", embeded_size));
//	EXPECT_EQ(embeded_size, s.size());
//	if (s.size() > 1) {
//		EXPECT_EQ(s[0],'<');
//		EXPECT_LE(100,s.size());
//	}
//}
//
//TEST_F(TEST_FIXTURE_NAME, GetEmbeddedResourceFileConfig)
//{
//
//  biogears::io::IOManager iom;
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
//  biogears::io::IOManager iom;
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
//  biogears::io::IOManager iom;
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
//  biogears::io::IOManager iom;
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
//  biogears::io::IOManager iom;
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
//  biogears::io::IOManager iom;
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
//  biogears::io::IOManager iom;
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
//	biogears::io::IOManager iom;
//	//EXPECT_EQ(150,iom.get_directory_count());
//}
//
//TEST_F(TEST_FIXTURE_NAME, GetDirectoryList)
//{
//  biogears::io::IOManager iom;
// // char const** directoryList = iom.get_directory_list();
// // int i = 0;
// // for (;directoryList[i] != nullptr;++i) {
//	//const char* ptr = directoryList[i];
// // }
// // EXPECT_GE(i,1);
//}
