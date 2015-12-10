// TimeCorrectDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "������ȡ.h"
#include "TimeCorrectDlg.h"
#include "afxdialogex.h"


// CTimeCorrectDlg �Ի���

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


// CTimeCorrectDlg ��Ϣ�������

int CALLBACK BrowseCallBackFun(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)  
{  
    switch(uMsg)  
    {  
    case BFFM_INITIALIZED:  //ѡ���ļ��жԻ����ʼ��  
        //����Ĭ��·��ΪlpData��'D:\'  
        ::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);  
        //��STATUSTEXT������ʾ��ǰ·��  
        ::SendMessage(hwnd, BFFM_SETSTATUSTEXT, 0, lpData);  
        //����ѡ���ļ��жԻ���ı���  
        ::SetWindowText(hwnd, TEXT("ѡ��·��"));   
        break;  
    case BFFM_SELCHANGED:   //ѡ���ļ��б��ʱ  
        {  
            TCHAR pszPath[MAX_PATH];  
            //��ȡ��ǰѡ��·��  
            SHGetPathFromIDList((LPCITEMIDLIST)lParam, pszPath);  
            //��STATUSTEXT������ʾ��ǰ·��  
            ::SendMessage(hwnd, BFFM_SETSTATUSTEXT, TRUE, (LPARAM)pszPath);  
        }  
        break;  
    }  
    return 0;  
}  
void CTimeCorrectDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	 TCHAR pszPath[MAX_PATH];  
    BROWSEINFO bi;   
    bi.hwndOwner      = this->GetSafeHwnd();  
    bi.pidlRoot       = NULL;  
    bi.pszDisplayName = NULL;   
    bi.lpszTitle      = NULL;   
    bi.ulFlags        = BIF_USENEWUI;  
    bi.lpfn           = BrowseCallBackFun;     //�ص�����  
    bi.lParam         =(LPARAM)(LPCTSTR)m_DataFilePath;  //�����ص������Ĳ���,����Ĭ��·��  
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	CDialogEx::OnOK();
}
