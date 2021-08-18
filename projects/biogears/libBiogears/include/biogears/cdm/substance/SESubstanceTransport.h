/**************************************************************************************
Copyright 2015 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/

#pragma once
#ifdef MSVC
#pragma warning(disable : 4503)
#endif

#include <biogears/cdm/CommonDataModel.h>
#include "biogears/cdm/properties/SEScalarVolumePerTime.h"
#include "biogears/cdm/properties/SEScalarMass.h"
#include "biogears/cdm/properties/SEScalarMassPerVolume.h"
#include "biogears/cdm/properties/SEScalarVolume.h"
#include "biogears/cdm/properties/SEScalarFraction.h"

namespace biogears {
#define SUBSTANCE_TRANSPORTER_TEMPLATE typename GraphType, typename FluxUnit, typename QuantityUnit, typename ExtensiveUnit, typename IntensiveUnit
#define TRANSPORT_AMOUNT_TYPES ExtensiveScalar, IntensiveScalar
template <typename ExtensiveScalar, typename IntensiveScalar>
class SESubstanceTransportAmount {
  template <SUBSTANCE_TRANSPORTER_TEMPLATE>
  friend class SESubstanceTransporter;

public:
  virtual ~SESubstanceTransportAmount() {}

  virtual void Invalidate() = 0;

  virtual bool HasExtensive() const = 0;
  virtual ExtensiveScalar& GetExtensive() = 0;

  virtual bool HasIntensive() const = 0;
  virtual IntensiveScalar& GetIntensive() = 0;
};
BG_EXT template class BIOGEARS_API SESubstanceTransportAmount<SEScalarVolume, SEScalarFraction>;
BG_EXT template class BIOGEARS_API SESubstanceTransportAmount<SEScalarMass, SEScalarMassPerVolume>;

using SEGasTransportSubstance = SESubstanceTransportAmount<SEScalarVolume, SEScalarFraction>;
using SELiquidTransportSubstance = SESubstanceTransportAmount<SEScalarMass, SEScalarMassPerVolume>;

#define TRANSPORT_VERTEX_TYPES QuantityScalar, ExtensiveScalar, IntensiveScalar
template <typename QuantityScalar, typename ExtensiveScalar, typename IntensiveScalar>
class SESubstanceTransportVertex {
  template <SUBSTANCE_TRANSPORTER_TEMPLATE>
  friend class SESubstanceTransporter;

public:
  virtual ~SESubstanceTransportVertex() {}

  virtual std::string GetName() const = 0;
  virtual const char* GetName_cStr() const = 0;

protected:
  virtual bool HasQuantity() const = 0;
  virtual QuantityScalar& GetQuantity() = 0;

  virtual std::vector<SESubstanceTransportAmount<TRANSPORT_AMOUNT_TYPES>*>& GetTransportSubstances() = 0;
};

BG_EXT template class BIOGEARS_API SESubstanceTransportVertex<SEScalarVolume, SEScalarVolume, SEScalarFraction>;
BG_EXT template class BIOGEARS_API SESubstanceTransportVertex<SEScalarVolume, SEScalarMass, SEScalarMassPerVolume>;

using SEGasTransportVertex = SESubstanceTransportVertex<SEScalarVolume, SEScalarVolume, SEScalarFraction>;
using SELiquidTransportVertex = SESubstanceTransportVertex<SEScalarVolume, SEScalarMass, SEScalarMassPerVolume>;

#define TRANSPORT_EDGE_TYPES FluxScalar, QuantityScalar, ExtensiveScalar, IntensiveScalar
template <typename FluxScalar, typename QuantityScalar, typename ExtensiveScalar, typename IntensiveScalar>
class SESubstanceTransportEdge {
  template <SUBSTANCE_TRANSPORTER_TEMPLATE>
  friend class SESubstanceTransporter;

public:
  virtual ~SESubstanceTransportEdge() {}

  virtual std::string GetName() const = 0;
  virtual const char* GetName_cStr() const = 0;

protected:
  virtual bool HasFlux() const = 0;
  virtual FluxScalar& GetFlux() = 0;

  virtual SESubstanceTransportVertex<TRANSPORT_VERTEX_TYPES>& GetSourceVertex() = 0;
  virtual SESubstanceTransportVertex<TRANSPORT_VERTEX_TYPES>& GetTargetVertex() = 0;
};

BG_EXT template class BIOGEARS_API SESubstanceTransportEdge<SEScalarVolumePerTime, SEScalarVolume, SEScalarVolume, SEScalarFraction>;
BG_EXT template class BIOGEARS_API SESubstanceTransportEdge<SEScalarVolumePerTime, SEScalarVolume, SEScalarMass, SEScalarMassPerVolume>;

using SEGasTransportEdge = SESubstanceTransportEdge<SEScalarVolumePerTime, SEScalarVolume, SEScalarVolume, SEScalarFraction>;
using SELiquidTransportEdge = SESubstanceTransportEdge<SEScalarVolumePerTime, SEScalarVolume, SEScalarMass, SEScalarMassPerVolume>;

template <typename FluxScalar, typename QuantityScalar, typename ExtensiveScalar, typename IntensiveScalar>
class SESubstanceTransportGraph {
  template <SUBSTANCE_TRANSPORTER_TEMPLATE>
  friend class SESubstanceTransporter;

public:
  virtual ~SESubstanceTransportGraph() {}

protected:
  virtual void BalanceByIntensive() = 0; // Transporter calculates the new concentration
  virtual size_t GetVertexIndex(const SESubstanceTransportVertex<TRANSPORT_VERTEX_TYPES>& v) const = 0;
  virtual const std::vector<SESubstanceTransportVertex<TRANSPORT_VERTEX_TYPES>*>& GetVerticies() const = 0;
  virtual const std::vector<SESubstanceTransportEdge<TRANSPORT_EDGE_TYPES>*>* GetSourceEdges(const SESubstanceTransportVertex<TRANSPORT_VERTEX_TYPES>& v) const = 0;
  virtual const std::vector<SESubstanceTransportEdge<TRANSPORT_EDGE_TYPES>*>* GetTargetEdges(const SESubstanceTransportVertex<TRANSPORT_VERTEX_TYPES>& v) const = 0;
};

BG_EXT template class BIOGEARS_API SESubstanceTransportGraph<SEScalarVolumePerTime, SEScalarVolume, SEScalarVolume, SEScalarFraction>;
BG_EXT template class BIOGEARS_API SESubstanceTransportGraph<SEScalarVolumePerTime, SEScalarVolume, SEScalarMass, SEScalarMassPerVolume>;

using SEGasTransportGraph = SESubstanceTransportGraph<SEScalarVolumePerTime, SEScalarVolume, SEScalarVolume, SEScalarFraction>;
using SELiquidTransportGraph = SESubstanceTransportGraph<SEScalarVolumePerTime, SEScalarVolume, SEScalarMass, SEScalarMassPerVolume>;

template <SUBSTANCE_TRANSPORTER_TEMPLATE>
class SESubstanceTransporter : public Loggable {
public:
  SESubstanceTransporter(const FluxUnit& fUnit, const QuantityUnit& qUnit, const ExtensiveUnit& eUnit, const IntensiveUnit& iUnit, Logger* logger);
  virtual ~SESubstanceTransporter() {}

  void Transport(GraphType& graph, double timeStep_s);

protected:
  const FluxUnit& m_FluxUnit;
  const QuantityUnit& m_QuantityUnit;
  const ExtensiveUnit& m_ExtensiveUnit;
  const IntensiveUnit& m_IntensiveUnit;
};

BG_EXT template class BIOGEARS_API SESubstanceTransporter<SEGasTransportGraph, VolumePerTimeUnit, VolumeUnit, VolumeUnit, NoUnit>;
BG_EXT template class BIOGEARS_API SESubstanceTransporter<SELiquidTransportGraph, VolumePerTimeUnit, VolumeUnit, MassUnit, MassPerVolumeUnit>;

using SEGasTransporter = SESubstanceTransporter<SEGasTransportGraph, VolumePerTimeUnit, VolumeUnit, VolumeUnit, NoUnit>;
using SELiquidTransporter = SESubstanceTransporter<SELiquidTransportGraph, VolumePerTimeUnit, VolumeUnit, MassUnit, MassPerVolumeUnit>;

}

#include <biogears/cdm/substance/SESubstanceTransport.inl>
