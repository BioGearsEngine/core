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
Refer to our github wiki for build requirments [here](https://github.com/BioGearsEngine/core/wiki)


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

## What's new in ver 7.1 (September 26, 2018)

- Patches to drug blood pressure modifications to restrict pathways to be more physiologically accurate
- Vasopressin support and validation
- Major patches to #include requirements, reduction in file dependencies
  - Increases modularity of the project, increase build times during development 
- Change in how we generate code from our CDM XSD files to one file per XSD file instead of per type 
  - Reduced build times for the full source from 40 to 10 min
  - empty constructors in SETypes to = default and adding override markers
  - no longer use stdafx.h while compiling and so many headers make direct reference to COmmonDataModel.h and Biogears.h which were previously bundled in these precompiled headers
- Override functionality now supported in BioGears 
  - May override any physiology data request with desired value 
  - Logging will document range of possible values if typing unsupported data 
  - Engine can now be globally flagged as conformant or non conformant to increase future development possibilities
  - Can be manipulated via action api calls 
  - Example xmls and sdks demonstrate functionality 
- Moved all BioGears functionality in to the BioGears namespace


## What's new in ver 7.0 (August 22, 2018)

- BioGears python plotting tool
- Max work rate now a patient parameter and is configurable
- Hemorrhage action updates, may now specify location and rate
  - Rate will diminish as pressure in the vessel decreases 
- Update build process to be entirely supported by CMAKE
  - Removed Apache Ant dependency 
  - Updated build directory and runtime directory dependencies
- Full build support for arm platforms 
 - Updates to source to support all major platforms: mac, win, linux, and Arm 
- Updated build architecture to python buildbot libraries 
  - 8 concurrent nightly builds to ensure multi-platform support
- Setup mirroring onto our new github repository 
  - All development now open to the community with feature branches also supporting nightly builds 
- Dockerfile and testing/support now supported, see more at https://cloud.docker.com/u/biogears/repository/docker/biogears/engine
- Pain model and patient pain susceptibility configuration flag
  - Validated pain model supported, stimulus can be specified with severity from 0-1
  - Works with all supported pain medication in the BioGears engine
    - Treat patient with Morphine, Fentynal, and/or Ketamine
  - New How-to-pain file to display sdk support
- Sepsis model 
  - Robust whole body inflammation model with severity and location specifiers in .xml and SDK
  - New How-to-sepsis file to show sdk functionality (command-line tool) 
  - Validated treatments with fluid resuscitation guidelines, vasopressin, norepinephrine, and antibiotics 
  - Validated blood chemistry markers such as bilirubin, white blood cell count, and lactate
- New antibiotic IV drip 
  - Can be used to treat sepsis
- Two new supported patients: toughguy and toughgirl 
- Sweat rate patches now meeting validation 
  - Better core temperature regulation during exercise 
  - Hyper/hypo-hidrosis now a supported patient parameter
- Updates and new 7.0 java GUI release to support users who want to create their own substance 
  - Includes ability to patch in new drugs
- Chemoreceptor method updated to track validation for hypercapnic and hypoxic conditions 
  - Better support for respiratory validation across the board, particularly supported respiratory conditions
- Patches to saline infusion loading on the patient for better respiratory validation 




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


