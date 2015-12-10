#pragma once
#include "afxwin.h"


// CMatchGPSGRAVDlg 对话框

class CMatchGPSGRAVDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMatchGPSGRAVDlg)

public:
	CMatchGPSGRAVDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMatchGPSGRAVDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG6 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_GravDataPath;
	CString m_GPSDataPath;
	CString m_MatchedGravDataPath;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedOk();
	CComboBox m_GravimeterVersionComb;
	CString m_GravimeterVersion;
	virtual BOOL OnInitDialog();
};
