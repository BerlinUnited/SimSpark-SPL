;
; This is an Inno Setup Script for SimSpark. Use the Inno Compiler
; available at http://www.jrsoftware.org/isinfo.php to create a
; setup.exe for Windows. This script uses the Inno preprocessor addon
; that has to be installed additionally
;

#define MyAppName "SimSpark"
#define MyAppVerName "SimSpark"
#define MyAppPublisher "RoboCup Soccer Server 3D Maintenance Group"
#define MyAppURL "http://sourceforge.net/projects/simspark"
#define MyAppExeName "rsgedit.exe"
#define MyVcReleaseDir "..\vcrelease"
#define MyRsgEditDir "..\..\contrib\rsgedit"
#define MyZeitgeistDir "..\zeitgeist"
#define MyOxygenDir "..\oxygen"
#define MyKerosinDir "..\kerosin"
#define MySparkDir "..\spark"
#define MyRsgDir "..\..\simulations\parts\rsg"

[Setup]
AppName={#MyAppName}
AppVerName={#MyAppVerName}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\simspark
DefaultGroupName={#MyAppName}
DisableProgramGroupPage=yes
LicenseFile=..\COPYING
OutputBaseFilename=setup
Compression=lzma
SolidCompression=yes

[Languages]
Name: english; MessagesFile: compiler:Default.isl

[Tasks]
Name: desktopicon; Description: {cm:CreateDesktopIcon}; GroupDescription: {cm:AdditionalIcons}; Flags: unchecked
Name: quicklaunchicon; Description: {cm:CreateQuickLaunchIcon}; GroupDescription: {cm:AdditionalIcons}; Flags: unchecked

[Files]
; applications
Source: {#MyVcReleaseDir}\rsgedit.exe; DestDir: {app}; Flags: ignoreversion
Source: {#MyVcReleaseDir}\agentspark.exe; DestDir: {app}; Flags: ignoreversion

; plugins
Source: {#MyVcReleaseDir}\inputwx.dll; DestDir: {app}; Flags: ignoreversion
Source: {#MyVcReleaseDir}\rubysceneimporter.dll; DestDir: {app}; Flags: ignoreversion
Source: {#MyVcReleaseDir}\sexpparser.dll; DestDir: {app}; Flags: ignoreversion
Source: {#MyVcReleaseDir}\filesystemstd.dll; DestDir: {app}; Flags: ignoreversion
Source: {#MyVcReleaseDir}\soccer.dll; DestDir: {app}; Flags: ignoreversion
Source: {#MyVcReleaseDir}\sparkagent.dll; DestDir: {app}; Flags: ignoreversion
Source: {#MyVcReleaseDir}\sceneeffector.dll; DestDir: {app}; Flags: ignoreversion

; ruby scripts
Source: {#MyZeitgeistDir}\zeitgeist.rb; DestDir: {app}; Flags: ignoreversion
Source: {#MyOxygenDir}\oxygen.rb; DestDir: {app}; Flags: ignoreversion
Source: {#MyKerosinDir}\kerosin.rb; DestDir: {app}; Flags: ignoreversion
Source: {#MySparkDir}\spark.rb; DestDir: {app}; Flags: ignoreversion
Source: {#MyRsgEditDir}\bindings.rb; DestDir: {app}; Flags: ignoreversion
Source: {#MyRsgEditDir}\german.scan.rb; DestDir: {app}; Flags: ignoreversion
Source: {#MyRsgEditDir}\rsgedit.rb; DestDir: {app}; Flags: ignoreversion

; rsg files
Source: {#MyRsgDir}\*.rsg; DestDir: {app}\rsg; Flags: recursesubdirs
Source: {#MyRsgDir}\*.rb; DestDir: {app}\rsg; Flags: recursesubdirs

; support dlls
Source: C:\ode\lib\releasedll\ode.dll; DestDir: {app}; Flags: ignoreversion
Source: C:\ruby\bin\msvcrt-ruby18.dll; DestDir: {app}; Flags: ignoreversion

[Icons]
Name: {group}\{#MyAppName}; Filename: {app}\{#MyAppExeName}; WorkingDir: {app}
Name: {group}\{cm:ProgramOnTheWeb,{#MyAppName}}; Filename: {#MyAppURL}
Name: {group}\{cm:UninstallProgram,{#MyAppName}}; Filename: {uninstallexe}
Name: {commondesktop}\{#MyAppName}; Filename: {app}\{#MyAppExeName}; WorkingDir: {app}; Tasks: desktopicon
Name: {userappdata}\Microsoft\Internet Explorer\Quick Launch\{#MyAppName}; Filename: {app}\{#MyAppExeName}; WorkingDir: {app}; Tasks: quicklaunchicon
