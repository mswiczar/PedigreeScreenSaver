// drawwnd.h : header file
//

#include "./SoundManager/CRecMic.h"
#include "cvideo.h"


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CDrawWnd window

#include "afxmt.h"

class CDrawWnd : public CWnd
{
// Construction
public:
	CDrawWnd(BOOL bAutoDelete = TRUE);

// Attributes

// Operations

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawWnd)
	public:
	CCriticalSection asection ;
	CWnd * thewin; 
	CWnd * thevideo;
	virtual BOOL Create(DWORD dwExStyle, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT &rect, CWnd *pWndParent, UINT nID = 0);
	protected:

	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDrawWnd();
	CRecMic* therec;
	cVideo mVideoClass;
	HBITMAP hBitmap;
	int		mBitmapWidth,mBitmapHeight;

	LRESULT OnGraphNotify ( WPARAM wParam, LPARAM lParam ) ;

protected:
	BOOL m_bAutoDelete;

	// Generated message map functions
protected:
	//{{AFX_MSG(CDrawWnd)
	afx_msg void OnPaint();
	afx_msg BOOL OnSetCursor(CWnd *pWnd, UINT nHitTest, UINT nMessage);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
