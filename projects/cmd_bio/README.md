## New Command Line Interface
```
cmd_bio [HELP, GENDATA, GENSTATES, GENSEPSIS, VERIFY, VERSION]
        [THREADS N]
        [CONFIG [FILE]...], [SCENARIO [FILE]...], [TEST cdm|bge], [VALIDATE patient|drug|system|all]
        [GENTABLES html|md|xml|web|all]
```

This project is a command line implementation of BioGears for processing Scenario files it also had the ability to run Circuit Test  and control threading

**HELP**: Print out help dialog.
 
**GENDATA**: Generate data XMLs from CSV files located in the runtime directory.

**GENSTATES**: Generate stabilized patient states for all patients.

**GENSEPSIS**: Generate stabilized sepsis states at various levels of infection and progression.

**VERIFY**: Run VerificationScenarios.config.

**VERSION**: Print out version string.

**THREADS**: Takes an integer to control the number of threads the pool will use when running multiple scenarios. 

**CONFIG**: Take a number of config files which will be loaded and run.

**SCENARIO**: Takes a number of scenario files which will be loaded and run. Global settings in the files will overwrite previous entries.

**TEST**: Run a series of predetermined Circuit Test. Arguments are cdm or bge.

**VALIDATE** Run one ore more validate senarios arguments include drug,patient,system or all. Where `VALIDATE all` is the same as providing the following `VALIDATE drug patient system VERIFY`.

**GENTABLES**: Generate validation tables for BioGears documentation. Inputs are html, md, xml, web, or all.

The system will execute as many command line arguments as you give in a predetermined order. HELP or VERSION however will immediately stop a run if they are included in the arguments.

***Examples*** 

cmd_bio THREADS 1 GENDATA GENSTATES  -- Generate Data and States
cmd_bio TEST cdm VALIDATE all -- Run CDM Test followed by all validations
cmd_bio SCENARIO RunBGETest.config -- Read in RunBGETest.config and process it 

## Help
The BioGears build system provides a helpful STAGE target to run cmakes fixup_bundle utility for consolidating all external DLLs to a single runtime directory. This is inteneded to assist users in avoiding putting all of their third party development DLLs in their system path. 

After BioGears has completed building run the STAGE target one time to ensure all the third party DLLS will be moved in to the output directory before attempting to launch the application. This functionality should only needed to be run once per build configuration.
