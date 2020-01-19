;--------------------------------
; The name of the installer
Name "Pedigree"

; The file to write
OutFile "SetupPedigree.exe"

Icon .\icon.ico
UninstallIcon .\Uninstall.ico

; The default installation directory
InstallDir $PROGRAMFILES\PedigreeScreenSaver


LicenseData .\eula.txt
LicenseBkColor /windows

XPStyle on
;--------------------------------

; Pages
Page license
Page directory
Page instfiles


LoadLanguageFile "${NSISDIR}\Contrib\Language files\Spanish.nlf"



Section "" ;No components page, name is not important

  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Put file there
  WriteUninstaller $INSTDIR\uninstaller.exe

  SetOutPath $SYSDIR
	File .\Bin\PedigreeSaver.scr


  SetOutPath $INSTDIR

  CreateDirectory "$INSTDIR\webserver"
  CreateDirectory "$INSTDIR\webserver\htdocs"
  CreateDirectory "$INSTDIR\webserver\conf"
  CreateDirectory "$INSTDIR\webserver\bin"
  CreateDirectory "$INSTDIR\webserver\logs"
  CreateDirectory "$INSTDIR\webserver\lib"
  CreateDirectory "$INSTDIR\webserver\styles"
  CreateDirectory "$INSTDIR\webserver\tmp"


  SetOutPath $INSTDIR\webserver
	File .\Bin\WebServer\COPYING.LIGHTTPD
	File .\Bin\WebServer\CygBZ2-1.dll
	File .\Bin\WebServer\CygCrypt-0.dll
	File .\Bin\WebServer\CygCrypto-0.9.8.dll
	File .\Bin\WebServer\CygGCC_S-1.dll
	File .\Bin\WebServer\CygLBER-2-3-0.dll
	File .\Bin\WebServer\CygLDAP-2-3-0.dll
	File .\Bin\WebServer\CygLightCOMP.dll
	File .\Bin\WebServer\CygMinires.dll
	File .\Bin\WebServer\CygMySQLClient-15.dll
	File .\Bin\WebServer\CygPCRE-0.dll
	File .\Bin\WebServer\CygSASL2-2.dll
	File .\Bin\WebServer\CygSSL-0.9.8.dll
	File .\Bin\WebServer\CygWin1.dll
	File .\Bin\WebServer\CygZ.dll
	File .\Bin\WebServer\LightTPD-Angel.exe
	File .\Bin\WebServer\LightTPD.exe
	File .\Bin\WebServer\NEWS.LIGHTTPD
	File .\Bin\WebServer\PACKINFO.TXT
	File .\Bin\WebServer\README.LIGHTTPD
	File .\Bin\WebServer\TestMode.bat


  SetOutPath $INSTDIR\webserver\htdocs
	File .\Bin\WebServer\htdocs\AC_RunActiveContent.js
  	File .\Bin\WebServer\htdocs\index.html
	File .\Bin\WebServer\htdocs\js_demo.swf
	File .\Bin\WebServer\htdocs\v04.flv
	File .\Bin\WebServer\htdocs\v07.flv
	File .\Bin\WebServer\htdocs\v09.flv
	File .\Bin\WebServer\htdocs\v10.flv
	File .\Bin\WebServer\htdocs\v12.flv
	File .\Bin\WebServer\htdocs\v13.flv
	File .\Bin\WebServer\htdocs\v15.flv
	File .\Bin\WebServer\htdocs\v21.flv
	File .\Bin\WebServer\htdocs\v23.flv
	File .\Bin\WebServer\htdocs\v25.flv

  SetOutPath $INSTDIR\webserver\conf
	File .\Bin\WebServer\conf\CONFIG.README
	File .\Bin\WebServer\conf\lighttpd-inc.conf
	File .\Bin\WebServer\conf\lighttpd-srv.conf
	File .\Bin\WebServer\conf\lighttpd-tag.conf


  SetOutPath $INSTDIR\webserver\bin
