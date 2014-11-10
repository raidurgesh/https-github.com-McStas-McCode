; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "McXtrace Application bundle"
#define MyAppVersion "@VERSION@"
#define MyAppPublisher "McXtrace"
#define MyAppURL "http://www.mcxtrace.org"
#define MyAppExeName "setup.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{9EB3C862-0C7C-489E-841F-76B6555E580A}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
CreateAppDir=no
LicenseFile=license_etc\COPYING.rtf
InfoBeforeFile=license_etc\Description.rtf
InfoAfterFile=license_etc\Description.rtf
OutputBaseFilename=setup
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Files]
Source: "Support\strawberry-perl-5.18.2.1-32bit.msi"; DestDir: "{tmp}"
Source: "Support\PPDs.zip"; DestDir: "{tmp}"
Source: "Support\unzip.exe"; DestDir: "{tmp}"
Source: "Support\unzip32.dll"; DestDir: "{tmp}"
Source: "dist\mcxtrace-NSIS-@VERSION@-mingw32.exe"; DestDir: "{tmp}"
Source: "dist\mcxtrace-comps-NSIS-@VERSION@-mingw32.exe"; DestDir: "{tmp}"
Source: "dist\mcxtrace-tools-perl-NSIS-@VERSION@-mingw32.exe"; DestDir: "{tmp}"
Source: "dist\mcxtrace-tools-python-mxrun-NSIS-@VERSION@-mingw32.exe"; DestDir: "{tmp}"
Source: "dist\mcxtrace-tools-python-mxplot-chaco-NSIS-@VERSION@-mingw32.exe"; DestDir: "{tmp}"
Source: "dist\mcxtrace-tools-python-mxplot-matplotlib-NSIS-@VERSION@-mingw32.exe"; DestDir: "{tmp}"
Source: "dist\mcxtrace-tools-python-mxdisplay-NSIS-@VERSION@-mingw32.exe"; DestDir: "{tmp}"

[Run]
Filename: "msiexec"; Parameters: "/i {tmp}\strawberry-perl-5.18.2.1-32bit.msi"
Filename: "{tmp}\unzip.exe"; Parameters: "{tmp}\PPDs.zip"
Filename: "{tmp}\PPDs\postsetup.bat"
Filename: "{tmp}\mcxtrace-NSIS-@VERSION@-mingw32.exe"; Parameters: "/S"
Filename: "{tmp}\mcxtrace-comps-NSIS-@VERSION@-mingw32.exe"; Parameters: "/S"
Filename: "{tmp}\mcxtrace-tools-perl-NSIS-@VERSION@-mingw32.exe"; Parameters: "/S"
Filename: "{tmp}\mcxtrace-tools-python-mxrun-NSIS-@VERSION@-mingw64.exe"; Parameters: "/S"
Filename: "{tmp}\mcxtrace-tools-python-mxplot-chaco-NSIS-@VERSION@-mingw64.exe"; Parameters: "/S"
Filename: "{tmp}\mcxtrace-tools-python-mxplot-matplotlib-NSIS-@VERSION@-mingw64.exe"; Parameters: "/S"
Filename: "{tmp}\mcxtrace-tools-python-mxdisplay-NSIS-@VERSION@-mingw64.exe"; Parameters: "/S"
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

