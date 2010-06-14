Delete '$INSTDIR\bin\rcssserver3d.cmd'
Delete '$INSTDIR\bin\rcssmonitor3d.cmd'
Delete '$INSTDIR\bin\rcssagent3d.cmd'
DeleteRegValue HKLM 'SYSTEM\CurrentControlSet\Control\Session Manager\Environment\' 'RCSSSERVER3D_DIR'

Var /GLOBAL STARTMENU_DIR_FORLINKS
!insertmacro MUI_STARTMENU_GETFOLDER Application $STARTMENU_DIR_FORLINKS
    
Delete "$SMPROGRAMS\$STARTMENU_DIR_FORLINKS\RCSSServer3D.lnk"
Delete "$SMPROGRAMS\$STARTMENU_DIR_FORLINKS\RCSSMonitor3D.lnk"
