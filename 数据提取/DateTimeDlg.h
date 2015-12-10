#pragma once
#include "atlcomtime.h"


// CDateTimeDlg 对话框

class CDateTimeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDateTimeDlg)

public:
	CDateTimeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDateTimeDlg();

// 对话框数据
	enum { IDD = IDD_DateTimeDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	COleDateTime m_Time;
	COleDateTime m_Date;
	CString m_RangeStr;
};
