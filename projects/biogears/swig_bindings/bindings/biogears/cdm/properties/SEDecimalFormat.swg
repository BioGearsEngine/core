%{
#include <biogears/cdm/properties/SEDecimalFormat.h>

USING_BIOGEARS_NAMESPACE;
using namespace CDM;
%}

%include <biogears/exports.h>

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


