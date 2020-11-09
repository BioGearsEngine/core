Overview
==========

BioGears® source code is hosted here and will be developed through this public repository to encourage community facing development. In addition, we also support official stable releases. Our latest deployment (found under releases) is still in a beta phase, and is intended to be an intermediate release to showcase the capabilities of the BioGears® Physiology Engine. The current version of the software includes examples of all types of engine interfaces, but does not include all of the functionality or physiologic systems that will be present in the final product. This version of the software is meant to elicit feedback and enhance community involvement in establishing end product expectations.


Build Status
-----------------
| Platform | Compiler | Architecture | Status |
|----------|----------|--------------|--------|
| Windows  |  msvc15  | amd64        | ![Windows msvc15 Build  Status](https://biogearsengine.com/content/badges/nightly_libBioGears_windows_msvc15.png) |
| Windows  |  msvc16  | amd64        | ![Windows msvc16 Build Status](https://biogearsengine.com/content/badges/nightly_libBioGears_windows_msvc16.png) |
| Linux  |  gcc5.4  | amd64 | ![Linux-gcc5.4-amd64 Build Status](https://biogearsengine.com/content/badges/nightly_libBioGears_linux_gcc5.4-core2_64.png) |
| Linux  |  gcc5.4  | armel | ![Linux-gcc5.4-armel Build Status](https://biogearsengine.com/content/badges/nightly_libBioGears_linux_gcc5.4-armel.png) |
| Linux  |  gcc5.4  | aarch64 | ![Linux-gcc5.4-aarch64 Build Status](https://biogearsengine.com/content/badges/nightly_libBioGears_linux_gcc5.4-aarch64.png) |
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
Refer to our GitHub Wiki for build requirments [here](https://github.com/BioGearsEngine/core/wiki)


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
  - `biogears-common` - Helper framework for c++ with no third party dependeincies
  - `bg-cli` - Primary Command Line Utility for using BioGears 
  - `howto/` - libbiogears SDK examples
  - `java/`  - JNI utilities for running biogears
  - `unit/`  - Unit test harness used for testing libbiogears components
  - `zip/`   - Self hosted copy of https://github.com/nmoinvaz/minizip. A bg-cli third part dep
  
biogears
biogears-common
circuit_profiler
bg-cli
howto
java
test_driver
unit
zip

## What's new in ver 7.3 (January 30, 2020)
- Custom Compound Infusion 
- Respiratory Improvments and Tuning
- Improved website generation targets
- Updates to CMD_BIO executable for batch validation and scenario execution
- Website generation python preprocess tool 
- Transmucosal Fentanyl implementation
  - oral diffusion through the mucosa layer and a model for GI transit and small intestine absorption
- Hemorrhage model updates 
  - Direct model connection to energy and nutrient model 
- Tourniquet action for use with Hemorrhage scenarios
- **New drug:** Tranexamic Acid
  - Validated for use in a hemorrhage scenario to decrease bleed rates 
- Propofol validation and bug fixes
- Expansion of the cerebral circuit to a larger, more complex system
  - Added cerebral auto-regulation, hemorrhage, and updated TBI model
- **New drugs:** Moxifloxacin and Ertapenem (intra-venous and intra-arterial)
  - Used in sepsis model, validated for treatment guidelines
- Validation updates to Fentanyl drug
  - Updated tissue volumes, updated perfusion limited diffusion method, and updates to PK values  
- New override functionality
  - Heart rate, respiration rate, and blood pressure values may now be set while running a conformant engine
    - Other physiology will change in collaboration with these values to accurately change major physiology during runtime without an injury or action
- New whole blood model/substance 
  - Antigens, agglutination model and typed blood substance files for administration during runtime
- **New drug administration route:** oral tablet 
  - Antibiotic with validated PK profiles and infection interactions
  - May be used in sepsis scenarios for management of infection 
  - New future tylenol drug for moderate pain management 
- python plotter util updates to batch plot csv files generated by BioGears
- New complex How-to files covering burn and sepsis, for developers use
- Modifications to SE classes to support Unreal Engine integration
- Updated website generation and layout
- Patient blood type added as patient parameter
- General CMAKE updates to the build system 





## What's new in ver 7.2 and 7.2.1 (January 29, 2019)

- General bug fixes and updates
  - Finalization for testing and implementation to BioGears override functionality with full physiology request data support
- Arterial and Venous PH data requests 
- Inflammation state data to support sepsis model serialization 
- Generalized sepsis model to a more generic inflammation model 
  - Will be critical to future modeling efforts (hemorrhage, burn, infection)
- New example sepsis xml files (SepsisSevere_Gut.xml)
- New lymph circuit
  - Handles Albumin transport and re-circulation 
  - Creates realistic oncotic pressure sources for substance transport 
  - Transport from tissue systems back into the vasculature via lymph 
- New command line utility project (bg-cli) for native c++ runtime, driver, batch run organizer/manager
- Optional name value for xml actions meal and environment
- New burn model 
  - User defined total body surface area input 
  - Inflammation cascade validated for long running scenarios (24 hr +)
  - Validated for traditional treatment protocols with USISR SMEs
- New unit testing framework (Google Test) to better support multi-platform functionality 
  - Unit Test harness is a separate project in CMAKE which can be controlled with Biogears_BUILD_TEST variable
- Introduced const char* DLL interfaces for all functions dealing with std::string to avoid Windows-related issues dealing with XSD implicitly exporting string through inheritance
- Updated functionality to tension pneumothorax to fix bug in bilateral behavior 
- Updated hemorrhage bugs to update blood gas levels and metabolic requirements 
  - Validated with University of Washington


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


