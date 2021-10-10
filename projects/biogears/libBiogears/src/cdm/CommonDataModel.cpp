#include <biogears/cdm/CommonDataModel.h>

namespace biogears {
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

}