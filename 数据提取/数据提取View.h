// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://go.microsoft.com/fwlink/?LinkId=238214��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// ������ȡView.h : C������ȡView ��Ľӿ�
//

#pragma once
#include "������ȡDoc.h"
#include "vector"
#include "afxwin.h"
#include "atltypes.h"
#include "progressdlg.h"
#include "datetimedlg.h"
#include "datarangedlg.h"
#include "timecorrectdlg.h"
#include "extractgpsdatadlg.h"
#include "matchgpsgravdlg.h"
//����word��ͷ�ļ�
#include "CTable0.h"
#include "CCell.h"
#include "CFont0.h"
#include "capplication.h"
#include "cdocument0.h"
#include "cdocuments.h"
#include "cselection.h"
#include "crange.h"
#include "CTables0.h"
#include "CParagraph.h"
#include "CBorder.h"
#include "CBorders.h"
#include "writesurveylinetoword.h"
#include "CRow.h"
#include "CRows.h"
#include "CColumn.h"
#include "CColumns0.h"
#include "CCells.h"
#include "mergesurveylinedlg.h"
#include "CPageSetup.h"
using namespace std;
class C������ȡView : public CView
{
protected: // �������л�����
	C������ȡView();
	DECLARE_DYNCREATE(C������ȡView)

// ����
public:
	C������ȡDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~C������ȡView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnButton3();
public:
	CString m_DataPathName;
	vector<CString> m_DataFileNameVector;
	int DataFileNumber;
	vector<OGDataStruct>m_OGDataVector;
	vector<double>m_VelocityVector,m_TimeVector;
	vector<double>m_RemoveXVector,m_RemoveYVector,m_ZoomUpXVector,m_ZoomUpYVector;
	double xmin,xmax,ymin,ymax;
public:
	int GetFileCount(CString PathName);
	void CollectData(CString DataFileName);
	void SelectLanLonGrav(CString DataFileName);
	afx_msg void OnRemovedata();
	afx_msg void OnRemovepathofdata();
	bool ReadDat(CString DataFileName);
	int PlotLine(CRect rc,CString Title,CString Title2,vector<OGDataStruct>OGDataVector,vector<double>RemoveXVector,vector<double>RemoveYVector);
	int m_LineWidth;
	COLORREF m_LineColor,m_RemoveDataLineColor;
	int m_GridLineWidth;
	COLORREF m_GridColor;
	CFont m_Font;
	COLORREF m_TextColor;
	CString m_LineTitle;
	int m_TextSpaceY;
	CRect m_Rect;
	COLORREF m_BkColor;
	bool m_IsShowGrid;
	int m_XGridSize;
	int m_YGridSize;
	CString m_CurrentDataFileName;
	
public:
	afx_msg void OnEdit3();
	afx_msg void OnClear();
	int PlotLine(CRect rc,CString Title,CString Title2,vector<double>xVector,vector<double>yVector);
	int PlotLine(CRect rc,CString Title,CString Title2,vector<double>yVector);
public:
	afx_msg void OnDisplayvelocity();
	afx_msg void OnDisplaygooddata();
	afx_msg void OnDisplayremovedata();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSmoothvelocity();
	CString m_DataInfo;
	CString DataRangeStr;

public:
	void PlotZoomUpLine(void);
	int m_ZoomUpRange;
	int m_CurrentZoomUpRangeLeft;
	int m_ZoomRangeMoveSpeed;
	CDC* m_pDC;
	CDC m_BitMapDC;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void InitBitMap(void);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnStart();
	int m_TimeSpace;
public:
	afx_msg void OnStop();
	int PlotVelocity(CRect rc,CString Title,CString Title2,vector<OGDataStruct>yVector,int currentZoomNumber);
private:
	int m_CurrentSelectPointNumber;
public:
	afx_msg void OnSelectpointslider();
	void UpdateMainPlot(void);
private:
	bool m_FlagFinishiPoint;
	bool m_FlagStartPoint;
	int m_StartPointNumber;
	int m_FinishPointNumber;
public:
	afx_msg void OnStartpoint();
	afx_msg void OnBack();
	afx_msg void OnFinishpoint();
	afx_msg void OnOutdataline();
	afx_msg void OnMainfigure();
	afx_msg void OnAdvance();
	int UpdateParameter(void);
	CProgressDlg *m_ProgressDlg;
public:
	int ShowProgressBar(int CurrentPos, int MaxPos);
	CWinThread* m_SelectAndRemoveDataThread;
	CWinThread* m_ReadGravDataThread;
	int m_SmoothPointNum;
	COLORREF m_AxisColor;
	COLORREF m_ZoomColor;
	COLORREF m_StartPointColor;
	COLORREF m_FinishPointColor;
private:
	float m_VelocityThreshold;
public:
	void SelectLanLonGravToFile(CString DataFileName,FILE *fpout);
	CString m_ZoomUpDataInfo;
	afx_msg void OnDisplaygravdata();
	int PlotGrav(void);
	int GetDistance(void);
	CString m_Distance;
	CDateTimeDlg m_DateTimeDlg;
	afx_msg void OnSelectstartdata();
	afx_msg void OnSelectenddate();
	CMFCRibbonProgressBar* m_ProgressBar;
	float m_ProgressSpace;
	afx_msg void OnSmoothgravity();
	afx_msg void OnButton2();
	CDataRangeDlg m_DataRangeDlg;
	afx_msg void OnBgcorrect();
	bool SearchIndex_ij(GRDSTRUCT datainfo, double distance0,double x0, double y0, int& row, int& col);
	void Correct_CubicForward(GRDSTRUCT datainfo,int correctnumber, GRDRANGEINDEX* indexarray, double* demdata,double* CorrectData);
	clock_t m_StartTime;
	clock_t m_FinishTime;
	int m_Frequency_MainFigure;
	int m_Frquency_ZoomFigure;
	afx_msg void OnFileSaveAs();
	afx_msg void OnFileSave();
	afx_msg void OnGetgpuproperty();
	afx_msg void OnTimecorrect();
	CTimeCorrectDlg m_TimeCorrectDlg;
	afx_msg void OnOutputfga();
	afx_msg void OnExtractgps();
	void ReadGPSDataFile(CString gpsdatafilename, vector<GPSSTRUCT>& GPSVECTOR);
	int GetFileCount(CString PathName,CString ExtenName, vector<CString>& filenamevector);
	void OutPutGPSData(CString pathname, vector<GPSSTRUCT> GPSDATAVECTOR);
	CExtractGPSDataDlg m_ExtractGPSDataDlg;
	int GetCharCountInCString(CString str, CString str0);
	afx_msg void OnCheckamend();
	void ReadExtractedGPSFile(CString filename, vector<GPSSTRUCT>& gpsdatavector);
	void AmendGPSTimeDate(CString pathname,vector<CString> filenamevector);
	void WriteExtractedGPSData(CString pathname, vector<GPSSTRUCT> GPSDATAVECTOR,int startrow=0,bool IsCheck=false);
	void CorrectGPSTimeData(CString pathname,int& row1,vector<GPSSTRUCT>firstGPSvector,vector<GPSSTRUCT>secondGPSvector);
	afx_msg void OnMatchgpsGrav();
	CMatchGPSGRAVDlg m_MatchGPSGravDlg;
	bool WriteGRAVGPSData(CString pathname,vector<OGDataStruct> ogdavavector, vector<GPSSTRUCT> gpsdatavector);
	POINTINDEXSTRUCT** m_PointIndex;//����ȷ��ͼ����һ�����������������Ķ�ά���飬�ڶ�ȡ�������ݵĺ��������ͼ��С�仯size������������㣨�ͷ��ڴ棩���ڻ���ͼ�����н��г�ʼ������new����һ����ͼ���������ش�Сһ���Ķ�ά����
	int m_Row_PointIndex;
	int m_Col_PointIndex;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	int ClearPointIndex(void);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	CRect m_SelectedRect;
	int UpdateSelectRectFigure(CRect SelectedRect);
	CRect m_CourseFigureRect;//����ͼ�Ļ�ͼ����
	int SetZoomUpSliderRange(int range);
	void ReadGPSDataFile_GPGGA(CString gpsdatafilename, vector<GPSSTRUCT>& GPSVECTOR);
	afx_msg void OnCreatewordreport();
	CApplication m_wdApp;
	CDocument0 m_wdDoc;
	CDocuments m_wdDocs;
	CSelection m_wdSel;
	CRange m_wdRange;
	BOOL CreateWordApp(void);
	BOOL CreateWordDocuments(void);
	BOOL CreateWordDocument(void);
	BOOL CreateWord(void);
	void ShowWordApp(void);
	void HideWordApp(void);
	int WriteTextToWord(CString szText);
	CWriteSurveyLineToWord m_WriteToWordDlg;
	void MergeCell(CTable0& wordTable,int row1,int col1,int row2,int col2,CString text);
	bool ReadGravData(CString DataFileName);
	void CloseWordApp(void);
	CMergeSurveyLineDlg m_MergeSurveyLineDlg;
	void OutPutData(CString filename,vector<OGDataStruct> gravdata);
	int CheckUse(void);
	int UpdataProgress(int pos,UINT ribbonelementID);
	int SetProgressRange(int min, int max,UINT ribbonelementID);
	bool InspectGPSFile(CString gpsfilename);
	//�Զ�����Ϣӳ�亯��
	afx_msg LRESULT OnUpdateProgress( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnSetProgressRange( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnUpdatePar( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnGetDistance( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnUpdateFigure( WPARAM wParam, LPARAM lParam );
	int m_ProgressMin;
	int m_ProgressMax;
	int m_CurrentProgressPos;
	bool m_Flag_ReadGravThread;
	vector<CString> m_FileNameVector;
	CString m_OutPutFileName;
	CString m_LogFileName;
	bool m_Flag_ReadMultiGravThread;
	CString GetFileNameByPath(CString pathname,bool isWithFileSuffix=false);
};


//extern "C"
//void GPU_Correct(double* CorrectData,int CorrectDataNumber,double* DemData,GRDRANGEINDEX* IndexArray,GRDSTRUCT demdatainfo);
//extern "C"
//void GetGPUProperty();

#ifndef _DEBUG  // ������ȡView.cpp �еĵ��԰汾
inline C������ȡDoc* C������ȡView::GetDocument() const
   { return reinterpret_cast<C������ȡDoc*>(m_pDocument); }
#endif

