@echo off
rem *************************************************************************************************
rem   usage: DolphinUTBuildAndTest.bat  Action-BuildOption
rem          Action:
rem             Clean-All     clean DolphinTest solution (Debug and Release)
rem             Clean-Debug   clean DolphinTest solution (Debug)
rem             Clean-Release clean DolphinTest solution (Release)
rem             Build-All     build DolphinTest solution (Debug and Release)
rem             Build-Debug   build DolphinTest solution (Debug)
rem             Build-Release build DolphinTest solution (Release)
rem             Test-All      clean and build MediaEngine and DolphinTest solution (Debug and Release)
rem                           launch DopthinTest App and get test result(Debug and Release)
rem             Test-Debug    clean and build MediaEngine and DolphinTest solution (Debug)
rem                           launch DopthinTest App and get test result(Debug)
rem             Test-Release  clean and build MediaEngine and DolphinTest solution (Release)
rem                           launch DopthinTest App and get test result(Release)
rem             Help          show usage info
rem   2015/03/15 huashi@cisco.com
rem *************************************************************************************************



cd %WORKSPACE%\%SourceDir%

cd %WORKSPACE%



:BasicSetting
  set WORKSPACE=%cd%
  set WorkingDir=..
  set SourceDir=CodecCisco
  set tmplibs=AutoBin
  set Configuration=Debug
  set GasScriptGitAddr=
  set GitExePath=C:\Users\Openh264\AppData\Local\GitHub\PortableGit_c2ba306e536fdf878271f7fe636a147ff37326ad\bin
  set PATH=%GitExePath%;%PATH%
goto :EOF

:VC2013Setting
  set VC2013Path=C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin
  set MinGWBinPath=C:\MinGW\bin
  set MinGWMsysPath=C:\MinGW\msys\1.0\bin
 
  set PATH=C:\MinGW\bin;C:\MinGW\msys\1.0\bin;C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin;%PATH%
  
  call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" x86_arm
  rem LibPathSetting
  set WP8LibPath=C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\lib\arm
  set VC2013ArmLibPath=C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\lib\store\arm;C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\lib\arm
  set set LIB=%VC2013ArmLibPath%;%WP8LibPath% 
 
  
goto :EOF

:EnvironmentCheck
WindowsPhoneCore.lib


goto :EOF


:GasScript

goto :EOF

:CleanAndBuild
  rem arm release
  rem wp_arm debug
  bash -c "make OS=msvc-wp ARCH=arm USE_ASM=Yes clean"
  bash -c "make OS=msvc-wp ARCH=arm USE_ASM=Yes BUILDTYPE=%Configuration%" 
goto :EOF


:CopyDll
  bash -c "mkdir -p %tmplibs%/wp_arm/%Configuration%"
  bash -c "cp -f *.pdb %tmplibs%/wp_arm/%Configuration%"
  bash -c "cp -f openh264.dll %tmplibs%/wp_arm/%Configuration%"
  bash -c "cp -f openh264.lib %tmplibs%/wp_arm/%Configuration%"
goto :EOF


