#pragma once


// CWriteSurveyLineToWord �Ի���

class CWriteSurveyLineToWord : public CDialogEx
{
	DECLARE_DYNAMIC(CWriteSurveyLineToWord)

public:
	CWriteSurveyLineToWord(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWriteSurveyLineToWord();

// �Ի�������
	enum { IDD = IDD_StatisticLineDataToWord };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
