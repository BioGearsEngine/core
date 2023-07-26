%{
#include <biogears/cdm/patient/actions/SEPupillaryResponse.h>
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
) SEPupillaryResponse
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%import <biogears/cdm/properties/SEScalar.swg>
%import <biogears/cdm/properties/SEScalarNeg1To1.swg>

%nspace BIOGEARS_NAMESPACE SEPupillaryResponse;

%ignore BIOGEARS_NAMESPACE SEPupillaryResponse::Unload;
%ignore BIOGEARS_NAMESPACE SEPupillaryResponse::Load;

%ignore BIOGEARS_NAMESPACE SEPupillaryResponse::GetScalar(std::string const &);

%rename(GetReactivityModifier_const) BIOGEARS_NAMESPACE SEPupillaryResponse::GetReactivityModifier() const;
%rename(GetShapeModifier_const) BIOGEARS_NAMESPACE SEPupillaryResponse::GetShapeModifier() const;
%rename(GetSizeModifier_const) BIOGEARS_NAMESPACE SEPupillaryResponse::GetSizeModifier() const;

%include <biogears/cdm/patient/actions/SEPupillaryResponse.h>

%nspace  std::vector<BIOGEARS_NAMESPACE SEPupillaryResponse*>;
%template (vector_SEPupillaryResponse_ptr)  std::vector<BIOGEARS_NAMESPACE SEPupillaryResponse*>;