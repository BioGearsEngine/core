%{
#include <biogears/cdm/properties/SEHistogramFractionVsLength.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEHistogram.swg>
%import <biogears/cdm/properties/SEScalarLength.swg>

%nspace BIOGEARS_NAMESPACE SEHistogramFractionVsLength;
%rename(Equals)  operator==(const SEHistogramFractionVsLength&) const;
%rename(NotEquals)  operator!=(const SEHistogramFractionVsLength&) const;

%ignore  BIOGEARS_NAMESPACE SEHistogramFractionVsLength::GetFraction() const;
%ignore  BIOGEARS_NAMESPACE SEHistogramFractionVsLength::GetLength() const; 

%include <biogears/cdm/properties/SEHistogramFractionVsLength.h>