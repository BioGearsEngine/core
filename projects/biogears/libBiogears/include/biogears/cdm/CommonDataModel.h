/**************************************************************************************
Copyright 2015 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/

#pragma once



#if (0)
#define DEBUGOUT(x) x
#else
#define DEBUGOUT(x)
#endif
namespace mil {
namespace tatrc {
  namespace physiology {
    namespace datamodel {
    }
  }
}
}
//using namespace mil::tatrc::phsyiology::datamodel;
#define CDM_BIND_DECL(type)   \
  namespace mil {             \
    namespace tatrc {         \
      namespace physiology {  \
        namespace datamodel { \
          class type;         \
        }                     \
      }                       \
    }                         \
  }
namespace CDM = mil::tatrc::physiology::datamodel;

#include <stdexcept>
#ifdef _MSC_VER
#include <direct.h>
#else
#include <dirent.h>
#endif

#if defined(_MSC_VER)
#include <unordered_map>
#else //if (__GCC__) || (__GNUC__)
#include <unordered_map>
#endif

//Utilities

#include <biogears/exports.h>
namespace biogears {

#pragma warning(disable : 4251)
struct CommonDataModelException : public std::runtime_error {
  CommonDataModelException()
    : std::runtime_error("Common Data Model Error")
  {
  }

  CommonDataModelException(const char* _Message)
    : std::runtime_error(_Message)
  {
  }

  CommonDataModelException(const std::string& _Message)
    : std::runtime_error(_Message)
  {
  }

  ~CommonDataModelException() override = default;
};
}
// Basics
#include <biogears/cdm/Macros.h>
#include <biogears/cdm/utils/Logger.h>