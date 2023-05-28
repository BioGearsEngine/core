#include <biogears/cdm/CommonDataModel.h>

OPEN_BIOGEARS_NAMESPACE
CommonDataModelException::CommonDataModelException()
  : std::runtime_error("")
{
}

CommonDataModelException::CommonDataModelException(const char* _Message)
  : std::runtime_error(_Message)
{
}

CommonDataModelException::CommonDataModelException(const std::string& _Message)
  : std::runtime_error(_Message)
{
}

CommonDataModelException::~CommonDataModelException()
{
}

CLOSE_BIOGEARS_NAMESPACE