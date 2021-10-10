%{
#include <biogears/cdm/properties/SEDecimalFormat.h>

using namespace biogears;
using namespace CDM;
%}

%include <biogears/exports.h>

%import <biogears/cdm/CommonDataModel.swg>

%nspace biogears::SEDecimalFormat;
%nspace biogears::DecimalNotation;

%ignore GetPrecision;
%ignore SetPrecision(std::streamsize p);
%ignore SetStream(std::ofstream& s);

%rename(Equals)  operator==(const SEDecimalFormat&) const;
%rename(NotEquals)  operator!=(const SEDecimalFormat&) const;
%rename(Equals)  operator==(const SEDecimalFormat&) const;
%rename(NotEquals)  operator!=(const SEDecimalFormat&) const; 

%include <biogears/cdm/properties/SEDecimalFormat.h>


