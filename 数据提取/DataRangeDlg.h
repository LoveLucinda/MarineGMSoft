#pragma once


// CDataRangeDlg �Ի���

class CDataRangeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDataRangeDlg)

public:
	CDataRangeDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDataRangeDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	double m_xmin;
	double m_xmax;
	double m_ymin;
	double m_ymax;
	afx_msg void OnBnClickedOk();
};
