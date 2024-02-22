Overview
==========

BioGears® source code is hosted here and will be developed through this public repository to encourage community facing development. In addition, we also support official stable releases. Our latest deployment (found under releases) is still in a beta phase, and is intended to be an intermediate release to showcase the capabilities of the BioGears® Physiology Engine. The current version of the software includes examples of all types of engine interfaces, but does not include all of the functionality or physiologic systems that will be present in the final product. This version of the software is meant to elicit feedback and enhance community involvement in establishing end product expectations.


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
  - `data/` - BioGears configuration files for various scenarios 
  - `doc/` - BioGears documentation files (doxygen, markdown, ...)
  - `etc/` - Other
  - `xsd/` - XSD definition files for the Common Data Model
- `projects/`
  - `biogears` - Main Engine Source for building libbiogears libbiogears-cdm
    - `swig_bindings` - Templates for generating language bindings using swig 
  - `biogears-common` - Helper framework for c++ with no third party dependencies
  - `bg-cli` - Primary Command Line Utility for using BioGears 
  - `howto/` - libbiogears SDK examples
  - `unit/`  - Unit test harness used for testing libbiogears components
  - `zip/`   - Self hosted copy of https://github.com/nmoinvaz/minizip. A bg-cli third part dep
  


Citing BioGears 
------------
If you'd like to use BioGears in your research, we ask that you cite the following publication:

[![DOI](https://joss.theoj.org/papers/10.21105/joss.02645/status.svg)](https://doi.org/10.21105/joss.02645)

We have made available to the medical simulation community a large set of burn injury physiology data, hosted on Zenodo. We have listed this dataset under the creative commons 4.0 license so that it be used for any reason you may deem fit. We only ask that any results properly cite the above article and the database DOI: 

[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.4606078.svg)](https://doi.org/10.5281/zenodo.4606078)

## Release Notes 

### 8.0.0 (November 2024)
- Modified Scenario CDM to require <ACTIONS> Tag around individual actions
- Allowed import of external ActionList and DataRequestList
- Updated CMAKE to minimum 3.20
- Improved various features

### 7.6.0 (December 2023)
- Improved SWIG bindings
- Improved Various Models


Programmatics
===============
BioGears is partially funded through DARPA under   HR0011-23-C-0129
Originally developed under the TATRC funded project: W81XWH-13-2-0068
it has additionally been developed under the following contracts:
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


