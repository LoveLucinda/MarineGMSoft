// MatchGPSGRAVDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "数据提取.h"
#include "MatchGPSGRAVDlg.h"
#include "afxdialogex.h"


// CMatchGPSGRAVDlg 对话框

IMPLEMENT_DYNAMIC(CMatchGPSGRAVDlg, CDialogEx)

CMatchGPSGRAVDlg::CMatchGPSGRAVDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMatchGPSGRAVDlg::IDD, pParent)
	, m_GravDataPath(_T(""))
	, m_GPSDataPath(_T(""))
	, m_MatchedGravDataPath(_T(""))
	, m_GravimeterVersion(_T("S-162"))
{

}

CMatchGPSGRAVDlg::~CMatchGPSGRAVDlg()
{
}

void CMatchGPSGRAVDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_GravDataPath);
	DDX_Text(pDX, IDC_EDIT2, m_GPSDataPath);
	DDX_Text(pDX, IDC_EDIT5, m_MatchedGravDataPath);
	DDX_Control(pDX, IDC_COMBO1, m_GravimeterVersionComb);
	DDX_CBString(pDX, IDC_COMBO1, m_GravimeterVersion);
}


BEGIN_MESSAGE_MAP(CMatchGPSGRAVDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CMatchGPSGRAVDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMatchGPSGRAVDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMatchGPSGRAVDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDOK, &CMatchGPSGRAVDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CMatchGPSGRAVDlg 消息处理程序

int CALLBACK BrowseCallBackFun6(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)  
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
void CMatchGPSGRAVDlg::OnBnClickedButton1()
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
    bi.lpfn           = BrowseCallBackFun6;     //回调函数  
	bi.lParam         =(LPARAM)(LPCTSTR)m_GravDataPath;  //传给回调函数的参数,设置默认路径  
    bi.iImage         = 0;   
    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);  
    if (pidl == NULL)  
    {  
        return;  
    }  
      
    if (SHGetPathFromIDList(pidl, pszPath))  
    {  
        m_GravDataPath=pszPath;
    }  
	UpdateData(false);
}


void CMatchGPSGRAVDlg::OnBnClickedButton2()
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
    bi.lpfn           = BrowseCallBackFun6;     //回调函数  
	bi.lParam         =(LPARAM)(LPCTSTR)m_GPSDataPath;  //传给回调函数的参数,设置默认路径  
    bi.iImage         = 0;   
    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);  
    if (pidl == NULL)  
    {  
        return;  
    }  
      
    if (SHGetPathFromIDList(pidl, pszPath))  
    {  
        m_GPSDataPath=pszPath;
    }  
	UpdateData(false);
}


void CMatchGPSGRAVDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR pszPath[MAX_PATH];  
    BROWSEINFO bi;   
    bi.hwndOwner      = this->GetSafeHwnd();  
    bi.pidlRoot       = NULL;  
    bi.pszDisplayName = NULL;   
    bi.lpszTitle      = NULL;   
    bi.ulFlags        = BIF_USENEWUI;  
    bi.lpfn           = BrowseCallBackFun6;     //回调函数  
	bi.lParam         =(LPARAM)(LPCTSTR)m_MatchedGravDataPath;  //传给回调函数的参数,设置默认路径  
    bi.iImage         = 0;   
    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);  
    if (pidl == NULL)  
    {  
        return;  
    }  
      
    if (SHGetPathFromIDList(pidl, pszPath))  
    {  
        m_MatchedGravDataPath=pszPath;
    }  
	UpdateData(false);
}


void CMatchGPSGRAVDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	CDialogEx::OnOK();
}


BOOL CMatchGPSGRAVDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_GravimeterVersionComb.AddString(_T("S-162"));
	m_GravimeterVersionComb.AddString(_T("S-129"));
	return true;
}
