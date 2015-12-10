// DateTimeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "������ȡ.h"
#include "DateTimeDlg.h"
#include "afxdialogex.h"


// CDateTimeDlg �Ի���

IMPLEMENT_DYNAMIC(CDateTimeDlg, CDialogEx)

CDateTimeDlg::CDateTimeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDateTimeDlg::IDD, pParent)
	, m_Time(COleDateTime::GetCurrentTime())
	, m_Date(COleDateTime::GetCurrentTime())
	, m_RangeStr(_T(""))
{

}

CDateTimeDlg::~CDateTimeDlg()
{
}

void CDateTimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER2, m_Time);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_Date);
	DDX_Text(pDX, IDC_Range, m_RangeStr);
}


BEGIN_MESSAGE_MAP(CDateTimeDlg, CDialogEx)
END_MESSAGE_MAP()


// CDateTimeDlg ��Ϣ�������
