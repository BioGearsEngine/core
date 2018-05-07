@echo off
set cp=
set cp=%cp%.\BioGears.jar;
set cp=%cp%..\lib\poi-3.13-20150929.jar;
set cp=%cp%..\lib\poi-ooxml-3.13-20150929.jar;
set cp=%cp%..\lib\poi-ooxml-schemas-3.13-20150929.jar;
set cp=%cp%..\lib\xmlbeans-2.6.0.jar;
set cp=%cp%..\lib\jdom-2.0.2.jar;
set cp=%cp%..\lib\reflections-0.9.9-RC1-uberjar.jar;

set JAVA_OPTS=-Xmx900m
set JAVA_OPTS=%JAVA_OPTS% -classpath %cp%
echo Using JAVA OPTS: %JAVA_OPTS%


PATH=%PATH%;%PATHFOLDER%

"%JAVA_HOME%\bin\java.exe" %JAVA_OPTS% mil.tatrc.physiology.datamodel.dataset.DataSetReader