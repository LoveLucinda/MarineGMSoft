#pragma once


// CMergeSurveyLineDlg �Ի���

class CMergeSurveyLineDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMergeSurveyLineDlg)

public:
	CMergeSurveyLineDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMergeSurveyLineDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG7 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CString m_PathName;
	CString m_ExtenFileName;
	afx_msg void OnBnClickedOk();
};
