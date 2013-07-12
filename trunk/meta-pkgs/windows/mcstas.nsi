#    This file is part of the McStas neutron ray-trace simulation package
#    Copyright (C) 1997-2004, All rights reserved
#    Risoe National Laborartory, Roskilde, Denmark
#    Institut Laue Langevin, Grenoble, France
#
#    This program is free software; you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation; version 2 of the License.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program; if not, write to the Free Software
#    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
#    mcstas.nsi: input file for the McStas NullSoft installer
#

!define TEMP1 $R0

!ifndef VERSION
  !define VERSION "@MCCODE_VERSION@"
!endif

 
!include MUI.nsh
!include Sections.nsh
 
Name "McStas plus tools"
OutFile "McStas-MetaPackage-${VERSION}-i686-Win32.exe"

##===========================================================================
## Modern UI Pages
##===========================================================================

!define MUI_WELCOMEFINISHPAGE_BITMAP "mcstas.bmp" 
!define MUI_WELCOMEPAGE_TEXT "This wizard will guide you through the installation of \
McStas release ${VERSION} on Windows systems.\
\n\nMcStas is a ray-tracing Monte Carlo neutron simulator (see <http://www.mcstas.org>).\
\n\nThis installer will set up McStas and support tools on your computer.\
\n\nNOTE: If you have a previous, working installation of McStas on your machine, you should be able\
to skip installation of the 'support tools' (skip by unchecking everything but McStas when prompted).\
\n\nNOTE: Please install all software on C:\ and do NOT USE SPACES in the McStas installation\
directory name."


!insertmacro MUI_PAGE_WELCOME 
!insertmacro MUI_PAGE_LICENSE "LICENSE.rtf"
 
!define MUI_PAGE_CUSTOMFUNCTION_PRE SelectFilesCheck
!define MUI_PAGE_CUSTOMFUNCTION_LEAVE ComponentsLeave
!insertmacro MUI_PAGE_COMPONENTS
 
## This is the title on the first Directory page
#!define MUI_DIRECTORYPAGE_TEXT_TOP "$(MUI_DIRECTORYPAGE_TEXT_TOP_A)"
 
!define MUI_PAGE_CUSTOMFUNCTION_PRE SelectFilesA
#!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES

Page custom SetCustom
 
## This is the title on the second Directory page
!define MUI_DIRECTORYPAGE_TEXT_TOP "$(MUI_DIRECTORYPAGE_TEXT_TOP_B)"
 
!define MUI_PAGE_CUSTOMFUNCTION_PRE SelectFilesB
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
 
!define MUI_PAGE_CUSTOMFUNCTION_LEAVE DeleteSectionsINI

!define MUI_FINISHPAGE_NOAUTOCLOSE
!insertmacro MUI_PAGE_FINISH
 
!insertmacro MUI_LANGUAGE "English"
 
##===========================================================================
## Language strings
##===========================================================================
 
LangString NoSectionsSelected ${LANG_ENGLSH} "You haven't selected any sections!"
 
LangString MUI_DIRECTORYPAGE_TEXT_TOP_B ${LANG_ENGLSH} "Setup will install \
McStas in the following folder... WARNING: Spaces and specials chars are unsupported here!"
 
##===========================================================================
## Start sections
##===========================================================================
 
## Sections Group 1
SectionGroup /e "Support tools" PROG1 

Section "Strawberry Perl"
   SetOutPath "$TEMP"
   File strawberry-perl-5.16.3.1-32bit.msi
   ExecWait "msiexec /i strawberry-perl-5.16.3.1-32bit.msi"
   messagebox mb_ok "Perl installation complete!"
SectionEnd

SectionGroupEnd
 
## Sections Group 2
SectionGroup /e "McStas" PROG2
  
