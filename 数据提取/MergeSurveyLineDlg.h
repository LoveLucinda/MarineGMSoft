#pragma once


// CMergeSurveyLineDlg 对话框

class CMergeSurveyLineDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMergeSurveyLineDlg)

public:
	CMergeSurveyLineDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMergeSurveyLineDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG7 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CString m_PathName;
	CString m_ExtenFileName;
	afx_msg void OnBnClickedOk();
};
