!insertmacro MUI_STARTMENU_WRITE_END
!define MUI_STARTMENUPAGE_CURRENT_ID "SOMETHING_NOT_AVAILABLE"

WriteRegExpandStr HKLM 'SYSTEM\CurrentControlSet\Control\Session Manager\Environment\' 'SPARK_DIR' '$INSTDIR'
