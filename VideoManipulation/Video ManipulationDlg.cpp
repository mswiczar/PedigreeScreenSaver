// Video ManipulationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Video Manipulation.h"
#include "Video ManipulationDlg.h"
#include ".\video manipulationdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define ID_TIMER_ANIMATE 100
// CAboutDlg dialog used for App About
bool runing=false;



void WINAPI startplay(void* parameter)
{
	if (runing==false)	
	{
		runing=true;

		CVideoManipulationDlg* thedialog = (CVideoManipulationDlg*)parameter;

		CString * path = new CString(L"c:\\1.avi");
		BSTR bstr = path->AllocSysString();  
		thedialog->mVideoClass.SetVideo(bstr);
		SysFreeString(bstr);
		thedialog->mVideoClass.Play();
	}
}



class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation 
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{



}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CVideoManipulationDlg dialog



CVideoManipulationDlg::CVideoManipulationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVideoManipulationDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

void CVideoManipulationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_TRANS, mSliderTrans);
}

BEGIN_MESSAGE_MAP(CVideoManipulationDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_BROWSE, OnBnClickedBtnBrowse)
	ON_BN_CLICKED(IDC_BTN_PLAY, OnBnClickedBtnPlay)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_SHOW_IMAGE, OnBnClickedBtnShowImage)
	ON_BN_CLICKED(IDC_BTN_HIDE_IMAGE, OnBnClickedBtnHideImage)
	ON_MESSAGE(WM_GRAPHNOTIFY,OnGraphNotify)
	ON_WM_HSCROLL()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CVideoManipulationDlg message handlers

BOOL CVideoManipulationDlg::OnInitDialog()
{

	hBitmap = LoadBitmap(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(IDB_BITMAP2));

	BITMAP bm;
    GetObject(hBitmap, sizeof(bm), &bm);
    mBitmapWidth = bm.bmWidth;
	mBitmapHeight= bm.bmHeight;


	CDialog::OnInitDialog();
	therec = new CRecMic();
	
	therec->StartRec((void*)startplay, (void*)this);
	
	mVideoClass.Clean();

	CWnd * pw = GetDlgItem(IDC_VIDEO_WINDOW);
	CRect rect;
	pw->GetClientRect(&rect);

	CString * path = new CString(L"c:\\video.avi");
	BSTR bstr = path->AllocSysString();  
	mVideoClass.InitInterfaces(NULL, pw,&rect);
	SysFreeString(bstr);

	(GetDlgItem(IDC_BTN_SHOW_IMAGE))->EnableWindow();
	(GetDlgItem(IDC_BTN_HIDE_IMAGE))->EnableWindow();


	/*
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	hBitmap = LoadBitmap(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(IDB_BITMAP_MAN));

	BITMAP bm;
    GetObject(hBitmap, sizeof(bm), &bm);
    mBitmapWidth = bm.bmWidth;
	mBitmapHeight= bm.bmHeight;
	
	mSliderTrans.SetRange(0,9);
	mSliderTrans.SetPos(9);
*/
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CVideoManipulationDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVideoManipulationDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CVideoManipulationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CVideoManipulationDlg::OnDestroy()
{
	KillTimer(ID_TIMER_ANIMATE);
	mVideoClass.Clean();
	CDialog::OnDestroy();
}
void CVideoManipulationDlg::OnBnClickedBtnBrowse()
{
/*
	therec = new CRecMic();
	therec->StartRec();
	mVideoClass.Clean();

	CWnd * pw = GetDlgItem(IDC_VIDEO_WINDOW);
	CRect rect;
	pw->GetClientRect(&rect);

	CString * path = new CString(L"c:\\video.avi");
	BSTR bstr = path->AllocSysString();  
	mVideoClass.InitInterfaces(bstr, pw,&rect);
	SysFreeString(bstr);

	(GetDlgItem(IDC_BTN_SHOW_IMAGE))->EnableWindow();
	(GetDlgItem(IDC_BTN_HIDE_IMAGE))->EnableWindow();
	mVideoClass.Play();
	
*/
}

/*
				
//	(GetDlgItem(IDC_BTN_PLAY))->EnableWindow();
	
	CFileDialog fd(true,0,0,0,"Media Files (*.avi)|*.avi|All Files (*.*)|*.*||");
	if(fd.DoModal() == IDCANCEL)
			return;	
	CString path = fd.GetPathName();
	*/


void CVideoManipulationDlg::OnBnClickedBtnPlay()
{

	mVideoClass.Play();
}


void CVideoManipulationDlg::OnBnClickedBtnShowImage()
{
	CDC * pdc = GetDC();
	CDC mCompatibleDC;
	mCompatibleDC.CreateCompatibleDC(pdc);
	mCompatibleDC.SelectObject(hBitmap);
	 CRect reSrc(0,0,mBitmapWidth,mBitmapHeight);
	CRect reDst;
	mVideoClass.ShowImage(&mCompatibleDC,reSrc);
	mSliderTrans.SetPos(9);
	SetTimer(ID_TIMER_ANIMATE,250,NULL);
	mVideoClass.Animate();
	mVideoClass.SetTransparency(9);
}

void CVideoManipulationDlg::OnBnClickedBtnHideImage()
{
	mVideoClass.HideImage();
	mSliderTrans.SetPos(0);
}

LRESULT CVideoManipulationDlg::OnGraphNotify ( WPARAM wParam, LPARAM lParam )
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
			runing=false;
				      // Seek to the beginning
			/*
			LONGLONG pos=0;
			mVideoClass.pMS->SetPositions(&pos, AM_SEEKING_AbsolutePositioning ,
					                   NULL, AM_SEEKING_NoPositioning);
			mVideoClass.pMC->Run();
			*/
		}
    }
    return 0L ;
}

void CVideoManipulationDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int pos = mSliderTrans.GetPos();	
	mVideoClass.SetTransparency(pos);
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CVideoManipulationDlg::OnTimer(UINT nIDEvent)
{
	mVideoClass.Animate();
	CDialog::OnTimer(nIDEvent);
}
