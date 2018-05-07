
SET FILE=%1
IF FILE =="" SET FILE=Doxyfile

SET START_PATH=%PATH%
:: Strawberry is for hooking up the references
:: Ghostscript(gs) is used in formula generation
:: You have to have these installed and on your path if you are not using the BioGears repo
:: Also, you should have a flavor of TeX installed on your path (i.e. miketex for windows)
SET PATH=%~dp0..\..\lib\strawberry\perl\bin;%~dp0..\..\lib\gs\bin;%PATH%

:: Don't preprocess if ant called us, ant already did this
IF(%2)==(ant) GOTO GEN

:: Run all of our preprocessing tools
cd ..\..\bin
set cp=./DataModelBindings.jar
set cp=%cp%;./CommonDataModel.jar
set cp=%cp%;../lib/jdom-2.0.2.jar
set cp=%cp%;../lib/jcommon-1.0.16.jar
set cp=%cp%;../lib/log4j-1.2.17.jar
set cp=%cp%;../lib/poi-3.13-20150929.jar
set cp=%cp%;../lib/poi-ooxml-3.13-20150929.jar
set cp=%cp%;../lib/poi-ooxml-schemas-3.13-20150929.jar
set cp=%cp%;../lib/mailapi.jar
set cp=%cp%;../lib/smtp.jar
set cp=%cp%;../lib/xml-apis-1.0.b2.jar
set cp=%cp%;../lib/xmlbeans-2.6.0.jar
set cp=%cp%;../lib/zip4j-1.3.1.jar
set cp=%cp%;../lib/jfreechart-1.0.13.jar
set cp=%cp%;../lib/guava-11.0.2.jar
set cp=%cp%;../lib/reflections-0.9.9-RC1-uberjar.jar
set cp=%cp%;../lib/javassist-3.16.1-GA.jar
set JAVA_OPTS= -classpath %cp%
:: Generate Tables
java.exe %JAVA_OPTS% mil.tatrc.physiology.utilities.testing.validation.SystemValidation
java.exe %JAVA_OPTS% mil.tatrc.physiology.utilities.testing.validation.PatientValidation
:: PostProcess markdown files into a temp location
:: Clean out processed directory
mkdir ..\docs\doxygen\processed_md
cd ..\docs\doxygen\processed_md
del /Q *
cd ..\..\..\bin
:: Clean out markdown directory
mkdir .\markdown
cd .\markdown
del /Q *
cd ..\
java.exe %JAVA_OPTS% mil.tatrc.physiology.datamodel.doxygen.CDM2MD
xcopy /S .\markdown\* ..\docs\doxygen\processed_md
java.exe %JAVA_OPTS% mil.tatrc.physiology.datamodel.doxygen.DoxygenPreprocessor ..\docs\markdown ..\docs\doxygen\processed_md
java.exe %JAVA_OPTS% mil.tatrc.physiology.datamodel.doxygen.DoxygenPreprocessor ..\docs\methodology ..\docs\doxygen\processed_md
java.exe %JAVA_OPTS% mil.tatrc.physiology.datamodel.doxygen.XSDToDoxygen ..\src\schema\xsd\BioGearsDataModel.xsd ..\docs\doxygen\processed_md\xsd.ixx
java.exe -Xmx900m %JAVA_OPTS% mil.tatrc.physiology.utilities.csv.plots.PlotDriver PlotRun.config
cd ..\docs\Doxygen

:GEN
:: Call Doxygen
call clean.bat
%~dp0/../../lib/doxygen/bin/doxygen %FILE%



set PATH=%START_PATH%