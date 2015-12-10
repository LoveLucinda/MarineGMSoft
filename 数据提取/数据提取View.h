// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://go.microsoft.com/fwlink/?LinkId=238214。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// 数据提取View.h : C数据提取View 类的接口
//

#pragma once
#include "数据提取Doc.h"
#include "vector"
#include "afxwin.h"
#include "atltypes.h"
#include "progressdlg.h"
#include "datetimedlg.h"
#include "datarangedlg.h"
#include "timecorrectdlg.h"
#include "extractgpsdatadlg.h"
#include "matchgpsgravdlg.h"
//加入word的头文件
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
class C数据提取View : public CView
{
protected: // 仅从序列化创建
	C数据提取View();
	DECLARE_DYNCREATE(C数据提取View)

// 特性
public:
	C数据提取Doc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~C数据提取View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
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
	POINTINDEXSTRUCT** m_PointIndex;//用于确定图像中一个区域内数据索引的二维数组，在读取重力数据的函数里和视图大小变化size函数里进行清零（释放内存）；在绘制图像函数中进行初始化，用new申请一个与图像区域像素大小一样的二维数据
	int m_Row_PointIndex;
	int m_Col_PointIndex;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	int ClearPointIndex(void);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	CRect m_SelectedRect;
	int UpdateSelectRectFigure(CRect SelectedRect);
	CRect m_CourseFigureRect;//航迹图的绘图区域
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
	//自定义消息映射函数
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

#ifndef _DEBUG  // 数据提取View.cpp 中的调试版本
inline C数据提取Doc* C数据提取View::GetDocument() const
   { return reinterpret_cast<C数据提取Doc*>(m_pDocument); }
#endif