;	File .\Bin\WebServer\bin\INSTSRV.EXE
;	File .\Bin\WebServer\bin\LIGHTSRC.EXE
;	File .\Bin\WebServer\bin\PROCESS.EXE
;	File .\Bin\WebServer\bin\Service-Install.exe
;	File .\Bin\WebServer\bin\Service-Remove.exe

  SetOutPath $INSTDIR\webserver\lib
	File .\Bin\WebServer\lib\mod_access.dll
	File .\Bin\WebServer\lib\mod_accesslog.dll
	File .\Bin\WebServer\lib\mod_alias.dll
	File .\Bin\WebServer\lib\mod_auth.dll
	File .\Bin\WebServer\lib\mod_cgi.dll
	File .\Bin\WebServer\lib\mod_cml.dll
	File .\Bin\WebServer\lib\mod_compress.dll
	File .\Bin\WebServer\lib\mod_dirlisting.dll
	File .\Bin\WebServer\lib\mod_evasive.dll
	File .\Bin\WebServer\lib\mod_evhost.dll
	File .\Bin\WebServer\lib\mod_expire.dll
	File .\Bin\WebServer\lib\mod_extforward.dll
	File .\Bin\WebServer\lib\mod_fastcgi.dll
	File .\Bin\WebServer\lib\mod_flv_streaming.dll
	File .\Bin\WebServer\lib\mod_indexfile.dll
	File .\Bin\WebServer\lib\mod_magnet.dll
	File .\Bin\WebServer\lib\mod_mysql_vhost.dll
	File .\Bin\WebServer\lib\mod_proxy.dll
	File .\Bin\WebServer\lib\mod_redirect.dll
	File .\Bin\WebServer\lib\mod_rewrite.dll
	File .\Bin\WebServer\lib\mod_rrdtool.dll
	File .\Bin\WebServer\lib\mod_scgi.dll
	File .\Bin\WebServer\lib\mod_secdownload.dll
	File .\Bin\WebServer\lib\mod_setenv.dll
	File .\Bin\WebServer\lib\mod_simple_vhost.dll
	File .\Bin\WebServer\lib\mod_ssi.dll
	File .\Bin\WebServer\lib\mod_staticfile.dll
	File .\Bin\WebServer\lib\mod_status.dll
	File .\Bin\WebServer\lib\mod_trigger_b4_dl.dll
	File .\Bin\WebServer\lib\mod_userdir.dll
	File .\Bin\WebServer\lib\mod_usertrack.dll
	File .\Bin\WebServer\lib\mod_webdav.dll





  SetOutPath $INSTDIR

  CreateDirectory "$SMPROGRAMS\PedigreeScreenSaver"					


WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\PedigreeScreenSaver" "DisplayName" "Pedigree Screen Saver (remove only)"
WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\PedigreeScreenSaver" "UninstallString" '"$INSTDIR\uninstaller.exe"'

WriteRegStr HKCU Software\pedigree\ InstallDir "$INSTDIR\"




ExecShell "open" "http://www.pedigree.com.ar/screensaver/gracias"
Sleep 3000
Exec "control desk.cpl ,1"



 

SectionEnd ; end the section



Section "Uninstall"

  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\PedigreeScreenSaver"
  DeleteRegKey HKLM Software\PedigreeScreenSaver\ 

  Delete $INSTDIR\Uninstaller.exe
  Delete $SYSDIR\PedigreeSaver.scr
	
  Delete "$INSTDIR\webserver\*.*"
  Delete "$INSTDIR\webserver\htdocs\*.*"
  Delete "$INSTDIR\webserver\conf\*.*"
  Delete "$INSTDIR\webserver\bin\*.*"
  Delete "$INSTDIR\webserver\logs\*.*"
  Delete "$INSTDIR\webserver\lib\*.*"
  Delete "$INSTDIR\webserver\styles\*.*"
  Delete "$INSTDIR\webserver\tmp\*.*"

  RMDir "$SMPROGRAMS\PedigreeScreenSaver"
  RMDir "$INSTDIR\webserver"
  RMDir "$INSTDIR\webserver\htdocs"
  RMDir "$INSTDIR\webserver\conf"
  RMDir "$INSTDIR\webserver\bin"
  RMDir "$INSTDIR\webserver\logs"
  RMDir "$INSTDIR\webserver\lib"
  RMDir "$INSTDIR\webserver\styles"
  RMDir "$INSTDIR\webserver\tmp"



SectionEnd
