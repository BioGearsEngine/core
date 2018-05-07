@echo off
set cp=
set cp=%cp%.\BioGears.jar;

set JAVA_OPTS=-Xmx900m
set JAVA_OPTS=%JAVA_OPTS% -classpath %cp%
echo Using JAVA OPTS: %JAVA_OPTS%


PATH=%PATH%;%PATHFOLDER%

start "" "%JAVA_HOME%\bin\javaw.exe" %JAVA_OPTS% mil.tatrc.physiology.biogears.gui.BioGearsGUI