Overview
==========

BioGears® source code is hosted here and will be developed through this public repository to encourage community facing development. In addition, we also support official stable releases. Our latest deployment (found under releases) is still in a beta phase, and is intended to be an intermediate release to showcase the capabilities of the BioGears® Physiology Engine. The current version of the software includes examples of all types of engine interfaces, but does not include all of the functionality or physiologic systems that will be present in the final product. This version of the software is meant to elicit feedback and enhance community involvement in establishing end product expectations.


Build Status
-----------------
| Platform | Compiler | Architecture | Status |
|----------|----------|--------------|--------|
| Windows  |  msvc15  | amd64        | ![Windows msvc15 Build  Status](https://biogearsengine.com/content/badges/nightly_libBioGears_windows_msvc15.png) |
| Windows  |  msvc16  | amd64        | ![Windows msvc16 Build Status](https://biogearsengine.com/content/badges/nightly_libBioGears_windows_msvc16.png) |
| Linux  |  gcc9  | amd64 | ![Linux-gcc9-amd64 Build Status](https://biogearsengine.com/content/badges/nightly_libBioGears_linux_gcc9-amd64.png) |
| Linux  |  gcc9  | armhf | ![Linux-gcc9-armhf Build Status](https://biogearsengine.com/content/badges/nightly_libBioGears_linux_gcc9-armhf.png) |
| Linux  |  gcc9  | aarch64 | ![Linux-gcc9-aarch64 Build Status](https://biogearsengine.com/content/badges/nightly_libBioGears_linux_gcc9-aarch64.png) |
| MacOS  Yosemite |  clang10  | amd64 | ![MacOS Yosemite clang11 Build Status](https://biogearsengine.com/content/badges/nightly_libBioGears_macos-yosemite.png) |
| MacOS  Catalina|  clang11  | amd64 | ![MacOS Catalina clang11 Build Status](https://biogearsengine.com/content/badges/nightly_libBioGears_macos-catalina.png) |

What is BioGears®
------------------
BioGears® is a C++ based, open source, multi-platform (Windows, Mac, and Linux), comprehensive human physiology engine that will drive medical education, research, and training technologies. BioGears enables accurate and consistent physiology simulation across the medical community. The engine can be used as a standalone application or integrated with simulators, sensor interfaces, and models of all fidelities.

BioGears high-level objectives
--------------------------------
* Create a publicly available physiology research platform that enables accurate and consistent simulated physiology across training applications
* Lower the barrier to create medical training content
* Engage the community to develop and extend physiology models
* Meet the training needs of the military
* Expand the body of knowledge regarding the use of simulated physiology for medical education

Building Source
======
Refer to our GitHub Wiki for build requirements [here](https://github.com/BioGearsEngine/core/wiki)


Structure 
------------
The BioGears source is structured as follows:
- `cmake/` - Custom CMake scripts needed by the build system, including some JAVA search behavior overrides
  - `common/` - package files for common CMake deps
  - `toolchains/` - Templates for CMake toolchain files
- `share/` - Non-source files
  - `astyle/`  - Astyle template for formating source (deprecated)
  - `data/` - BioGears configuration files for various scenarios 
  - `doc/` - BioGears documentation files (doxygen, markdown, ...)
  - `etc/` - Other
  - `jar/` - Jar files for BioGears JAVA based components hopefully deprecated soon
  - `xsd/` - XSD definition files for the Common Data Model
- `projects/`
  - `biogears` - Main Engine Source for building libbiogears libbiogears-cdm
  - `biogears-common` - Helper framework for c++ with no third party dependencies
  - `bg-cli` - Primary Command Line Utility for using BioGears 
  - `howto/` - libbiogears SDK examples
  - `java/`  - JNI utilities for running biogears
  - `unit/`  - Unit test harness used for testing libbiogears components
  - `zip/`   - Self hosted copy of https://github.com/nmoinvaz/minizip. A bg-cli third part dep
  
<!-- biogears
biogears-common
circuit_profiler
bg-cli
howto
java
test_driver
unit
zip -->


Citing BioGears 
------------
If you'd like to use BioGears in your research, we ask that you cite the following publication

[![DOI](https://joss.theoj.org/papers/10.21105/joss.02645/status.svg)](https://doi.org/10.21105/joss.02645)

## What's new in ver 7.3.2 and 7.3.1 (Dec 10, 2020)
- **ver 1.0 release of the UI**
- **ver 1.0 release of BioGears Lite**
- Plasma Lyte compound substance added 
- **New drug administration route** nasal administration 
- **New drug** nasal naloxone
- CPACK installer functionality added 
- Moved data tracking to advance model time method
- Added sequential organ failure (SOFA score) as an assessment
- Changed website documentation tools to python, removed Java requirements 
- Minor drug model updates for LD50 and PD modifiers
- Implemented improved command line interface, named bg-cli to support better threading and logging
- Integrated inflammation model into hemorrhage model to make it more physiologically accurate
- Implemented acute respiratory distress model 
- Implemented a model of sleep and the metabolic consequences of sleep
- added new psychomotor vigilance test (PVT) patient assessment
- Removed legacy functionality of the GI system 
- Implemented Richmond agitation sedation (RASS) scale as a patient request 
  - Is a function of the pharmacokinetics of propofol and other anesthesia drugs
- Implemented a **new exercise model** that supported weighted exercise, cycling, and rowing
- Updated nervous system model, validated for inflammation and other injury responses
  - Including localized autoregulation 
- Implemented a tourniquet action for specific body regions
  - will perform locally, reducing hemorrhage in downstream vessels
- Compatibility updates to support Unreal Engine integration 
- General CMAKE updates to the build system 
- Minor bug updates and validation changes to models
- Finished all conformant override parameters (pressure, heart rate, respiration rate, and oxygen saturation)


For a list of historical releases please see our [website](https://www.biogearsengine.com/documentation/index.html).


Programmatics
===============
BioGears was originally developed under the TATRC funded project: W81XWH-13-2-0068  
BioGears development is currently funded under the following contracts:
  - W81XWH-17-C-0172
  - W911NF-17-D-0006
  - W81XWH-18-C-0169

Disclaimer:

This work is supported by the US Army Medical Research and Materiel Command. The views, opinions and/or findings contained in this report are those of the author(s) and should not be construed as an official Department of the Army position, policy, or decision unless so designated by other documentation.
BioGears is released under the Apache 2.0 License.

BioGears has Publication papers and abstracts on several systems and clinical scenarios.

Additional Information
======================

Code of Conduct
------------------
We support the [contributor covenant](https://github.com/BioGearsEngine/Engine/blob/master/CODE_OF_CONDUCT.md) and the scope and enforcement it details. We reserve the right of enforcement that we feel is appropriate given the nature of the offense up to and including a permanent ban from the project.


Contributing 
-------------
Details will be filled in shortly. In the meantime, if you have a contribution or issue to post/push feel free to contact abaird@ara.com with the details. 

You can access our public Slack through our [development website](https://www.biogears.dev/).

Additional Documentation
--------------------------
For more detailed documentation including model discussions and implementation details can be found at www.BioGearsEngine.com


