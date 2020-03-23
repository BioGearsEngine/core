## New Command Line Interface
```
scenario_builder [FILE]
```
scenario_builder executes proper BioGears Scenario File Formats. Several examples are listed in the Scenario folder in your runtime directory after compilation. For Legacy purposes scenario_builder auto appends Scenarios/ to all strings passed to it and assumes relative URIs

## Help
The BioGears build system provides a helpful STAGE target to run cmakes fixup_bundle utility for consolidating all e    xternal DLLs to a single runtime directory. This is inteneded to assist users in avoiding putting all of their third     party development DLLs in their system path.

After BioGears has completed building run the STAGE target one time to ensure all the third party DLLS will be moved     in to the output directory before attempting to launch the application. This functionality should only needed to be     run once per build configuration.
