// TestExecute.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "windows.h"
DWORD EjecutarProgram(WCHAR *execute, bool insyncro, int showmode )
{
     WCHAR malditostack[512];
	ZeroMemory(malditostack,1024);
	wcscat(malditostack,execute);

	STARTUPINFOW si;
    PROCESS_INFORMATION pi;
    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow =SW_HIDE;
	ZeroMemory( &pi, sizeof(pi) );


    // Start the child process. 
    if( !CreateProcessW	( NULL,   // No module name (use command line)
        malditostack,        // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        0,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi )           // Pointer to PROCESS_INFORMATION structure
    ) 
    {
//        printf( "CreateProcess failed (%d)\n", GetLastError() );
        return 0;
    }

    // Wait until child process exits.
	DWORD salida=0;
	if (insyncro) 
	{
		WaitForSingleObject( pi.hProcess, INFINITE );
		GetExitCodeProcess(pi.hProcess,&salida);
    // Close process and thread handles. 
		CloseHandle( pi.hProcess );
		CloseHandle( pi.hThread );
	};
         return salida;

}



int _tmain(int argc, _TCHAR* argv[])
{
	::SetCurrentDirectory(L"C:\\Users\\6910p\\Documents\\voip_SVN\\voip\\VoiceSpectogram\\Setup\\Bin\\WebServer\\");
	EjecutarProgram(L"C:\\Users\\6910p\\Documents\\voip_SVN\\voip\\VoiceSpectogram\\Setup\\Bin\\WebServer\\lighttpd.exe -f ./conf/lighttpd-inc.conf -m lib -D",false,SW_HIDE);

	return 0;
}

