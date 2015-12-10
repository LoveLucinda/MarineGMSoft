// DataRangeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "������ȡ.h"
#include "DataRangeDlg.h"
#include "afxdialogex.h"


// CDataRangeDlg �Ի���

IMPLEMENT_DYNAMIC(CDataRangeDlg, CDialogEx)

CDataRangeDlg::CDataRangeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDataRangeDlg::IDD, pParent)
	, m_xmin(0)
	, m_xmax(0)
	, m_ymin(0)
	, m_ymax(0)
{

}

CDataRangeDlg::~CDataRangeDlg()
{
}

void CDataRangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_xmin);
	DDX_Text(pDX, IDC_EDIT2, m_xmax);
	DDX_Text(pDX, IDC_EDIT3, m_ymin);
	DDX_Text(pDX, IDC_EDIT4, m_ymax);
}


BEGIN_MESSAGE_MAP(CDataRangeDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDataRangeDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CDataRangeDlg ��Ϣ�������


void CDataRangeDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	CDialogEx::OnOK();
}
