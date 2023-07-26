

%{
#include <biogears/cdm/patient/actions/SERadiationAbsorbedDose.h>
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
) SERadiationAbsorbedDose
#endif

%include <biogears/exports.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/properties/SEScalarEnergyPerMass.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace BIOGEARS_NAMESPACE SERadiationAbsorbedDose;

%ignore BIOGEARS_NAMESPACE SERadiationAbsorbedDose::Unload;
%ignore BIOGEARS_NAMESPACE SERadiationAbsorbedDose::Load;

%include <biogears/cdm/patient/actions/SERadiationAbsorbedDose.h>

%nspace  std::vector<BIOGEARS_NAMESPACE SERadiationAbsorbedDose*>;
%template (vector_SERadiationAbsorbedDose_ptr)  std::vector<BIOGEARS_NAMESPACE SERadiationAbsorbedDose*>;