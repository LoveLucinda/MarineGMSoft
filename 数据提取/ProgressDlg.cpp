// ProgressDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "������ȡ.h"
#include "ProgressDlg.h"
#include "afxdialogex.h"


// CProgressDlg �Ի���

IMPLEMENT_DYNAMIC(CProgressDlg, CDialogEx)

CProgressDlg::CProgressDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProgressDlg::IDD, pParent)
{

}

CProgressDlg::~CProgressDlg()
{
}

void CProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_ProgressCtrl);
}


BEGIN_MESSAGE_MAP(CProgressDlg, CDialogEx)
END_MESSAGE_MAP()


// CProgressDlg ��Ϣ�������
