// MergeSurveyLineDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "������ȡ.h"
#include "MergeSurveyLineDlg.h"
#include "afxdialogex.h"


// CMergeSurveyLineDlg �Ի���

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


// CMergeSurveyLineDlg ��Ϣ�������

int CALLBACK BrowseCallBackFun_merge(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)  
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
void CMergeSurveyLineDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TCHAR pszPath[MAX_PATH];  
    BROWSEINFO bi;   
    bi.hwndOwner      = this->GetSafeHwnd();  
    bi.pidlRoot       = NULL;  
    bi.pszDisplayName = NULL;   
    bi.lpszTitle      = NULL;   
    bi.ulFlags        = BIF_USENEWUI;  
    bi.lpfn           = BrowseCallBackFun_merge;     //�ص�����  
    bi.lParam         =(LPARAM)(LPCTSTR)m_PathName;  //�����ص������Ĳ���,����Ĭ��·��  
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	CDialogEx::OnOK();
}
