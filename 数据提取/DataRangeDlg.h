#pragma once


// CDataRangeDlg 对话框

class CDataRangeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDataRangeDlg)

public:
	CDataRangeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDataRangeDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double m_xmin;
	double m_xmax;
	double m_ymin;
	double m_ymax;
	afx_msg void OnBnClickedOk();
};
