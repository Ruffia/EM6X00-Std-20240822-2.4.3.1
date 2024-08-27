@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by XXX2.HPJ. >"hlp\XXX2.hm"
echo. >>"hlp\XXX2.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\XXX2.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\XXX2.hm"
echo. >>"hlp\XXX2.hm"
echo // Prompts (IDP_*) >>"hlp\XXX2.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\XXX2.hm"
echo. >>"hlp\XXX2.hm"
echo // Resources (IDR_*) >>"hlp\XXX2.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\XXX2.hm"
echo. >>"hlp\XXX2.hm"
echo // Dialogs (IDD_*) >>"hlp\XXX2.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\XXX2.hm"
echo. >>"hlp\XXX2.hm"
echo // Frame Controls (IDW_*) >>"hlp\XXX2.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\XXX2.hm"
REM -- Make help for Project XXX2


echo Building Win32 Help files
start /wait hcw /C /E /M "hlp\XXX2.hpj"
if errorlevel 1 goto :Error
if not exist "hlp\XXX2.hlp" goto :Error
if not exist "hlp\XXX2.cnt" goto :Error
echo.
if exist Debug\nul copy "hlp\XXX2.hlp" Debug
if exist Debug\nul copy "hlp\XXX2.cnt" Debug
if exist Release\nul copy "hlp\XXX2.hlp" Release
if exist Release\nul copy "hlp\XXX2.cnt" Release
echo.
goto :done

:Error
echo hlp\XXX2.hpj(1) : error: Problem encountered creating help file

:done
echo.