Section "McStas ${VERSION} (required)" MCSTAS
  ; Start by going to the temp folder...
  SetOutPath "$TEMP"
  File mcstas-NSIS-${VERSION}-mingw32.exe
  ExecWait "$TEMP\mcstas-NSIS-${VERSION}-mingw32.exe"
SectionEnd

Section "McStas comps ${VERSION} (required)" MCSTAS
  ; Start by going to the temp folder...
  SetOutPath "$TEMP"
  File mcstas-comps-NSIS-${VERSION}-mingw32.exe
  ExecWait "$TEMP\mcstas-comps-NSIS-${VERSION}-mingw32.exe"
SectionEnd

Section "McStas tools ${VERSION} (required)" MCSTAS
  ; Start by going to the temp folder...
  SetOutPath "$TEMP"
  File mcstas-NSIS-${VERSION}-mingw32.exe
  ExecWait "$TEMP\mcstas-tools-perl-NSIS-${VERSION}-mingw32.exe"
SectionEnd
 
SectionGroupEnd
 
##===========================================================================
## Settings
##===========================================================================
 
#!define PROG1_InstDir    "C:\PROG1"
!define PROG1_StartIndex ${PROG1}
!define PROG1_EndIndex   ${CORT}
 
!define PROG2_InstDir "C:\McStas"
!define PROG2_StartIndex ${PROG2}
!define PROG2_EndIndex   ${WIN}
 
##===========================================================================
## Please don't modify below here unless you're a NSIS 'wiz-kid'
##===========================================================================
 
## Create $PLUGINSDIR
Function .onInit

  ;Extract InstallOptions files
  ;$PLUGINSDIR will automatically be removed when the installer closes
  
  InitPluginsDir
  File /oname=$PLUGINSDIR\mcstas.ini "mcstas.ini"
  
FunctionEnd

Function SetCustom
  !insertmacro MUI_HEADER_TEXT "Support app location" ""
  ;Display the InstallOptions dialog
  Push ${TEMP1}
    InstallOptions::dialog "$PLUGINSDIR\mcstas.ini"
  Pop ${TEMP1}

    ReadINIStr $3 "$PLUGINSDIR\mcstas.ini" "Field 3" "State"
    ReadINIStr $4 "$PLUGINSDIR\mcstas.ini" "Field 5" "State"
    ReadINIStr $5 "$PLUGINSDIR\mcstas.ini" "Field 7" "State"
    ReadINIStr $6 "$PLUGINSDIR\mcstas.ini" "Field 9" "State"

FunctionEnd


## If user goes back to this page from 1st Directory page
## we need to put the sections back to how they were before
Var IfBack
Function SelectFilesCheck
 StrCmp $IfBack 1 0 NoCheck
  Call ResetFiles
 NoCheck:
FunctionEnd
 
## Also if no sections are selected, warn the user!
Function ComponentsLeave
Push $R0
Push $R1
 
 Call IsPROG1Selected
  Pop $R0
 Call IsPROG2Selected
  Pop $R1
 StrCmp $R0 1 End
 StrCmp $R1 1 End
  Pop $R1
  Pop $R0
 MessageBox MB_OK|MB_ICONEXCLAMATION "$(NoSectionsSelected)"
 Abort
 
End:
Pop $R1
Pop $R0
FunctionEnd
 
Function IsPROG1Selected
Push $R0
Push $R1
 
 StrCpy $R0 ${PROG1_StartIndex} # Group 1 start
 
  Loop:
   IntOp $R0 $R0 + 1
   SectionGetFlags $R0 $R1			# Get section flags
    IntOp $R1 $R1 & ${SF_SELECTED}
    StrCmp $R1 ${SF_SELECTED} 0 +3		# If section is selected, done
     StrCpy $R0 1
     Goto Done
    StrCmp $R0 ${PROG1_EndIndex} 0 Loop
 
 Done:
Pop $R1
Exch $R0
FunctionEnd
 
