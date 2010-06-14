CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\Monitorspark.lnk" "$INSTDIR\bin\monitorspark.cmd"
CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\SampleAgent.lnk" "$INSTDIR\bin\sampleagent.cmd"
CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\SampleSim.lnk" "$INSTDIR\bin\samplesim.cmd"
!insertmacro MUI_STARTMENU_WRITE_END
!define MUI_STARTMENUPAGE_CURRENT_ID "SOMETHING_NOT_AVAILABLE"

WriteRegExpandStr HKLM 'SYSTEM\CurrentControlSet\Control\Session Manager\Environment\' 'SIMSPARK-UTILITIES_DIR' '$INSTDIR'
ClearErrors
FileOpen $0 $INSTDIR\bin\monitorspark.cmd w
FileWrite $0 'PATH=%PATH%;%SPARK_DIR%\lib\simspark$\n'
FileWrite $0 'cd "%SIMSPARK-UTILITIES_DIR%\bin\"$\n'
FileWrite $0 'monitorspark.exe %1 %2 %3 %4'
FileClose $0
ClearErrors
FileOpen $0 $INSTDIR\bin\sampleagent.cmd w
FileWrite $0 'PATH=%PATH%;%SPARK_DIR%\lib\simspark$\n'
FileWrite $0 'cd "%SIMSPARK-UTILITIES_DIR%\bin\"$\n'
FileWrite $0 'sampleagent.exe %1 %2 %3 %4'
FileClose $0
ClearErrors
FileOpen $0 $INSTDIR\bin\samplesim.cmd w
FileWrite $0 'PATH=%PATH%;%SPARK_DIR%\lib\simspark$\n'
FileWrite $0 'cd "%SIMSPARK-UTILITIES_DIR%\bin\"$\n'
FileWrite $0 'samplesim.exe %1 %2 %3 %4'
FileClose $0
