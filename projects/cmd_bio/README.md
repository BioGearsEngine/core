## New Command Line Interface
```
cmd_bio [GENDATA] [GENSTATES] [VERIFY]
        [THREADS N]
        [VALIDATE] [ [drug|patient|system|validation|all ]...]
        [TEST [cdm bge]...] [SCENARIO [FILE] ]
```

This project is a command line implementation of BioGears for processing Scenario files it also had the ability to run Circuit Test  and control threading
 
**GENDATA**: Generate Data XMLs from CSV files located in the runtime directory.

**GENSTATES**: Generate Stabalized Patient States for all Patients

**VALIDATE** Run one ore more validate senarios arguments include drug,patient,system or all. Where `VALIDATE all` is the same as providing the following `VALIDATE drug patient system VERIFY`

**VERIFY**: Run Verification.config

**THREADS**: Takes and Integer to control the number of threads the pool will run when running multiple scnearios. 

**TEST**: Run a series of predetermined Circuit Test.  arguments are cdm or bge

**SCENARIO**: Takes a number of files which will be loaded and run. Global settings in the files will overwrite previous entires.

The system will execute as many command line arguments as you give in a predetermined order

***Examples*** 

cmd_bio THREADS 1 GENDATA GENSTATES  -- Generate Data and States
cmd_bio TEST cdm VALIDATE all -- Run CDM Test followed by all validations
cmd_bio SCENARIO RunBGETest.config -- Readin RunBGETest.config and process it 
