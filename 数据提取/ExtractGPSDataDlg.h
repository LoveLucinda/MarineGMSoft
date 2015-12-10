#pragma once
#include "afxwin.h"


// CExtractGPSDataDlg 对话框

class CExtractGPSDataDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CExtractGPSDataDlg)

public:
	CExtractGPSDataDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CExtractGPSDataDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	int m_SingleOrMulti;
	BOOL m_IsExtentName;
	CString m_ExtentName;
	CString m_PathName;
	CEdit m_PathEdit;
	CButton m_PathNameButton;
	CEdit m_ExtentNameEdit;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedCheck1();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	int m_GPSFormat;
};