Function IsPROG2Selected
Push $R0
Push $R1
 
 StrCpy $R0 ${PROG2_StartIndex}    # Group 2 start
 
  Loop:
   IntOp $R0 $R0 + 1
   SectionGetFlags $R0 $R1			# Get section flags
    IntOp $R1 $R1 & ${SF_SELECTED}
    StrCmp $R1 ${SF_SELECTED} 0 +3		# If section is selected, done
     StrCpy $R0 1
     Goto Done
    StrCmp $R0 ${PROG2_EndIndex} 0 Loop
 
 Done:
Pop $R1
Exch $R0
FunctionEnd
 
## Here we are selecting first sections to install
## by unselecting all the others!
Function SelectFilesA
 
 # If user clicks Back now, we will know to reselect Group 2's sections for
 # Components page
 StrCpy $IfBack 1
 
 # We need to save the state of the Group 2 Sections
 # for the next InstFiles page
Push $R0
Push $R1
 
 StrCpy $R0 ${PROG2_StartIndex} # Group 2 start
 
  Loop:
   IntOp $R0 $R0 + 1
   SectionGetFlags $R0 $R1				    # Get section flags
    WriteINIStr "$PLUGINSDIR\sections.ini" Sections $R0 $R1 # Save state
    !insertmacro UnselectSection $R0			    # Then unselect it
    StrCmp $R0 ${PROG2_EndIndex} 0 Loop
 
 # Don't install prog 1?
 Call IsPROG1Selected
 Pop $R0
 StrCmp $R0 1 +4
  Pop $R1
  Pop $R0
  Abort
 
 # Set current $INSTDIR to PROG1_InstDir define
 #StrCpy $INSTDIR "${PROG1_InstDir}"
 
Pop $R1
Pop $R0
FunctionEnd
 
## Here we need to unselect all Group 1 sections
## and then re-select those in Group 2 (that the user had selected on
## Components page)
Function SelectFilesB
Push $R0
Push $R1
 
 StrCpy $R0 ${PROG1_StartIndex}    # Group 1 start
 
  Loop:
   IntOp $R0 $R0 + 1
    !insertmacro UnselectSection $R0		# Unselect it
    StrCmp $R0 ${PROG1_EndIndex} 0 Loop
 
 Call ResetFiles
 
 # Don't install prog 2?
 Call IsPROG2Selected
 Pop $R0
 StrCmp $R0 1 +4
  Pop $R1
  Pop $R0
  Abort
 
 # Set current $INSTDIR to PROG2_InstDir define
 StrCpy $INSTDIR "${PROG2_InstDir}"
 
Pop $R1
Pop $R0
FunctionEnd
 
## This will set all sections to how they were on the components page
## originally
Function ResetFiles
Push $R0
Push $R1
 
 StrCpy $R0 ${PROG2_StartIndex}    # Group 2 start
 
  Loop:
   IntOp $R0 $R0 + 1
   ReadINIStr "$R1" "$PLUGINSDIR\sections.ini" Sections $R0 # Get sec flags
    SectionSetFlags $R0 $R1				  # Re-set flags for this sec
    StrCmp $R0 ${PROG2_EndIndex} 0 Loop
 
Pop $R1
Pop $R0
FunctionEnd
 
## Here we are deleting the temp INI file at the end of installation
Function DeleteSectionsINI
 Delete "$PLUGINSDIR\Sections.ini"
 FlushINI "$PLUGINSDIR\Sections.ini"
FunctionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"

   ; Remove desktop link etc. for all users:
   SetShellVarContext all  

  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\McStas"
  DeleteRegKey HKLM SOFTWARE\McStas

  ; Remove files and uninstaller
  Delete $INSTDIR\uninstall.exe

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\McStas\*.*"

  ; Remove directories and files used
  Delete "$DESKTOP\McStas.lnk"
  RMDir "$SMPROGRAMS\McStas"
  RMDir /R "$INSTDIR"
  

SectionEnd
