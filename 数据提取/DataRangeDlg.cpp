// DataRangeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "数据提取.h"
#include "DataRangeDlg.h"
#include "afxdialogex.h"


// CDataRangeDlg 对话框

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


// CDataRangeDlg 消息处理程序


void CDataRangeDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	CDialogEx::OnOK();
}
