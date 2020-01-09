import zipfile
import glob, os
import shutil
import sys

"""
Author: Lucas Marin
Date: 4/24/2019
So we're currently phasing out and replacing a lot of Java code that helped run biogears, however for website
generation we still rely on a big chunk of java code that calls C++ hooks, if you want to see precisely which
Java functions are called, take a look at biogears/core/projects/java/biogears/CMakeLists.txt, ctrl+f 'gen-website-docs'.

The point of this file is to facilitate generating our website by zipping/moving files to the place
where doxygen expects to find them.

This file lives in core/share/website, if you want to modify it you should modify it there, not in runtime, then 
configure/generate with CMake and build. 
"""
help_message =  """
  This is a utility which moves files into their expected location for biogears documentation generation.
  Proper usage is:

  python WebsiteGenerationPreProcess.py <INDIRECTORY> <OUTDIRECTORY>

  <INDIRECTORY> is the location where the files to be moved are stored, for a standard installation
  of biogears this should be 'core/share/website'
  <OUTDIRECTORY> is the location where the actual preprocessing occurs, for a standard installation
  of biogears this should be 'core/build/runtime'
  Note that these should both be relative paths

  If you are running WebsiteGenerationPreProcess.py from 'core/share/website' the command would be
  python WebsiteGenerationPreProcess.py . ../../build/runtime

  If you are running WebsiteGenerationPreProcess.py from 'core/build/runtime' the command would be
  python WebsiteGenerationPreProcess.py . ../../share/website 
"""

error_message_argc = """
  ERROR: Incorrect number of arguments
"""

warning_message = """
  WARNING: You have not selected 'runtime' as your working directory. 
  This program assumes that 'runtime' will be the working directory. 
  Running it somewhere else could have unforeseen consequences.
  Do you still want to run the program? (y/n)
"""

# This script places temporary files in the correct directory structure for website generation
# Every file in this folder represents a gap in our website generation
# The goal should be to patch the build system overtime to remove the need for each file
# Remove the file from the directory once it is no longer needed.

def preprocess(sourceDir,destination):
  scenario_directories = ['Scenarios/Patient',
                 'Scenarios/Validation',
                 'Scenarios/Showcase',
                 'Scenarios/EnergyEnvironment',
                 'Scenarios/Compartments',
                 'Scenarios/Miscellaneous',
                 'Scenarios/Combined',
                 'Scenarios/ConsumeMeal',
                 'Scenarios/Drug',
                 'Scenarios/AnesthesiaMachine'] 
  source_dir_abs = os.path.abspath(sourceDir)
  dest_dir_abs = os.path.abspath(destination)

  source_map  = [
     ("baselines/CardiovascularCircuitOutput.zip", "UnitTests/BioGearsTests/baselines")
     ,("validation/RespiratoryCompartmentsValidationTable.md", "doc/validation")
     ,("validation/RespiratoryValidationTable.md", "doc/validation")
     ,("validation/UrinalysisValidationTable.md", "doc/validation")
     ,("validation/patient/BradycardicPatientValidationTable.md", "doc/validation")
     ,("validation/patient/CarolPatientValidationTable.md", "doc/validation")
     ,("validation/patient/CynthiaPatientValidationTable.md", "doc/validation")
     ,("validation/patient/DefaultFemalePatientValidationTable.md", "doc/validation")
     ,("validation/patient/DefaultMalePatientValidationTable.md", "doc/validation")
     ,("validation/patient/ExtremeFemalePatientValidationTable.md", "doc/validation")
     ,("validation/patient/ExtremeMalePatientValidationTable.md", "doc/validation")
     ,("validation/patient/GusPatientValidationTable.md", "doc/validation")
     ,("validation/patient/HassanPatientValidationTable.md", "doc/validation")
     ,("validation/patient/JanePatientValidationTable.md", "doc/validation")
     ,("validation/patient/JeffPatientValidationTable.md", "doc/validation")
     ,("validation/patient/JoelPatientValidationTable.md", "doc/validation")
     ,("validation/patient/NathanPatientValidationTable.md", "doc/validation")
     ,("validation/patient/OverweightPatientValidationTable.md", "doc/validation")
     ,("validation/patient/SoldierPatientValidationTable.md", "doc/validation")
     ,("validation/patient/StandardFemalePatientValidationTable.md", "doc/validation")
     ,("validation/patient/StandardMalePatientValidationTable.md", "doc/validation")
     ,("validation/patient/TachycardicPatientValidationTable.md", "doc/validation")
     ,("validation/patient/UnderweightPatientValidationTable.md", "doc/validation")
  ]
  if os.path.basename(dest_dir_abs) != 'runtime':
    print(warning_message)
    while True:
      ans = input()
      if (ans.lower() == 'y') or (ans.lower() == 'yes'):
        break
      elif (ans.lower() == 'n') or (ans.lower() == 'no'):
        return
      else:
        print(" Input not recognized, enter 'yes' if you still want to run the program, or 'no' if you would like to abort.")

  for pair in source_map:
    source = os.path.normpath(os.path.os.path.join(source_dir_abs,pair[0]))
    target = os.path.normpath(os.path.join(dest_dir_abs,pair[1]))
    if not os.path.exists(target):
      print("Creating %s" % target)
      os.makedirs(target, exist_ok=True)
    elif os.path.exists(target) and not os.path.isdir(target):
      print ("Error %s is a file not a directory" % target)
    print("Copy %s => %s" % (source,target))
    shutil.copy(source, target);

if __name__ == "__main__":
  if ( (len(sys.argv) == 1) or ( (sys.argv[1].lower() == '--help') or (sys.argv[1].lower() == 'help') ) ):
    print(help_message)
    exit()
  if len(sys.argv) != 3:
    print(error_message_argc + help_message)
    exit()
  preprocess(sys.argv[1],sys.argv[2])
