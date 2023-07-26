%{
#include <biogears/cdm/properties/SEFunctionVolumeVsTime.h>
USING_BIOGEARS_NAMESPACE;
using namespace CDM;
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
) SEFunctionVolumeVsTime
#endif

%include <biogears/exports.swg>
%include <biogears/config.h>

%rename(Equals)  operator==(const SEFunctionElectricPotentialVsTime&) const;
%rename(NotEquals)  operator!=(const SEFunctionElectricPotentialVsTime&) const;

%nspace std::vector<double>;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<TimeUnit>;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<ElectricPotentialUnit>;

%import <biogears/cdm/properties/SEFunction.swg>
%import <biogears/cdm/properties/SEScalarTime.swg>
%import <biogears/cdm/properties/SEScalarVolume.swg>

%include <biogears/cdm/properties/SEFunctionVolumeVsTime.h>


