// MatchGPSGRAVDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "������ȡ.h"
#include "MatchGPSGRAVDlg.h"
#include "afxdialogex.h"


// CMatchGPSGRAVDlg �Ի���

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


// CMatchGPSGRAVDlg ��Ϣ�������

int CALLBACK BrowseCallBackFun6(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)  
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
void CMatchGPSGRAVDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//��ȡGPS�ļ�·��
	TCHAR pszPath[MAX_PATH];  
    BROWSEINFO bi;   
    bi.hwndOwner      = this->GetSafeHwnd();  
    bi.pidlRoot       = NULL;  
    bi.pszDisplayName = NULL;   
    bi.lpszTitle      = NULL;   
    bi.ulFlags        = BIF_USENEWUI;  
    bi.lpfn           = BrowseCallBackFun6;     //�ص�����  
	bi.lParam         =(LPARAM)(LPCTSTR)m_GravDataPath;  //�����ص������Ĳ���,����Ĭ��·��  
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//��ȡGPS�ļ�·��
	TCHAR pszPath[MAX_PATH];  
    BROWSEINFO bi;   
    bi.hwndOwner      = this->GetSafeHwnd();  
    bi.pidlRoot       = NULL;  
    bi.pszDisplayName = NULL;   
    bi.lpszTitle      = NULL;   
    bi.ulFlags        = BIF_USENEWUI;  
    bi.lpfn           = BrowseCallBackFun6;     //�ص�����  
	bi.lParam         =(LPARAM)(LPCTSTR)m_GPSDataPath;  //�����ص������Ĳ���,����Ĭ��·��  
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TCHAR pszPath[MAX_PATH];  
    BROWSEINFO bi;   
    bi.hwndOwner      = this->GetSafeHwnd();  
    bi.pidlRoot       = NULL;  
    bi.pszDisplayName = NULL;   
    bi.lpszTitle      = NULL;   
    bi.ulFlags        = BIF_USENEWUI;  
    bi.lpfn           = BrowseCallBackFun6;     //�ص�����  
	bi.lParam         =(LPARAM)(LPCTSTR)m_MatchedGravDataPath;  //�����ص������Ĳ���,����Ĭ��·��  
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
