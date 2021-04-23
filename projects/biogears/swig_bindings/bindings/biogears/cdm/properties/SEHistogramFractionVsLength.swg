%{
#include <biogears/cdm/properties/SEHistogramFractionVsLength.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEHistogram.swg>
%import <biogears/cdm/properties/SEScalarLength.swg>

%nspace biogears::SEHistogramFractionVsLength;
%rename(Equals)  operator==(const SEHistogramFractionVsLength&) const;
%rename(NotEquals)  operator!=(const SEHistogramFractionVsLength&) const;

%ignore  biogears::SEHistogramFractionVsLength::GetFraction() const;
%ignore  biogears::SEHistogramFractionVsLength::GetLength() const; 

%include <biogears/cdm/properties/SEHistogramFractionVsLength.h>