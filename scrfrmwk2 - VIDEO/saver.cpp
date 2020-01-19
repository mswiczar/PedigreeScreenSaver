// Saver.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Saver.h"
#include "drawwnd.h"
#include "saverwnd.h"
#pragma comment (lib,"strmiids.lib") 

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSaverApp

BEGIN_MESSAGE_MAP(CSaverApp, CWinApp)
	//{{AFX_MSG_MAP(CSaverApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSaverApp construction

CSaverApp::CSaverApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSaverApp object

CSaverApp theApp;

//*****************************************************************************
/////////////////////////////////////////////////////////////////////////////
// CSaverApp initialization
//-----------------------------------------------------------------------------
BOOL CSaverApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	this->Enable3dControls();		// Call this when using MFC in a shared DLL
#else
	this->Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	//this->SetRegistryKey(IDS_COMPANYNAME);

	HWND hWnd = NULL;
	switch (this->GetScrMode(hWnd))
	{
		case SMSAVER:
		{
			// Run the screen saver
			CSaverWnd *const pWnd = new CSaverWnd;
			pWnd->Create();
			m_pMainWnd = pWnd;
			return TRUE;
		}
		
		case SMPREVIEW:
		{
			// Preview
			CWnd *const pWndParent = CWnd::FromHandle(hWnd);
			ASSERT(pWndParent != NULL);

			CDrawWnd *const pWnd = new CDrawWnd();
			CRect rect;
			pWndParent->GetClientRect(&rect);
			pWnd->Create(0, CString(reinterpret_cast<LPCTSTR>(IDS_PREVIEW)),
						 WS_VISIBLE | WS_CHILD, rect, pWndParent);
			m_pMainWnd = pWnd;
			return TRUE;
		}
		
		case SMCONFIG:
			// Configure
			this->DoConfig(hWnd);
			break;
		
		case SMPASSWORD:
			// Password
			this->DoChangePassword(hWnd);
			break;

		case SMNONE:
			// Nothing
			break;

		default:
			ASSERT(FALSE);
			break;
	}

	return FALSE;
}
//-----------------------------------------------------------------------------
CSaverApp::SCRMODE CSaverApp::GetScrMode(HWND &hWnd) const
{
	// parse the command line to determine what to do
	// return value is SMNONE, SMSAVER, SMPREVIEW, SMCONFIG, or SMPASSWORD
	// hWnd is the parent window handle
	SCRMODE ScrMode = SMNONE;
	hWnd = NULL;

	TCHAR *pc = ::GetCommandLine();
	ASSERT(pc != NULL);

	if (*pc == _T('\"'))
	{
		pc++;
		while (*pc != _T('\0') && *pc != _T('\"'))
		{
			pc++;
		}
	}
	else
	{
		while (*pc != _T('\0') && *pc != _T(' '))
		{
			pc++;
		}
	}

	if (*pc != _T('\0'))
	{
		pc++;
	}

	while (*pc == _T(' '))
	{
		pc++;
	}

	if (*pc == _T('\0'))
	{
		ScrMode = SMCONFIG;
		hWnd = NULL;
	}
	else
	{
		if (*pc == _T('-') || *pc == _T('/'))
		{
			pc++;
		}

		if (*pc == _T('s') || *pc == _T('S'))
		{
			ScrMode = SMSAVER;
		}
		else
		{
			if (*pc == _T('p') || *pc == _T('P') ||
				*pc == _T('l') || *pc == _T('L'))
			{
				pc++;
				while (*pc == _T(' ') || *pc == _T(':'))
				{
					pc++;
				}

				ScrMode = SMPREVIEW;
				hWnd = reinterpret_cast<HWND>(::_ttoi(pc));
			}
			else
			{
				if (*pc == _T('c') || *pc == _T('C'))
				{
					pc++;
					while (*pc == _T(' ') || *pc == _T(':'))
					{
						pc++;
					}
					
					ScrMode = SMCONFIG;
					hWnd = (*pc == _T('\0'))
						   ? ::GetForegroundWindow()
						   : reinterpret_cast<HWND>(::_ttoi(pc));
				}
				else
				{
					if (*pc == _T('a') || *pc == _T('A'))
					{
						pc++;
						while (*pc == _T(' ') || *pc == _T(':'))
						{
							pc++;
						}
						
						ScrMode = SMPASSWORD;
						hWnd = reinterpret_cast<HWND>(::_ttoi(pc));
					}
				}
			}
		}
	}

	return ScrMode;
}
//-----------------------------------------------------------------------------
void CSaverApp::DoConfig(HWND hWndParent) const
{
	// You may create a modal dialog box for configuration.
	::MessageBox(hWndParent,
				 CString(reinterpret_cast<LPCTSTR>(IDS_NOOPTIONS)),
				 CString(reinterpret_cast<LPCTSTR>(IDS_MSGCAPTION)),
				 MB_OK | MB_ICONEXCLAMATION);
}
//-----------------------------------------------------------------------------
void CSaverApp::DoChangePassword(HWND hWndParent) const
{
	// This only gets called on Windows 9x, when started with the /a option.
	const HINSTANCE hmpr = ::LoadLibrary(_T("MPR.DLL"));

	if (hmpr != NULL)
	{
		typedef VOID (WINAPI *PPWDCHANGEPASSWORD)
			(LPCSTR lpcRegkeyname, HWND hWnd,
			 UINT uiReserved1, UINT uiReserved2);

		const PPWDCHANGEPASSWORD pPwdChangePassword =
			reinterpret_cast<PPWDCHANGEPASSWORD>
				(::GetProcAddress(hmpr, "PwdChangePasswordA"));

		if (pPwdChangePassword != NULL)
		{
			pPwdChangePassword("SCRSAVE", hWndParent, 0, 0);
		}
		else
		{
			TRACE(_T("PwdChangeProc not found: cannot change password.\n"));
		}
	
		VERIFY(::FreeLibrary(hmpr));
	}
	else
	{
		TRACE(_T("MPR.DLL not found: cannot change password.\n"));
	}
}
//-----------------------------------------------------------------------------
//*****************************************************************************
