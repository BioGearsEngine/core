%{
#include <biogears/cdm/scenario/requests/SEDataRequest.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

#if defined(SWIGPYTHON)
%module(package="biogears", 
moduleimport=
   "# Import the low-level C/C++ module\n"
    "if __package__ or \".\" in __name__:\n"
    "    from . import _biogears\n"
    "else:\n"
    "    import _biogears\n"
) SEDataRequest
#endif

%include <biogears/exports.swg>

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
