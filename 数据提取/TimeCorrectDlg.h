#pragma once


// CTimeCorrectDlg 对话框

class CTimeCorrectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTimeCorrectDlg)

public:
	CTimeCorrectDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTimeCorrectDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_TimeDelay;
	CString m_DataFilePath;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedOk();
};
