// drawwnd.cpp : implementation file
//

#include "stdafx.h"
#include "Saver.h"
#include "drawwnd.h"




#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif



 


bool runing=false;

static int total=0;

void WINAPI startplay(void* parameter)
{
		CDrawWnd* thedialog = (CDrawWnd*)parameter;

		thedialog->asection.Lock();

	if (runing==false)	
	{
		runing=true;

		CString * path;
		path = new CString(L"c:\\1.avi");
//		thedialog->MessageBox("","",0);
		/*
		total++;
		CString * path;
		switch (total)
		{
		case 1:
			path = new CString(L"c:\\1.wmv");
			break;
		case 2:
			path = new CString(L"c:\\1.wmv");
			break;
		case 3:
			path = new CString(L"c:\\1.wmv");
			break;
		}

		if (total==3)
		{
			total=0;
		}
		*/

		BSTR bstr = path->AllocSysString();  
		thedialog->mVideoClass.SetVideo(bstr);
		SysFreeString(bstr);
		//thedialog->mVideoClass.Play();

		LONGLONG pos=0;
		thedialog->mVideoClass.pMS->SetPositions(&pos, AM_SEEKING_AbsolutePositioning ,
					                   NULL, AM_SEEKING_NoPositioning);
		thedialog->mVideoClass.pMC->Run();

/*
		CDC * pdc = thedialog->GetDC();
		CDC mCompatibleDC;
		mCompatibleDC.CreateCompatibleDC(pdc);
		mCompatibleDC.SelectObject(thedialog->hBitmap);
		CRect reSrc(0,0,thedialog->mBitmapWidth,thedialog->mBitmapHeight);
		CRect reDst;
		thedialog->mVideoClass.ShowImage(&mCompatibleDC,reSrc);
*/

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

BOOL CDrawWnd::Create(DWORD dwExStyle, LPCTSTR lpszWindowName, DWORD dwStyle,
					  
					  const RECT &rect, CWnd *pWndParent, UINT nID /* = 0 */)
{

	CoInitialize(NULL);
	InitCommonControls();




	hBitmap = LoadBitmap(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(IDB_BITMAP3));

	BITMAP bm;
    GetObject(hBitmap, sizeof(bm), &bm);
    mBitmapWidth = bm.bmWidth;
	mBitmapHeight= bm.bmHeight;

	therec = new CRecMic();
	LONG tamanoH = 720;
	LONG tamanoV = 450;
	float escaladoporc = ((float)(rect.right)) * 100 / ((float)(tamanoH))/100;
	tamanoH= tamanoH * escaladoporc;
	tamanoV= tamanoV * escaladoporc;

	BOOL salida = this->CreateEx(dwExStyle,
						  ::AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW,
												NULL,
							static_cast<HBRUSH>(::GetStockObject(BLACK_BRUSH)),
										::AfxGetApp()->LoadIcon(IDR_MAINFRAME)),
						  lpszWindowName, dwStyle,
						  rect, pWndParent, nID);




	RECT a2;
	a2.top = (rect.bottom - tamanoV)/2;

	a2.bottom = tamanoV + a2.top;
	a2.left=0;
	a2.right=rect.right;

	
	thevideo = new CWnd();
/*
	 salida = thevideo->CreateEx(dwExStyle,
						  ::AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW,
												NULL,
						static_cast<HBRUSH>(::GetStockObject(BLACK_BRUSH)),
										::AfxGetApp()->LoadIcon(IDR_MAINFRAME)),
						  "videito", dwStyle,
						  a2, this, nID);

	CWnd * pw = thevideo;

*/
	

	therec->StartRec((void*)startplay, (void*)this);
	CWnd * pw = this;

	CRect arect;
	pw->GetClientRect(&arect);

	CString * path = new CString(L"c:\\video.avi");
	BSTR bstr = path->AllocSysString();  
	mVideoClass.InitInterfaces(NULL, pw,&arect);
	SysFreeString(bstr);
/*

	CDC * pdc = GetDC();
	CDC mCompatibleDC;
	mCompatibleDC.CreateCompatibleDC(pdc);
	mCompatibleDC.SelectObject(hBitmap);
	CRect reSrc(0,0,mBitmapWidth,mBitmapHeight);
*/	
startplay(this);
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
	ON_MESSAGE(WM_GRAPHNOTIFY,OnGraphNotify)
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

LRESULT CDrawWnd::OnGraphNotify ( WPARAM wParam, LPARAM lParam )
{
	LONG evCode, evParam1, evParam2 ;
    HRESULT hr = S_OK ;
    while ( SUCCEEDED ( mVideoClass.pME -> GetEvent ( &evCode, &evParam1, &evParam2, 0 ) ) )
    {
        // Spin through the events
        hr = mVideoClass.pME -> FreeEventParams ( evCode, evParam1, evParam2 ) ;
        if ( evCode == EC_COMPLETE )
		{
			mVideoClass.pMC->Stop();
			asection.Lock();
			runing=false;
			asection.Unlock();
		}

    }
    return 0L ;
}




	