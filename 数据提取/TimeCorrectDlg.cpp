// TimeCorrectDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "数据提取.h"
#include "TimeCorrectDlg.h"
#include "afxdialogex.h"


// CTimeCorrectDlg 对话框

IMPLEMENT_DYNAMIC(CTimeCorrectDlg, CDialogEx)

CTimeCorrectDlg::CTimeCorrectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTimeCorrectDlg::IDD, pParent)
	, m_TimeDelay(0)
	, m_DataFilePath(_T(""))
{

}

CTimeCorrectDlg::~CTimeCorrectDlg()
{
}

void CTimeCorrectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_TimeDelay);
	DDX_Text(pDX, IDC_EDIT2, m_DataFilePath);
}


BEGIN_MESSAGE_MAP(CTimeCorrectDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CTimeCorrectDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &CTimeCorrectDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CTimeCorrectDlg 消息处理程序

int CALLBACK BrowseCallBackFun(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)  
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
void CTimeCorrectDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	 TCHAR pszPath[MAX_PATH];  
    BROWSEINFO bi;   
    bi.hwndOwner      = this->GetSafeHwnd();  
    bi.pidlRoot       = NULL;  
    bi.pszDisplayName = NULL;   
    bi.lpszTitle      = NULL;   
    bi.ulFlags        = BIF_USENEWUI;  
    bi.lpfn           = BrowseCallBackFun;     //回调函数  
    bi.lParam         =(LPARAM)(LPCTSTR)m_DataFilePath;  //传给回调函数的参数,设置默认路径  
    bi.iImage         = 0;   
    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);  
    if (pidl == NULL)  
    {  
        return;  
    }  
      
    if (SHGetPathFromIDList(pidl, pszPath))  
    {  
        m_DataFilePath=pszPath;
    }  
}


void CTimeCorrectDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	CDialogEx::OnOK();
}
