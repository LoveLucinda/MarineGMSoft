// ExtractGPSDataDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "数据提取.h"
#include "ExtractGPSDataDlg.h"
#include "afxdialogex.h"


// CExtractGPSDataDlg 对话框

IMPLEMENT_DYNAMIC(CExtractGPSDataDlg, CDialogEx)

CExtractGPSDataDlg::CExtractGPSDataDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CExtractGPSDataDlg::IDD, pParent)
	, m_SingleOrMulti(0)
	, m_IsExtentName(FALSE)
	, m_ExtentName(_T("txt"))
	, m_PathName(_T(""))
	, m_GPSFormat(0)
{

}

CExtractGPSDataDlg::~CExtractGPSDataDlg()
{
}

void CExtractGPSDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_SingleOrMulti);
	DDX_Check(pDX, IDC_CHECK1, m_IsExtentName);
	DDX_Text(pDX, IDC_EDIT2, m_ExtentName);
	DDX_Text(pDX, IDC_EDIT1, m_PathName);
	DDX_Control(pDX, IDC_EDIT1, m_PathEdit);
	DDX_Control(pDX, IDC_BUTTON1, m_PathNameButton);
	DDX_Control(pDX, IDC_EDIT2, m_ExtentNameEdit);
	DDX_Radio(pDX, IDC_RADIO3, m_GPSFormat);
}


BEGIN_MESSAGE_MAP(CExtractGPSDataDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CExtractGPSDataDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO1, &CExtractGPSDataDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CExtractGPSDataDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_CHECK1, &CExtractGPSDataDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_BUTTON1, &CExtractGPSDataDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CExtractGPSDataDlg 消息处理程序


void CExtractGPSDataDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	CDialogEx::OnOK();
}


void CExtractGPSDataDlg::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	m_PathEdit.EnableWindow(m_SingleOrMulti);
	m_PathNameButton.EnableWindow(m_SingleOrMulti);
}


void CExtractGPSDataDlg::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	m_PathEdit.EnableWindow(m_SingleOrMulti);
	m_PathNameButton.EnableWindow(m_SingleOrMulti);
}


void CExtractGPSDataDlg::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	m_ExtentNameEdit.EnableWindow(m_IsExtentName);
}


BOOL CExtractGPSDataDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_PathEdit.EnableWindow(m_SingleOrMulti);
	m_PathNameButton.EnableWindow(m_SingleOrMulti);
	m_ExtentNameEdit.EnableWindow(m_IsExtentName);
	return true;
}

int CALLBACK BrowseCallBackFun5(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)  
{  
    switch(uMsg)  
    {  
    case BFFM_INITIALIZED:  //选择文件夹对话框初始化  
        //设置默认路径为lpData即'D:\'  
        ::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);  
        //在STATUSTEXT区域显示当前路径  
        ::SendMessage(hwnd, BFFM_SETSTATUSTEXT, 0, lpData);  
        //设置选择文件夹对话框的标题  
        ::SetWindowText(hwnd, TEXT("选择路径"));   
        break;  
    case BFFM_SELCHANGED:   //选择文件夹变更时  
        {  
            TCHAR pszPath[MAX_PATH];  
            //获取当前选择路径  
            SHGetPathFromIDList((LPCITEMIDLIST)lParam, pszPath);  
            //在STATUSTEXT区域显示当前路径  
            ::SendMessage(hwnd, BFFM_SETSTATUSTEXT, TRUE, (LPARAM)pszPath);  
        }  
        break;  
    }  
    return 0;  
}  
void CExtractGPSDataDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//获取GPS文件路径
	TCHAR pszPath[MAX_PATH];  
    BROWSEINFO bi;   
    bi.hwndOwner      = this->GetSafeHwnd();  
    bi.pidlRoot       = NULL;  
    bi.pszDisplayName = NULL;   
    bi.lpszTitle      = NULL;   
    bi.ulFlags        = BIF_USENEWUI;  
    bi.lpfn           = BrowseCallBackFun5;     //回调函数  
	bi.lParam         =(LPARAM)(LPCTSTR)m_PathName;  //传给回调函数的参数,设置默认路径  
    bi.iImage         = 0;   
    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);  
    if (pidl == NULL)  
    {  
        return;  
    }  
      
    if (SHGetPathFromIDList(pidl, pszPath))  
    {  
        m_PathName=pszPath;
    }  
	UpdateData(false);
}
