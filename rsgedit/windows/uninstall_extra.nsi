Delete '$INSTDIR\bin\rsgedit.cmd'
DeleteRegValue HKLM 'SYSTEM\CurrentControlSet\Control\Session Manager\Environment\' 'RSGEDIT_DIR'

Var /GLOBAL STARTMENU_DIR_FORLINKS
!insertmacro MUI_STARTMENU_GETFOLDER Application $STARTMENU_DIR_FORLINKS
    
Delete "$SMPROGRAMS\$STARTMENU_DIR_FORLINKS\Rsgedit.lnk"