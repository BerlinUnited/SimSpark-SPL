CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\Rsgedit.lnk" "$INSTDIR\bin\rsgedit.cmd"
!insertmacro MUI_STARTMENU_WRITE_END
!define MUI_STARTMENUPAGE_CURRENT_ID "SOMETHING_NOT_AVAILABLE"

WriteRegExpandStr HKLM 'SYSTEM\CurrentControlSet\Control\Session Manager\Environment\' 'RSGEDIT_DIR' '$INSTDIR'
ClearErrors
FileOpen $0 $INSTDIR\bin\rsgedit.cmd w
FileWrite $0 'PATH=%PATH%;%SPARK_DIR%\lib\simspark$\n'
FileWrite $0 'cd "%RSGEDIT_DIR%\bin\"$\n'
FileWrite $0 'rsgedit.exe %1 %2 %3 %4'
FileClose $0