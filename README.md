Overview
==========

BioGears® source code is hosted here and will be developed through this public repository to encourage community facing development. In addition we also support official stable releases. Our latest deployment (found under releases) is still in a beta phase, and is intended to be an intermediate release to showcase the capabilities of the BioGears® Physiology Engine. The current version of the software includes examples of all types of engine interfaces, but does not include all of the functionality or physiologic systems that will be present in the final product. This version of the software is meant to elicit feedback and enhance community involvement in establishing end product expectations.

What is BioGears®
------------------
BioGears®  is a C++ based, open source, multi-platform (Windows, Mac, and Linux), comprehensive human physiology engine that will drive medical education, research, and training technologies. BioGears enables accurate and consistent physiology simulation across the medical community. The engine can be used as a standalone application or integrated with simulators, sensor interfaces, and models of all fidelities.

BioGears high-level objectives
--------------------------------
* Create a publicly available physiology research platform that enables accurate and consistent simulated physiology across training applications
* Lower the barrier to create medical training content
* Engage the community to develop and extend physiology models
* Meet the training needs of the military
* Expand the body of knowledge regarding the use of simulated physiology for medical education

Building Source
======
Dependancies 
-------------
Biogears® engine relies on the following dependancies: 

* Cmake 3.5 or greater (3.11 On Windows)
* JDK 1.7 or greater

Please make sure that all dependancies are installed and linked properly to your PATH before building. 

Tested Platforms
----------------
The BioGears CI server test against the following platforms. Supported platforms are subject to change over time, but our goal is maintain support for x86_64 (windows,linux,osx) in addition to arm8_64 support for linux.

 - windows-vc15-md-x64
 - windows-vc14-md-x64
 - linux-gcc5.4-core2_64
 - linux-gcc5.4-arm8
 - macos-llvm-yosemite

If you are trying to build BioGears on a non tested platform. You can contact us on our [forums](https://www.biogearsengine.com/forums/) for assistance.

## Dependency list
Biogears has a light dep list.  You will need a copy of cmake 3.11 or higher to generate the build files and 
installed version of log4cpp xerces-c xsd-4.0 and eigen3.  For Eigen you mus thave a version of eigen which includes the modern Eigen3Config.cmake file witch exported target Eigen::eigen3. We currently suggest eigen 3.3.4

|Library|Version|Date|Source|
|-------|-------|----|------|
|log4cpp| 1.1   |2017-07-13  |https://sourceforge.net/projects/log4cpp/|
|xerces-c| 3.2.0   |2017-08-28  |https://xerces.apache.org/xerces-c/
|xsd| 4.0   |2017-07-13  |https://www.codesynthesis.com/products/xsd/
|eigen3|  3.3.4.   |2017-06-25  |  http://bitbucket.org/eigen/eigen/get/3.3.4.tar.gz/
|cmake | > 3.11     |2018-03-17  |  https://github.com/Kitware/CMake/releases 


##Build Instructions

Structure 
------------
The BioGears source is structured as follows:
- cmake/ - Custom CMake scripts needed by the biuld system including some JAVA search behavior overrides
  - common/ - package files for common cmake deps
  - toolchains/ - Templates for cmake toolchain files
- share/ - Non source files
  - astyle/  - Astyle template for formating source (depricated)
  - data/ - BioGears configuration files for various senarios 
  - doc/ - BioGears documentation files (doxygen,markdown,...)
  - etc/ - Other
  - jar/ - Jar files for BioGears JAVA based components hopefully depricated soon.
  - xsd/ - XSD defintion files for the Common Data Model.
- projects/
  - java/  - JNI utilities for running biogears
    - biogears/ - Core code for libbiogears 
  - howto/ - libbiogears SDK examples
    -  unit/ - Unit test harness used for testing libbiogears components
   

## What's new in ver 6.3 (March 1, 2018)
The latest deployment includes the following notable updates:

- General bug fixes, system improvements, and tools/solver improvements
- Fasciculation patient event flags 
- Updated sweat methodology (fixes to ions lost in sweat)
- Updated substance and compound infusion functionality
  - Added Ringers lactate and updated 
  - Saline compound ion concentrations corrected
  - Hardened implementation 
- MuscleMass new patient data request
  - Muscle catabolism patient flag
- Added dehydration condition 
  - Implemented as scalar 0to1 representing fractional total body water lost
  - Fluid removed from patient compartments 
  - Updated patient flag for event and track body weight change (validated)
  - Added totalbodyfluidVolume as data request
  - Updated patient weight as a function of condition
- Added starvation condition
  - TimeSinceMeal determines how long since the patient's last meal 
  - Scales internal nutrient storages from validated starvation data
  - Removed ConsumeMeal condition, now replaced by starvation condition
  - Validated blood concentrations for ketones, glucose, and amino acids
  - Updated patient weight as a function of condition
- Intracellular ion transport
  - Model uses membrane potential  (see @ref TissueMethodology for details)
  - Michaelis coefficient could support more ion regulation in the future
  - Gated ion transport allows for differences between intra/extracellular spaces
- COPD now supports elevated anaerobic metabolism
- Ion transport model in the small intestine
- Updated drug library so all drugs support an effects site transport rate
- Diabetes type 1 and type 2 conditions
  - insulin resistance and insulin production effects
- Hemorrhage action now initialized with a 0-1 severity and a location (MCIS SDK example still exists)
- New drug Vasopressin
- New drug classifications in the CDM for better grouping in-code 
  - Include anesthetic, sedative, opioid, and reversal agent
  - More grouping in future work

Programmatics
===============
BioGears is being developed under the TATRC funded project: W81XWH-13-2-0068.

Disclaimer:

This work is supported by the US Army Medical Research and Materiel Command. The views, opinions and/or findings contained in this report are those of the author(s) and should not be construed as an official Department of the Army position, policy, or decision unless so designated by other documentation.
BioGears is released under the Apache 2.0 liscense.

BioGears has Publications papers and abstracts on several systems and clinical scenarios.

Additional Information
======================

Code of Conduct
------------------
We support the [contributor covenant](https://github.com/BioGearsEngine/Engine/blob/master/CODE_OF_CONDUCT.md) and the scope and enforcement it details. We reserve the right of enforcement that we feel is appropriate given the nature of the offense up to and including a permanent ban from the project.


Contributing 
-------------
Details will be filled in shortly. In the meantime if you have a contribution or issue to post/push feel free to contact abaird@ara.com with the details. 

Additional Documentation
--------------------------
For more detailed documentation including model discussions and implementation details can be found at www.BioGearsEngine.com


