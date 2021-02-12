#include "PatientNutrition.h"

#include "Property.h"

#include <biogears/cdm/properties/SEScalarMass.h>
#include <biogears/cdm/properties/SEScalarMassPerTime.h>
#include <biogears/cdm/properties/SEScalarVolume.h>

#include <biogears/cdm/patient/SENutrition.h>

namespace biogears {
namespace io {
  //class SENutrition
  void PatientNutrition::Marshall(const CDM::NutritionData& in, SENutrition& out)
  {
    out.Clear();

    if (in.Name().present()) {
      out.m_Name = in.Name().get();
    } else {
      out.m_Name = "Standard Meal";
    }
    io::Property::Marshall(in.Carbohydrate(), out.GetCarbohydrate());
    io::Property::Marshall(in.Fat(), out.GetFat());
    io::Property::Marshall(in.Protein(), out.GetProtein());
    io::Property::Marshall(in.Calcium(), out.GetCalcium());
    io::Property::Marshall(in.Sodium(), out.GetSodium());
    io::Property::Marshall(in.Water(), out.GetWater());
  }
  //----------------------------------------------------------------------------------
  void PatientNutrition::UnMarshall(const SENutrition& in, CDM::NutritionData& out)
  {
    if (!in.m_Name.empty()) {
      out.Name(in.m_Name);
    }
    if (in.m_Carbohydrate != nullptr) {
      io::Property::UnMarshall(*in.m_Carbohydrate, out.Carbohydrate());
    }
    if (in.m_Fat != nullptr) {
      io::Property::UnMarshall(*in.m_Fat, out.Fat());
    }
    if (in.m_Protein != nullptr) {
      io::Property::UnMarshall(*in.m_Protein, out.Protein());
    }
    if (in.m_Calcium != nullptr) {
      io::Property::UnMarshall(*in.m_Calcium, out.Calcium());
    }
    if (in.m_Sodium != nullptr) {
      io::Property::UnMarshall(*in.m_Sodium, out.Sodium());
    }
    if (in.m_Water != nullptr) {
      io::Property::UnMarshall(*in.m_Water, out.Water());
    }
  }
  //----------------------------------------------------------------------------------
}
}