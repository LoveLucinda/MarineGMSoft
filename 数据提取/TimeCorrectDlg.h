#pragma once


// CTimeCorrectDlg �Ի���

class CTimeCorrectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTimeCorrectDlg)

public:
	CTimeCorrectDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTimeCorrectDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_TimeDelay;
	CString m_DataFilePath;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedOk();
};
