// WriteSurveyLineToWord.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "������ȡ.h"
#include "WriteSurveyLineToWord.h"
#include "afxdialogex.h"


// CWriteSurveyLineToWord �Ի���

IMPLEMENT_DYNAMIC(CWriteSurveyLineToWord, CDialogEx)

CWriteSurveyLineToWord::CWriteSurveyLineToWord(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWriteSurveyLineToWord::IDD, pParent)
	, m_SurveyLinePath(_T(""))
	, m_Title(_T(""))
	, m_ExtName(_T("DAT"))
	, m_surveylinename(_T(""))
	, m_DanWeiJiGou(_T("���Һ���ֵڶ������о���"))
	, m_DiaoChaChuan(_T("����һ��"))
	, m_HangCi(_T(""))
	, m_HangDuan(_T(""))
	, m_person(_T(""))
{

}

CWriteSurveyLineToWord::~CWriteSurveyLineToWord()
{
}

void CWriteSurveyLineToWord::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_SurveyLinePath);
	DDX_Text(pDX, IDC_EDIT4, m_ExtName);
	DDX_Text(pDX, IDC_EDIT5, m_surveylinename);
	DDX_Text(pDX, IDC_EDIT7, m_DanWeiJiGou);
	DDX_Text(pDX, IDC_EDIT8, m_DiaoChaChuan);
	DDX_Text(pDX, IDC_EDIT2, m_HangCi);
	DDX_Text(pDX, IDC_EDIT9, m_HangDuan);
	DDX_Text(pDX, IDC_EDIT6, m_person);
}


BEGIN_MESSAGE_MAP(CWriteSurveyLineToWord, CDialogEx)
	ON_BN_CLICKED(IDOK, &CWriteSurveyLineToWord::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CWriteSurveyLineToWord::OnBnClickedButton1)
END_MESSAGE_MAP()


// CWriteSurveyLineToWord ��Ϣ�������


void CWriteSurveyLineToWord::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	m_Title=_T("DY")+m_HangCi+_T("-")+m_HangDuan;
	CDialogEx::OnOK();
}


void CWriteSurveyLineToWord::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
