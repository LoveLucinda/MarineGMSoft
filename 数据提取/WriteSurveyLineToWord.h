#pragma once


// CWriteSurveyLineToWord 对话框

class CWriteSurveyLineToWord : public CDialogEx
{
	DECLARE_DYNAMIC(CWriteSurveyLineToWord)

public:
	CWriteSurveyLineToWord(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWriteSurveyLineToWord();

// 对话框数据
	enum { IDD = IDD_StatisticLineDataToWord };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_SurveyLinePath;
	CString m_Title;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	CString m_ExtName;
	CString m_surveylinename;
	CString m_DanWeiJiGou;
	CString m_DiaoChaChuan;
	CString m_HangCi;
	CString m_HangDuan;
	CString m_person;
};
