%{
#include <biogears/cdm/properties/SEDecimalFormat.h>

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
) SEDecimalFormat
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/CommonDataModel.swg>

%nspace BIOGEARS_NAMESPACE SEDecimalFormat;
%nspace BIOGEARS_NAMESPACE DecimalNotation;

%ignore GetPrecision;
%ignore SetPrecision(std::streamsize p);
%ignore SetStream(std::ofstream& s);

%rename(Equals)  operator==(const SEDecimalFormat&) const;
%rename(NotEquals)  operator!=(const SEDecimalFormat&) const;
%rename(Equals)  operator==(const SEDecimalFormat&) const;
%rename(NotEquals)  operator!=(const SEDecimalFormat&) const; 

%include <biogears/cdm/properties/SEDecimalFormat.h>


