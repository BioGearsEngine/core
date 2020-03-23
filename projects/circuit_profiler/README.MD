
## New Command Line Interface
```
circuit_profiler
```

This project is for internal benchmarking.

***Examples*** 

cmd_bio THREADS 1 GENDATA GENSTATES  -- Generate Data and States
cmd_bio TEST cdm VALIDATE all -- Run CDM Test followed by all validations
cmd_bio SCENARIO RunBGETest.config -- Read in RunBGETest.config and process it 

## Help
The BioGears build system provides a helpful STAGE target to run cmakes fixup_bundle utility for consolidating all external DLLs to a single runtime directory. This is inteneded to assist users in avoiding putting all of their third party development DLLs in their system path. 

After BioGears has completed building run the STAGE target one time to ensure all the third party DLLS will be moved in to the output directory before attempting to launch the application. This functionality should only needed to be run once per build configuration.
