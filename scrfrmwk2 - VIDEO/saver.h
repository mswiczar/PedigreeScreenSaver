// Saver.h : main header file for the SAVER application
//

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSaverApp:
// See Saver.cpp for the implementation of this class
//

class CSaverApp : public CWinApp
{
public:
	CSaverApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSaverApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSaverApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	enum SCRMODE { SMNONE, SMCONFIG, SMPASSWORD, SMPREVIEW, SMSAVER };

	SCRMODE GetScrMode(HWND &hWnd) const;

	void DoConfig(HWND hWndParent) const;
	void DoChangePassword(HWND hWndParent) const;
};


/////////////////////////////////////////////////////////////////////////////
