// Video ManipulationDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "cvideo.h"
#include "afxcmn.h"

#include "./SoundManager/CRecMic.h"






// CVideoManipulationDlg dialog
class CVideoManipulationDlg : public CDialog
{
// Construction
public:
	CVideoManipulationDlg(CWnd* pParent = NULL);	// standard constructor
	CRecMic* therec;
	cVideo mVideoClass;

// Dialog Data
	enum { IDD = IDD_VIDEOMANIPULATION_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	CFont				    mFont;
	bool					mbRunning;
	HBITMAP hBitmap;
	int		mBitmapWidth,mBitmapHeight;

public:
	afx_msg void OnBnClickedBtnBrowse();
	afx_msg void OnBnClickedBtnPlay();
	afx_msg void OnDestroy();
private:
public:
	afx_msg void OnBnClickedBtnShowImage();
	afx_msg void OnBnClickedBtnHideImage();
public:
	LRESULT OnGraphNotify ( WPARAM wParam, LPARAM lParam ) ;
	CSliderCtrl mSliderTrans;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTimer(UINT nIDEvent);
};
