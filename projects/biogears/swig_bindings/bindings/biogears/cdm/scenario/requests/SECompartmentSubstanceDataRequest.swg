%{
#include <biogears/cdm/scenario/requests/SECompartmentSubstanceDataRequest.h>
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%include <std_map.i>

%import <biogears/cdm/substance/SESubstance.swg>
%import <biogears/cdm/scenario/requests/SEDataRequest.swg>
%import <biogears/cdm/scenario/requests/SECompartmentDataRequest.swg>

%nspace biogears::SECompartmentSubstanceDataRequest;


namespace biogears {
class  SECompartmentSubstanceDataRequest : public SECompartmentDataRequest {

protected:
  SECompartmentSubstanceDataRequest(const SEDecimalFormat* dfault = nullptr);


  virtual SESubstance* GetSubstance() const;
  virtual void SetSubstance(SESubstance* sub);
  virtual bool HasSubstance() const;
  virtual void InvalidateSubstance();
  virtual void Set(const char* cmpt, SESubstance& substance, const char* name, const char* unit = "");


};
}

