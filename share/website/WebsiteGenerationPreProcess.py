import zipfile
import glob, os
import shutil

directories = ['/Scenarios/Patient',
               '/Scenarios/Validation',
               '/Scenarios/Showcase',
               '/Scenarios/EnergyEnvironment',
               '/Scenarios/Compartments',
               '/Scenarios/Miscellaneous',
               '/Scenarios/Combined',
               '/Scenarios/ConsumeMeal',
               '/Scenarios/Drug',
               '/Scenarios/AnesthesiaMachine']

runtime_dir = os.path.dirname(os.path.realpath(__file__))
os.chdir(runtime_dir)
print(runtime_dir)

for dir in directories:
     os.chdir(runtime_dir + dir)
     dir_path = os.path.dirname(os.path.realpath(__file__))
     print(dir_path)
     if not os.path.exists('baselines'):
          os.mkdir('baselines')
     for file in glob.glob('*.csv'):
          print("  "+file)
          zip_name = 'baselines/' + file.split('.')[0] + '.zip'
          file_name = file
          zipfile.ZipFile(zip_name,mode='w').write(file)

print("")
os.chdir(runtime_dir + "/Scenarios")
print(os.path.dirname(os.path.realpath(__file__)))
for file in glob.glob(os.path.join(runtime_dir,'/Scenarios/Drug/baselines/','*.*')):
     print(file)
     shutil.copy(file,runtime_dir+'/Scenarios/Validation/baselines')

print("")
os.chdir(runtime_dir)
for file in glob.glob(os.path.join(runtime_dir,'*PatientValidationTable.md')):
     print(file)
     shutil.copy(file,runtime_dir+'/validation')
     print("")

os.chdir(runtime_dir)
for file in glob.glob(os.path.join(runtime_dir,'*ValidationResults.zip')):
     print(file)
     shutil.copy(file,runtime_dir+'/Scenarios/Validation/baselines')
     print("")

if not os.path.exists('UnitTests'):
     os.mkdir('UnitTests')

os.chdir(runtime_dir+'/UnitTests')     
if not os.path.exists('BioGearsTests'):
     os.mkdir('BioGearsTests')

os.chdir(runtime_dir+'/UnitTests/BioGearsTests')
if not os.path.exists('baselines'):
     os.mkdir('baselines')

os.chdir(runtime_dir)
shutil.copy('RenalTGFFeedbackOutput.zip',runtime_dir+'/UnitTests/BioGearsTests/baselines')
shutil.copy('SimpleDiffusionFourCompartmentTest.zip',runtime_dir+'/UnitTests/BioGearsTests/baselines')
shutil.copy('RespiratoryValidationPFT@120.02s.xml',runtime_dir+'/Scenarios/Validation')

print("Preprocess successful")