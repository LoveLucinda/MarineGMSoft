// ExtractGPSDataDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "������ȡ.h"
#include "ExtractGPSDataDlg.h"
#include "afxdialogex.h"


// CExtractGPSDataDlg �Ի���

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


// CExtractGPSDataDlg ��Ϣ�������


void CExtractGPSDataDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	CDialogEx::OnOK();
}


void CExtractGPSDataDlg::OnBnClickedRadio1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	m_PathEdit.EnableWindow(m_SingleOrMulti);
	m_PathNameButton.EnableWindow(m_SingleOrMulti);
}


void CExtractGPSDataDlg::OnBnClickedRadio2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	m_PathEdit.EnableWindow(m_SingleOrMulti);
	m_PathNameButton.EnableWindow(m_SingleOrMulti);
}


void CExtractGPSDataDlg::OnBnClickedCheck1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
void CExtractGPSDataDlg::OnBnClickedButton1()
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
    bi.lpfn           = BrowseCallBackFun5;     //�ص�����  
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
