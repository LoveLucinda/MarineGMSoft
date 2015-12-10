// MergeSurveyLineDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "数据提取.h"
#include "MergeSurveyLineDlg.h"
#include "afxdialogex.h"


// CMergeSurveyLineDlg 对话框

IMPLEMENT_DYNAMIC(CMergeSurveyLineDlg, CDialogEx)

CMergeSurveyLineDlg::CMergeSurveyLineDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMergeSurveyLineDlg::IDD, pParent)
	, m_PathName(_T(""))
	, m_ExtenFileName(_T("DAT"))
{

}

CMergeSurveyLineDlg::~CMergeSurveyLineDlg()
{
}

void CMergeSurveyLineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_PathName);
	DDX_Text(pDX, IDC_EDIT2, m_ExtenFileName);
}


BEGIN_MESSAGE_MAP(CMergeSurveyLineDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CMergeSurveyLineDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &CMergeSurveyLineDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CMergeSurveyLineDlg 消息处理程序

int CALLBACK BrowseCallBackFun_merge(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)  
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
void CMergeSurveyLineDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR pszPath[MAX_PATH];  
    BROWSEINFO bi;   
    bi.hwndOwner      = this->GetSafeHwnd();  
    bi.pidlRoot       = NULL;  
    bi.pszDisplayName = NULL;   
    bi.lpszTitle      = NULL;   
    bi.ulFlags        = BIF_USENEWUI;  
    bi.lpfn           = BrowseCallBackFun_merge;     //回调函数  
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


void CMergeSurveyLineDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	CDialogEx::OnOK();
}
