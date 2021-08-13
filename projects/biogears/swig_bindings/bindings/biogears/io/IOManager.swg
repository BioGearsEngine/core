
%{
   #include <stdexcept>
   #include <string>
   #include <vector>

  #include <biogears/filesystem/path.h>
  #include <biogears/io/io-manager.h>
%}

%import <biogears/exports.swg>
%import <biogears/filesystem/path.swg>

%nspace biogears::IOManager;

%ignore IOManager(IOManager&& iom);
%ignore biogears::IOManager(biogears::IOManager&& iom);
%ignore biogears::IOManager::IOManager(biogears::IOManager&& iom);
%ignore biogears::IOManager::IOManager(IOManager&& iom);

%ignore  biogears::IOManager::operator=(IOManager&& rhs);
%ignore  biogears::IOManager::operator=(IOManager const& rhs);

%include <biogears/io/io-manager.h>
