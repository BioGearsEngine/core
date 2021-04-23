%{
  #include <biogears/container/Tree.tci.h>
  #include <biogears/cdm/utils/Logger.h>
  #include <biogears/engine/Controller/BioGears.h>
%}


%include <biogears/exports.h>
%import <biogears/cdm/utils/Logger.swg>

%nspace biogears::BioGears;

namespace biogears {
  class BioGears {
    BioGears (Logger* logger);
  };
}
