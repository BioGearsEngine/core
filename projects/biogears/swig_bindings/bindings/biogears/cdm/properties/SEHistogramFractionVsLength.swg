%{
#include <biogears/cdm/properties/SEHistogramFractionVsLength.h>
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
) SEHistogramFractionVsLength
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/properties/SEHistogram.swg>
%import <biogears/cdm/properties/SEScalarLength.swg>

%nspace BIOGEARS_NAMESPACE SEHistogramFractionVsLength;
%rename(Equals)  operator==(const SEHistogramFractionVsLength&) const;
%rename(NotEquals)  operator!=(const SEHistogramFractionVsLength&) const;

%ignore  BIOGEARS_NAMESPACE SEHistogramFractionVsLength::GetFraction() const;
%ignore  BIOGEARS_NAMESPACE SEHistogramFractionVsLength::GetLength() const; 

%include <biogears/cdm/properties/SEHistogramFractionVsLength.h>