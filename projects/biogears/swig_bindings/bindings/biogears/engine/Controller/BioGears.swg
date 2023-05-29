%{
  #include <biogears/container/Tree.tci.h>
  #include <biogears/cdm/utils/Logger.h>
  #include <biogears/engine/Controller/BioGears.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}


%include <biogears/exports.h>
%import <biogears/cdm/utils/Logger.swg>

%nspace BIOGEARS_NAMESPACE BioGears;

OPEN_BIOGEARS_NAMESPACE
  class BioGears : public Loggable{
    BioGears (Logger* logger);
  };
CLOSE_BIOGEARS_NAMESPACE
