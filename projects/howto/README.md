## HowTo Examples
This directory is a list of howto's for the BioGears API. Several of the examples are short examples of common actions in BioGears. A few are more complete integration examples for the API and threading control.  

TODO: Add README.MD to each howto

## Help
The BioGears build system provides a helpful STAGE target to run cmakes fixup_bundle utility for consolidating all external DLLs to a single runtime directory. This is inteneded to assist users in avoiding putting all of their third party development DLLs in their system path. 

After BioGears has completed building run the STAGE target one time to ensure all the third party DLLS will be moved in to the output directory before attempting to launch the application. This functionality should only needed to be run once per build configuration.
