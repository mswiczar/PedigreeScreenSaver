// drawwnd.cpp : implementation file
//

#include "stdafx.h"
#include "Saver.h"
#include "drawwnd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

CWebBrowser2 * auxtheweb;

bool silvo=false;
bool runing=false;



static int total=0;

IHTMLWindow2 *spWindow=NULL;

void WINAPI startplay(void* parameter)
{
	CDrawWnd* thedialog = (CDrawWnd*)parameter;
	thedialog->asection.Lock();
	if (runing==false)	
	{
		runing=true;
		silvo=true;
	}
	thedialog->asection.Unlock();

}

//*****************************************************************************
//-----------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////
// CDrawWnd
//-----------------------------------------------------------------------------
CDrawWnd::CDrawWnd(BOOL bAutoDelete /* = TRUE */)
{
	m_bAutoDelete = bAutoDelete;
}
//-----------------------------------------------------------------------------
CDrawWnd::~CDrawWnd()
{
}
//-----------------------------------------------------------------------------


LRESULT CALLBACK	WndProcBIG(HWND, UINT, WPARAM, LPARAM)
{
	return 0;
}




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


void ReadRegistryPath(WCHAR* apath)
{
	WCHAR ou[512];
	ZeroMemory(ou,1024);
	bool salida;
	HKEY hKey;
	DWORD dwBufLen;
	RegOpenKeyExW( HKEY_CURRENT_USER,L"SoftWare\\Pedigree",0, KEY_ALL_ACCESS , &hKey );
	dwBufLen = 512;
	if (ERROR_SUCCESS==RegQueryValueExW( hKey, L"InstallDir", NULL, NULL,(LPBYTE) ou, &dwBufLen))
	{
		salida=true;
		wcscat(apath,ou);
	};

	RegCloseKey( hKey );
    return ;
}



BOOL CDrawWnd::Create(DWORD dwExStyle, LPCTSTR lpszWindowName, DWORD dwStyle,
					  const RECT &rect, CWnd *pWndParent, UINT nID /* = 0 */)
{
	CoInitialize(NULL);
	InitCommonControls();
	OleInitialize (NULL);
	AfxEnableControlContainer(); 
	WCHAR url[2048];
	::ZeroMemory(url,sizeof(url));

	ReadRegistryPath(url);
	wcscat(url,L"\\webserver");
	::SetCurrentDirectoryW(url);
	EjecutarProgram(L"lighttpd.exe -f ./conf/lighttpd-inc.conf -m lib -D",false,SW_HIDE);

	therec = new CRecMic();
	BOOL salida = this->CreateEx(dwExStyle,
						  ::AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW,
												NULL,
							static_cast<HBRUSH>(::GetStockObject(BLACK_BRUSH)),
										::AfxGetApp()->LoadIcon(IDR_MAINFRAME)),
						  lpszWindowName, dwStyle,
						  rect, pWndParent, nID);


	theweb = new CWebBrowser2;
	RECT a ;

	a.top=-5;
	a.left=-5;
	a.right = rect.right+30;
	a.bottom = rect.bottom+10;
	
	theweb->Create(NULL,WS_CHILD|WS_VISIBLE|BS_OWNERDRAW,a,this,nID,0,0,0);
	auxtheweb = theweb;
	COleVariant varEmpty;
	
	VARIANT var;
    var.vt = VT_I4;
    var.lVal = 0;

	theweb->Navigate("http://127.0.0.1:999/index.html",&var,&var,&var,&var);
	theweb->SetFullScreen(true);
	therec->StartRec((void*)startplay, (void*)this);


	return salida;
}


BOOL CDrawWnd::Create2(DWORD dwExStyle, LPCTSTR lpszWindowName, DWORD dwStyle,
					  const RECT &rect, CWnd *pWndParent, UINT nID /* = 0 */)
{
	BOOL salida = this->CreateEx(dwExStyle,
						  ::AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW,
												NULL,
							static_cast<HBRUSH>(::GetStockObject(BLACK_BRUSH)),
										::AfxGetApp()->LoadIcon(IDR_MAINFRAME)),
						  lpszWindowName, dwStyle,
						  rect, pWndParent, nID);

	return salida;
}




BOOL CDrawWnd::OnSetCursor(CWnd * /*pWnd*/,
							UINT /*nHitTest*/,
							UINT /*nMessage*/)
{
	::SetCursor( NULL);
	return TRUE;
}



//-----------------------------------------------------------------------------
void CDrawWnd::PostNcDestroy()
{
	if (m_bAutoDelete)
	{
		delete this;
	}
}


//------------------------------------------------------------
BEGIN_MESSAGE_MAP(CDrawWnd, CWnd)
	//{{AFX_MSG_MAP(CDrawWnd)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-----------------------------------------------------------------------------
void CDrawWnd::OnPaint()
{
	CPaintDC dc(this);
	// Add your drawing code here.
}
//-----------------------------------------------------------------------------
//*****************************************************************************


void CDrawWnd::check()
{
	if(silvo)
	{
		CComPtr<IHTMLWindow2> HTML2Wind;
		IHTMLDocument*	m_spDoc;
		IHTMLDocument2 *spDocument2;
		m_spDoc =(IHTMLDocument*) theweb->GetDocument();
		if (m_spDoc==NULL)
		{
			return;
		}
		if (theweb->GetBusy())
		{
			return;
		}
		m_spDoc->QueryInterface(IID_IHTMLDocument2, (LPVOID*)&spDocument2);
		spDocument2->get_parentWindow(&spWindow);
		VARIANT x;
		BSTR  MyBstr = SysAllocString(L"js_to_as()");
		BSTR  MyBstr2 = SysAllocString(L"javascript");
		spWindow->execScript(MyBstr, MyBstr2,&x);
		silvo=false;
		runing=false;
	}
}




