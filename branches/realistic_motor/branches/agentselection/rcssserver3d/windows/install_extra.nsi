CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\RCSSServer3D.lnk" "$INSTDIR\bin\rcssserver3d.cmd"
CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\RCSSMonitor3D.lnk" "$INSTDIR\bin\rcssmonitor3d.cmd"
!insertmacro MUI_STARTMENU_WRITE_END
!define MUI_STARTMENUPAGE_CURRENT_ID "SOMETHING_NOT_AVAILABLE"

WriteRegExpandStr HKLM 'SYSTEM\CurrentControlSet\Control\Session Manager\Environment\' 'RCSSSERVER3D_DIR' '$INSTDIR'
ClearErrors
FileOpen $0 $INSTDIR\bin\rcssserver3d.cmd w
FileWrite $0 'PATH=%PATH%;%SPARK_DIR%\lib\simspark;%SPARK_DIR%\lib\thirdparty;%RCSSSERVER3D_DIR%\lib\rcssserver3d$\n'
FileWrite $0 'cd "%RCSSSERVER3D_DIR%\bin\"$\n'
FileWrite $0 'rcssserver3d.exe %1 %2 %3 %4'
FileClose $0
ClearErrors
FileOpen $0 $INSTDIR\bin\rcssmonitor3d.cmd w
FileWrite $0 'PATH=%PATH%;%SPARK_DIR%\lib\simspark;%SPARK_DIR%\lib\thirdparty;%RCSSSERVER3D_DIR%\lib\rcssserver3d$\n'
FileWrite $0 'cd "%RCSSSERVER3D_DIR%\bin\"$\n'
FileWrite $0 'rcssmonitor3d.exe %1 %2 %3 %4'
FileClose $0
ClearErrors
FileOpen $0 $INSTDIR\bin\rcssagent3d.cmd w
FileWrite $0 'PATH=%PATH%;%SPARK_DIR%\lib\simspark;%SPARK_DIR%\lib\thirdparty;%RCSSSERVER3D_DIR%\lib\rcssserver3d$\n'
FileWrite $0 'cd "%RCSSSERVER3D_DIR%\bin\"$\n'
FileWrite $0 'rcssagent3d.exe %1 %2 %3 %4'
FileClose $0
