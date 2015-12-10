#pragma once
#include "afxwin.h"


// CMatchGPSGRAVDlg �Ի���

class CMatchGPSGRAVDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMatchGPSGRAVDlg)

public:
	CMatchGPSGRAVDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMatchGPSGRAVDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG6 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
