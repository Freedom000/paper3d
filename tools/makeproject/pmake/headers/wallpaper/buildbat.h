const char BUILDBAT_STR[] = 
"@ECHO OFF\n"
"IF \"%%1\" == \"\" GOTO USAGE\n"
"IF \"%%1\" == \"debug\" GOTO DEBUG\n"
"IF \"%%1\" == \"release\" GOTO RELEASE\n"
"IF \"%%1\" == \"clean\" GOTO CLEAN\n"
":DEBUG\n"
"ECHO ********************************************\n"
"ECHO Start building .so in debug version\n"
"ECHO ********************************************\n"
"ECHO ndk-build USE_STL=0 ENABLE_LOGGING=1 OPENGL=ES2 NDK_DEBUG=1 > temp.bat\n"
"CALL temp.bat\n"
"ECHO ********************************************\n"
"ECHO Start building Android APK in debug version\n"
"ECHO ********************************************\n"
"ant debug\n"
"GOTO EXIT\n"
":RELEASE\n"
"ECHO ********************************************\n"
"ECHO Start building .so in release version\n"
"ECHO ********************************************\n"
"ECHO ndk-build USE_STL=0 ENABLE_LOGGING=1 OPENGL=ES2 NDK_DEBUG=0 -j4 > temp.bat\n"
"CALL temp.bat\n"
"ECHO ********************************************\n"
"ECHO Start building Android APK in release version\n"
"ECHO ********************************************\n"
"ant release\n"
"GOTO EXIT\n"
":CLEAN\n"
"del /f /q ..\\..\\bin\\android\\*\n"
"GOTO EXIT\n"
":USAGE\n"
"ECHO Usage: build.bat {debug ^| release ^| clean}\n"
":EXIT\n"
"del /q /f temp.bat\n"
"\n"
;