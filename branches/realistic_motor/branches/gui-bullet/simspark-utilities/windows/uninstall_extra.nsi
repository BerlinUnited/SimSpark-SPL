Delete '$INSTDIR\bin\monitorspark.cmd'
Delete '$INSTDIR\bin\sampleagent.cmd'
Delete '$INSTDIR\bin\samplesim.cmd'
DeleteRegValue HKLM 'SYSTEM\CurrentControlSet\Control\Session Manager\Environment\' 'SIMSPARK-UTILITIES_DIR'

Var /GLOBAL STARTMENU_DIR_FORLINKS
!insertmacro MUI_STARTMENU_GETFOLDER Application $STARTMENU_DIR_FORLINKS
    
Delete "$SMPROGRAMS\$STARTMENU_DIR_FORLINKS\Monitorspark.lnk"
Delete "$SMPROGRAMS\$STARTMENU_DIR_FORLINKS\SampleAgent.lnk"
Delete "$SMPROGRAMS\$STARTMENU_DIR_FORLINKS\SampleSim.lnk"
