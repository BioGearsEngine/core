#include "PatientNutrition.h"

#include "Property.h"

#include <biogears/cdm/properties/SEScalarMass.h>
#include <biogears/cdm/properties/SEScalarMassPerTime.h>
#include <biogears/cdm/properties/SEScalarVolume.h>

#include <biogears/cdm/patient/SENutrition.h>

namespace biogears {
namespace io {
  // class SENutrition
  void PatientNutrition::UnMarshall(const CDM::NutritionData& in, SENutrition& out, std::default_random_engine* rd)
  {
    out.Clear();

    if (in.Name().present()) {
      out.m_Name = in.Name().get();
    } else {
      out.m_Name = "Standard Meal";
    }
    io::Property::UnMarshall(in.Carbohydrate(), out.GetCarbohydrate(), rd);
    io::Property::UnMarshall(in.Fat(), out.GetFat(), rd);
    io::Property::UnMarshall(in.Protein(), out.GetProtein(), rd);
    io::Property::UnMarshall(in.Calcium(), out.GetCalcium(), rd);
    io::Property::UnMarshall(in.Sodium(), out.GetSodium(), rd);
    io::Property::UnMarshall(in.Water(), out.GetWater(), rd);
  }
  //----------------------------------------------------------------------------------
  void PatientNutrition::Marshall(const SENutrition& in, CDM::NutritionData& out)
  {
    if (!in.m_Name.empty()) {
      out.Name(in.m_Name);
    }
    if (in.m_Carbohydrate != nullptr) {
      io::Property::Marshall(*in.m_Carbohydrate, out.Carbohydrate());
    }
    if (in.m_Fat != nullptr) {
      io::Property::Marshall(*in.m_Fat, out.Fat());
    }
    if (in.m_Protein != nullptr) {
      io::Property::Marshall(*in.m_Protein, out.Protein());
    }
    if (in.m_Calcium != nullptr) {
      io::Property::Marshall(*in.m_Calcium, out.Calcium());
    }
    if (in.m_Sodium != nullptr) {
      io::Property::Marshall(*in.m_Sodium, out.Sodium());
    }
    if (in.m_Water != nullptr) {
      io::Property::Marshall(*in.m_Water, out.Water());
    }
  }
  //----------------------------------------------------------------------------------
}
}