%{
#include <biogears/cdm/scenario/requests/SEDataRequest.h>
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <biogears/cdm/utils/unitconversion/CompoundUnit.swg>
%import <biogears/cdm/properties/SEDecimalFormat.swg>

%import <std_map.i>

%nspace BIOGEARS_NAMESPACE SEDataRequest;

OPEN_BIOGEARS_NAMESPACE
class  SEDataRequest : public SEDecimalFormat {

public:
  SEDataRequest(const SEDecimalFormat* dfault = nullptr);
  virtual size_t HashCode() const = 0;

  virtual std::string GetName() const;
  virtual void SetName(const char* name);
  virtual bool HasName() const;
  virtual void InvalidateName();


  virtual const char* GetRequestedUnit() const;
  virtual void SetRequestedUnit(const char* unit);
  virtual bool HasRequestedUnit() const;
  virtual void InvalidateRequestedUnit();


  virtual const CCompoundUnit* GetUnit() const;
  virtual void SetUnit(const CCompoundUnit& unit);
  virtual bool HasUnit() const;
  virtual void InvalidateUnit();


  virtual void Set(const char* name, const CCompoundUnit& unit);
  virtual void Set(const char* name, const char* unit = "");
};
CLOSE_BIOGEARS_NAMESPACE
