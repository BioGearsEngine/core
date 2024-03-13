# Scenario Generator Templates

## Population Generation


## Scenario Generator

The BG-CLI Scenario Generator takes in an input file in the formation of a CDM::ScenarioGenerator. The full CDM::ScenarioGenerator specification 
can be found inside the BioGears::CDM XSD Specification. Currently the format follows the following generalities

```xml
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<ScenarioGenerator xmlns="uri:/mil/tatrc/physiology/datamodel" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="uri:/mil/tatrc/physiology/datamodel xsd/BioGearsDataModel.xsd">

<ScenarioTemplate name="SkinyWhiteMales" 
                  timeline="timelines/AsthmaAttack.xml.in" 
                  count="10">
  <NormalDistribution mean="120" deviation="7">BODYWIEGHT</NormalDistribution>
  <NormalDistribution mean="180" deviation="10">BLOOD_PRESSURE</NormalDistribution>
  <NormalDistribution mean="76" deviation="6">HEIGHT</NormalDistribution>
  
  <NormalDistribution mean="5" deviation="1">DURATION</NormalDistribution>
  <NormalDistribution mean=".5" deviation=".06" min="0.0" max="1.0">SEVERITY</NormalDistribution>
  <NormalDistribution mean="200" deviation="6">INTERVENTION</NormalDistribution> 
  
  <WeightedDistribution name="BLOOD_TYPE" >
    <Value weight="20">AB</Value>
    <Value weight="80">A</Value>
  </WeightedDistribution> 
  
    <WeightedDistribution name="BLOOD_TYPE_RH" >
    <Value weight="90">true</Value>
    <Value weight="10">false</Value>
  </WeightedDistribution>

</ScenarioTemplate>

<ScenarioTemplate ...</SceVnarioTemplate>

</ScenarioGenerator>
```

Each ScenarioGenerator contains on or more ScenarioTemplates. The ScenarioTemplate contains three attributes

1. name - Prefix of all Generator Scenario files for this template. If name = ShortHungryMercenaries then all files will be of the form ShortHungryMercenaries_###.xml
2. count - Number of files to be generated from this template
3. timeline - Path to a CDM::Timeline or CDM::Scenario. These input files will not be valid CDM::Objects as they will have place holder strings which match the name or value of each distribution tag
   Inside the file each name will be surrounded by the '@' character so  <NormalDistribution>BODY_WEIGHT</NormalDistribution> will appear in the Scenario file as @BODY_WEIGHT@.

Place holders can appear any number of times in the file. 

### Distribution

#### Normal Distribution
  1. mean : average value of the distribution.
  2. divation : sigma value of the normal distribution
  3. value    : Name of place holder in the timeline to be replaced.

#### Wieghted Distribution
  1. Name : The Name of the place holder value in the timeline
  2. List of <Value> Tags
     a. weight of value / sum of all weights is the probability of a specific entry occuring
     b. value : The value that will appear in the file when this entry is generated
#### Discrete Uniform Distribution
  1. Value : String : Name of placeholder value in timeline
  2. a     : lower bound of the interval
  4. b     : upper bound of the interval
  
  Replaces @Value@ with integral values that is [a,b] such that all values. [1,6] would simulate a fair dice roll
#### Continuous Uniform Distribution
  1. Value : String : Name of placeholder value in timeline
  2. a     : lower bound of the interval
  4. b     : upper bound of the interval
  
  Replaces @Value@ with a value that is [a,b) such that all values in the interval have equal probability of occuring
