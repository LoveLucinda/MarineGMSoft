#pragma once
#include "atlcomtime.h"


// CDateTimeDlg �Ի���

class CDateTimeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDateTimeDlg)

public:
	CDateTimeDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDateTimeDlg();

// �Ի�������
	enum { IDD = IDD_DateTimeDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	COleDateTime m_Time;
	COleDateTime m_Date;
	CString m_RangeStr;
};
