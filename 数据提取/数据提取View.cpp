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

// 数据提取View.cpp : C数据提取View 类的实现


#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "数据提取.h"
#endif


#include "数据提取Doc.h"
#include "数据提取View.h"
#include "MainFrm.h"
#include "CubicForward.h"
#include "io.h"



//读取数据线程函数
UINT ReadGravDataTreadFunc(LPVOID ThreadParam);//读取单个重力文件线程函数
UINT MergeGravityFileThreadFunc(LPVOID ThreadParam);//读取多个重力文件线程函数
UINT UpdateFigureThrFunAfterRead(LPVOID ThreadParam);//读取重力数据完了以后的更新图像
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// C数据提取View

IMPLEMENT_DYNCREATE(C数据提取View, CView)

BEGIN_MESSAGE_MAP(C数据提取View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &C数据提取View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_BUTTON3, &C数据提取View::OnButton3)
	ON_COMMAND(ID_RemoveData, &C数据提取View::OnRemovedata)
	ON_COMMAND(ID_RemovePathOfData, &C数据提取View::OnRemovepathofdata)
	ON_COMMAND(ID_EDIT3, &C数据提取View::OnEdit3)
	ON_COMMAND(ID_Clear, &C数据提取View::OnClear)
	ON_COMMAND(ID_DisplayVelocity, &C数据提取View::OnDisplayvelocity)
	ON_COMMAND(ID_DisplayGoodData, &C数据提取View::OnDisplaygooddata)
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_SmoothVelocity, &C数据提取View::OnSmoothvelocity)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_COMMAND(ID_Start, &C数据提取View::OnStart)
	ON_COMMAND(ID_Stop, &C数据提取View::OnStop)
	ON_COMMAND(ID_SelectPointSlider, &C数据提取View::OnSelectpointslider)
	ON_COMMAND(ID_StartPoint, &C数据提取View::OnStartpoint)
	ON_COMMAND(ID_Back, &C数据提取View::OnBack)
	ON_COMMAND(ID_FinishPoint, &C数据提取View::OnFinishpoint)
	ON_COMMAND(ID_OutDataLine, &C数据提取View::OnOutdataline)
	ON_COMMAND(ID_MainFigure, &C数据提取View::OnMainfigure)
	ON_COMMAND(ID_Advance, &C数据提取View::OnAdvance)
	ON_COMMAND(ID_DisplayGravData, &C数据提取View::OnDisplaygravdata)
	ON_COMMAND(ID_SelectStartData, &C数据提取View::OnSelectstartdata)
	ON_COMMAND(ID_SelectEndDate, &C数据提取View::OnSelectenddate)
	ON_COMMAND(ID_SmoothGravity, &C数据提取View::OnSmoothgravity)
	ON_COMMAND(ID_BUTTON2, &C数据提取View::OnButton2)
	ON_COMMAND(ID_BGCorrect, &C数据提取View::OnBgcorrect)
	ON_COMMAND(ID_FILE_SAVE_AS, &C数据提取View::OnFileSaveAs)
	ON_COMMAND(ID_FILE_SAVE, &C数据提取View::OnFileSave)
	ON_COMMAND(ID_GetGPUProperty, &C数据提取View::OnGetgpuproperty)
	ON_COMMAND(ID_TimeCorrect, &C数据提取View::OnTimecorrect)
	ON_COMMAND(ID_OutPutFGA, &C数据提取View::OnOutputfga)
	ON_COMMAND(ID_ExtractGPS, &C数据提取View::OnExtractgps)
	ON_COMMAND(ID_CheckAmend, &C数据提取View::OnCheckamend)
	ON_COMMAND(ID_MatchGPS_GRAV, &C数据提取View::OnMatchgpsGrav)
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_CreateWordReport, &C数据提取View::OnCreatewordreport)
	//自定义消息映射
	ON_MESSAGE(WM_PROGRESS_MESSAGE, OnUpdateProgress )
	ON_MESSAGE(WM_PROGRESS_Range, OnSetProgressRange )
	ON_MESSAGE(WM_UpdatePar, OnUpdatePar )
	ON_MESSAGE(WM_GetDistance, OnGetDistance )
	ON_MESSAGE(WM_UpdateFigure, OnUpdateFigure )
END_MESSAGE_MAP()

// C数据提取View 构造/析构

C数据提取View::C数据提取View()
	: m_DataPathName(_T(""))
	, m_LineWidth(1)
	, m_GridLineWidth(1)
	, m_LineTitle(_T("航迹图示"))
	, m_TextSpaceY(2)
	, m_IsShowGrid(false)
	, m_XGridSize(30)
	, m_YGridSize(20)
	,xmin(999999)
	,xmax(-999999)
	,ymin(9999999)
	,ymax(-9999999)
	, m_CurrentDataFileName(_T(""))
	, m_DataInfo(_T(""))
	, DataRangeStr(_T(""))
	, m_ZoomUpRange(1000)
	, m_CurrentZoomUpRangeLeft(0)
	, m_ZoomRangeMoveSpeed(1000)
	, m_pDC(NULL)
	, m_TimeSpace(200)
	, m_CurrentSelectPointNumber(0)
	, m_FlagFinishiPoint(false)
	, m_FlagStartPoint(false)
	, m_StartPointNumber(0)
	, m_FinishPointNumber(0)
	,m_ProgressDlg(NULL)
	, m_SelectAndRemoveDataThread(NULL)
	,m_ReadGravDataThread(NULL)
	, m_SmoothPointNum(1501)
	, m_VelocityThreshold(0)
	, m_ZoomUpDataInfo(_T(""))
	, m_Distance(_T(""))
	, m_ProgressSpace(0.2)
	, m_Frequency_MainFigure(0)
	, m_Frquency_ZoomFigure(0)
	, m_PointIndex(NULL)
	, m_Row_PointIndex(0)
	, m_Col_PointIndex(0)
	, m_ProgressMin(0)
	, m_ProgressMax(0)
	, m_CurrentProgressPos(0)
	, m_Flag_ReadGravThread(false)
	, m_OutPutFileName(_T(""))
	, m_LogFileName(_T(""))
	, m_Flag_ReadMultiGravThread(false)
{
	// TODO: 在此处添加构造代码
	m_BkColor=RGB(0,0,0);
	m_LineColor=RGB(255,255,0);
	m_RemoveDataLineColor=RGB(255,0,255);
	m_TextColor=RGB(100,100,100);
	m_GridColor=RGB(20,20,20);
	m_AxisColor=RGB(100,100,100);
	m_ZoomColor=RGB(0,0,255);
	m_StartPointColor=RGB(0,255,0);
	m_FinishPointColor=RGB(255,0,255);
	m_Font.CreatePointFont(180,_T("宋体"));
	
	m_ZoomRangeMoveSpeed=m_ZoomUpRange/4*3;//移动速度（移动量大小），范围宽度的四分之三(* )
	m_SelectedRect=CRect(CPoint(0,0),CPoint(0,0));
	CheckUse();
}

C数据提取View::~C数据提取View()
{
	ClearPointIndex();
	if (m_wdApp)
	{
		m_wdApp.ReleaseDispatch();
	}
	if (m_wdDocs)
	{
		m_wdDocs.ReleaseDispatch();
	}
}

//读取多个重力文件线程函数
UINT MergeGravityFileThreadFunc(LPVOID ThreadParam)
{
	 C数据提取View * pView = (C数据提取View *)ThreadParam;
	 pView->m_Flag_ReadMultiGravThread=true;//多个重力读取线程被占用
	 
	 int filenum=pView->m_FileNameVector.size();
	pView->m_ProgressMin=0;pView->m_ProgressMax=filenum+1;					//进度条范围变量
	pView->SendMessage( WM_PROGRESS_Range, 0, IDR_MAINPROGRESSBAR );	//更新进度条范围
	
	 //1. 读取多个文件
	for (int i = 0; i < filenum; i++)
	{
		pView->m_CurrentProgressPos=i;
		pView->SendMessage( WM_PROGRESS_MESSAGE, 0, IDR_MAINPROGRESSBAR );	//更新进度条状态
		while (pView->m_Flag_ReadGravThread)//还没读完
		{

		}
		pView->m_Flag_ReadGravThread=true;
		pView->m_CurrentDataFileName=pView->m_FileNameVector[i];
		AfxBeginThread((AFX_THREADPROC)ReadGravDataTreadFunc,pView);
	}
	pView->m_CurrentProgressPos=filenum;
	pView->SendMessage( WM_PROGRESS_MESSAGE, 0, IDR_MAINPROGRESSBAR );	//更新进度条状态
	while (pView->m_Flag_ReadGravThread)//还没读完
	{

	}
	//2. 输出合并文件
	FILE *fpOut=NULL;
	if ((fpOut=fopen((_bstr_t)pView->m_OutPutFileName,"w"))==NULL)
	{
		AfxMessageBox(_T("打开")+pView->m_OutPutFileName+_T("失败"));
		pView->m_Flag_ReadMultiGravThread=false;//多个重力读取线程已用完
		return 0;
	}
	vector<OGDataStruct> gravdata=pView->m_OGDataVector;
	for (int i = 0; i <gravdata.size(); i++)
	{
		fprintf(fpOut,"$DAT\t%s   \t%s\t%8.2f\t%8.2f\t%11.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%13.8f\t%13.8f\t%8.3f\t%8.3f\n",
			(PCSTR)(_bstr_t)(PCTSTR)gravdata[i].datatimeStr,(PCSTR)(_bstr_t)(PCTSTR)gravdata[i].day,
			gravdata[i].gravity,gravdata[i].spinttension,
			gravdata[i].beamposition,gravdata[i].VCC,gravdata[i].AL,gravdata[i].AX,gravdata[i].VE,gravdata[i].AX2,gravdata[i].XACC2,
			gravdata[i].LACC2,gravdata[i].CROSSACCEL,gravdata[i].LONGACCEL,gravdata[i].EOTVOS,gravdata[i].LONGITUDE,gravdata[i].LATITUDE,
			gravdata[i].VELOCITY,gravdata[i].Heading);
	}
	fclose(fpOut);
	pView->m_CurrentProgressPos=filenum+1;
	pView->SendMessage( WM_PROGRESS_MESSAGE, 0, IDR_MAINPROGRESSBAR );	//更新进度条状态
	//3. 输出合并日志
	FILE *fplog=NULL;
	if ((fplog=fopen((_bstr_t)pView->m_LogFileName,"w"))==NULL)
	{
		AfxMessageBox(_T("打开")+pView->m_LogFileName+_T("失败"));
		pView->m_Flag_ReadMultiGravThread=true;//多个重力读取线程已用完
		return 0;
	} 
	for (int i = 0; i < filenum; i++)
	{
		fprintf(fplog,"%s\n",(PCSTR)(_bstr_t)(PCTSTR)pView->m_FileNameVector[i]/*.Right(m_DataFileNameVector[i].GetLength()-m_DataPathName.GetLength())*/);
	}
	fclose(fplog);
	
	CString str;str.Format(_T("共整合了%d个文件"),filenum);AfxMessageBox(str);

	pView->m_Flag_ReadMultiGravThread=true;//多个重力读取线程已用完
	pView->m_CurrentProgressPos=0;
	pView->SendMessage( WM_PROGRESS_MESSAGE, 0, IDR_MAINPROGRESSBAR );	//更新进度条状态
	pView->SendMessage( WM_PROGRESS_MESSAGE, 0, IDR_CHILDROGRESSBAR );	//更新进度条状态
	return 0;
}
//设置进度条当前位置
LRESULT C数据提取View::OnUpdateProgress( WPARAM wParam, LPARAM lParam )
{
	int ribbonelementID = lParam;
	CMainFrame* pMainFrm=(CMainFrame*)AfxGetApp()->GetMainWnd();
	CMFCRibbonProgressBar* prog = DYNAMIC_DOWNCAST(CMFCRibbonProgressBar, pMainFrm->m_wndStatusBar.FindElement(ribbonelementID));
	prog->SetPos(m_CurrentProgressPos);
	return NULL;
}
//设置进度条范围
LRESULT C数据提取View::OnSetProgressRange( WPARAM wParam, LPARAM lParam )
{
	int ribbonelementID = lParam;
	CMainFrame* pMainFrm=(CMainFrame*)AfxGetApp()->GetMainWnd();
	CMFCRibbonProgressBar* prog = DYNAMIC_DOWNCAST(CMFCRibbonProgressBar, pMainFrm->m_wndStatusBar.FindElement(ribbonelementID));
	prog->SetRange(m_ProgressMin,m_ProgressMax);
	return NULL;
}
//设置更新绘图参数
LRESULT C数据提取View::OnUpdatePar( WPARAM wParam, LPARAM lParam )
{
	UpdateParameter();
	return NULL;
}
//获取航线长度
LRESULT C数据提取View::OnGetDistance( WPARAM wParam, LPARAM lParam )
{
	GetDistance();
	return NULL;
}
//获取航线长度
LRESULT C数据提取View::OnUpdateFigure( WPARAM wParam, LPARAM lParam )
{
	UpdateMainPlot();
	return NULL;
}

/*DWORD WINAPI C数据提取View::*/
UINT ReadGravDataTreadFunc(LPVOID ThreadParam)
{
	//HWND hWnd=(HWND)ThreadParam;
    C数据提取View * pView = (C数据提取View *)ThreadParam;
	pView->m_Flag_ReadGravThread=true;//重力读取线程标志表示被占用
	//根据S型重礼仪软件说明书（Micro-g LaCoste Air-Sea Gravity Meter –SystemII ）第73页数据个数说明，每个项目的起始位置和列宽
	//读取ID
	CStdioFile fp;
	if (fp.Open(pView->m_CurrentDataFileName,CFile::modeRead)==FALSE)
	{
		AfxMessageBox(_T("打开")+pView->m_CurrentDataFileName+_T("失败"));
		pView->m_Flag_ReadGravThread=false;//重力读取线程已用完
		return false;
	}
	
	long progressspace=fp.GetLength()/1990;//标准重力文件每行有199个字节，然后把行数10等分，显示进度条
	pView->m_ProgressMin=0;pView->m_ProgressMax=10;					//进度条范围变量
	pView->SendMessage( WM_PROGRESS_Range, 0, IDR_CHILDROGRESSBAR );	//更新进度条范围
	pView->SendMessage( WM_PROGRESS_MESSAGE, 0, IDR_CHILDROGRESSBAR );	//更新进度条状态

	CString LineStr,IDSTR;
	OGDataStruct OceanGravityData;
	CString datestr,timestr;
	long linenum=0;
	char IDCHAR[10],DATECHAR[20],TIMECHAR[20],DAYCHAR[20];
	fp.ReadString(LineStr);
	IDSTR=LineStr.Left(4);
	if (IDSTR==_T("$DAT"))		//原始数据
	{
		fp.SeekToBegin();
		if (LineStr.GetLength()==197)		//（标准格式）原始数据格式或者改进版本的写入格式
		{
			while (fp.ReadString(LineStr))
			{
				datestr=LineStr.Mid(5,10);
				timestr=LineStr.Mid(16,8);
				OceanGravityData.datatimeStr.Format(_T("%s %s"),datestr,timestr);
				OceanGravityData.DataTime.ParseDateTime(OceanGravityData.datatimeStr);
				OceanGravityData.day=LineStr.Mid(28,3);
				OceanGravityData.gravity=atof(LineStr.Mid(32,8));
				OceanGravityData.spinttension=atof(LineStr.Mid(41,8));
				OceanGravityData.beamposition=atof(LineStr.Mid(50,11));
				OceanGravityData.VCC=atof(LineStr.Mid(62,8));
				OceanGravityData.AL=atof(LineStr.Mid(71,8));
				OceanGravityData.AX=atof(LineStr.Mid(80,8));
				OceanGravityData.VE=atof(LineStr.Mid(89,8));
				OceanGravityData.AX2=atof(LineStr.Mid(98,8));
				OceanGravityData.XACC2=atof(LineStr.Mid(107,8));
				OceanGravityData.LACC2=atof(LineStr.Mid(116,8));
				OceanGravityData.CROSSACCEL=atof(LineStr.Mid(125,8));
				OceanGravityData.LONGACCEL=atof(LineStr.Mid(134,8));
				OceanGravityData.EOTVOS=atof(LineStr.Mid(143,8));
				OceanGravityData.LONGITUDE=atof(LineStr.Mid(152,13));
				OceanGravityData.LATITUDE=atof(LineStr.Mid(166,13));
				OceanGravityData.VELOCITY=atof(LineStr.Mid(180,8));
				OceanGravityData.Heading=atof(LineStr.Mid(188,8));
				pView->m_OGDataVector.push_back(OceanGravityData);
				
				linenum++;
				if (linenum%progressspace==0)
				{
					//UpdataProgress(linenum/progressspace,IDR_CHILDROGRESSBAR);//刷新进度条
					pView->m_CurrentProgressPos=linenum/progressspace;
					pView->SendMessage( WM_PROGRESS_MESSAGE, 0, IDR_CHILDROGRESSBAR );
				}
				
			}
			pView->m_DataInfo=datestr;
			pView->m_DataInfo=_T("日期:")+pView->m_DataInfo;
		}else
		{
			while (fp.ReadString(LineStr))
			{
				sscanf(LineStr,"%s%s%s%s%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf",IDCHAR,DATECHAR,TIMECHAR,DAYCHAR,&OceanGravityData.gravity,&OceanGravityData.spinttension,&OceanGravityData.beamposition,&OceanGravityData.VCC,&OceanGravityData.AL,
				&OceanGravityData.AX,&OceanGravityData.VE,&OceanGravityData.AX2,&OceanGravityData.XACC2,&OceanGravityData.LACC2,
				&OceanGravityData.CROSSACCEL,&OceanGravityData.LONGACCEL,&OceanGravityData.EOTVOS,&OceanGravityData.LONGITUDE,&OceanGravityData.LATITUDE,
				&OceanGravityData.VELOCITY,&OceanGravityData.Heading);
				OceanGravityData.datatimeStr.Format(_T("%s %s"),DATECHAR,TIMECHAR);
				OceanGravityData.day=(PCTSTR)(_bstr_t)DAYCHAR;
				//pView->m_OGDataVector.push_back(OceanGravityData);
				
				OceanGravityData.DataTime.ParseDateTime(OceanGravityData.datatimeStr);
				linenum++;
				if (linenum%progressspace==0)
				{
					//UpdataProgress(linenum/progressspace,IDR_CHILDROGRESSBAR);//刷新进度条
					pView->m_CurrentProgressPos=linenum/progressspace;
					pView->SendMessage( WM_PROGRESS_MESSAGE, 0, IDR_CHILDROGRESSBAR );
				}
			}
			pView->m_DataInfo.Format(_T("%s"),DATECHAR);
			pView->m_DataInfo=_T("日期:")+pView->m_DataInfo;
		}
		
		
	}
	else
	{
		AfxMessageBox(_T("文件格式不正确,除了坐标其他列将以字符形式处理"));
		//pView->m_Flag_ReadGravThread=false;//重力读取线程已用完
		//return false;
		//=============================================================================
		char tempchar[500];
		while (fp.ReadString(LineStr))
		{
			sscanf(LineStr, "%lf%lf", &OceanGravityData.LONGITUDE, &OceanGravityData.LATITUDE);
			////OceanGravityData.datatimeStr.Format(_T("%s %s"), DATECHAR, TIMECHAR);
			OceanGravityData.tempstr = LineStr;
			OceanGravityData.VELOCITY = 12.0;
			/*long maxsize = pView->m_OGDataVector.max_size();
			CString str; str.Format(_T("%ld"), maxsize); AfxMessageBox(str);*/
			pView->m_OGDataVector.push_back(OceanGravityData);
			linenum++;
			if (linenum%progressspace == 0)
			{
				//UpdataProgress(linenum/progressspace,IDR_CHILDROGRESSBAR);//刷新进度条
				pView->m_CurrentProgressPos = linenum / progressspace;
				pView->SendMessage(WM_PROGRESS_MESSAGE, 0, IDR_CHILDROGRESSBAR);
			}
		}
		pView->m_DataInfo.Format(_T("XXXXX"));
		AfxMessageBox(_T("读取完毕"));
	}

	//关闭文件
	fp.Close();
	
	pView->m_Flag_ReadGravThread=false;//重力读取线程已用完
	return 0;
}
UINT UpdateFigureThrFunAfterRead(LPVOID ThreadParam)
{
	 C数据提取View * pView = (C数据提取View *)ThreadParam;
	 while (pView->m_Flag_ReadGravThread)//读取文件线程没有结束则执行while空循环
	 {

	 }
	pView->m_CurrentProgressPos=2;
	pView->SendMessage( WM_PROGRESS_MESSAGE, 0, IDR_CHILDROGRESSBAR );

	//刷新视图
	pView->SendMessage( WM_UpdatePar, 0, 0 );//更新参数
	//1.1 计算航迹长度
	pView->SendMessage( WM_GetDistance, 0, 0 );	
	pView->m_CurrentProgressPos=3;
	pView->SendMessage( WM_PROGRESS_MESSAGE, 0, IDR_CHILDROGRESSBAR );
	
	//1.2 调用绘图函数
	pView->SendMessage( WM_UpdateFigure, 0, 0 );	
	pView->m_CurrentProgressPos=4;
	pView->SendMessage( WM_PROGRESS_MESSAGE, 0, IDR_CHILDROGRESSBAR );
	
	pView->m_CurrentProgressPos=0;
	pView->SendMessage( WM_PROGRESS_MESSAGE, 0, IDR_CHILDROGRESSBAR );

	return 0;
}
BOOL C数据提取View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// C数据提取View 绘制

void C数据提取View::OnDraw(CDC* pDC)
{
	C数据提取Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	CRect rc;GetClientRect(&rc);
	pDC->BitBlt(rc.left,rc.top,rc.Width(),rc.Height(),&m_BitMapDC,0,0,SRCCOPY);

}


// C数据提取View 打印


void C数据提取View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL C数据提取View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void C数据提取View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void C数据提取View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void C数据提取View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void C数据提取View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// C数据提取View 诊断

#ifdef _DEBUG
void C数据提取View::AssertValid() const
{
	CView::AssertValid();
}

void C数据提取View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

C数据提取Doc* C数据提取View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(C数据提取Doc)));
	return (C数据提取Doc*)m_pDocument;
}
#endif //_DEBUG


// C数据提取View 消息处理程序


int CALLBACK BrowseCallBackFun3(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)  
{  
    switch(uMsg)  
    {  
    case BFFM_INITIALIZED:  //选择文件夹对话框初始化  
        //设置默认路径为lpData即'D:\'  
        ::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);  
        //在STATUSTEXT区域显示当前路径  
        ::SendMessage(hwnd, BFFM_SETSTATUSTEXT, 0, lpData);  
        //设置选择文件夹对话框的标题  
        ::SetWindowText(hwnd, TEXT("选择路径"));   
        break;  
    case BFFM_SELCHANGED:   //选择文件夹变更时  
        {  
            TCHAR pszPath[MAX_PATH];  
            //获取当前选择路径  
            SHGetPathFromIDList((LPCITEMIDLIST)lParam, pszPath);  
            //在STATUSTEXT区域显示当前路径  
            ::SendMessage(hwnd, BFFM_SETSTATUSTEXT, TRUE, (LPARAM)pszPath);  
        }  
        break;  
    }  
    return 0;  
}  
void C数据提取View::OnButton3()
{
	//从路径中获取文件名
	if (m_Flag_ReadGravThread || m_Flag_ReadMultiGravThread)
	{
		MessageBox(_T("读取重力数据线程正在使用，等待完成"));
		return;
	}

	if (m_MergeSurveyLineDlg.DoModal()!=IDOK)
	{
		return;
	}
	vector<CString>filenamevector;
	int filenum=GetFileCount(m_MergeSurveyLineDlg.m_PathName,m_MergeSurveyLineDlg.m_ExtenFileName,filenamevector);

	//依此调用打开命令
	m_FileNameVector=filenamevector;//文件名向量
	m_OutPutFileName=m_MergeSurveyLineDlg.m_PathName+_T("\\测线合并.dat");
	m_LogFileName=m_MergeSurveyLineDlg.m_PathName+_T("\\合并日志.log");

	//调用合并测线线程函数
	AfxBeginThread((AFX_THREADPROC)MergeGravityFileThreadFunc,this);

	/*SetProgressRange(0,3,IDR_MAINPROGRESSBAR);UpdataProgress(1,IDR_MAINPROGRESSBAR);
	SetProgressRange(0,filenum,IDR_CHILDROGRESSBAR);
	for (int i = 0; i < filenum; i++)
	{
		ReadGravData(filenamevector[i]);
		UpdataProgress(i,IDR_CHILDROGRESSBAR);
	}*/

	//输出合并文件
	//OnFileSave();
	//UpdataProgress(2,IDR_MAINPROGRESSBAR);
	//CString outputfilename=m_MergeSurveyLineDlg.m_PathName+_T("\\测线合并.dat");
	//OutPutData(m_OutPutFileName,m_OGDataVector);
	////输出合并日志
	//CString logfilename=m_MergeSurveyLineDlg.m_PathName+_T("\\合并日志.log");
	//FILE *fpOut=NULL;
	//if ((fpOut=fopen((_bstr_t)m_LogFileName,"w"))==NULL)
	//{
	//	AfxMessageBox(_T("打开")+m_LogFileName+_T("失败"));
	//	return ;
	//} 
	//UpdataProgress(3,IDR_MAINPROGRESSBAR);
	//for (int i = 0; i < filenum; i++)
	//{
	//	fprintf(fpOut,"%s\n",(PCSTR)(_bstr_t)(PCTSTR)filenamevector[i]/*.Right(m_DataFileNameVector[i].GetLength()-m_DataPathName.GetLength())*/);
	//}
	//fclose(fpOut);
	//UpdataProgress(4,IDR_MAINPROGRESSBAR);UpdataProgress(0,IDR_MAINPROGRESSBAR);
	//CString str;str.Format(_T("共整合了%d个文件"),filenum);MessageBox(str);

}

int C数据提取View::GetFileCount(CString PathName,CString ExtenName, vector<CString>& filenamevector)
{
	//计数器先清零
	DataFileNumber=0;
	CFileFind finder;
	//路径
	CString strWildcard(PathName); 
	strWildcard += _T("//*.*");

// 开始查找第一个文件
	BOOL bWorking = finder.FindFile(strWildcard);
	filenamevector.clear();//使用向量前先清空
	//判断循环，直到下一个不存在
	while (bWorking) 
	{ 
		bWorking = finder.FindNextFile();

		if (finder.IsDots()) //判断是否是文件 
		continue; 
		if(!finder.IsDirectory()) 
		{ 
			if (ExtenName.GetLength()==0)
			{
				DataFileNumber ++; 
				filenamevector.push_back(finder.GetFilePath());
			}else
			{
				if (finder.GetFilePath().Right(ExtenName.GetLength())==ExtenName)
				{
					DataFileNumber ++; 
					filenamevector.push_back(finder.GetFilePath());
				}
			 }
			
		 } 
	 }
	finder.Close(); 

	return DataFileNumber;
}
int C数据提取View::GetFileCount(CString PathName)
{
	//计数器先清零
	DataFileNumber=0;
	CFileFind finder;
	//路径
	CString strWildcard(PathName); 
	strWildcard += _T("//*.*");

// 开始查找第一个文件
	BOOL bWorking = finder.FindFile(strWildcard);
	m_DataFileNameVector.clear();//使用向量前先清空
	//判断循环，直到下一个不存在
	while (bWorking) 
	{ 
		bWorking = finder.FindNextFile();

		if (finder.IsDots()) //判断是否是文件 
		continue; 
		if(!finder.IsDirectory()) 
		{ 
			if (finder.GetFilePath().Right(3)==_T("DAT"))
			{
				DataFileNumber ++; 
				m_DataFileNameVector.push_back(finder.GetFilePath());
			}
			//else
			//{
			//	if (finder.GetFilePath().Right(3)==_T("inf"))
			//	{
			//		m_ZoneRangeFilename=finder.GetFilePath();
			//	}
			//}
		} 
		//if (finder.IsDirectory()) //判断是否是文件夹 //路径 
		//{ 
		//	CString str = finder.GetFilePath(); 
		//	//如果想获得子路径下的文件个数，可以继续调用该函数
		//	//GetFileCount(str);
		//}
	}
	finder.Close(); 

	return DataFileNumber;
}


void C数据提取View::CollectData(CString DataFileName)
{
	CString NewDataFileName;
	NewDataFileName=DataFileName.Left(DataFileName.GetLength()-4)+_T("-整理后.DAT");
	//AfxMessageBox(NewDataFileName);
	//打开文件
	FILE *fp=NULL,*fpout=NULL;
	if ((fp=fopen((_bstr_t)DataFileName,"r"))==NULL)
	{
		AfxMessageBox(_T("打开")+DataFileName+_T("失败"));
		return ;
	}
	if ((fpout=fopen((_bstr_t)NewDataFileName,"w"))==NULL)
	{
		AfxMessageBox(_T("打开")+NewDataFileName+_T("失败"));
		return ;
	}

	//读取文件
	char symbol[20];
	double /*Gravity,SpringTension,*/BeamPosition,VCC,AL,AX,VE,AX2,XACC2,LACC2,CROSS,LONGACCEL,Eotvos,Longtitude,Latitude,Heading,Velocity;
	CString tempstr,IDStr,DATE_year_Str,MonthStr,DayStr,HourStr,MinuteStr,SecondStr,YearStr,GravStr,SpringTensitonStr;

	
	char firstchar[50];
	while (!feof(fp))
	//for (int j = 0; j < 2; j++)
	{
		for (int i = 0; i < 50; i++)
		{
			firstchar[i]=fgetc(fp);
		}
		//fscanf(fp,"%s",tempchar);
		tempstr=(PCTSTR)(_bstr_t)firstchar;
		tempstr=tempstr.Left(49);
		if (tempstr.Left(1)!=_T("$"))
		{
			break;
		}
		SpringTensitonStr=tempstr.Right(8);tempstr=tempstr.Left(tempstr.GetLength()-8-1);
		GravStr=tempstr.Right(8);tempstr=tempstr.Left(tempstr.GetLength()-8-1);
		YearStr=tempstr.Right(3);tempstr=tempstr.Left(tempstr.GetLength()-3-1-3);
		SecondStr=tempstr.Right(2);tempstr=tempstr.Left(tempstr.GetLength()-2-1);
		MinuteStr=tempstr.Right(2);tempstr=tempstr.Left(tempstr.GetLength()-2-1);
		HourStr=tempstr.Right(2);tempstr=tempstr.Left(tempstr.GetLength()-2-1);
		DayStr=tempstr.Right(2);tempstr=tempstr.Left(tempstr.GetLength()-2-1);
		MonthStr=tempstr.Right(2);tempstr=tempstr.Left(tempstr.GetLength()-2-1);
		DATE_year_Str=tempstr.Right(4);tempstr=tempstr.Left(tempstr.GetLength()-2-1);

		fscanf(fp,"%lf%s%lf%s%lf%s%lf%s%lf%s%lf%s%lf%s%lf%s%lf%s%lf%s%lf%s%lf%s%lf%s%lf%s%lf%s",&BeamPosition,symbol,&VCC,symbol,&AL,symbol,&AX,symbol,
			&VE,symbol,&AX2,symbol,&XACC2,symbol,&LACC2,symbol,&CROSS,symbol,&LONGACCEL,symbol,&Eotvos,symbol,&Longtitude,symbol,&Latitude,symbol,&Velocity,symbol,&Heading,symbol);
		fgetc(fp);
		//下面的作用是去掉逗号
		/*AfxMessageBox(tempstr);
		AfxMessageBox(IDStr);
		AfxMessageBox(DATEStr);
		AfxMessageBox(TimeStr);
		AfxMessageBox(YearStr);
		AfxMessageBox(GravStr);
		AfxMessageBox(SpringTensitonStr);*/
		//写文件
		/*fprintf(fpout,"%s\t%s\t%s\t%s\t%s\t%s\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.8f\t%.8f\t%.3f\t%.3f\n",(PCSTR)(_bstr_t)(PCTSTR)IDStr,
			(PCSTR)(_bstr_t)(PCTSTR)DATEStr,(PCSTR)(_bstr_t)(PCTSTR)TimeStr,(PCSTR)(_bstr_t)(PCTSTR)YearStr,(PCSTR)(_bstr_t)(PCTSTR)GravStr,
			(PCSTR)(_bstr_t)(PCTSTR)SpringTensitonStr,BeamPosition,VCC,AL,AX,VE,AX2,XACC2,LACC2,CROSS,LONGACCEL,Eotvos,Longtitude,Latitude,Heading,Velocity);*/
		fprintf(fpout,"%s %s %s\t%s %s %s\t%s\t%s\t%s\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.8f\t%.8f\t%.3f\t%.3f\n",
			(PCSTR)(_bstr_t)(PCTSTR)DATE_year_Str,(PCSTR)(_bstr_t)(PCTSTR)MonthStr,(PCSTR)(_bstr_t)(PCTSTR)DayStr,(PCSTR)(_bstr_t)(PCTSTR)HourStr,(PCSTR)(_bstr_t)(PCTSTR)MinuteStr,
			(PCSTR)(_bstr_t)(PCTSTR)SecondStr,(PCSTR)(_bstr_t)(PCTSTR)YearStr,(PCSTR)(_bstr_t)(PCTSTR)GravStr,(PCSTR)(_bstr_t)(PCTSTR)SpringTensitonStr,
			BeamPosition,VCC,AL,AX,VE,AX2,XACC2,LACC2,CROSS,LONGACCEL,Eotvos,Longtitude,Latitude,Heading,Velocity);
	}
	
	fclose(fp);fclose(fpout);
}


void C数据提取View::SelectLanLonGrav(CString DataFileName)
{
	CString NewDataFileName;
	NewDataFileName=DataFileName.Left(DataFileName.GetLength()-4)+_T("-提取重力-经纬度-速度.DAT");
	//AfxMessageBox(NewDataFileName);
	//打开文件
	FILE *fp=NULL,*fpout=NULL;
	if ((fp=fopen((_bstr_t)DataFileName,"r"))==NULL)
	{
		AfxMessageBox(_T("打开")+DataFileName+_T("失败"));
		return ;
	}
	if ((fpout=fopen((_bstr_t)NewDataFileName,"w"))==NULL)
	{
		AfxMessageBox(_T("打开")+NewDataFileName+_T("失败"));
		return ;
	}

	//读取文件
	char symbol[20];
	double /*Gravity,SpringTension,*/BeamPosition,VCC,AL,AX,VE,AX2,XACC2,LACC2,CROSS,LONGACCEL,Eotvos,Longtitude,Latitude,Heading,Velocity;
	CString tempstr,IDStr,DATE_year_Str,MonthStr,DayStr,HourStr,MinuteStr,SecondStr,YearStr,GravStr,SpringTensitonStr;

	
	char firstchar[50];
	while (!feof(fp))
	//for (int j = 0; j < 2; j++)
	{
		for (int i = 0; i < 50; i++)
		{
			firstchar[i]=fgetc(fp);
		}
		//fscanf(fp,"%s",tempchar);
		tempstr=(PCTSTR)(_bstr_t)firstchar;
		tempstr=tempstr.Left(49);
		if (tempstr.Left(1)!=_T("$"))
		{
			break;
		}
		SpringTensitonStr=tempstr.Right(8);tempstr=tempstr.Left(tempstr.GetLength()-8-1);
		GravStr=tempstr.Right(8);tempstr=tempstr.Left(tempstr.GetLength()-8-1);
		YearStr=tempstr.Right(3);tempstr=tempstr.Left(tempstr.GetLength()-3-1-3);
		SecondStr=tempstr.Right(2);tempstr=tempstr.Left(tempstr.GetLength()-2-1);
		MinuteStr=tempstr.Right(2);tempstr=tempstr.Left(tempstr.GetLength()-2-1);
		HourStr=tempstr.Right(2);tempstr=tempstr.Left(tempstr.GetLength()-2-1);
		DayStr=tempstr.Right(2);tempstr=tempstr.Left(tempstr.GetLength()-2-1);
		MonthStr=tempstr.Right(2);tempstr=tempstr.Left(tempstr.GetLength()-2-1);
		DATE_year_Str=tempstr.Right(4);tempstr=tempstr.Left(tempstr.GetLength()-2-1);

		fscanf(fp,"%lf%s%lf%s%lf%s%lf%s%lf%s%lf%s%lf%s%lf%s%lf%s%lf%s%lf%s%lf%s%lf%s%lf%s%lf%s",&BeamPosition,symbol,&VCC,symbol,&AL,symbol,&AX,symbol,
			&VE,symbol,&AX2,symbol,&XACC2,symbol,&LACC2,symbol,&CROSS,symbol,&LONGACCEL,symbol,&Eotvos,symbol,&Longtitude,symbol,&Latitude,symbol,&Velocity,symbol,&Heading,symbol);
		fgetc(fp);
		//下面的作用是去掉逗号
		/*AfxMessageBox(tempstr);
		AfxMessageBox(IDStr);
		AfxMessageBox(DATEStr);
		AfxMessageBox(TimeStr);
		AfxMessageBox(YearStr);
		AfxMessageBox(GravStr);
		AfxMessageBox(SpringTensitonStr);*/
		//写文件
		fprintf(fpout,"%.8f\t%.8f\t%s\t%.3f\n",Longtitude,Latitude,(PCSTR)(_bstr_t)(PCTSTR)GravStr,Velocity);
	}
	
	fclose(fp);fclose(fpout);
}


void C数据提取View::OnRemovedata()
{
	if (m_Flag_ReadGravThread || m_Flag_ReadMultiGravThread)
	{
		MessageBox(_T("读取重力数据线程正在使用，等待完成"));
		return;
	}
	// TODO: 在此添加命令处理程序代码
	//UpdateParameter();//更新参数
	TCHAR szFilter[] = _T("海洋重力数据 文件（*.DAT)|*.DAT|所有文件(*.*)|*.*||");
	CFileDialog FileDlg(TRUE, _T("DAT"),NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL);
	FileDlg.m_ofn.lpstrTitle=_T("打开海洋重力数据文件");

	/*TCHAR szBuf[MAX_PATH];
	ZeroMemory(szBuf,MAX_PATH);
	GetCurrentDirectory(MAX_PATH,szBuf);
	CString currentpath(szBuf);*/
	//设置对话框当前路径
	//FileDlg.GetOFN().lpstrInitialDir =currentpath;
	CString DataFileName;
	if (IDOK == FileDlg.DoModal())
	{   
		DataFileName = FileDlg.GetPathName();  
	}
	else
	{
		return;
	}
	
	if (m_CurrentDataFileName==DataFileName)	//如果两次选择同一个文件则清空前面所有数据，重新绘制这个文件的处理结果，不连接
	{
		xmin=99999;xmax=-99999;
		ymin=99999;ymax=-99999;
		m_OGDataVector.clear();
		m_RemoveXVector.clear();
		m_RemoveYVector.clear();
		m_VelocityVector.clear();
		m_TimeVector.clear();
		m_ZoomUpXVector.clear();
		m_ZoomUpYVector.clear();
	}else
	{
		m_CurrentDataFileName=DataFileName;
	}
	//ReadDat(m_CurrentDataFileName);
	//工作者线程读取数据,读取数据函数里面有更新图像代码
	m_ReadGravDataThread=AfxBeginThread((AFX_THREADPROC)ReadGravDataTreadFunc,this);
	
	//进入绘图线程，等待读取完毕更新视图
	AfxBeginThread((AFX_THREADPROC)UpdateFigureThrFunAfterRead,this);
	
	//ReadGravData(m_CurrentDataFileName);   UpdataProgress(2,IDR_MAINPROGRESSBAR);
	////UpdateParameter();//更新参数
	////1.1 计算航迹长度
	//GetDistance();		UpdataProgress(3,IDR_MAINPROGRESSBAR);
	////1.2 调用绘图函数
	//UpdateMainPlot();	UpdataProgress(4,IDR_MAINPROGRESSBAR);
	//UpdataProgress(0,IDR_MAINPROGRESSBAR);
}


void C数据提取View::OnRemovepathofdata()
{
	// TODO: 在此添加命令处理程序代码
	TCHAR pszPath[MAX_PATH];  
    BROWSEINFO bi;   
    bi.hwndOwner      = this->GetSafeHwnd();  
    bi.pidlRoot       = NULL;  
    bi.pszDisplayName = NULL;   
    bi.lpszTitle      = NULL;   
    bi.ulFlags        = BIF_USENEWUI;  
    bi.lpfn           = BrowseCallBackFun3;     //回调函数  
    bi.lParam         =(LPARAM)(LPCTSTR)m_DataPathName;  //传给回调函数的参数,设置默认路径  
    bi.iImage         = 0;   
    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);  
    if (pidl == NULL)  
    {  
        return;  
    }  
      
    if (SHGetPathFromIDList(pidl, pszPath))  
    {  
        m_DataPathName=pszPath;
    }  

	//从路径中获取文件名
	int filenum=GetFileCount(m_DataPathName);
	for(int i=0;i<filenum;i++)
	{
		ReadDat(m_DataFileNameVector[i]);
	}
}


bool C数据提取View::ReadDat(CString DataFileName)
{

	//打开文件
	//m_ProgressBar->SetPos(m_ProgressBar->GetRangeMax()*0.4);		//进度条
	FILE *fp=NULL/*,*fpout=NULL,*fpRemove=NULL*/;
	if ((fp=fopen((_bstr_t)DataFileName,"r"))==NULL)
	{
		//AfxMessageBox(_T("打开")+DataFileName+_T("失败"));
		//m_ProgressBar->SetPos(m_ProgressBar->GetRangeMin());
		return false;
	}
	//m_ProgressBar->SetPos(m_ProgressBar->GetRangeMax());		//进度条
	ClearPointIndex();
	//读取文件
	char symbol;
	//double /*Gravity,SpringTension,*/BeamPosition,VCC,AL,AX,VE,AX2,XACC2,LACC2,CROSS,LONGACCEL,Eotvos,Longtitude,Latitude,Heading,Velocity;
	OGDataStruct OceanGravityData;
	CString tempstr,IDStr,DATE_year_Str,MonthStr,DayStr,HourStr,MinuteStr,SecondStr,YearStr,GravStr,SpringTensitonStr;

	////读取数据之前先清空向量
	//xVector.clear();yVector.clear();
	//RemoveXVector.clear();RemoveYVector.clear();
	char firstchar[50],char_4[4],char_8[8],char_11[11];
	int timenum=0;
	for (int i = 0; i < 50; i++)
	{
		firstchar[i]=fgetc(fp);
	}

	tempstr=(PCTSTR)(_bstr_t)firstchar;
	if (tempstr.Left(5)==_T("$DAT,"))
	{
		//m_ProgressBar->SetRange(0,86400);
		int space=86400*m_ProgressSpace;
		rewind(fp);
		while (!feof(fp))
		//for (int j = 0; j < 1500; j++)
		{

			for (int i = 0; i < 50; i++)
			{
				firstchar[i]=fgetc(fp);
			}
			//fscanf(fp,"%s",tempchar);
			tempstr=(PCTSTR)(_bstr_t)firstchar;
			tempstr=tempstr.Left(49);
			//判断是否是有效的字符串
			if (tempstr.Left(1)!=_T("$"))
			{
				break;
			}
			SpringTensitonStr=tempstr.Right(8);tempstr=tempstr.Left(tempstr.GetLength()-8-1);
			GravStr=tempstr.Right(8);tempstr=tempstr.Left(tempstr.GetLength()-8-1);
			YearStr=tempstr.Right(3);tempstr=tempstr.Left(tempstr.GetLength()-3-1-3);
			SecondStr=tempstr.Right(2);tempstr=tempstr.Left(tempstr.GetLength()-2-1);
			MinuteStr=tempstr.Right(2);tempstr=tempstr.Left(tempstr.GetLength()-2-1);
			HourStr=tempstr.Right(2);tempstr=tempstr.Left(tempstr.GetLength()-2-1);
			DayStr=tempstr.Right(2);tempstr=tempstr.Left(tempstr.GetLength()-2-1);
			MonthStr=tempstr.Right(2);tempstr=tempstr.Left(tempstr.GetLength()-2-1);
			DATE_year_Str=tempstr.Right(4);tempstr=tempstr.Left(tempstr.GetLength()-2-1);
			OceanGravityData.day=YearStr;
			OceanGravityData.gravity=atof(GravStr);
			OceanGravityData.spinttension=atof(SpringTensitonStr);
			OceanGravityData.date=DATE_year_Str+_T("/")+MonthStr+_T("/")+DayStr;
			OceanGravityData.time=HourStr+_T(":")+MinuteStr+_T(":")+SecondStr;
			OceanGravityData.DataTime.ParseDateTime(OceanGravityData.date+_T(" ")+OceanGravityData.time);						//把时间日期字符串转化为时间类型变量
			OceanGravityData.datatimeStr=OceanGravityData.DataTime.Format("%Y-%m-%d\t%H:%M:%S");									//日期字符串
			fscanf(fp,"%lf%c%lf%c%lf%c%lf%c%lf%c%lf%c%lf%c%lf%c%lf%c%lf%c%lf%c%lf%c%lf%c%lf%c%lf%c",&OceanGravityData.beamposition,&symbol,&OceanGravityData.VCC,&symbol,&OceanGravityData.AL,
				&symbol,&OceanGravityData.AX,&symbol,&OceanGravityData.VE,&symbol,&OceanGravityData.AX2,&symbol,&OceanGravityData.XACC2,&symbol,&OceanGravityData.LACC2,&symbol,
				&OceanGravityData.CROSSACCEL,&symbol,&OceanGravityData.LONGACCEL,&symbol,&OceanGravityData.EOTVOS,&symbol,&OceanGravityData.LONGITUDE,&symbol,&OceanGravityData.LATITUDE,
				&symbol,&OceanGravityData.VELOCITY,&symbol,&OceanGravityData.Heading,&symbol);
			fgetc(fp);//读取回车符
		
			//保存时间和速度到向量中
			//m_TimeVector.push_back(timenum);timenum++;
			//保存经纬度到内存
			m_OGDataVector.push_back(OceanGravityData);
			
			//if (m_OGDataVector.size()%space==0)		//进度条
			//{
			//	m_ProgressBar->SetPos(m_OGDataVector.size());
			//}
		}
		//m_ProgressBar->SetPos(m_ProgressBar->GetRangeMin());
		//m_DataInfo=_T("日期:")+DATE_year_Str+_T("年")+MonthStr+_T("月")+DayStr+_T("日   ");
		m_DataInfo=OceanGravityData.DataTime.Format("%Y-%m-%d");
		m_DataInfo=_T("日期:")+m_DataInfo;
	}
	if (tempstr.Left(5)==_T("$DAT "))
	{
		//m_ProgressBar->SetRange(0,86400);
		int space=86400*m_ProgressSpace;
		rewind(fp);
		while (!feof(fp))
		{
			for (int i = 0; i < 28; i++)
			{
				firstchar[i]=fgetc(fp);
			}
			//fscanf(fp,"%s",tempchar);
			tempstr=(PCTSTR)(_bstr_t)firstchar;
			tempstr=tempstr.Left(28);
			//判断是否是有效的字符串
			if (tempstr.Left(1)!=_T("$"))
			{
				break;
			}
			OceanGravityData.day=tempstr.Right(3);tempstr=tempstr.Left(tempstr.GetLength()-4);
			OceanGravityData.time=tempstr.Right(8);tempstr=tempstr.Left(tempstr.GetLength()-9);
			OceanGravityData.date=tempstr.Right(10);//MessageBox(OceanGravityData.date);
			OceanGravityData.DataTime.ParseDateTime(OceanGravityData.date+_T(" ")+OceanGravityData.time);						//把时间日期字符串转化为时间类型变量
			OceanGravityData.datatimeStr=OceanGravityData.DataTime.Format("%Y-%m-%d\t%H:%M:%S");									//日期字符串
			fscanf(fp,"%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf",&OceanGravityData.gravity,&OceanGravityData.spinttension,&OceanGravityData.beamposition,&OceanGravityData.VCC,&OceanGravityData.AL,
				&OceanGravityData.AX,&OceanGravityData.VE,&OceanGravityData.AX2,&OceanGravityData.XACC2,&OceanGravityData.LACC2,
				&OceanGravityData.CROSSACCEL,&OceanGravityData.LONGACCEL,&OceanGravityData.EOTVOS,&OceanGravityData.LONGITUDE,&OceanGravityData.LATITUDE,
				&OceanGravityData.VELOCITY,&OceanGravityData.Heading,symbol);
			fgetc(fp);//读取回车符
		
			//保存时间和速度到向量中
			//m_TimeVector.push_back(timenum);timenum++;
			//保存经纬度到内存
			m_OGDataVector.push_back(OceanGravityData);
			/*if (m_OGDataVector.size()%space==0)
			{
				m_ProgressBar->SetPos(m_OGDataVector.size());
			}*/
		}
		//m_ProgressBar->SetPos(m_ProgressBar->GetRangeMax());
		//m_ProgressBar->SetPos(m_ProgressBar->GetRangeMin());
		m_DataInfo=OceanGravityData.DataTime.Format("%Y-%m-%d");
		m_DataInfo=_T("日期:")+m_DataInfo;
	}
	
	fclose(fp);/*fclose(fpout);fclose(fpRemove);*/
	
	CMainFrame* pMainFrm=(CMainFrame*)AfxGetApp()->GetMainWnd();
	COleVariant PropValue=pMainFrm->m_wndProperties.m_wndPropList.GetProperty(0)->GetSubItem(0)->GetValue();
	CMFCPropertyGridProperty* m_pProp=pMainFrm->m_wndProperties.m_wndPropList.GetProperty(0)->GetSubItem(0);
	m_ZoomUpRange=atoi((CString)PropValue);
	if (m_ZoomUpRange>=m_OGDataVector.size())
	{
		m_ZoomUpRange=m_OGDataVector.size()/2;
		m_ZoomRangeMoveSpeed=m_ZoomUpRange/2;
		//CString tempstr;
		//tempstr.Format(_T("%d"),m_ZoomUpRange/4);
		
		//m_pProp->SetValue((COleVariant)_T("12"));
		//tempstr.Format(_T("%d"),m_ZoomUpRange/2);MessageBox(tempstr);
		//pMainFrm->m_wndProperties.m_wndPropList.GetProperty(0)->GetSubItem(1)->SetValue((COleVariant)tempstr);
	}
	return true;
}

bool C数据提取View::ReadGravData(CString DataFileName)
{
	//防止界面卡死
     // OutputDebugString("gdsgsd");
	//根据S型重礼仪软件说明书（Micro-g LaCoste Air-Sea Gravity Meter –SystemII ）第73页数据个数说明，每个项目的起始位置和列宽
	//读取ID
	CStdioFile fp;
	if (fp.Open(DataFileName,CFile::modeRead)==FALSE)
	{
		//AfxMessageBox(_T("打开")+DataFileName+_T("失败"));
		return false;
	}
	int progressspace=86400/100;
	SetProgressRange(0,100,IDR_CHILDROGRESSBAR);//设置进度条范围
	CString LineStr,IDSTR;
	OGDataStruct OceanGravityData;
	CString datestr,timestr;
	int linenum=0;
	char IDCHAR[10],DATECHAR[20],TIMECHAR[20],DAYCHAR[20];
	fp.ReadString(LineStr);
	IDSTR=LineStr.Left(4);
	if (IDSTR==_T("$DAT"))		//原始数据
	{
		fp.SeekToBegin();
		if (LineStr.GetLength()==197)		//（标准格式）原始数据格式或者改进版本的写入格式
		{
			while (fp.ReadString(LineStr))
			{
				datestr=LineStr.Mid(5,10);
				timestr=LineStr.Mid(16,8);
				OceanGravityData.datatimeStr.Format(_T("%s %s"),datestr,timestr);
				OceanGravityData.DataTime.ParseDateTime(OceanGravityData.datatimeStr);
				OceanGravityData.day=LineStr.Mid(28,3);
				OceanGravityData.gravity=atof(LineStr.Mid(32,8));
				OceanGravityData.spinttension=atof(LineStr.Mid(41,8));
				OceanGravityData.beamposition=atof(LineStr.Mid(50,11));
				OceanGravityData.VCC=atof(LineStr.Mid(62,8));
				OceanGravityData.AL=atof(LineStr.Mid(71,8));
				OceanGravityData.AX=atof(LineStr.Mid(80,8));
				OceanGravityData.VE=atof(LineStr.Mid(89,8));
				OceanGravityData.AX2=atof(LineStr.Mid(98,8));
				OceanGravityData.XACC2=atof(LineStr.Mid(107,8));
				OceanGravityData.LACC2=atof(LineStr.Mid(116,8));
				OceanGravityData.CROSSACCEL=atof(LineStr.Mid(125,8));
				OceanGravityData.LONGACCEL=atof(LineStr.Mid(134,8));
				OceanGravityData.EOTVOS=atof(LineStr.Mid(143,8));
				OceanGravityData.LONGITUDE=atof(LineStr.Mid(152,13));
				OceanGravityData.LATITUDE=atof(LineStr.Mid(166,13));
				OceanGravityData.VELOCITY=atof(LineStr.Mid(180,8));
				OceanGravityData.Heading=atof(LineStr.Mid(188,8));
				m_OGDataVector.push_back(OceanGravityData);
				
				linenum++;
				if (linenum%progressspace==0)
				{
					UpdataProgress(linenum/progressspace,IDR_CHILDROGRESSBAR);//刷新进度条
				}
				
			}
			m_DataInfo=datestr;
			m_DataInfo=_T("日期:")+m_DataInfo;
		}else
		{
			while (fp.ReadString(LineStr))
			{
				sscanf(LineStr,"%s%s%s%s%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf",IDCHAR,DATECHAR,TIMECHAR,DAYCHAR,&OceanGravityData.gravity,&OceanGravityData.spinttension,&OceanGravityData.beamposition,&OceanGravityData.VCC,&OceanGravityData.AL,
				&OceanGravityData.AX,&OceanGravityData.VE,&OceanGravityData.AX2,&OceanGravityData.XACC2,&OceanGravityData.LACC2,
				&OceanGravityData.CROSSACCEL,&OceanGravityData.LONGACCEL,&OceanGravityData.EOTVOS,&OceanGravityData.LONGITUDE,&OceanGravityData.LATITUDE,
				&OceanGravityData.VELOCITY,&OceanGravityData.Heading);
				OceanGravityData.datatimeStr.Format(_T("%s %s"),DATECHAR,TIMECHAR);
				OceanGravityData.day=(PCTSTR)(_bstr_t)DAYCHAR;
				m_OGDataVector.push_back(OceanGravityData);
				OceanGravityData.DataTime.ParseDateTime(OceanGravityData.datatimeStr);

				linenum++;
				if (linenum%progressspace==0)
				{
					UpdataProgress(linenum/progressspace,IDR_CHILDROGRESSBAR);//刷新进度条
				}
			}
			m_DataInfo.Format(_T("%s"),DATECHAR);
			m_DataInfo=_T("日期:")+m_DataInfo;
		}
		
		
	}
	else
	{
		MessageBox(_T("文件格式不正确"));
		return false;
	}

	//关闭文件
	fp.Close();
}
//绘制航迹图
int C数据提取View::PlotLine(CRect rc,CString Title,CString Title2,vector<OGDataStruct>OGDataVector,vector<double>RemoveXVector,vector<double>RemoveYVector)
{
	if (OGDataVector.size()<2 && RemoveXVector.size()<2)
	{
		return 0;
	}
	//1.1 求最大值最小值
	xmin=OGDataVector[0].LONGITUDE;xmax=xmin;ymin=OGDataVector[0].LATITUDE;ymax=ymin;
	for (int i = 0; i < OGDataVector.size(); i++)
	{
		if (xmin>OGDataVector[i].LONGITUDE)
		{
			xmin=OGDataVector[i].LONGITUDE;
		}
		if (xmax<OGDataVector[i].LONGITUDE)
		{
			xmax=OGDataVector[i].LONGITUDE;
		}

		if (ymin>OGDataVector[i].LATITUDE)
		{
			ymin=OGDataVector[i].LATITUDE;
		}
		if (ymax<OGDataVector[i].LATITUDE)
		{
			ymax=OGDataVector[i].LATITUDE;
		}
	}
	
	//绘图到位图中
	
	//***********************
//	CClientDC dc(this);
	CPen linePen(PS_SOLID,m_LineWidth,m_LineColor);
	CPen RemoveDataPen(PS_SOLID,m_LineWidth,m_RemoveDataLineColor);
	CPen GridPen(PS_SOLID,m_GridLineWidth,m_GridColor);
	CPen AxisPen(PS_SOLID,m_GridLineWidth,m_AxisColor);
	
	CFont* oldFont=(CFont*)m_BitMapDC.SelectObject(&m_Font);
	
	//绘制边框
	CString yminStr,xminStr;
	yminStr.Format(_T("%.8f"),(fabs(ymin)>fabs(ymax)?ymin:ymax));
	xminStr.Format(_T("%.5f"),(fabs(xmin)>fabs(xmax)?xmin:xmax));
	m_Rect.left=(m_BitMapDC.GetTextExtent(yminStr).cx);
	m_Rect.right=rc.Width()-m_TextSpaceY-20-rc.Width()/2.0;
	m_Rect.top=m_BitMapDC.GetTextExtent(Title).cy+m_TextSpaceY+m_BitMapDC.GetTextExtent(Title2).cy+m_TextSpaceY;
	m_Rect.bottom=rc.Height()-m_BitMapDC.GetTextExtent(_T("x")).cy-20;
	m_CourseFigureRect=m_Rect;
	CPen* oldpen=m_BitMapDC.SelectObject(&AxisPen);
	m_BitMapDC.MoveTo(m_Rect.left,m_Rect.top);m_BitMapDC.LineTo(m_Rect.left,m_Rect.bottom);
	m_BitMapDC.MoveTo(m_Rect.left,m_Rect.bottom);m_BitMapDC.LineTo(m_Rect.right,m_Rect.bottom);
	m_BitMapDC.MoveTo(m_Rect.right,m_Rect.bottom);m_BitMapDC.LineTo(m_Rect.right,m_Rect.top);
	m_BitMapDC.MoveTo(m_Rect.right,m_Rect.top);m_BitMapDC.LineTo(m_Rect.left,m_Rect.top);
	
	//绘制网格
	m_BitMapDC.SelectObject(&GridPen);
	m_BitMapDC.SetTextColor(m_TextColor);
	m_BitMapDC.SetBkMode(TRANSPARENT);
	m_IsShowGrid=true;
	float m_Ybeishu=(ymax-ymin)/(float)m_Rect.Height();
	//float m_Xbeishu=(float)(xVector.size()+RemoveXVector.size())/(float)m_Rect.Width();
	float m_Xbeishu=(xmax-xmin)/(float)m_Rect.Width();
	int XMarkLength=7;
	int YMarkLength=7;
	if (m_IsShowGrid)
	{
		XMarkLength=m_Rect.Height();
		YMarkLength=m_Rect.Width();
	}
	//x轴标记10个刻度，y标注5个
	m_XGridSize=m_BitMapDC.GetTextExtent(xminStr).cx;
	m_YGridSize=m_Rect.Height()/10;
	int xgridnum=m_Rect.Width()/m_XGridSize;
	int ygridnum=m_Rect.Height()/m_YGridSize;
	int x,y;
	int textx,texty;
	CString MarkStr;
	//m_BitMapDC.SetTextColor(RGB(0,0,0));				//设置坐标轴标注文字颜色
	CString XLabelFormat,YLabelFormat;
	for (int i = 0; i < 10; i++)
	{
		if ((m_Xbeishu*m_XGridSize*pow(10,i))>1)
		{
			XLabelFormat.Format(_T("%d"),i);
			break;
		}
	}
	for (int i = 0; i < 10; i++)
	{
		if ((m_Ybeishu*m_YGridSize*pow(10,i))>1)
		{
			YLabelFormat.Format(_T("%d"),i);
			break;
		}
	}
	XLabelFormat=_T("%.")+XLabelFormat+_T("f");
	YLabelFormat=_T("%.")+YLabelFormat+_T("f");
	for (int i = 1; i < xgridnum; i++)			//X轴
	{
		x=m_Rect.left+i*m_XGridSize;y=m_Rect.bottom;
		m_BitMapDC.MoveTo(x,y);
		y-=XMarkLength;
		m_BitMapDC.LineTo(x,y);

		MarkStr.Format(XLabelFormat,xmin+i*m_Xbeishu*m_XGridSize);		//标注文字
		textx=x-m_BitMapDC.GetTextExtent(MarkStr).cx/2;
		texty=m_Rect.bottom;
		m_BitMapDC.TextOut(textx,texty,MarkStr);
	}
	for (int i = 1; i < ygridnum; i++)		//Y轴
	{
		x=m_Rect.left;y=m_Rect.bottom-i*m_YGridSize;
		m_BitMapDC.MoveTo(x,y);
		x+=YMarkLength;
		m_BitMapDC.LineTo(x,y);

		MarkStr.Format(YLabelFormat,ymin+i*m_Ybeishu*m_YGridSize);		//标注文字
		textx=m_Rect.left-m_BitMapDC.GetTextExtent(MarkStr).cx;
		texty=y-m_BitMapDC.GetTextExtent(MarkStr).cy/2;
		m_BitMapDC.TextOut(textx,texty,MarkStr);
	}

	m_BitMapDC.SelectObject(&linePen);
	/*for (int i = 0; i < yVector.size()-1; i++)
	{
		m_BitMapDC.MoveTo(m_Rect.left+(xVector[i]-xmin)/m_Xbeishu,m_Rect.bottom-(yVector[i]-ymin)/m_Ybeishu);
		m_BitMapDC.LineTo(m_Rect.left+(xVector[i+1]-xmin)/m_Xbeishu,m_Rect.bottom-(yVector[i+1]-ymin)/m_Ybeishu);
	}*/
	//描绘整理后的数据
	if (!m_PointIndex)
	{
		m_Row_PointIndex=m_Rect.Height();//
		m_Col_PointIndex=m_Rect.Width();
		m_PointIndex=new POINTINDEXSTRUCT*[m_Row_PointIndex];
		for (int i = 0; i < m_Row_PointIndex; i++)
		{
			m_PointIndex[i]=new POINTINDEXSTRUCT[m_Col_PointIndex];
			for (int j = 0; j < m_Col_PointIndex; j++)
			{
				m_PointIndex[i][j].PointExit=false;
			}
		}
		
	}
	//
	int space=m_OGDataVector.size()*m_ProgressSpace;
	if (OGDataVector.size()!=0)
	{
		for (int i = 0; i < OGDataVector.size()-1; i++)
		{
			int x=m_Rect.left+(OGDataVector[i].LONGITUDE-xmin)/m_Xbeishu;
			int y=m_Rect.bottom-(OGDataVector[i].LATITUDE-ymin)/m_Ybeishu;
			m_BitMapDC.Ellipse(x,y,x+2,y+2);
			if (m_PointIndex)
			{
				int rows=y-m_Rect.top,cols=x-m_Rect.left;
				if (!((rows >= m_Row_PointIndex || rows<0) || (cols>=m_Col_PointIndex || cols<0)))//保证数组不越界，加入判断
				{
					m_PointIndex[rows][cols].PointExit=true;
					m_PointIndex[rows][cols].IndexInOGdataVector=i;
				}
				
			}
		}
	}
	//绘制局部范围标志
	space=m_OGDataVector.size()*m_ProgressSpace;
	CPen ZoomRangePen(PS_SOLID,3,m_ZoomColor);
	m_BitMapDC.SelectObject(&ZoomRangePen);
	for (int i = m_CurrentZoomUpRangeLeft; i < m_CurrentZoomUpRangeLeft+m_ZoomUpRange; i++)
	{
		int x=m_Rect.left+(m_OGDataVector[i].LONGITUDE-xmin)/m_Xbeishu;
		int y=m_Rect.bottom-(m_OGDataVector[i].LATITUDE-ymin)/m_Ybeishu;
		m_BitMapDC.Ellipse(x,y,x+2,y+2);
	}
	//绘制起点或者终点
	if (m_FlagStartPoint)
	{
		CBrush SelectPointBrush(m_StartPointColor);
		m_BitMapDC.SelectObject(&SelectPointBrush);
		x=m_Rect.left+(OGDataVector[m_StartPointNumber].LONGITUDE-xmin)/m_Xbeishu;
		y=m_Rect.bottom-(OGDataVector[m_StartPointNumber].LATITUDE-ymin)/m_Ybeishu;
		m_BitMapDC.Ellipse(x-10,y-10,x+10,y+10);
	}
	if (m_FlagFinishiPoint)
	{
		CBrush SelectPointBrush(m_FinishPointColor);
		m_BitMapDC.SelectObject(&SelectPointBrush);
		x=m_Rect.left+(OGDataVector[m_FinishPointNumber].LONGITUDE-xmin)/m_Xbeishu;
		y=m_Rect.bottom-(OGDataVector[m_FinishPointNumber].LATITUDE-ymin)/m_Ybeishu;
		m_BitMapDC.Ellipse(x-10,y-10,x+10,y+10);
	}
	//数据范围
	DataRangeStr.Format(_T("经度:")+XLabelFormat+_T("  纬度:")+YLabelFormat+_T("  "),xmax-xmin,ymax-ymin);
	//图像标题
	CSize m_StrSize;							//字符串大小（宽高）
	m_StrSize=m_BitMapDC.GetTextExtent(DataRangeStr+Title2);
	int textX=rc.left+rc.Width()/2-m_StrSize.cx/2;		//曲线标题
	m_BitMapDC.TextOut(textX,rc.top,DataRangeStr+Title2);
	
	int textY=m_StrSize.cy+m_TextSpaceY;
	textX=m_Rect.left+(m_Rect.Width()-m_BitMapDC.GetTextExtent(Title).cx)/2;
	m_BitMapDC.TextOut(textX,textY,Title);
	//恢复设备
	m_BitMapDC.SelectObject(oldpen);
	m_BitMapDC.SelectObject(oldFont);
	return 0;
}


void C数据提取View::OnEdit3()
{
	// TODO: 在此添加命令处理程序代码

}


void C数据提取View::OnClear()
{
	// TODO: 在此添加命令处理程序代码
	if (m_Flag_ReadGravThread || m_Flag_ReadMultiGravThread)
	{
		MessageBox(_T("读取重力数据线程正在使用，等待完成"));
		return;
	}

	xmin=99999;xmax=-99999;
	ymin=99999;ymax=-99999;
	m_OGDataVector.clear();
	m_RemoveXVector.clear();
	m_RemoveYVector.clear();
	m_VelocityVector.clear();
	m_TimeVector.clear();
	m_ZoomUpXVector.clear();
	m_ZoomUpYVector.clear();
	//局部放大区域的参数
	m_CurrentZoomUpRangeLeft=0;
	m_StartPointNumber=0;
	m_FinishPointNumber=0;
	CMainFrame* pMainFrm=(CMainFrame*)AfxGetApp()->GetMainWnd();
	CMFCRibbonSlider* PointNumber=DYNAMIC_DOWNCAST(CMFCRibbonSlider,pMainFrm->m_wndRibbonBar.FindByID(ID_SelectPointSlider));
	PointNumber->SetPos(0);
	m_CurrentSelectPointNumber=0;
	Invalidate(false);
}

//绘制局部放大图
int C数据提取View::PlotLine(CRect rc,CString Title,CString Title2,vector<double>xVector,vector<double>yVector)
{
	if (xVector.size()<2)
	{
		return 0;
	}
	//1.1 求最大值最小值
	xmin=xVector[0];xmax=xmin;ymin=yVector[0];ymax=ymin;
	for (int i = 0; i < xVector.size(); i++)
	{
		if (xmin>xVector[i])
		{
			xmin=xVector[i];
		}
		if (xmax<xVector[i])
		{
			xmax=xVector[i];
		}

		if (ymin>yVector[i])
		{
			ymin=yVector[i];
		}
		if (ymax<yVector[i])
		{
			ymax=yVector[i];
		}
	}
	
	//CClientDC dc(this);
	CPen linePen(PS_SOLID,m_LineWidth,m_ZoomColor);
	CPen RemoveDataPen(PS_SOLID,m_LineWidth,m_RemoveDataLineColor);
	CPen GridPen(PS_SOLID,m_GridLineWidth,m_GridColor);
	CPen AxisPen(PS_SOLID,m_GridLineWidth,m_AxisColor);
	CFont* oldFont=(CFont*)m_BitMapDC.SelectObject(&m_Font);
	m_BitMapDC.SetBkMode(TRANSPARENT);
	m_BitMapDC.SetTextColor(m_TextColor);
	CSize m_StrSize;							//字符串大小（宽高）
	m_StrSize=m_BitMapDC.GetTextExtent(Title2);
	int textX=rc.left+rc.Width()/2-m_StrSize.cx/2;		//曲线标题
	m_BitMapDC.TextOut(textX,0,Title2);
	
	int textY=rc.top+m_StrSize.cy;
	textX=rc.left+(rc.Width()-m_BitMapDC.GetTextExtent(Title).cx)/2;
	m_BitMapDC.TextOut(textX,textY,Title);
	//绘制边框
	CString yminStr,xminStr;
	yminStr.Format(_T("%.8f"),(fabs(ymin)>fabs(ymax)?ymin:ymax));
	xminStr.Format(_T("%.5f"),(fabs(xmin)>fabs(xmax)?xmin:xmax));
	m_Rect.left=rc.left+(m_BitMapDC.GetTextExtent(yminStr).cx);
	m_Rect.right=rc.right-m_TextSpaceY-20;
	m_Rect.top=rc.top+m_BitMapDC.GetTextExtent(Title).cy+m_TextSpaceY+m_BitMapDC.GetTextExtent(Title2).cy+m_TextSpaceY;
	m_Rect.bottom=rc.bottom-m_BitMapDC.GetTextExtent(_T("x")).cy-20;
	
	CPen* oldpen=m_BitMapDC.SelectObject(&AxisPen);
	m_BitMapDC.MoveTo(m_Rect.left,m_Rect.top);m_BitMapDC.LineTo(m_Rect.left,m_Rect.bottom);
	m_BitMapDC.MoveTo(m_Rect.left,m_Rect.bottom);m_BitMapDC.LineTo(m_Rect.right,m_Rect.bottom);
	m_BitMapDC.MoveTo(m_Rect.right,m_Rect.bottom);m_BitMapDC.LineTo(m_Rect.right,m_Rect.top);
	m_BitMapDC.MoveTo(m_Rect.right,m_Rect.top);m_BitMapDC.LineTo(m_Rect.left,m_Rect.top);
	//绘制网格
	m_BitMapDC.SelectObject(&GridPen);
	m_IsShowGrid=true;
	float m_Ybeishu=(ymax-ymin)/(float)m_Rect.Height();
	//float m_Xbeishu=(float)(xVector.size()+RemoveXVector.size())/(float)m_Rect.Width();
	float m_Xbeishu=(xmax-xmin)/(float)m_Rect.Width();
	int XMarkLength=7;
	int YMarkLength=7;
	if (m_IsShowGrid)
	{
		XMarkLength=m_Rect.Height();
		YMarkLength=m_Rect.Width();
	}
	//x轴标记10个刻度，y标注5个
	m_XGridSize=m_BitMapDC.GetTextExtent(xminStr).cx;
	m_YGridSize=m_Rect.Height()/10;
	int xgridnum=m_Rect.Width()/m_XGridSize;
	int ygridnum=m_Rect.Height()/m_YGridSize;
	int x,y;
	int textx,texty;
	CString MarkStr;
	//m_BitMapDC.SetTextColor(RGB(0,0,0));				//设置坐标轴标注文字颜色
	CString XLabelFormat,YLabelFormat;
	for (int i = 0; i < 10; i++)
	{
		if ((m_Xbeishu*m_XGridSize*pow(10,i))>1)
		{
			XLabelFormat.Format(_T("%d"),i);
			break;
		}
	}
	for (int i = 0; i < 10; i++)
	{
		if ((m_Ybeishu*m_YGridSize*pow(10,i))>1)
		{
			YLabelFormat.Format(_T("%d"),i);
			break;
		}
	}
	XLabelFormat=_T("%.")+XLabelFormat+_T("f");
	YLabelFormat=_T("%.")+YLabelFormat+_T("f");
	for (int i = 1; i < xgridnum; i++)			//X轴
	{
		x=m_Rect.left+i*m_XGridSize;y=m_Rect.bottom;
		m_BitMapDC.MoveTo(x,y);
		y-=XMarkLength;
		m_BitMapDC.LineTo(x,y);

		MarkStr.Format(XLabelFormat,xmin+i*m_Xbeishu*m_XGridSize);		//标注文字
		textx=x-m_BitMapDC.GetTextExtent(MarkStr).cx/2;
		texty=m_Rect.bottom;
		m_BitMapDC.TextOut(textx,texty,MarkStr);
	}
	for (int i = 1; i < ygridnum; i++)		//Y轴
	{
		x=m_Rect.left;y=m_Rect.bottom-i*m_YGridSize;
		m_BitMapDC.MoveTo(x,y);
		x+=YMarkLength;
		m_BitMapDC.LineTo(x,y);

		MarkStr.Format(YLabelFormat,ymin+i*m_Ybeishu*m_YGridSize);		//标注文字
		textx=m_Rect.left-m_BitMapDC.GetTextExtent(MarkStr).cx;
		texty=y-m_BitMapDC.GetTextExtent(MarkStr).cy/2;
		m_BitMapDC.TextOut(textx,texty,MarkStr);
	}

	m_BitMapDC.SelectObject(&linePen);
	/*for (int i = 0; i < yVector.size()-1; i++)
	{
		m_BitMapDC.MoveTo(m_Rect.left+(xVector[i]-xmin)/m_Xbeishu,m_Rect.bottom-(yVector[i]-ymin)/m_Ybeishu);
		m_BitMapDC.LineTo(m_Rect.left+(xVector[i+1]-xmin)/m_Xbeishu,m_Rect.bottom-(yVector[i+1]-ymin)/m_Ybeishu);
	}*/
	//描绘整理后的数据
	int space=yVector.size()*m_ProgressSpace;
	if (xVector.size()!=0)
	{
		for (int i = 0; i < yVector.size()-1; i++)
		{
			int x=m_Rect.left+(xVector[i]-xmin)/m_Xbeishu;
			int y=m_Rect.bottom-(yVector[i]-ymin)/m_Ybeishu;
			m_BitMapDC.Ellipse(x,y,x+2,y+2);
		}
	}
	//绘制局部放大图中当前选定的点
	CBrush SelectPointBrush(RGB(255,0,0));
	m_BitMapDC.SelectObject(&SelectPointBrush);
	x=m_Rect.left+(xVector[m_CurrentSelectPointNumber]-xmin)/m_Xbeishu;
	y=m_Rect.bottom-(yVector[m_CurrentSelectPointNumber]-ymin)/m_Ybeishu;
	m_BitMapDC.Ellipse(x-10,y-10,x+10,y+10);
	//恢复设备
	m_BitMapDC.SelectObject(oldpen);
	m_BitMapDC.SelectObject(oldFont);
	return 0;
}


int C数据提取View::PlotLine(CRect rc,CString Title,CString Title2,vector<double>yVector)
{
	if (yVector.size()<2)
	{
		return 0;
	}
	//1.1 求最大值最小值
	xmin=1;xmax=yVector.size();
	ymin=yVector[0];ymax=ymin;
	for (int i = 0; i < yVector.size(); i++)
	{
		if (ymin>yVector[i])
		{
			ymin=yVector[i];
		}
		if (ymax<yVector[i])
		{
			ymax=yVector[i];
		}
	}
	//CClientDC dc(this);
	CPen linePen(PS_SOLID,m_LineWidth,m_LineColor);
	CPen RemoveDataPen(PS_SOLID,m_LineWidth,m_RemoveDataLineColor);
	CPen GridPen(PS_SOLID,m_GridLineWidth,m_GridColor);
	CPen AxisPen(PS_SOLID,m_GridLineWidth,m_AxisColor);
	
	CFont* oldFont=(CFont*)m_BitMapDC.SelectObject(&m_Font);
	m_BitMapDC.SetBkMode(TRANSPARENT);
	m_BitMapDC.SetTextColor(m_TextColor);
	CSize m_StrSize;							//字符串大小（宽高）
	m_StrSize=m_BitMapDC.GetTextExtent(Title2);
	int textX=rc.Width()/2-m_StrSize.cx/2;		//曲线标题
	//m_BitMapDC.TextOut(textX,rc.top,Title);
	
	int textY=rc.top+m_StrSize.cy+m_TextSpaceY;
	textX=rc.left+(rc.Width()-m_BitMapDC.GetTextExtent(Title).cx)/2;
	m_BitMapDC.TextOut(textX,textY,Title);
	//绘制边框
	CString yminStr,xminStr;
	yminStr.Format(_T("%.8f"),(fabs(ymin)>fabs(ymax)?ymin:ymax));
	xminStr.Format(_T("%.5f"),(fabs(xmin)>fabs(xmax)?xmin:xmax));
	m_Rect.left=rc.left+(m_BitMapDC.GetTextExtent(yminStr).cx);
	m_Rect.right=rc.right-m_TextSpaceY-20;
	m_Rect.top=rc.top+m_BitMapDC.GetTextExtent(Title).cy+m_TextSpaceY+m_BitMapDC.GetTextExtent(Title2).cy+m_TextSpaceY;
	m_Rect.bottom=rc.bottom-m_BitMapDC.GetTextExtent(_T("x")).cy-20;
	CPen* oldpen=m_BitMapDC.SelectObject(&AxisPen);
	m_BitMapDC.MoveTo(m_Rect.left,m_Rect.top);m_BitMapDC.LineTo(m_Rect.left,m_Rect.bottom);
	m_BitMapDC.MoveTo(m_Rect.left,m_Rect.bottom);m_BitMapDC.LineTo(m_Rect.right,m_Rect.bottom);
	m_BitMapDC.MoveTo(m_Rect.right,m_Rect.bottom);m_BitMapDC.LineTo(m_Rect.right,m_Rect.top);
	m_BitMapDC.MoveTo(m_Rect.right,m_Rect.top);m_BitMapDC.LineTo(m_Rect.left,m_Rect.top);
	//绘制网格
	m_BitMapDC.SelectObject(&GridPen);
	m_IsShowGrid=true;
	float m_Ybeishu=(ymax-ymin)/(float)m_Rect.Height();
	//float m_Xbeishu=(float)(xVector.size()+RemoveXVector.size())/(float)m_Rect.Width();
	float m_Xbeishu=(xmax-xmin)/(float)m_Rect.Width();
	int XMarkLength=7;
	int YMarkLength=7;
	if (m_IsShowGrid)
	{
		XMarkLength=m_Rect.Height();
		YMarkLength=m_Rect.Width();
	}
	//x轴标记10个刻度，y标注5个
	m_XGridSize=m_BitMapDC.GetTextExtent(xminStr).cx;
	m_YGridSize=m_Rect.Height()/10;
	int xgridnum=m_Rect.Width()/m_XGridSize;
	int ygridnum=m_Rect.Height()/m_YGridSize;
	int x,y;
	int textx,texty;
	CString MarkStr;
	//m_BitMapDC.SetTextColor(RGB(0,0,0));				//设置坐标轴标注文字颜色
	//计算默认标注小数位数
	CString XLabelFormat,YLabelFormat;
	for (int i = 0; i < 10; i++)
	{
		if ((m_Xbeishu*m_XGridSize*pow(10,i))>1)
		{
			XLabelFormat.Format(_T("%d"),i);
			break;
		}
	}
	for (int i = 0; i < 10; i++)
	{
		if ((m_Ybeishu*m_YGridSize*pow(10,i))>1)
		{
			YLabelFormat.Format(_T("%d"),i);
			break;
		}
	}
	XLabelFormat=_T("%.")+XLabelFormat+_T("f");
	YLabelFormat=_T("%.")+YLabelFormat+_T("f");
	for (int i = 1; i < xgridnum; i++)			//X轴
	{
		x=m_Rect.left+i*m_XGridSize;y=m_Rect.bottom;
		m_BitMapDC.MoveTo(x,y);
		y-=XMarkLength;
		m_BitMapDC.LineTo(x,y);

		MarkStr.Format(XLabelFormat,xmin+i*m_Xbeishu*m_XGridSize);		//标注文字
		textx=x-m_BitMapDC.GetTextExtent(MarkStr).cx/2;
		texty=m_Rect.bottom;
		m_BitMapDC.TextOut(textx,texty,MarkStr);
	}
	for (int i = 1; i < ygridnum; i++)		//Y轴
	{
		x=m_Rect.left;y=m_Rect.bottom-i*m_YGridSize;
		m_BitMapDC.MoveTo(x,y);
		x+=YMarkLength;
		m_BitMapDC.LineTo(x,y);

		MarkStr.Format(YLabelFormat,ymin+i*m_Ybeishu*m_YGridSize);		//标注文字
		textx=m_Rect.left-m_BitMapDC.GetTextExtent(MarkStr).cx;
		texty=y-m_BitMapDC.GetTextExtent(MarkStr).cy/2;
		m_BitMapDC.TextOut(textx,texty,MarkStr);
	}

	m_BitMapDC.SelectObject(&linePen);
	/*for (int i = 0; i < yVector.size()-1; i++)
	{
		m_BitMapDC.MoveTo(m_Rect.left+(xVector[i]-xmin)/m_Xbeishu,m_Rect.bottom-(yVector[i]-ymin)/m_Ybeishu);
		m_BitMapDC.LineTo(m_Rect.left+(xVector[i+1]-xmin)/m_Xbeishu,m_Rect.bottom-(yVector[i+1]-ymin)/m_Ybeishu);
	}*/
	//描绘整理后的数据
	if (yVector.size()!=0)
	{
		for (int i = 0; i < yVector.size()-1; i++)
		{
			int x=m_Rect.left+i/m_Xbeishu;
			int y=m_Rect.bottom-(yVector[i]-ymin)/m_Ybeishu;
			m_BitMapDC.Ellipse(x,y,x+2,y+2);
		}
	}
	//数据范围
	//DataRangeStr.Format(_T("经度:")+XLabelFormat+_T("  纬度:")+YLabelFormat+_T("  "),xmax-xmin,ymax-ymin);
	//恢复设备
	m_BitMapDC.SelectObject(oldpen);
	m_BitMapDC.SelectObject(oldFont);
	return 0;
}


void C数据提取View::OnDisplayvelocity()
{
	// TODO: 在此添加命令处理程序代码
	//UpdateParameter();
	m_LineTitle=_T("速度时间图");
	InitBitMap();//初始化位图
	CRect rc;GetClientRect(&rc);
	PlotLine(rc,m_LineTitle,m_DataInfo+m_CurrentDataFileName,m_VelocityVector);

	Invalidate();
}


void C数据提取View::OnDisplaygooddata()
{
	// TODO: 在此添加命令处理程序代码
	m_LineTitle=_T("有效航迹");
	InitBitMap();//初始化位图
	CRect rc;GetClientRect(&rc);
//	PlotLine(rc,m_LineTitle,m_DataInfo+m_CurrentDataFileName,m_xVector,m_yVector);
	Invalidate();
	
}


BOOL C数据提取View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CBrush backBrush(m_BkColor);

	//保存旧的brush
	CBrush* pOldBrush = pDC->SelectObject(&backBrush);
	CRect rect;
	pDC->GetClipBox(&rect);

	//画需要的区域
	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);

	pDC->SelectObject(pOldBrush);
	backBrush.DeleteObject();
	pOldBrush->DeleteObject();
	DeleteObject(&rect);
	return true;
	return CView::OnEraseBkgnd(pDC);
}


void C数据提取View::OnSmoothvelocity()
{
	// TODO: 在此添加命令处理程序代码
	//对速度进行1117点圆滑处理
	int PointNum=m_SmoothPointNum;
	int FirstNum=PointNum/2;
	int UpRange=m_OGDataVector.size()-1-FirstNum;
	double sum=0;
	int space=UpRange*m_ProgressSpace;
	for (int i = FirstNum; i < UpRange; i++)
	{
		sum= m_OGDataVector[i].VELOCITY;
		for (int j= 1; j <= FirstNum; j++)
		{
			sum=sum+ m_OGDataVector[i+j].VELOCITY+ m_OGDataVector[i-j].VELOCITY;
		}
		m_OGDataVector[i].VELOCITY=sum/PointNum;
	}
	 UpdateMainPlot();
}


void C数据提取View::PlotZoomUpLine(void)
{
	if (m_OGDataVector.size()<1)
	{
		return;
	}
	if (m_OGDataVector.size()<m_ZoomUpRange)
	{
		m_ZoomUpRange=m_OGDataVector.size();
		SetZoomUpSliderRange(m_ZoomUpRange);
	}
	m_ZoomUpYVector.clear();
	m_ZoomUpXVector.clear();
	for (int i = m_CurrentZoomUpRangeLeft; i < m_CurrentZoomUpRangeLeft+m_ZoomUpRange; i++)
	{
		m_ZoomUpXVector.push_back(m_OGDataVector[i].LONGITUDE);
		m_ZoomUpYVector.push_back(m_OGDataVector[i].LATITUDE);
	}
	CRect rc;
	GetClientRect(&rc);
	rc.left=rc.left+rc.Width()/2;
	rc.bottom=rc.Height()/2;
	int index=m_CurrentZoomUpRangeLeft;
	m_ZoomUpDataInfo=m_OGDataVector[index].time+_T("—");
	index=m_CurrentZoomUpRangeLeft+m_ZoomUpRange-1;
	m_ZoomUpDataInfo+=m_OGDataVector[index].time;
	CString LongLatStr;CString str;
	LongLatStr.Format(_T(" 经度:%.6f 纬度:%.6f"),m_ZoomUpXVector[m_CurrentSelectPointNumber],m_ZoomUpYVector[m_CurrentSelectPointNumber]);
	m_ZoomUpDataInfo+=LongLatStr;
	PlotLine(rc,_T("局部")+m_ZoomUpDataInfo,_T("  "),m_ZoomUpXVector,m_ZoomUpYVector);
}


int C数据提取View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pDC=new CClientDC(this);		//初始化DC
		//创建进度条
	//m_ProgressDlg=new CProgressDlg;
	//m_ProgressDlg->Create(IDD_ProgressDlg,this);
	//
	CMainFrame* pMainFrm=(CMainFrame*)AfxGetApp()->GetMainWnd();
	m_ProgressBar=DYNAMIC_DOWNCAST(CMFCRibbonProgressBar,pMainFrm->m_wndRibbonBar.FindByID(ID_PROGRESS1));
	return 0;
}

//初始化位图
void C数据提取View::InitBitMap(void)
{
	CRect BitMapRect;GetClientRect(&BitMapRect);
	CBitmap memBitmap;
	m_BitMapDC.DeleteDC();//创建之前释放以前的
	m_BitMapDC.CreateCompatibleDC(NULL);
	memBitmap.CreateCompatibleBitmap(m_pDC,BitMapRect.Width(),BitMapRect.Height());//加入边界宽度
	m_BitMapDC.SelectObject(&memBitmap);
	CBrush newBrush;
	newBrush.CreateSolidBrush(m_BkColor);
	m_BitMapDC.SelectObject(newBrush);//背景色
	m_BitMapDC.FillRect(BitMapRect,&newBrush);
}


void C数据提取View::OnTimer(UINT_PTR nIDEvent)
{
	if (m_Flag_ReadGravThread || m_Flag_ReadMultiGravThread)
	{
		MessageBox(_T("读取重力数据线程正在使用，等待完成"));
		return;
	}
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_CurrentZoomUpRangeLeft+=m_ZoomRangeMoveSpeed;	//每来一个时钟信号，局部放大区域下边界增加
	if (m_CurrentZoomUpRangeLeft+m_ZoomUpRange>m_OGDataVector.size())
	{
		KillTimer(1);
		m_CurrentZoomUpRangeLeft=0;
		return;
	}
	UpdateMainPlot();

	CView::OnTimer(nIDEvent);
}


void C数据提取View::OnStart()
{
	if (m_Flag_ReadGravThread || m_Flag_ReadMultiGravThread)
	{
		MessageBox(_T("读取重力数据线程正在使用，等待完成"));
		return;
	}
	// TODO: 在此添加命令处理程序代码
	//UpdateParameter();
	SetTimer(1,m_TimeSpace,NULL);
}


void C数据提取View::OnStop()
{
	// TODO: 在此添加命令处理程序代码
	KillTimer(1);
}


int C数据提取View::PlotVelocity(CRect rc,CString Title,CString Title2,vector<OGDataStruct>OGDataVector,int currentNumber)
{
	if (OGDataVector.size()<2)
	{
		return 0;
	}
	//1.1 求最大值最小值
	xmin=1;xmax=OGDataVector.size();
	ymin=OGDataVector[0].VELOCITY;ymax=ymin;
	for (int i = 0; i < OGDataVector.size(); i++)
	{
		if (ymin>OGDataVector[i].VELOCITY)
		{
			ymin=OGDataVector[i].VELOCITY;
		}
		if (ymax<OGDataVector[i].VELOCITY)
		{
			ymax=OGDataVector[i].VELOCITY;
		}
	}
	//CClientDC dc(this);
	CPen linePen(PS_SOLID,m_LineWidth,m_LineColor);
	CPen RemoveDataPen(PS_SOLID,m_LineWidth,m_RemoveDataLineColor);
	CPen GridPen(PS_SOLID,m_GridLineWidth,m_GridColor);
	CPen AxisPen(PS_SOLID,m_GridLineWidth,m_AxisColor);
	
	CFont* oldFont=(CFont*)m_BitMapDC.SelectObject(&m_Font);
	m_BitMapDC.SetBkMode(TRANSPARENT);
	m_BitMapDC.SetTextColor(m_TextColor);
	CSize m_StrSize;							//字符串大小（宽高）
	m_StrSize=m_BitMapDC.GetTextExtent(Title2);
	int textX=rc.Width()/2-m_StrSize.cx/2;		//曲线标题
	//m_BitMapDC.TextOut(textX,rc.top,Title);
	
	int textY=rc.top+m_StrSize.cy+m_TextSpaceY;
	textX=rc.left+(rc.Width()-m_BitMapDC.GetTextExtent(Title).cx)/2;
	m_BitMapDC.TextOut(textX,textY,Title);
	//绘制边框
	CString yminStr,xminStr;
	yminStr.Format(_T("%.8f"),(fabs(ymin)>fabs(ymax)?ymin:ymax));
	xminStr.Format(_T("%.5f"),(fabs(xmin)>fabs(xmax)?xmin:xmax));
	m_Rect.left=rc.left+(m_BitMapDC.GetTextExtent(yminStr).cx);
	m_Rect.right=rc.right-m_TextSpaceY-20;
	m_Rect.top=rc.top+m_BitMapDC.GetTextExtent(Title).cy+m_TextSpaceY+m_BitMapDC.GetTextExtent(Title2).cy+m_TextSpaceY;
	m_Rect.bottom=rc.bottom-m_BitMapDC.GetTextExtent(_T("x")).cy-20;
	CPen* oldpen=m_BitMapDC.SelectObject(&AxisPen);
	m_BitMapDC.MoveTo(m_Rect.left,m_Rect.top);m_BitMapDC.LineTo(m_Rect.left,m_Rect.bottom);
	m_BitMapDC.MoveTo(m_Rect.left,m_Rect.bottom);m_BitMapDC.LineTo(m_Rect.right,m_Rect.bottom);
	m_BitMapDC.MoveTo(m_Rect.right,m_Rect.bottom);m_BitMapDC.LineTo(m_Rect.right,m_Rect.top);
	m_BitMapDC.MoveTo(m_Rect.right,m_Rect.top);m_BitMapDC.LineTo(m_Rect.left,m_Rect.top);
	//m_BitMapDC.Rectangle(m_Rect);
	//绘制网格
	m_BitMapDC.SelectObject(&GridPen);
	m_IsShowGrid=true;
	float m_Ybeishu=(ymax-ymin)/(float)m_Rect.Height();
	//float m_Xbeishu=(float)(xVector.size()+RemoveXVector.size())/(float)m_Rect.Width();
	float m_Xbeishu=(xmax-xmin)/(float)m_Rect.Width();
	int XMarkLength=7;
	int YMarkLength=7;
	if (m_IsShowGrid)
	{
		XMarkLength=m_Rect.Height();
		YMarkLength=m_Rect.Width();
	}
	//x轴标记10个刻度，y标注5个
	m_XGridSize=m_BitMapDC.GetTextExtent(xminStr).cx;
	m_YGridSize=m_Rect.Height()/10;
	int xgridnum=m_Rect.Width()/m_XGridSize;
	int ygridnum=m_Rect.Height()/m_YGridSize;
	int x,y;
	int textx,texty;
	CString MarkStr;
	//m_BitMapDC.SetTextColor(RGB(0,0,0));				//设置坐标轴标注文字颜色
	//计算默认标注小数位数
	CString XLabelFormat,YLabelFormat;
	for (int i = 0; i < 10; i++)
	{
		if ((m_Xbeishu*m_XGridSize*pow(10,i))>1)
		{
			XLabelFormat.Format(_T("%d"),i);
			break;
		}
	}
	for (int i = 0; i < 10; i++)
	{
		if ((m_Ybeishu*m_YGridSize*pow(10,i))>1)
		{
			YLabelFormat.Format(_T("%d"),i);
			break;
		}
	}
	XLabelFormat=_T("%.")+XLabelFormat+_T("f");
	YLabelFormat=_T("%.")+YLabelFormat+_T("f");
	for (int i = 1; i < xgridnum; i++)			//X轴
	{
		x=m_Rect.left+i*m_XGridSize;y=m_Rect.bottom;
		m_BitMapDC.MoveTo(x,y);
		y-=XMarkLength;
		m_BitMapDC.LineTo(x,y);

		MarkStr.Format(XLabelFormat,xmin+i*m_Xbeishu*m_XGridSize);		//标注文字
		textx=x-m_BitMapDC.GetTextExtent(MarkStr).cx/2;
		texty=m_Rect.bottom;
		m_BitMapDC.TextOut(textx,texty,MarkStr);
	}
	for (int i = 1; i < ygridnum; i++)		//Y轴
	{
		x=m_Rect.left;y=m_Rect.bottom-i*m_YGridSize;
		m_BitMapDC.MoveTo(x,y);
		x+=YMarkLength;
		m_BitMapDC.LineTo(x,y);

		MarkStr.Format(YLabelFormat,ymin+i*m_Ybeishu*m_YGridSize);		//标注文字
		textx=m_Rect.left-m_BitMapDC.GetTextExtent(MarkStr).cx;
		texty=y-m_BitMapDC.GetTextExtent(MarkStr).cy/2;
		m_BitMapDC.TextOut(textx,texty,MarkStr);
	}

	m_BitMapDC.SelectObject(&linePen);
	/*for (int i = 0; i < yVector.size()-1; i++)
	{
		m_BitMapDC.MoveTo(m_Rect.left+(xVector[i]-xmin)/m_Xbeishu,m_Rect.bottom-(yVector[i]-ymin)/m_Ybeishu);
		m_BitMapDC.LineTo(m_Rect.left+(xVector[i+1]-xmin)/m_Xbeishu,m_Rect.bottom-(yVector[i+1]-ymin)/m_Ybeishu);
	}*/
	//描绘整理后的数据
	int space=OGDataVector.size()*m_ProgressSpace;
	if (OGDataVector.size()!=0)
	{
		for (int i = 0; i < OGDataVector.size(); i++)
		{
			int x=m_Rect.left+i/m_Xbeishu;
			int y=m_Rect.bottom-(OGDataVector[i].VELOCITY-ymin)/m_Ybeishu;
			m_BitMapDC.Ellipse(x,y,x+2,y+2);
		}
	}
	//数据范围
	//DataRangeStr.Format(_T("经度:")+XLabelFormat+_T("  纬度:")+YLabelFormat+_T("  "),xmax-xmin,ymax-ymin);
	//绘制当前局部位置指示标
	CPen ZoomRangePen(PS_SOLID,1,m_ZoomColor);
	m_BitMapDC.SelectObject(&ZoomRangePen);
	x=m_Rect.left+currentNumber/m_Xbeishu;
	y=m_Rect.bottom;
	m_BitMapDC.MoveTo(x,y);
	y=m_Rect.top;
	m_BitMapDC.LineTo(x,y);

	x=m_Rect.left+(currentNumber+m_ZoomUpRange)/m_Xbeishu;
	y=m_Rect.bottom;
	m_BitMapDC.MoveTo(x,y);
	y=m_Rect.top;
	m_BitMapDC.LineTo(x,y);

	//恢复设备
	m_BitMapDC.SelectObject(oldpen);
	m_BitMapDC.SelectObject(oldFont);
	return 0;
}


void C数据提取View::OnSelectpointslider()
{
	// TODO: 在此添加命令处理程序代码
	CMainFrame* pMainFrm=(CMainFrame*)AfxGetApp()->GetMainWnd();
	CMFCRibbonSlider* PointNumber=DYNAMIC_DOWNCAST(CMFCRibbonSlider,pMainFrm->m_wndRibbonBar.FindByID(ID_SelectPointSlider));
	m_CurrentSelectPointNumber=PointNumber->GetPos();
	UpdateMainPlot();
}


void C数据提取View::UpdateMainPlot(void)
{
	if (m_Flag_ReadGravThread || m_Flag_ReadMultiGravThread)
	{
		MessageBox(_T("读取重力数据线程正在使用，等待完成"));
		return;
	}
	SetProgressRange(0,3,IDR_CHILDROGRESSBAR); UpdataProgress(1,IDR_CHILDROGRESSBAR);//进度条
	m_LineTitle=_T("航迹图示 ")+m_Distance;
	InitBitMap();//初始化位图
	CRect rc;GetClientRect(&rc);
	CString filename=_T("   ")+m_CurrentDataFileName.Mid(m_CurrentDataFileName.ReverseFind( '\\')+1,m_CurrentDataFileName.GetLength());
	PlotLine(rc,m_LineTitle,m_DataInfo+filename,m_OGDataVector,m_RemoveXVector,m_RemoveYVector);   UpdataProgress(2,IDR_CHILDROGRESSBAR);
	//UpdateSelectRectFigure(m_SelectedRect);
	PlotZoomUpLine();	UpdataProgress(3,IDR_CHILDROGRESSBAR);
	//速度图
	m_LineTitle=_T("速度时间图");
	//InitBitMap();//初始化位图
	CRect rc2;GetClientRect(&rc2);
	rc2.left=rc2.Width()/2;
	rc2.top=rc2.Height()/2;
	PlotVelocity(rc2,m_LineTitle,_T(""),m_OGDataVector,m_CurrentZoomUpRangeLeft);UpdataProgress(4,IDR_CHILDROGRESSBAR);
	//绘制鼠标选择的方形区域
	
	Invalidate();//将绘制的位图复制到视图窗口里
	UpdataProgress(0,IDR_CHILDROGRESSBAR);
}


void C数据提取View::OnStartpoint()
{
	if (m_Flag_ReadGravThread || m_Flag_ReadMultiGravThread)
	{
		MessageBox(_T("读取重力数据线程正在使用，等待完成"));
		return;
	}
	// TODO: 在此添加命令处理程序代码
	m_StartPointNumber=m_CurrentZoomUpRangeLeft+m_CurrentSelectPointNumber;
	m_FlagStartPoint=true;
	UpdateMainPlot();
}


void C数据提取View::OnBack()
{
	// TODO: 在此添加命令处理程序代码
	//UpdateParameter();
	m_CurrentZoomUpRangeLeft-=m_ZoomRangeMoveSpeed;	//倒退

	if (m_CurrentZoomUpRangeLeft<=0)
	{
		m_CurrentZoomUpRangeLeft=m_OGDataVector.size()-m_ZoomUpRange;
	}
	UpdateMainPlot();

}


void C数据提取View::OnFinishpoint()
{
	// TODO: 在此添加命令处理程序代码
	m_FinishPointNumber=m_CurrentZoomUpRangeLeft+m_CurrentSelectPointNumber;
	m_FlagFinishiPoint=true;
	UpdateMainPlot();
}

void C数据提取View::OnButton2()
{
	// TODO: 在此添加命令处理程序代码
	if (m_DataRangeDlg.DoModal()!=IDOK)
	{
		return;
	}
	double xmin=m_DataRangeDlg.m_xmin,xmax=m_DataRangeDlg.m_xmax,ymin=m_DataRangeDlg.m_ymin,ymax=m_DataRangeDlg.m_ymax;
	

	CString OutFileName=m_CurrentDataFileName.Left(m_CurrentDataFileName.GetLength()-4)+_T("-区域选择.DAT");

	TCHAR szFilter[] = _T("测线按区域选择 文件（*.DAT)|*.DAT|所有文件(*.*)|*.*||");
	CFileDialog FileDlg(FALSE, _T("DAT"),OutFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL);
	FileDlg.m_ofn.lpstrTitle=_T("输出按区域选择测线文件");
	if (IDOK == FileDlg.DoModal())
	{   
		OutFileName = FileDlg.GetPathName();  
	}
	else
	{
		return;
	}


	FILE *fpOut=NULL;
	if ((fpOut=fopen((_bstr_t)OutFileName,"w"))==NULL)
	{
		AfxMessageBox(_T("打开")+OutFileName+_T("失败"));
		return ;
	}
	//输出测线
	//int progspace=m_OGDataVector.size()/100;
	//SetProgressRange(0,100,IDR_MAINPROGRESSBAR);//进度条
	int Max=m_OGDataVector.size();
	int space=Max*m_ProgressSpace;
	for (int i = 0; i <m_OGDataVector.size(); i++)
	{
		if (m_OGDataVector[i].LONGITUDE<xmax && m_OGDataVector[i].LONGITUDE>xmin && m_OGDataVector[i].LATITUDE<ymax && m_OGDataVector[i].LATITUDE>ymin)
		{
			fprintf(fpOut,"$DAT\t%s   \t%s\t%8.2f\t%8.2f\t%11.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%13.8f\t%13.8f\t%8.3f\t%8.3f\n",
			(PCSTR)(_bstr_t)(PCTSTR)m_OGDataVector[i].datatimeStr,(PCSTR)(_bstr_t)(PCTSTR)m_OGDataVector[i].day,
			m_OGDataVector[i].gravity,m_OGDataVector[i].spinttension,
			m_OGDataVector[i].beamposition,m_OGDataVector[i].VCC,m_OGDataVector[i].AL,m_OGDataVector[i].AX,m_OGDataVector[i].VE,m_OGDataVector[i].AX2,m_OGDataVector[i].XACC2,
			m_OGDataVector[i].LACC2,m_OGDataVector[i].CROSSACCEL,m_OGDataVector[i].LONGACCEL,m_OGDataVector[i].EOTVOS,m_OGDataVector[i].LONGITUDE,m_OGDataVector[i].LATITUDE,
			m_OGDataVector[i].VELOCITY,m_OGDataVector[i].Heading);
		}

	/*	if (i%progspace==0)
		{
			UpdataProgress(i/progspace,IDR_MAINPROGRESSBAR);
		}*/
	}
	fclose(fpOut);
}
void C数据提取View::OnOutdataline()
{
	// TODO: 在此添加命令处理程序代码
	if (!m_FlagStartPoint)
	{
		MessageBox(_T("请选择测线起点"));
		return;
	}
	if (!m_FlagFinishiPoint)
	{
		MessageBox(_T("请选择测线终点"));
		return;
	}
	if (m_StartPointNumber>=m_FinishPointNumber)
	{
		MessageBox(_T("起点不能在终点之后"));
		return;
	}
	CString OutFileName=m_CurrentDataFileName.Left(m_CurrentDataFileName.GetLength()-4)+_T("-提取测线.DAT");

	TCHAR szFilter[] = _T("测线提取 文件（*.DAT)|*.DAT|所有文件(*.*)|*.*||");
	CFileDialog FileDlg(FALSE, _T("DAT"),OutFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL);
	FileDlg.m_ofn.lpstrTitle=_T("输出测线文件");
	if (IDOK == FileDlg.DoModal())
	{   
		OutFileName = FileDlg.GetPathName();  
	}
	else
	{
		return;
	}


	FILE *fpOut=NULL;
	if ((fpOut=fopen((_bstr_t)OutFileName,"w"))==NULL)
	{
		AfxMessageBox(_T("打开")+OutFileName+_T("失败"));
		return ;
	}
	//输出测线
	int Max=m_FinishPointNumber-m_StartPointNumber;
	int space=Max*m_ProgressSpace;
	int progspace=(m_FinishPointNumber-m_StartPointNumber)/100;
	for (int i = m_StartPointNumber; i <= m_FinishPointNumber; i++)
	{
		if (m_OGDataVector[0].tempstr==_T(""))//是标准的海洋重力格式
		{
			fprintf(fpOut, "$DAT\t%s   \t%s\t%8.2f\t%8.2f\t%11.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%13.8f\t%13.8f\t%8.3f\t%8.3f\n",
				(PCSTR)(_bstr_t)(PCTSTR)m_OGDataVector[i].datatimeStr, (PCSTR)(_bstr_t)(PCTSTR)m_OGDataVector[i].day,
				m_OGDataVector[i].gravity, m_OGDataVector[i].spinttension,
				m_OGDataVector[i].beamposition, m_OGDataVector[i].VCC, m_OGDataVector[i].AL, m_OGDataVector[i].AX, m_OGDataVector[i].VE, m_OGDataVector[i].AX2, m_OGDataVector[i].XACC2,
				m_OGDataVector[i].LACC2, m_OGDataVector[i].CROSSACCEL, m_OGDataVector[i].LONGACCEL, m_OGDataVector[i].EOTVOS, m_OGDataVector[i].LONGITUDE, m_OGDataVector[i].LATITUDE,
				m_OGDataVector[i].VELOCITY, m_OGDataVector[i].Heading);
		}
		else //一般的数据可视化提取
		{
			fprintf(fpOut, "%s\n",(PCSTR)(_bstr_t)(PCTSTR)m_OGDataVector[i].tempstr);
		}
		
	}
	fclose(fpOut);
	//UpdataProgress(0,IDR_MAINFRAME);
}


void C数据提取View::OnMainfigure()
{
	// TODO: 在此添加命令处理程序代码
	if (m_Flag_ReadGravThread || m_Flag_ReadMultiGravThread)
	{
		MessageBox(_T("读取重力数据线程正在使用，等待完成"));
		return;
	}
	UpdateMainPlot();
}


void C数据提取View::OnAdvance()
{
	// TODO: 在此添加命令处理程序代码
	//UpdateParameter();
	m_CurrentZoomUpRangeLeft+=m_ZoomRangeMoveSpeed;	//倒退
	if (m_CurrentZoomUpRangeLeft+m_ZoomUpRange>m_OGDataVector.size())
	{
		//MessageBox(_T("已经到航线的终点"));
		m_CurrentZoomUpRangeLeft=0;
		//return;
	}
	
	UpdateMainPlot();
}


int C数据提取View::UpdateParameter(void)
{
	
	CMainFrame* pMainFrm=(CMainFrame*)AfxGetApp()->GetMainWnd();
	COleVariant PropValue=pMainFrm->m_wndProperties.m_wndPropList.GetProperty(0)->GetSubItem(0)->GetValue();
	m_ZoomUpRange=atoi((CString)PropValue);

	if (m_ZoomUpRange>=m_OGDataVector.size())
	{
		m_ZoomUpRange=m_OGDataVector.size()/2;
		m_ZoomRangeMoveSpeed=m_ZoomUpRange/2;
		m_Frquency_ZoomFigure=1;
		m_Frequency_MainFigure=1;
	}else
	{
		

		PropValue=pMainFrm->m_wndProperties.m_wndPropList.GetProperty(0)->GetSubItem(1)->GetValue();
		m_ZoomRangeMoveSpeed=atoi((CString)PropValue);

		PropValue=pMainFrm->m_wndProperties.m_wndPropList.GetProperty(0)->GetSubItem(3)->GetValue();
		m_Frquency_ZoomFigure=atoi((CString)PropValue);

		PropValue=pMainFrm->m_wndProperties.m_wndPropList.GetProperty(2)->GetSubItem(0)->GetValue();
		m_Frequency_MainFigure=atoi((CString)PropValue);
	}
	SetZoomUpSliderRange(m_ZoomUpRange);

	PropValue=pMainFrm->m_wndProperties.m_wndPropList.GetProperty(0)->GetSubItem(2)->GetValue();
	m_TimeSpace=atoi((CString)PropValue);

	
	

	PropValue=pMainFrm->m_wndProperties.m_wndPropList.GetProperty(1)->GetSubItem(0)->GetValue();
	m_SmoothPointNum=atoi((CString)PropValue);

	PropValue=pMainFrm->m_wndProperties.m_wndPropList.GetProperty(1)->GetSubItem(1)->GetValue();
	m_VelocityThreshold=atof((CString)PropValue);
	//更改字体大小
	PropValue=pMainFrm->m_wndProperties.m_wndPropList.GetProperty(2)->GetSubItem(1)->GetValue();
	int fontsize=atoi((CString)PropValue);
	m_Font.DeleteObject();
	m_Font.CreatePointFont(fontsize,_T("宋体"));
	return 0;
}


int C数据提取View::ShowProgressBar(int CurrentPos, int MaxPos)
{
	if (m_ProgressDlg!=NULL)
	{
		m_ProgressDlg->SendMessage(WM_CLOSE);
	}
	//创建进度条
	m_ProgressDlg=new CProgressDlg;
	m_ProgressDlg->Create(IDD_ProgressDlg,this);
	m_ProgressDlg->ShowWindow(SW_SHOW);
	CProgressCtrl *pProgCtrl=(CProgressCtrl*)m_ProgressDlg->GetDlgItem(IDC_PROGRESS1);
	pProgCtrl->SetBarColor(RGB(255,0,0));
	pProgCtrl->SetRange(0,MaxPos);
	pProgCtrl->SetPos(CurrentPos);
	return 0;
}


void C数据提取View::SelectLanLonGravToFile(CString DataFileName,FILE *fpout)
{
	/*CString NewDataFileName;
	NewDataFileName=DataFileName.Left(DataFileName.GetLength()-4)+_T("-提取重力-经纬度-速度.DAT");*/
	//AfxMessageBox(NewDataFileName);
	//打开文件
	FILE *fp=NULL/*,*fpout=NULL*/;
	if ((fp=fopen((_bstr_t)DataFileName,"r"))==NULL)
	{
		AfxMessageBox(_T("打开")+DataFileName+_T("失败"));
		return ;
	}
	//if ((fpout=fopen((_bstr_t)NewDataFileName,"w"))==NULL)
	//{
	//	AfxMessageBox(_T("打开")+NewDataFileName+_T("失败"));
	//	return ;
	//}

	//读取文件
	char symbol[20];
	double /*Gravity,SpringTension,*/BeamPosition,VCC,AL,AX,VE,AX2,XACC2,LACC2,CROSS,LONGACCEL,Eotvos,Longtitude,Latitude,Heading,Velocity;
	CString tempstr,IDStr,DATE_year_Str,MonthStr,DayStr,HourStr,MinuteStr,SecondStr,YearStr,GravStr,SpringTensitonStr;
	char firstchar[50];
	while (!feof(fp))
	//for (int j = 0; j < 2; j++)
	{
		for (int i = 0; i < 50; i++)
		{
			firstchar[i]=fgetc(fp);
		}
		//fscanf(fp,"%s",tempchar);
		tempstr=(PCTSTR)(_bstr_t)firstchar;
		tempstr=tempstr.Left(49);
		if (tempstr.Left(1)!=_T("$"))
		{
			break;
		}
		SpringTensitonStr=tempstr.Right(8);tempstr=tempstr.Left(tempstr.GetLength()-8-1);
		GravStr=tempstr.Right(8);tempstr=tempstr.Left(tempstr.GetLength()-8-1);
		YearStr=tempstr.Right(3);tempstr=tempstr.Left(tempstr.GetLength()-3-1-3);
		SecondStr=tempstr.Right(2);tempstr=tempstr.Left(tempstr.GetLength()-2-1);
		MinuteStr=tempstr.Right(2);tempstr=tempstr.Left(tempstr.GetLength()-2-1);
		HourStr=tempstr.Right(2);tempstr=tempstr.Left(tempstr.GetLength()-2-1);
		DayStr=tempstr.Right(2);tempstr=tempstr.Left(tempstr.GetLength()-2-1);
		MonthStr=tempstr.Right(2);tempstr=tempstr.Left(tempstr.GetLength()-2-1);
		DATE_year_Str=tempstr.Right(4);tempstr=tempstr.Left(tempstr.GetLength()-2-1);

		fscanf(fp,"%lf%s%lf%s%lf%s%lf%s%lf%s%lf%s%lf%s%lf%s%lf%s%lf%s%lf%s%lf%s%lf%s%lf%s%lf%s",&BeamPosition,symbol,&VCC,symbol,&AL,symbol,&AX,symbol,
			&VE,symbol,&AX2,symbol,&XACC2,symbol,&LACC2,symbol,&CROSS,symbol,&LONGACCEL,symbol,&Eotvos,symbol,&Longtitude,symbol,&Latitude,symbol,&Velocity,symbol,&Heading,symbol);
		fgetc(fp);
		//下面的作用是去掉逗号
		/*AfxMessageBox(tempstr);
		AfxMessageBox(IDStr);
		AfxMessageBox(DATEStr);
		AfxMessageBox(TimeStr);
		AfxMessageBox(YearStr);
		AfxMessageBox(GravStr);
		AfxMessageBox(SpringTensitonStr);*/
		//写文件
		//fprintf(fpout,"%.8f\t%.8f\t%s\t%.3f\n",Longtitude,Latitude,(PCSTR)(_bstr_t)(PCTSTR)GravStr,Velocity);
		fprintf(fpout,"%.8f\t%.8f\n",Longtitude,Latitude);
	}
	
	fclose(fp);//fclose(fpout);
}


void C数据提取View::OnDisplaygravdata()
{
	// TODO: 在此添加命令处理程序代码
	
	InitBitMap();//初始化位图
	PlotGrav();
	Invalidate();
}


int C数据提取View::PlotGrav(void)
{
	if (m_OGDataVector.size()<2)
	{
		return 0;
	}
	CString Title2=_T("重力图示");
	CString Title=m_CurrentDataFileName;
	CRect rc;
	GetClientRect(&rc);
	//1.1 求最大值最小值
	xmin=1;xmax=m_OGDataVector.size();
	ymin=m_OGDataVector[0].gravity;ymax=ymin;
	for (int i = 0; i < m_OGDataVector.size(); i++)
	{
		if (ymin>m_OGDataVector[i].gravity)
		{
			ymin=m_OGDataVector[i].gravity;
		}
		if (ymax<m_OGDataVector[i].gravity)
		{
			ymax=m_OGDataVector[i].gravity;
		}
	}
	//CClientDC dc(this);
	CPen linePen(PS_SOLID,m_LineWidth,m_LineColor);
	CPen RemoveDataPen(PS_SOLID,m_LineWidth,m_RemoveDataLineColor);
	CPen GridPen(PS_SOLID,m_GridLineWidth,m_GridColor);
	CPen AxisPen(PS_SOLID,m_GridLineWidth,m_AxisColor);
	
	CFont* oldFont=(CFont*)m_BitMapDC.SelectObject(&m_Font);
	m_BitMapDC.SetBkMode(TRANSPARENT);
	m_BitMapDC.SetTextColor(m_TextColor);
	CSize m_StrSize;							//字符串大小（宽高）
	m_StrSize=m_BitMapDC.GetTextExtent(Title2);
	int textX=rc.Width()/2-m_StrSize.cx/2;		//曲线标题
	//m_BitMapDC.TextOut(textX,rc.top,Title);
	
	int textY=rc.top+m_StrSize.cy+m_TextSpaceY;
	textX=rc.left+(rc.Width()-m_BitMapDC.GetTextExtent(Title).cx)/2;
	m_BitMapDC.TextOut(textX,textY,Title);
	//绘制边框
	CString yminStr,xminStr;
	yminStr.Format(_T("%.8f"),(fabs(ymin)>fabs(ymax)?ymin:ymax));
	xminStr.Format(_T("%.5f"),(fabs(xmin)>fabs(xmax)?xmin:xmax));
	m_Rect.left=rc.left+(m_BitMapDC.GetTextExtent(yminStr).cx);
	m_Rect.right=rc.right-m_TextSpaceY-20;
	m_Rect.top=rc.top+m_BitMapDC.GetTextExtent(Title).cy+m_TextSpaceY+m_BitMapDC.GetTextExtent(Title2).cy+m_TextSpaceY;
	m_Rect.bottom=rc.bottom-m_BitMapDC.GetTextExtent(_T("x")).cy-20;
	CPen* oldpen=m_BitMapDC.SelectObject(&AxisPen);
	m_BitMapDC.MoveTo(m_Rect.left,m_Rect.top);m_BitMapDC.LineTo(m_Rect.left,m_Rect.bottom);
	m_BitMapDC.MoveTo(m_Rect.left,m_Rect.bottom);m_BitMapDC.LineTo(m_Rect.right,m_Rect.bottom);
	m_BitMapDC.MoveTo(m_Rect.right,m_Rect.bottom);m_BitMapDC.LineTo(m_Rect.right,m_Rect.top);
	m_BitMapDC.MoveTo(m_Rect.right,m_Rect.top);m_BitMapDC.LineTo(m_Rect.left,m_Rect.top);
	//绘制网格
	m_BitMapDC.SelectObject(&GridPen);
	m_IsShowGrid=true;
	float m_Ybeishu=(ymax-ymin)/(float)m_Rect.Height();
	//float m_Xbeishu=(float)(xVector.size()+RemoveXVector.size())/(float)m_Rect.Width();
	float m_Xbeishu=(xmax-xmin)/(float)m_Rect.Width();
	int XMarkLength=7;
	int YMarkLength=7;
	if (m_IsShowGrid)
	{
		XMarkLength=m_Rect.Height();
		YMarkLength=m_Rect.Width();
	}
	//x轴标记10个刻度，y标注5个
	m_XGridSize=m_BitMapDC.GetTextExtent(xminStr).cx;
	m_YGridSize=m_Rect.Height()/10;
	int xgridnum=m_Rect.Width()/m_XGridSize;
	int ygridnum=m_Rect.Height()/m_YGridSize;
	int x,y;
	int textx,texty;
	CString MarkStr;
	//m_BitMapDC.SetTextColor(RGB(0,0,0));				//设置坐标轴标注文字颜色
	//计算默认标注小数位数
	CString XLabelFormat,YLabelFormat;
	for (int i = 0; i < 10; i++)
	{
		if ((m_Xbeishu*m_XGridSize*pow(10,i))>1)
		{
			XLabelFormat.Format(_T("%d"),i);
			break;
		}
	}
	for (int i = 0; i < 10; i++)
	{
		if ((m_Ybeishu*m_YGridSize*pow(10,i))>1)
		{
			YLabelFormat.Format(_T("%d"),i);
			break;
		}
	}
	XLabelFormat=_T("%.")+XLabelFormat+_T("f");
	YLabelFormat=_T("%.")+YLabelFormat+_T("f");
	for (int i = 1; i < xgridnum; i++)			//X轴
	{
		x=m_Rect.left+i*m_XGridSize;y=m_Rect.bottom;
		m_BitMapDC.MoveTo(x,y);
		y-=XMarkLength;
		m_BitMapDC.LineTo(x,y);

		MarkStr.Format(XLabelFormat,xmin+i*m_Xbeishu*m_XGridSize);		//标注文字
		textx=x-m_BitMapDC.GetTextExtent(MarkStr).cx/2;
		texty=m_Rect.bottom;
		m_BitMapDC.TextOut(textx,texty,MarkStr);
	}
	for (int i = 1; i < ygridnum; i++)		//Y轴
	{
		x=m_Rect.left;y=m_Rect.bottom-i*m_YGridSize;
		m_BitMapDC.MoveTo(x,y);
		x+=YMarkLength;
		m_BitMapDC.LineTo(x,y);

		MarkStr.Format(YLabelFormat,ymin+i*m_Ybeishu*m_YGridSize);		//标注文字
		textx=m_Rect.left-m_BitMapDC.GetTextExtent(MarkStr).cx;
		texty=y-m_BitMapDC.GetTextExtent(MarkStr).cy/2;
		m_BitMapDC.TextOut(textx,texty,MarkStr);
	}

	m_BitMapDC.SelectObject(&linePen);
	/*for (int i = 0; i < yVector.size()-1; i++)
	{
		m_BitMapDC.MoveTo(m_Rect.left+(xVector[i]-xmin)/m_Xbeishu,m_Rect.bottom-(m_OGDataVector[i].gravity-ymin)/m_Ybeishu);
		m_BitMapDC.LineTo(m_Rect.left+(xVector[i+1]-xmin)/m_Xbeishu,m_Rect.bottom-(yVector[i+1]-ymin)/m_Ybeishu);
	}*/
	//描绘整理后的数据
	if (m_OGDataVector.size()!=0)
	{
		for (int i = 0; i < m_OGDataVector.size()-1; i++)
		{
			int x=m_Rect.left+i/m_Xbeishu;
			int y=m_Rect.bottom-(m_OGDataVector[i].gravity-ymin)/m_Ybeishu;
			m_BitMapDC.Ellipse(x,y,x+2,y+2);
		}
	}
	//数据范围
	//DataRangeStr.Format(_T("经度:")+XLabelFormat+_T("  纬度:")+YLabelFormat+_T("  "),xmax-xmin,ymax-ymin);
	//恢复设备
	m_BitMapDC.SelectObject(oldpen);
	m_BitMapDC.SelectObject(oldFont);
	return 0;
}


int C数据提取View::GetDistance(void)
{
	//由船速计算航程,以一分钟为一小段（微元）
	const int timespace=60;		//一段微元是多少秒
	double distance=0;
	int datanum=m_OGDataVector.size();
	int SegmentNum=datanum/timespace;			//微元数
	double AverageVelocity=0;
	for (int i = 0; i < SegmentNum; i++)
	{
		AverageVelocity=0;
		for (int j = 0; j < timespace; j++)
		{
			AverageVelocity+=m_OGDataVector[i*timespace+j].VELOCITY;
		}
		AverageVelocity=AverageVelocity/timespace;
		distance+=AverageVelocity/3600.0*timespace;
	}
	//计算剩余的不足一个微元的一小段
	AverageVelocity=0;
	for (int i=SegmentNum*timespace ; i < datanum; i++)
	{
		AverageVelocity+=m_OGDataVector[i].VELOCITY;
	}
	AverageVelocity=AverageVelocity/timespace;
	distance+=AverageVelocity/60.0;

	m_Distance.Format(_T("航程:%.2f km"),distance*1.852);
	return 0;
}


void C数据提取View::OnSelectstartdata()
{
	// TODO: 在此添加命令处理程序代码
	if (m_OGDataVector.size()>0)
	{
		m_DateTimeDlg.m_RangeStr=_T("范围:")+m_OGDataVector[0].datatimeStr+_T(" 至 ")+m_OGDataVector[m_OGDataVector.size()-1].datatimeStr;
	}else
	{
		m_DateTimeDlg.m_RangeStr=_T("");
	}
	
	if (IDOK==m_DateTimeDlg.DoModal())
	{
		int hour,minute,second,year,month,day;
		hour=m_DateTimeDlg.m_Time.GetHour();
		minute=m_DateTimeDlg.m_Time.GetMinute();
		second=m_DateTimeDlg.m_Time.GetSecond();
		year=m_DateTimeDlg.m_Date.GetYear();
		month=m_DateTimeDlg.m_Date.GetMonth();
		day=m_DateTimeDlg.m_Date.GetDay();
		CString time,date;
		time.Format(_T("%2d:%2d:%2d"),hour,minute,second);
		date.Format(_T("%d/%2d/%2d"),year,month,day);
		int i=0;
		for ( i = 0; i < m_OGDataVector.size(); i++)
		{
			if (m_OGDataVector[i].time==time)
			{
				if (m_OGDataVector[i].date==date)
				{
					//MessageBox(_T("匹配正确"));
					m_StartPointNumber=i;
					m_FlagStartPoint=true;
					UpdateMainPlot();
					break;
				}else
				{
					//MessageBox(_T("日期不匹配"));
				}
				
			}
		}
		if (i>=m_OGDataVector.size())
		{
			MessageBox(_T("未找到匹配测点"));
		}
	}
	
}


void C数据提取View::OnSelectenddate()
{
	// TODO: 在此添加命令处理程序代码
	if (m_OGDataVector.size()>0)
	{
		m_DateTimeDlg.m_RangeStr=_T("范围:")+m_OGDataVector[0].datatimeStr+_T(" 至 ")+m_OGDataVector[m_OGDataVector.size()-1].datatimeStr;
	}else
	{
		m_DateTimeDlg.m_RangeStr=_T("");
	}
	
	if (IDOK==m_DateTimeDlg.DoModal())
	{
		int hour,minute,second,year,month,day;
		hour=m_DateTimeDlg.m_Time.GetHour();
		minute=m_DateTimeDlg.m_Time.GetMinute();
		second=m_DateTimeDlg.m_Time.GetSecond();
		year=m_DateTimeDlg.m_Date.GetYear();
		month=m_DateTimeDlg.m_Date.GetMonth();
		day=m_DateTimeDlg.m_Date.GetDay();
		CString time,date;
		time.Format(_T("%2d:%2d:%2d"),hour,minute,second);
		date.Format(_T("%d/%2d/%2d"),year,month,day);
		int i=0;
		for ( i = m_StartPointNumber+1; i < m_OGDataVector.size(); i++)
		{
			if (m_OGDataVector[i].time==time)
			{
				if (m_OGDataVector[i].date==date)
				{
					//MessageBox(_T("匹配正确"));
					m_FinishPointNumber=i;
					m_FlagFinishiPoint=true;
					UpdateMainPlot();
					break;
				}else
				{
					//MessageBox(_T("日期不匹配"));
				}
				
			}
		}
		if (i>=m_OGDataVector.size())
		{
			MessageBox(_T("未找到匹配测点"));
		}
	}
}


void C数据提取View::OnSmoothgravity()
{
	// TODO: 在此添加命令处理程序代码
	//对速度进行1117点圆滑处理
	int PointNum=m_SmoothPointNum;
	int FirstNum=PointNum/2;
	int UpRange=m_OGDataVector.size()-1-FirstNum;
	double sum=0;
	
	int space=UpRange*m_ProgressSpace;
	for (int i = FirstNum; i < UpRange; i++)
	{
		sum= m_OGDataVector[i].gravity;
		for (int j= 1; j <= FirstNum; j++)
		{
			sum=sum+ m_OGDataVector[i+j].gravity+ m_OGDataVector[i-j].gravity;
		}
		m_OGDataVector[i].gravity=sum/PointNum;
	
	}
	
	InitBitMap();//初始化位图
	PlotGrav();
	Invalidate();
}





void C数据提取View::OnBgcorrect()
{
	// TODO: 在此添加命令处理程序代码

	//1.1 水深数据（grd）
//	m_StartTime=clock();
//	TCHAR szFilter[] = _T("水深数据 文件（*.grd)|*.grd|所有文件(*.*)|*.*||");
//	CFileDialog FileDlg(TRUE, _T("DAT"),NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL);
//	FileDlg.m_ofn.lpstrTitle=_T("打开水深数据-用于布格校正");
//	//设置对话框当前路径
//	//FileDlg.GetOFN().lpstrInitialDir =currentpath;
//	CString DataFileName;
//	if (IDOK == FileDlg.DoModal())
//	{   
//		DataFileName = FileDlg.GetPathName();  
//	}
//	else
//	{
//		return;
//	}
//	//读取水深数据
//	GRDSTRUCT DemDataInfo;
//	FILE *fp=NULL/*,*fpout=NULL,*fpRemove=NULL*/;
//	if ((fp=fopen((_bstr_t)DataFileName,"r"))==NULL)
//	{
//		AfxMessageBox(_T("打开")+DataFileName+_T("失败"));
//		return ;
//	}
//	char dsaa[20];
//	fscanf(fp,"%s",dsaa);
//	fscanf(fp,"%d%d%lf%lf%lf%lf%lf%lf",&DemDataInfo.cols,&DemDataInfo.rows,&DemDataInfo.xmin,
//		&DemDataInfo.xmax,&DemDataInfo.ymin,&DemDataInfo.ymax,&DemDataInfo.zmin,&DemDataInfo.zmax);
//	double* demdata=new double[DemDataInfo.rows*DemDataInfo.cols];
//	
//	for (int i = 0; i < DemDataInfo.rows; i++)
//	{
//		for (int j = 0; j < DemDataInfo.cols; j++)
//		{
//			fscanf(fp,"%lf",&demdata[i*DemDataInfo.cols+j]);
//		}
//	}
//	fclose(fp);
//
//	//1.2 读取待改正的测点坐标文件
//
//	TCHAR szFilter2[] = _T("大地坐标（*.dat)|*.dat|大地坐标（*.txt)|*.txt|所有文件(*.*)|*.*||");
//	CFileDialog FileDlg2(TRUE, _T("DAT"),NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter2, NULL);
//	FileDlg2.m_ofn.lpstrTitle=_T("打开测点坐标文件（大地坐标）-用于布格校正");
//	if (IDOK == FileDlg2.DoModal())
//	{   
//		DataFileName = FileDlg2.GetPathName();  
//	}
//	else
//	{
//		return;
//	}
//	FILE *fp2=NULL/*,*fpout=NULL,*fpRemove=NULL*/;
//	if ((fp2=fopen((_bstr_t)DataFileName,"r"))==NULL)
//	{
//		AfxMessageBox(_T("打开")+DataFileName+_T("失败"));
//		return ;
//	}
//	vector<double> x_vector,y_vector;
//	double tempx,tempy;
//	while (!feof(fp2))
//	{
//		fscanf(fp2,"%lf%lf",&tempx,&tempy);
//		x_vector.push_back(tempx);
//		y_vector.push_back(tempy);
//	}
//	if (x_vector[x_vector.size()-1]==x_vector[x_vector.size()-2]  && y_vector[y_vector.size()-1]==y_vector[y_vector.size()-2])
//	{
//		x_vector.pop_back();
//		y_vector.pop_back();
//	}fclose(fp2);
//
//
//	//1.3 根据测点坐标计算校正范围
//	double dx=(DemDataInfo.xmax-DemDataInfo.xmin)/(DemDataInfo.cols-1);
//	double dy=(DemDataInfo.ymax-DemDataInfo.ymin)/(DemDataInfo.rows-1);
//	DemDataInfo.dx=dx,DemDataInfo.dy=dy;
//	double Distance_min=sqrt(dx*dx+dy*dy)/2.0;
//	double Distance,x0,y0;
//	int row0,col0;																		//距离测点最近的网格节点索引
//	double range=166000;																//改正范围是166km
//	int Xgrdnum_half=int(range/dx/2)+1,Ygrdnum_half=int(range/dy/2)+1;					//改正范围在x，y方向上的网格数的一半
//	double rowmin,rowmax,colmin,colmax;
//	double *CorrectData=new double[x_vector.size()];									//地形改正值数组
//	GRDRANGEINDEX *IndexArray=new GRDRANGEINDEX[x_vector.size()];										//测点周围区域角点索引值数组
//	m_ProgressBar->SetRange(0,100);
//	int temp=x_vector.size()/100.0+1;
//	
//	for (int i = 0; i < x_vector.size(); i++)
//	{
//		x0=x_vector[i],y0=y_vector[i];													//测点坐标
//		if (SearchIndex_ij(DemDataInfo,Distance_min,x0,y0,row0,col0))
//		{
//			//计算测点周围改正范围网格四个角点的索引
//			rowmin=((row0-Ygrdnum_half)>0?row0-Ygrdnum_half:0);
//			colmin=((col0-Xgrdnum_half)>0?col0-Xgrdnum_half:0);
//			rowmax=((row0+Ygrdnum_half)<DemDataInfo.rows?row0+Ygrdnum_half:DemDataInfo.rows);
//			colmax=((col0+Xgrdnum_half)<DemDataInfo.cols?col0+Xgrdnum_half:DemDataInfo.cols);
//			//调用这个范围内的地形正演计算函数
//			IndexArray[i].colmin=colmin;IndexArray[i].colmax=colmax;IndexArray[i].rowmax=rowmax;IndexArray[i].rowmin=rowmin;
//			IndexArray[i].x0=x0;IndexArray[i].y0=y0;
//			//CorrectData[i]=Correct_CubicForward(DemDataInfo,rowmin,rowmax,colmin,colmax,demdata,x0,y0);		//第i个测点的地形改正值
//		}else
//		{
//			MessageBox(_T("测点不在水深数据范围内，计算程序将退出"));
//			return;
//		}
//		if (i%temp==0)
//		{
//			m_ProgressBar->SetPos(i/temp);
//		}
//	}
//	//GPU_Correct(CorrectData,x_vector.size(),demdata,IndexArray,DemDataInfo);								//调用GPU计算程序得到地改数组CorrectData
//	//Correct_CubicForward(DemDataInfo,x_vector.size(),IndexArray,demdata,CorrectData);
//	m_FinishTime=clock();
////	CString str;str.Format(_T("耗时：%d 毫秒    %d    %d"),m_FinishTime-m_StartTime,row0,col0);MessageBox(str);
//	//str.Format(_T("%lf    %lf"),CorrectData[0],CorrectData[100]);MessageBox(str);
//	
//	//1.4 输出地形改正值
//	CString OutFileName=DataFileName+_T("-地形改正.DAT");
//
//	TCHAR szFilter3[] = _T("地形改正 文件（*.DAT)|*.DAT|所有文件(*.*)|*.*||");
//	CFileDialog OutFileDlg(FALSE, _T("DAT"),OutFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter3, NULL);
//	OutFileDlg.m_ofn.lpstrTitle=_T("输出地形改正数据");
//	if (IDOK == OutFileDlg.DoModal())
//	{   
//		OutFileName = OutFileDlg.GetPathName();  
//	}
//	else
//	{
//		return;
//	}
//	FILE *fpOut=NULL;
//	if ((fpOut=fopen((_bstr_t)OutFileName,"w"))==NULL)
//	{
//		AfxMessageBox(_T("打开")+OutFileName+_T("失败"));
//		return ;
//	}
//	for (int i = 0; i < x_vector.size(); i++)
//	{
//		fprintf(fpOut,"%lf\t%lf\t%lf\n",x_vector[i],y_vector[i],CorrectData[i]);
//	}
//	fclose(fpOut);
//	//释放资源
//	delete CorrectData;

	//CString str;str.Format(_T("%lf    %lf"),dx,dy);MessageBox(str);
}


bool C数据提取View::SearchIndex_ij(GRDSTRUCT datainfo, double distance0,double x0, double y0, int& row, int& col)
{
	//全区搜索
	//double distance;
	//double xij,yij;
	//double dx=datainfo.dx;
	//double dy=datainfo.dy;

	//for (int i = 0; i < datainfo.rows; i++)
	//{
	//	for (int j = 0; j < datainfo.cols;j++)
	//	{
	//		xij=datainfo.xmin+j*dx;
	//		yij=datainfo.ymin+i*dy;
	//		distance=sqrt(pow(xij-x0,2.0)+pow(yij-y0,2.0));
	//		if (distance<=distance0)
	//		{
	//			row=i;col=j;
	//			return true;
	//		}
	//	}
	//}
	//return false;

	//直接判断
	double xgridnum,ygridnum;
	xgridnum=(x0-datainfo.xmin)/datainfo.dx;
	ygridnum=(y0-datainfo.ymin)/datainfo.dy;
	if (xgridnum > datainfo.cols || xgridnum <0 || ygridnum > datainfo.rows || ygridnum <0)
	{
		return false;
	}else
	{
		col=((xgridnum-(int)xgridnum)>0.5?(int)xgridnum+1:(int)xgridnum);
		row=((ygridnum-(int)ygridnum)>0.5?(int)ygridnum+1:(int)ygridnum);
	}
	return true;
}

void  C数据提取View::Correct_CubicForward(GRDSTRUCT datainfo,int correctnumber, GRDRANGEINDEX* indexarray, double* demdata,double* CorrectData)
{
	//输出测点周围地改范围内的坐标
	//FILE *fp=NULL/*,*fpout=NULL,*fpRemove=NULL*/;
	//if ((fp=fopen("地改范围.txt","w"))==NULL)
	//{
	//	AfxMessageBox(_T("打开地改范围文件失败"));
	//	return 0;
	//}
	//for (int i = rowmin; i < rowmax; i++)
	//{
	//	for (int j = colmin; j < colmax; j++)
	//	{
	//		fprintf(fp,"%lf\t%lf\n",datainfo.xmin+j*datainfo.dx,datainfo.ymin+i*datainfo.dy);
	//	}
	//}
	//fclose(fp);
	m_ProgressBar->SetRange(0,100);
	int temp=correctnumber/100.0+1;
	
	

	for (int k = 0; k < correctnumber; k++)
	{
		CUBIC_FORWARD_INFO cubic_grav_info;
		cubic_grav_info.z=-0.001;									//观测面在地面
		//给模型体参数赋值
		for (int i = indexarray[k].rowmin; i < indexarray[k].rowmax; i++)
		{
			for (int j = indexarray[k].colmin; j < indexarray[k].colmax; j++)
			{
				cubic_grav_info.m_density_vector.push_back(1.64);
				cubic_grav_info.x1_vector.push_back(datainfo.xmin+j*datainfo.dx);
				cubic_grav_info.y1_vector.push_back(datainfo.ymin+i*datainfo.dy);
				cubic_grav_info.x2_vector.push_back(datainfo.xmin+(j+1)*datainfo.dx);
				cubic_grav_info.y2_vector.push_back(datainfo.ymin+(i+1)*datainfo.dy);
				cubic_grav_info.z1_vector.push_back(0);
				cubic_grav_info.z2_vector.push_back(demdata[j+i*datainfo.cols]);
			}
		}
		//1.1 调用正演函数
		CorrectData[k]=Cubic_Vz_Forward(cubic_grav_info,indexarray[k].x0,indexarray[k].y0);
		if (k%temp==0)
		{
			m_ProgressBar->SetPos(k/temp);
		}
	}
	
}


void C数据提取View::OnFileSaveAs()
{
	// TODO: 在此添加命令处理程序代码
	
}


void C数据提取View::OnFileSave()
{

	// TODO: 在此添加命令处理程序代码
	CString OutFileName=m_CurrentDataFileName.Left(m_CurrentDataFileName.GetLength()-4)+_T("重力数据格式化.DAT");

	TCHAR szFilter[] = _T("测线提取 文件（*.DAT)|*.DAT|所有文件(*.*)|*.*||");
	CFileDialog FileDlg(FALSE, _T("DAT"),OutFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL);
	FileDlg.m_ofn.lpstrTitle=_T("输出测线文件");
	if (IDOK == FileDlg.DoModal())
	{   
		OutFileName = FileDlg.GetPathName();  
	}
	else
	{
		return;
	}


	FILE *fpOut=NULL;
	if ((fpOut=fopen((_bstr_t)OutFileName,"w"))==NULL)
	{
		AfxMessageBox(_T("打开")+OutFileName+_T("失败"));
		return ;
	}
	//输出测线
	int progspace=m_OGDataVector.size()/100;
	SetProgressRange(0,100,IDR_CHILDROGRESSBAR);//进度条
	for (int i = 0; i <m_OGDataVector.size(); i++)
	{
		fprintf(fpOut,"$DAT\t%s   \t%s\t%8.2f\t%8.2f\t%11.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%13.8f\t%13.8f\t%8.3f\t%8.3f\n",
			(PCSTR)(_bstr_t)(PCTSTR)m_OGDataVector[i].datatimeStr,(PCSTR)(_bstr_t)(PCTSTR)m_OGDataVector[i].day,
			m_OGDataVector[i].gravity,m_OGDataVector[i].spinttension,
			m_OGDataVector[i].beamposition,m_OGDataVector[i].VCC,m_OGDataVector[i].AL,m_OGDataVector[i].AX,m_OGDataVector[i].VE,m_OGDataVector[i].AX2,m_OGDataVector[i].XACC2,
			m_OGDataVector[i].LACC2,m_OGDataVector[i].CROSSACCEL,m_OGDataVector[i].LONGACCEL,m_OGDataVector[i].EOTVOS,m_OGDataVector[i].LONGITUDE,m_OGDataVector[i].LATITUDE,
			m_OGDataVector[i].VELOCITY,m_OGDataVector[i].Heading);
		if (i%progspace==0)
		{
			UpdataProgress(i/progspace,IDR_CHILDROGRESSBAR);
		}
	}
	fclose(fpOut);
	UpdataProgress(0,IDR_CHILDROGRESSBAR);
}


void C数据提取View::OnGetgpuproperty()
{
	// TODO: 在此添加命令处理程序代码
//	GetGPUProperty();
}


void C数据提取View::OnTimecorrect()
{
	// TODO: 在此添加命令处理程序代码
	if (m_TimeCorrectDlg.DoModal()!=IDOK)
	{
		return;
	}
	int timedelay=m_TimeCorrectDlg.m_TimeDelay;
	int filecount=GetFileCount(m_TimeCorrectDlg.m_DataFilePath);


	//输出合并日志
	CString logfilename;
	TCHAR szFilter[] = _T("时间校正 日志（*.log)|*.log|所有文件(*.*)|*.*||");
	CFileDialog FileDlg(FALSE, _T("DAT"),"时间校正", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL);
	FileDlg.m_ofn.lpstrTitle=_T("时间校正日志文件");
	if (IDOK == FileDlg.DoModal())
	{   
		logfilename = FileDlg.GetPathName();  
		FILE *fpOut=NULL;
		if ((fpOut=fopen((_bstr_t)logfilename,"w"))==NULL)
		{
			AfxMessageBox(_T("打开")+logfilename+_T("失败"));
			return ;
		}
		for (int i = 0; i < filecount; i++)
		{
			fprintf(fpOut,"%s\n",(PCSTR)(_bstr_t)(PCTSTR)m_DataFileNameVector[i]/*.Right(m_DataFileNameVector[i].GetLength()-m_DataPathName.GetLength())*/);
		}
		fclose(fpOut);
	}
	
	CString str;str.Format(_T("共整合了%d个文件"),filecount);MessageBox(str);
}


void C数据提取View::OnOutputfga()
{
	// TODO: 在此添加命令处理程序代码
	if (m_OGDataVector.size()==0)
	{
		MessageBox(_T("此视图窗口没有读入任何海洋重力数据\n先读入原始数据"));
		return;
	}
	//读取基点数据
	CString BasePointFileName1,BasePointFileName2;
	TCHAR szFilter[] = _T("基点比对 文件（*.inf)|*.inf|所有文件(*.*)|*.*||");
	CFileDialog FileDlg1(TRUE, _T("inf"),NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL);
	FileDlg1.m_ofn.lpstrTitle=_T("打开测量前基点比对数据文件");

	if (IDOK == FileDlg1.DoModal())
	{   
		BasePointFileName1 = FileDlg1.GetPathName();  
	}
	else
	{
		return;
	}
	CFileDialog FileDlg2(TRUE, _T("inf"),NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL);
	FileDlg2.m_ofn.lpstrTitle=_T("打开测量后基点比对数据文件");

	if (IDOK == FileDlg2.DoModal())
	{   
		BasePointFileName2 = FileDlg2.GetPathName();  
	}
	else
	{
		return;
	}
	CString OutFileName=m_CurrentDataFileName.Left(m_CurrentDataFileName.GetLength()-4)+_T("-自由空间重力异常.dat");
	TCHAR szFilter2[] = _T("自由空间重力异常 文件（*.dat)|*.dat|所有文件(*.*)|*.*||");
	CFileDialog OutputFileDlg(FALSE, _T("DAT"),OutFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter2, NULL);
	OutputFileDlg.m_ofn.lpstrTitle=_T("输出自由空间重力异常");
	if (IDOK == OutputFileDlg.DoModal())
	{   
		OutFileName = OutputFileDlg.GetPathName();  
	}
	else
	{
		return;
	}

	FILE* fp1,*fp2;
	BASEPOINTDATASTRUCT basedata1,basedata2;
	if ((fp1=fopen((_bstr_t)BasePointFileName1,"r"))==NULL)
	{
		AfxMessageBox(_T("打开")+BasePointFileName1+_T("失败"));
		return ;
	}
	if ((fp2=fopen((_bstr_t)BasePointFileName2,"r"))==NULL)
	{
		AfxMessageBox(_T("打开")+BasePointFileName2+_T("失败"));
		fclose(fp1);
		return ;
	}
	FILE *fpOut=NULL;
	if ((fpOut=fopen((_bstr_t)OutFileName,"w"))==NULL)//打开输出自由空间重力异常文件
	{
		AfxMessageBox(_T("打开")+OutFileName+_T("失败"));
		fclose(fp1);
		fclose(fp2);
		return ;
	}
	char tempchar[50];
	fscanf(fp1,"%s%lf%s%s%s%lf%lf%s%lf%lf%s%lf%s%lf%s%lf%s%lf%s%lf%s%lf",tempchar,&basedata1.grav,tempchar,basedata1.datetimechar,tempchar,
		&basedata1.longitude_degree,&basedata1.longitude_minute,tempchar,&basedata1.latitude_degree,&basedata1.latitude_minute,
		tempchar,&basedata1.reading,tempchar,&basedata1.Heitht_BaseToSea,tempchar,&basedata1.height_l,tempchar,&basedata1.height_r,tempchar,
		&basedata1.HeightToDeck,tempchar,&basedata1.DistanceToBasePoint);
	basedata1.longitude=basedata1.longitude_degree+basedata1.longitude_minute/60.0;			//经纬度由度分转化为度
	basedata1.latitude=basedata1.latitude_degree+basedata1.latitude_minute/60.0;
	basedata1.datetime.ParseDateTime((PCTSTR)(_bstr_t)basedata1.datetimechar);			//比对日期
	fscanf(fp2,"%s%lf%s%s%s%lf%lf%s%lf%lf%s%lf%s%lf%s%lf%s%lf%s%lf%s%lf",tempchar,&basedata2.grav,tempchar,basedata2.datetimechar,tempchar,
		&basedata2.longitude_degree,&basedata2.longitude_minute,tempchar,&basedata2.latitude_degree,&basedata2.latitude_minute,
		tempchar,&basedata2.reading,tempchar,&basedata2.Heitht_BaseToSea,tempchar,&basedata2.height_l,tempchar,&basedata2.height_r,tempchar,
		&basedata2.HeightToDeck,tempchar,&basedata2.DistanceToBasePoint);
	basedata2.longitude=basedata2.longitude_degree+basedata2.longitude_minute/60.0;			//经纬度由度分转化为度
	basedata2.latitude=basedata2.latitude_degree+basedata2.latitude_minute/60.0;
	basedata2.datetime.ParseDateTime((PCTSTR)(_bstr_t)basedata2.datetimechar);		//比对日期
	//CString str;str.Format(_T("%lf"),basedata2.DistanceToBasePoint);MessageBox(str);
	fclose(fp1);fclose(fp2);

	//2. 重力基点比对
	double dgB=4.741636224*(0.01060488*sin(basedata1.latitude)*cos(basedata1.latitude)-0.0000234*sin(2.0*basedata1.latitude)*cos(2.0*basedata1.latitude))*basedata1.DistanceToBasePoint;
	basedata1.HeightToBasePoint=basedata1.Heitht_BaseToSea-(basedata1.height_l+basedata1.height_r)/2.0+basedata1.HeightToDeck;
	basedata1.reading_corrected=basedata1.reading-0.3086*basedata1.HeightToBasePoint;

	dgB=4.741636224*(0.01060488*sin(basedata2.latitude)*cos(basedata2.latitude)-0.0000234*sin(2.0*basedata2.latitude)*cos(2.0*basedata2.latitude))*basedata2.DistanceToBasePoint;
	basedata2.HeightToBasePoint=basedata2.Heitht_BaseToSea-(basedata2.height_l+basedata2.height_r)/2.0+basedata2.HeightToDeck;
	basedata2.reading_corrected=basedata2.reading-0.3086*basedata2.HeightToBasePoint;

	double K_gravmetor=0.998;//仪器格值
	double H=0.31;			//重力仪弹性系统到平均海平面的高度（m），全航程用一个值合理吗？应不应该从GPS数据里面提取天线到海平面高度然后换算出这个值？理论上这个值是随测点不同而变的
	double K_zeropoint=K_gravmetor*(basedata2.reading_corrected-basedata1.reading_corrected)/(basedata2.datetime.GetDay()-basedata1.datetime.GetDay());//每天的漂移量
	double K_draft=0.3086*(basedata2.HeightToBasePoint-basedata1.HeightToBasePoint)/(basedata2.datetime.GetDay()-basedata1.datetime.GetDay());//吃水改正系数
	double *dGravity=new double[m_OGDataVector.size()];		//用于累加各种改正值
	double dgravityTemp=0;
	double d_day;//测点距离起航的时间差（天）
	//3. 自由空间重力异常计算
	for (int i = 0; i < m_OGDataVector.size(); i++)
	{
		dgravityTemp=0;
		if (m_OGDataVector[i].DataTime < basedata1.datetime || m_OGDataVector[i].DataTime > basedata2.datetime)
		{
			CString errorinfo;errorinfo.Format(_T("测点日期不在前后基点比对范围内\n请检查第%d行重力数据"),i+1);
			MessageBox(errorinfo);
			return;
		}
		d_day=m_OGDataVector[i].DataTime.GetDay()-basedata1.datetime.GetDay();
		//0. 测点与基点的读数差
		//dgravityTemp+=K_gravmetor*(m_OGDataVector[i].gravity-basedata1.reading_corrected);//读书差乘以格值就是与基点绝对重力值之间的差值

		//1. 零漂改正
		//dgravityTemp+=K_zeropoint*(d_day);	//零漂改正量

		//2. 吃水改正
		//dgravityTemp+=K_draft*(d_day);		//吃水改正值

		//3. 厄特弗斯改正
		dgravityTemp+=7.499*m_OGDataVector[i].VELOCITY*sin(m_OGDataVector[i].Heading)*cos(m_OGDataVector[i].LATITUDE)+0.004*pow(m_OGDataVector[i].VELOCITY,2.0);

		//4. 正常场改正
		//dgravityTemp+=-978032.67714*(1+0.00193185138639*pow(sin(m_OGDataVector[i].Heading),2.0))/sqrt(1-0.00669437999013*pow(sin(m_OGDataVector[i].Heading),2.0));

		//5. 自由空间改正
		//dgravityTemp+=0.3086*H;

		dGravity[i]=dgravityTemp;
	}
	//测点的绝对重力值加上各项改正值即为重力异常
	//4. 自由空间重力异常输出
	for (int i = 0; i < m_OGDataVector.size(); i++)
	{
		fprintf(fpOut,"%s\t%13.8f\t%13.8f\t%8.2f\t%8.2f\n",(PCSTR)(_bstr_t)(PCTSTR)m_OGDataVector[i].datatimeStr,m_OGDataVector[i].LONGITUDE,m_OGDataVector[i].LATITUDE,
			m_OGDataVector[i].gravity,/*basedata1.grav+*/dGravity[i]);
	}

	//释放动态数组资源
	delete dGravity;
	fclose(fpOut);
}


void C数据提取View::OnExtractgps()
{
	// TODO: 在此添加命令处理程序代码

	if (m_ExtractGPSDataDlg.DoModal()!=IDOK)
	{
		return;
	}

	vector<GPSSTRUCT> gpsdatavector;
	if (m_ExtractGPSDataDlg.m_SingleOrMulti==0)		//单个文件提取
	{
		CString GPSfilename;
		TCHAR szFilter[] = _T("GPS 文件（*.txt)|*.txt|（*.dat)|*.dat|所有文件(*.*)|*.*||");
		CFileDialog FileDlg(TRUE, m_ExtractGPSDataDlg.m_ExtentName,NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL);
		FileDlg.m_ofn.lpstrTitle=_T("打开测量后基点比对数据文件");

		if (IDOK == FileDlg.DoModal())
		{   
			GPSfilename = FileDlg.GetPathName();  
		}
		else
		{
			return;
		}
		//读取gps文件
		switch (m_ExtractGPSDataDlg.m_GPSFormat)
		{
		case 0:
			ReadGPSDataFile(GPSfilename,gpsdatavector);			//读取GPRMC行
			break;
		case 1:
			ReadGPSDataFile_GPGGA(GPSfilename,gpsdatavector);	//有时候原始文件里面没有GPRMC，读取GPGGA和GPVTG行组合得到经纬度航速航向
			break;
		}
		
		if (gpsdatavector.size()==0)
		{
			MessageBox("共提取了0个文件");
			return;
		}
		////输出GPS数据到文件
		CString OutFileName=gpsdatavector[0].datatimeStr.Left(10)+_T("-GPS.DAT");
		TCHAR szFilter2[] = _T("提取的GPS 文件（*.DAT)|*.DAT|所有文件(*.*)|*.*||");
		CFileDialog FileDlg2(FALSE, _T("DAT"),OutFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter2, NULL);
		FileDlg2.m_ofn.lpstrTitle=_T("输出整理后的GPS数据");
		if (IDOK == FileDlg2.DoModal())
		{   
			OutFileName = FileDlg2.GetPathName();  
		}
		else
		{
			return;
		}
		FILE *fpOut=NULL;
		if ((fpOut=fopen((_bstr_t)OutFileName,"w"))==NULL)
		{
			AfxMessageBox(_T("打开")+OutFileName+_T("失败"));
			return ;
		}

		for (int i = 0; i < gpsdatavector.size(); i++)
		{
			fprintf(fpOut,"%s\t%13.8f\t%13.8f\t%8.1f\t%8.2f\n",(PCSTR)(_bstr_t)(PCTSTR)gpsdatavector[i].datatimeStr,gpsdatavector[i].longitude,
				gpsdatavector[i].latitude,gpsdatavector[i].velocity,gpsdatavector[i].Heading);
		}
		fclose(fpOut);
	}else if(m_ExtractGPSDataDlg.m_SingleOrMulti==1)		//批量提取
	{
		//获取GPS文件路径
		CString gpspathname=m_ExtractGPSDataDlg.m_PathName;
		//1. 从路径中获取文件名
		vector<CString>filenamevector;
		CString extentname;
		if (m_ExtractGPSDataDlg.m_IsExtentName)
		{
			extentname=m_ExtractGPSDataDlg.m_ExtentName;
		}else
		{
			extentname=_T("");
		}
		
		int filenum=GetFileCount(gpspathname,extentname,filenamevector);
		//2. 调用gps文件读取函数
		m_ProgressBar->SetRange(0,filenamevector.size());
		
		for (int i = 0; i < filenamevector.size(); i++)
		{
			switch (m_ExtractGPSDataDlg.m_GPSFormat)
			{
			case 0:
				ReadGPSDataFile(filenamevector[i],gpsdatavector);			//读取GPRMC行
				break;
			case 1:
				ReadGPSDataFile_GPGGA(filenamevector[i],gpsdatavector);	//有时候原始文件里面没有GPRMC，读取GPGGA和GPVTG行组合得到经纬度航速航向
				break;
			}
			
			m_ProgressBar->SetPos(i);
		}
		
		//3. 输出gps数据到文件（按日期命名文件，每天一个文件）
		OutPutGPSData(gpspathname,gpsdatavector);
	}
	
	
	

	
}
void C数据提取View::OutPutGPSData(CString pathname, vector<GPSSTRUCT> GPSDATAVECTOR)
{
	if (GPSDATAVECTOR.size()==0)
	{
		MessageBox("共提取了0个文件");
		return;
	}
	//输出GPS数据到文件
	CString OutFileName=GPSDATAVECTOR[0].datatimeStr.Left(10)+_T("-GPS.DAT");

	OutFileName=pathname+_T("\\")+OutFileName;
	//计算需要导出的文件个数（首尾日期天数差值+1）
	int numberoffile=0;
	COleDateTime firstdate,currentdate,lastdate;
	if(!firstdate.ParseDateTime(GPSDATAVECTOR[0].datatimeStr))
	{
		MessageBox(_T("第一个日期格式有错误:")+GPSDATAVECTOR[0].datatimeStr+_T("\n请检查原始文件"));
		return;
	}
	if (!lastdate.ParseDateTime(GPSDATAVECTOR[GPSDATAVECTOR.size()-1].datatimeStr))
	{
		MessageBox(_T("最后一个日期格式有错误:")+GPSDATAVECTOR[GPSDATAVECTOR.size()-1].datatimeStr+_T("\n请检查原始文件"));
		return;
	}
	COleDateTimeSpan tt;
	tt=lastdate-firstdate;
	numberoffile=tt.GetDays()+1;
	m_ProgressBar->SetRange(0,numberoffile);
	vector<FILE*> fpvector;fpvector.push_back(NULL);
	int errordatanumber=0;
	FILE* fp_error=NULL;					//错误报告文件
	if ((fp_error=fopen((_bstr_t)pathname+_T("\\错误报告.log"),"w"))==NULL)
	{
		AfxMessageBox(_T("打开")+pathname+_T("\\错误报告.log 失败"));
		return ;
	}fprintf(fp_error,"错误数据的日期\t错误数据的前一条记录\n");
	int indexoffile=0;
	if ((fpvector[indexoffile]=fopen((_bstr_t)OutFileName,"w"))==NULL)
	{
		AfxMessageBox(_T("打开")+OutFileName+_T("失败"));
		return ;
	}
	CString firstdatestr=GPSDATAVECTOR[0].datatimeStr.Left(10);
	for (int i = 0; i < GPSDATAVECTOR.size(); i++)
	{
		if(!currentdate.ParseDateTime(GPSDATAVECTOR[i].datatimeStr))			//因为原始数据可能有错误，导致读取的参数有错误，这里判断日期字符串是否是合理的日期
		{
			fprintf(fp_error,"%s---------%s\n",(PCSTR)(_bstr_t)(PCTSTR)GPSDATAVECTOR[i].datatimeStr,(PCSTR)(_bstr_t)(PCTSTR)GPSDATAVECTOR[i-1].datatimeStr);
			errordatanumber++;
			continue;
		}
		tt=currentdate-firstdate;
		
		if (currentdate.GetDay()!=firstdate.GetDay())	//gps文件是连续记录的，相邻两个日期差值不应该大于首尾日期差值
		{
			indexoffile++;m_ProgressBar->SetPos(indexoffile);
			firstdate=currentdate;
			OutFileName=GPSDATAVECTOR[i].datatimeStr.Left(10)+_T("-GPS.DAT");
			OutFileName=pathname+_T("\\")+OutFileName;
			/*if (_access((_bstr_t)OutFileName,00)==-1)
			{
				MessageBox(OutFileName+_T("文件已经存在"));
				OutFileName=OutFileName+_T(".1");
			}*/
			fpvector.push_back(NULL);
			if ((fpvector[indexoffile]=fopen((_bstr_t)OutFileName,"w"))==NULL)
			{
				AfxMessageBox(_T("打开")+OutFileName+_T("失败"));
				return ;
			}
		}
		fprintf(fpvector[indexoffile],"%s\t%13.8f\t%13.8f\t%8.1f\t%8.2f\n",(PCSTR)(_bstr_t)(PCTSTR)GPSDATAVECTOR[i].datatimeStr,GPSDATAVECTOR[i].longitude,
			GPSDATAVECTOR[i].latitude,GPSDATAVECTOR[i].velocity,GPSDATAVECTOR[i].Heading);
	}
	for (int i = 0; i < fpvector.size(); i++)
	{
		if (fpvector[i])
		{
			fclose(fpvector[i]);
		}
	}
	fclose(fp_error);
	CString infostr;
	infostr.Format(_T("GPS数据整理完毕，共整理出%d个文件\n%s--%s\n错误数据：%d 条（错误报告.log）"),numberoffile,GPSDATAVECTOR[0].datatimeStr.Left(10),GPSDATAVECTOR[GPSDATAVECTOR.size()-1].datatimeStr.Left(10),errordatanumber);
	MessageBox(infostr);
}

void C数据提取View::ReadGPSDataFile(CString gpsdatafilename, vector<GPSSTRUCT>& GPSVECTOR)
{
	////读取文件
	CStdioFile fp;
	CString str_Line;
	if (fp.Open(gpsdatafilename,CFile::modeRead)==FALSE)
	{
		AfxMessageBox(_T("打开")+gpsdatafilename+_T("失败"));
		return;
	}
	//提取GPS信息
	GPSSTRUCT gpsdata;
	char headchar[7];
	CString headstr;
	char comma;
	double temp;
	char *tempchar[100];
	char lonsign,latsign;
	const int headlength=7;
	const int headflaglength=6;
	double time1;
	int date1;
	int dd;
	double mm;
	int HH,MM,SS;
	int day,year,month;
	CString timestr,datestr;
	FILE* fp_error=NULL;
	CString errorfilename=gpsdatafilename+_T(".log");
	if ((fp_error=fopen((_bstr_t)errorfilename,"w"))==NULL)
	{
		AfxMessageBox(_T("打开")+errorfilename+_T("失败"));
		return;
	}
	while (fp.ReadString(str_Line))
	{
		//str_Line=str_Line.Left(str_Line.GetLength()-23);//提取22航次磁测GPS时，每行后面都跟23个时间日期字符串，去掉它
		headstr=str_Line.Left(headflaglength);
		str_Line=str_Line.Right(str_Line.GetLength()-headlength);
		if (headstr==_T("$GPRMC") && str_Line.Mid(str_Line.GetLength()-3,1)==_T("*"))			//带星号说明这行是完整的，但是字段不一定一样长，因此这里读取数据时不能用Mid读取，应该格式化读取%lf等
		{
			//有的记录，虽然是PGRMC开头，末尾也有*，但是中间记录有错误或缺失导致不能正确读取，因此需要判断都好个数进一步确定正确的记录
			if (GetCharCountInCString(str_Line,_T(","))!=11)
			{
				fprintf(fp_error,"逗号个数不是11：   %s\n",(PCSTR)(_bstr_t)(PCTSTR)str_Line);
			}else
			{
				sscanf(str_Line,"%lf%c%c%c%lf%c%c%c%lf%c%c%c%lf%c%lf%c%d",&time1,&comma,&comma,&comma,&gpsdata.latitude,
				&comma,&latsign,&comma,&gpsdata.longitude,&comma,&lonsign,&comma,&gpsdata.velocity,&comma,&gpsdata.Heading,&comma,&date1);
				//转换日期格式
				HH=time1/10000;MM=(time1-HH*10000)/100;SS=(time1-HH*10000-MM*100);
				timestr.Format(_T("%02d:%02d:%02d"),HH,MM,SS);
				day=date1/10000;month=(date1-day*10000)/100;year=date1-day*10000-month*100;
				datestr.Format(_T("20%02d-%02d-%02d"),year,month,day);
				gpsdata.datatimeStr=datestr+_T(" ")+timestr;
				//转换经纬度(经度格式为：dddmm.mmmm;纬度)
				//纬度位数不够造成错误，怎么解决？？？？（放在检查改正那里统一改？？判断前后三个经纬度差值不能超过1度）
				dd=(int)gpsdata.latitude/100.0;			//纬度
				mm=gpsdata.latitude-dd*100;
				if (latsign=='S')
				{
					gpsdata.latitude=-((double)dd+mm/60.0);
				}else
				{
					gpsdata.latitude=(double)dd+mm/60.0;
				}
				dd=(int)gpsdata.longitude/100.0;			//经度
				mm=gpsdata.longitude-dd*100;
				if (lonsign=='W')
				{
					gpsdata.longitude=-((double)dd+mm/60.0);
				}else
				{
					gpsdata.longitude=(double)dd+mm/60.0;
				}
				//----------------------------------
				//判断经纬度范围，防止错误
				if (gpsdata.longitude >=-180 && gpsdata.longitude<=180 && gpsdata.latitude >=-90 && gpsdata.latitude<=90)
				{
					GPSVECTOR.push_back(gpsdata);
				}
			}
		}
		
	}fp.Close();fclose(fp_error);
}



int C数据提取View::GetCharCountInCString(CString str, CString str0)
{
	int count=0;
	
	for (int i = 0; i < str.GetLength(); i++)
	{
		if (str0==str.Mid(i,1))
		{
			count++;
		}
	}
	return count;
}


void C数据提取View::OnCheckamend()
{
	// TODO: 在此添加命令处理程序代码
	if (m_ExtractGPSDataDlg.DoModal()!=IDOK)
	{
		return;
	}

	vector<GPSSTRUCT> gpsdatavector;
	if (m_ExtractGPSDataDlg.m_SingleOrMulti==0)		//单个文件提取
	{
		CString GPSfilename;
		TCHAR szFilter[] = _T("GPS 文件（*.dat)|*.dat|（*.txt)|*.txt|所有文件(*.*)|*.*||");
		CFileDialog FileDlg(TRUE, m_ExtractGPSDataDlg.m_ExtentName,NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL);
		FileDlg.m_ofn.lpstrTitle=_T("打开测量后基点比对数据文件");

		if (IDOK == FileDlg.DoModal())
		{   
			GPSfilename = FileDlg.GetPathName();  
		}
		else
		{
			return;
		}
		//读取gps文件
		ReadExtractedGPSFile(GPSfilename,gpsdatavector);
		if (gpsdatavector.size()==0)
		{
			MessageBox("共检查了0个文件");
			return;
		}
		////输出GPS数据到文件
		CString OutFileName=gpsdatavector[0].datatimeStr.Left(10)+_T("-GPS.DAT");
		TCHAR szFilter2[] = _T("提取的GPS 文件（*.DAT)|*.DAT|所有文件(*.*)|*.*||");
		CFileDialog FileDlg2(FALSE, _T("DAT"),OutFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter2, NULL);
		FileDlg2.m_ofn.lpstrTitle=_T("输出整理后的GPS数据");
		if (IDOK == FileDlg2.DoModal())
		{   
			OutFileName = FileDlg2.GetPathName();  
		}
		else
		{
			return;
		}
		FILE *fpOut=NULL;
		if ((fpOut=fopen((_bstr_t)OutFileName,"w"))==NULL)
		{
			AfxMessageBox(_T("打开")+OutFileName+_T("失败"));
			return ;
		}

		for (int i = 0; i < gpsdatavector.size(); i++)
		{
			fprintf(fpOut,"%s\t%13.8f\t%13.8f\t%8.1f\t%8.2f\n",(PCSTR)(_bstr_t)(PCTSTR)gpsdatavector[i].datatimeStr,gpsdatavector[i].longitude,
				gpsdatavector[i].latitude,gpsdatavector[i].velocity,gpsdatavector[i].Heading);
		}
		fclose(fpOut);
	}else if(m_ExtractGPSDataDlg.m_SingleOrMulti==1)		//批量提取
	{
		//获取GPS文件路径
		CString gpspathname=m_ExtractGPSDataDlg.m_PathName;
		//1. 从路径中获取文件名
		vector<CString>filenamevector;
		CString extentname;
		if (m_ExtractGPSDataDlg.m_IsExtentName)
		{
			extentname=m_ExtractGPSDataDlg.m_ExtentName;
		}else
		{
			extentname=_T("");
		}
		
		int filenum=GetFileCount(gpspathname,extentname,filenamevector);
		if (filenum<3)
		{
			MessageBox(_T("文件个数小于3!手动处理"));
			return;
		}
		AmendGPSTimeDate(gpspathname,filenamevector);//改正gps时间与日期错位的问题(保证每个文件第一行是0:0:0)
		//查询每个文件，输出文件第一行不是0：0:0的文件名到修正报告里
		FILE* fp;
		if ((fp=fopen((_bstr_t)(gpspathname+_T("\\检查报告.log")),"w"))==NULL)
		{
			MessageBox(_T("打开检查报告文件失败"));
			return ;
		}
		fprintf(fp,"第一行不是00:00:00的GPS文件有：\n");
		for (int i = 0; i < filenamevector.size(); i++)
		{
			if (!InspectGPSFile(filenamevector[i]))
			{
				fprintf(fp,"%s\n",(PCSTR)(_bstr_t)(PCTSTR)filenamevector[i]);
			}
		}
		fclose(fp);
	}
	
	MessageBox(_T("修正完毕"));
}
void C数据提取View::CorrectGPSTimeData(CString pathname,int& row1,vector<GPSSTRUCT>firstGPSvector,vector<GPSSTRUCT>secondGPSvector)
{
	GPSSTRUCT gpsdata;
	int row2;
	for (int i = 0; i < secondGPSvector.size(); i++)
	{
		if (secondGPSvector[i].datatimeStr.Right(8)!=_T("00:00:00"))
		{
			gpsdata.datatimeStr.Format(_T("%s%s"),firstGPSvector[0].datatimeStr.Left(11),secondGPSvector[i].datatimeStr.Right(8));
			gpsdata.latitude=secondGPSvector[i].latitude;
			gpsdata.longitude=secondGPSvector[i].longitude;
			gpsdata.velocity=secondGPSvector[i].velocity;
			gpsdata.Heading=secondGPSvector[i].Heading;
			firstGPSvector.push_back(gpsdata);
		}else
		{
			row2=i;
			WriteExtractedGPSData(pathname,firstGPSvector,row1);//
			firstGPSvector.clear();
			row1=row2;
			break;
		}
	}
}
void C数据提取View::AmendGPSTimeDate(CString pathname,vector<CString> filenamevector)
{
	m_ProgressBar->SetRange(0,filenamevector.size());
	CString firstfile,secondfile,thirdfile;
	vector<GPSSTRUCT>firstGPSvector,secondGPSvector,thirdGPSvector;
	int row1=0,row2=0;
	GPSSTRUCT gpsdata;
	firstfile=filenamevector[0];
	secondfile=filenamevector[1];
	thirdfile=filenamevector[2];
	ReadExtractedGPSFile(firstfile,firstGPSvector);
	ReadExtractedGPSFile(secondfile,secondGPSvector);
	ReadExtractedGPSFile(thirdfile,thirdGPSvector);
	//首先从第二个文件中挑出属于第一个文件（日期）的数据，并写入第一个文件
	m_ProgressBar->SetPos(1);
	for (int i = 0; i < secondGPSvector.size(); i++)
	{
		if (secondGPSvector[i].datatimeStr.Right(8)!=_T("00:00:00"))
		{
			gpsdata.datatimeStr.Format(_T("%s%s"),firstGPSvector[0].datatimeStr.Left(11),secondGPSvector[i].datatimeStr.Right(8));
			gpsdata.latitude=secondGPSvector[i].latitude;
			gpsdata.longitude=secondGPSvector[i].longitude;
			gpsdata.velocity=secondGPSvector[i].velocity;
			gpsdata.Heading=secondGPSvector[i].Heading;
			firstGPSvector.push_back(gpsdata);
		}else
		{
			row1=i;
			break;
		}
	}
	WriteExtractedGPSData(pathname,firstGPSvector,0);//第一个文件修改完成，写入
	firstGPSvector.clear();
	m_ProgressBar->SetPos(2);
	for (int j = 3; j < filenamevector.size(); j++)
	{
		firstGPSvector=secondGPSvector;secondGPSvector.clear();
		secondGPSvector=thirdGPSvector;thirdGPSvector.clear();
		ReadExtractedGPSFile(filenamevector[j],thirdGPSvector);
		
		CorrectGPSTimeData(pathname,row1,firstGPSvector,secondGPSvector);
		m_ProgressBar->SetPos(j);
	}
	//处理最后两个
	for (int i = 0; i < thirdGPSvector.size(); i++)
	{
		if (thirdGPSvector[i].datatimeStr.Right(8)!=_T("00:00:00"))
		{
			gpsdata.datatimeStr.Format(_T("%s%s"),secondGPSvector[0].datatimeStr.Left(11),thirdGPSvector[i].datatimeStr.Right(8));
			gpsdata.latitude=thirdGPSvector[i].latitude;
			gpsdata.longitude=thirdGPSvector[i].longitude;
			gpsdata.velocity=thirdGPSvector[i].velocity;
			gpsdata.Heading=thirdGPSvector[i].Heading;
			secondGPSvector.push_back(gpsdata);
		}else
		{
			row2=i;
			WriteExtractedGPSData(pathname,secondGPSvector,row1);
			break;
		}
	}
	
	WriteExtractedGPSData(pathname,thirdGPSvector,row2);
	m_ProgressBar->SetPos(filenamevector.size());
}

void C数据提取View::WriteExtractedGPSData(CString pathname, vector<GPSSTRUCT> GPSDATAVECTOR,int startrow,bool IsCheck)
{
	CString OutFileName=pathname+_T("\\")+GPSDATAVECTOR[0].datatimeStr.Left(10)+_T("-GPS.DAT");
	FILE *fpOut=NULL;
	if ((fpOut=fopen((_bstr_t)OutFileName,"w"))==NULL)
	{
		AfxMessageBox(_T("打开")+OutFileName+_T("失败"));
		return ;
	}
	if (IsCheck)
	{
		/*fprintf(fpOut,"%s\t%13.8f\t%13.8f\t%8.1f\t%8.2f\n",(PCSTR)(_bstr_t)(PCTSTR)GPSDATAVECTOR[startrow].datatimeStr,GPSDATAVECTOR[startrow].longitude,
				GPSDATAVECTOR[startrow].latitude,GPSDATAVECTOR[startrow].velocity,GPSDATAVECTOR[startrow].Heading);
		for (int i = startrow+1; i < GPSDATAVECTOR.size(); i++)
		{
			if (GPSDATAVECTOR[i].latitude)
			{

			}
			fprintf(fpOut,"%s\t%13.8f\t%13.8f\t%8.1f\t%8.2f\n",(PCSTR)(_bstr_t)(PCTSTR)GPSDATAVECTOR[i].datatimeStr,GPSDATAVECTOR[i].longitude,
				GPSDATAVECTOR[i].latitude,GPSDATAVECTOR[i].velocity,GPSDATAVECTOR[i].Heading);
		}*/
	}else
	{
		for (int i = startrow; i < GPSDATAVECTOR.size(); i++)
		{
			fprintf(fpOut,"%s\t%13.8f\t%13.8f\t%8.1f\t%8.2f\n",(PCSTR)(_bstr_t)(PCTSTR)GPSDATAVECTOR[i].datatimeStr,GPSDATAVECTOR[i].longitude,
				GPSDATAVECTOR[i].latitude,GPSDATAVECTOR[i].velocity,GPSDATAVECTOR[i].Heading);
		}
	}
	
	fclose(fpOut);
}
void C数据提取View::ReadExtractedGPSFile(CString filename, vector<GPSSTRUCT>& gpsdatavector)
{
	GPSSTRUCT gpsdata;
	CString datestr,timestr;
	CString firstdatestr;
	char* datechar[20],*timechar[20];
	FILE *fp=NULL;
	if ((fp=fopen((_bstr_t)filename,"r"))==NULL)
	{
		AfxMessageBox(_T("打开")+filename+_T("失败"));
		return ;
	}
	//读取文件
	fscanf(fp,"%s%s%lf%lf%lf%lf",datechar,timechar,&gpsdata.longitude,&gpsdata.latitude,&gpsdata.velocity,&gpsdata.Heading);
	datestr.Format(_T("%s"),datechar);timestr.Format(_T(" %s"),timechar);
	gpsdata.datatimeStr=datestr+timestr;
	firstdatestr=datestr;
	gpsdatavector.push_back(gpsdata);
	while (!feof(fp))
	{
		fscanf(fp,"%s%s%lf%lf%lf%lf",datechar,timechar,&gpsdata.longitude,&gpsdata.latitude,&gpsdata.velocity,&gpsdata.Heading);
		datestr.Format(_T("%s"),datechar);timestr.Format(_T(" %s"),timechar);
		gpsdata.datatimeStr=datestr+timestr;
		if (firstdatestr==datestr)
		{
			gpsdatavector.push_back(gpsdata);
		}
	}
	if (gpsdatavector[gpsdatavector.size()-1].datatimeStr==gpsdatavector[gpsdatavector.size()-2].datatimeStr)
	{
		gpsdatavector.pop_back();
	}
	fclose(fp);
}

void C数据提取View::OnMatchgpsGrav()
{
	// TODO: 在此添加命令处理程序代码
	if (m_MatchGPSGravDlg.DoModal()!=IDOK)
	{
		return;
	}
	CString gravpath=m_MatchGPSGravDlg.m_GravDataPath,gpspath=m_MatchGPSGravDlg.m_GPSDataPath,matchedgravpath=m_MatchGPSGravDlg.m_MatchedGravDataPath;
	//先获取GPS文件向量
	vector<CString>gpsfilenamevector;
	vector<GPSSTRUCT>gpsdatavector;
	GetFileCount(gpspath,_T("DAT"),gpsfilenamevector);
	int gpsfilenum=gpsfilenamevector.size();
	CString gravfilename;			//根据gravpath和gps文件里的日期确定相匹配的重力数据文件名
	COleDateTime currentdate;
	int matchedfilenum=0;
	m_ProgressBar->SetRange(1,gpsfilenum);
	FILE* fp_log;
	CString filename;
	filename.Format(_T("%s\\匹配报告.log.dat"),matchedgravpath);
	if ((fp_log=fopen((_bstr_t)filename,"w"))==NULL)
	{
		AfxMessageBox(_T("打开")+filename+_T("失败"));
		return ;
	}
	fprintf(fp_log,"GPS数据共%d天\n缺失重力数据的日期:\n",gpsfilenum);
	for (int i = 0; i < gpsfilenum; i++)
	{
		gpsdatavector.clear();
		ReadExtractedGPSFile(gpsfilenamevector[i],gpsdatavector);
		//获取日期
		if(!currentdate.ParseDateTime(gpsdatavector[0].datatimeStr))
		{
			MessageBox(gpsfilenamevector[i]+_T("\n第一个日期格式有错误:")+gpsdatavector[0].datatimeStr+_T("\n请检查原始文件"));
			return;
		}
		//currentdate.GetDayOfYear();		//儒略历天
		gravfilename.Format(_T("%s\\%d_%03d.%s.DAT"),gravpath,currentdate.GetYear(),currentdate.GetDayOfYear(),m_MatchGPSGravDlg.m_GravimeterVersion);
		//打开重力数据
		m_OGDataVector.clear();//读取重力数据之前先清空向量
		if (!ReadGravData(gravfilename))
		{
			fprintf(fp_log,"%s\n",(PCSTR)(_bstr_t)(PCTSTR)gpsdatavector[0].datatimeStr.Left(10));
			continue;
		}
		     
		//匹配重力与gps并输出
		if (WriteGRAVGPSData(matchedgravpath,m_OGDataVector,gpsdatavector))
		{
			matchedfilenum++;
		}
		m_ProgressBar->SetPos(i);
	}
	fclose(fp_log);
	CString strinfo;
	strinfo.Format(_T("共匹配%d个重力文件,见匹配报告"),matchedfilenum);
	MessageBox(strinfo);
}


bool C数据提取View::WriteGRAVGPSData(CString pathname,vector<OGDataStruct> ogdavavector, vector<GPSSTRUCT> gpsdatavector)
{
	if (ogdavavector.size()==0 || gpsdatavector.size()==0)
	{
		MessageBox(_T("GPS或者重力数据没有读取"));
		return false;
	}
	FILE* FPOUT=NULL;
	CString filename;
	filename.Format(_T("%s\\%s_GravGPS.dat"),pathname,gpsdatavector[0].datatimeStr.Left(10));
	if ((FPOUT=fopen((_bstr_t)filename,"w"))==NULL)
	{
		AfxMessageBox(_T("打开")+filename+_T("失败"));
		return false;
	}
	COleDateTime currenttime;
	int currentsecond;//当前时间距离00:00:00：的秒数
	int GravIndex;
	int dIndex/*=FirstLineInGravFile-1*/;
	for (int i = 0; i < ogdavavector.size(); i++)
	{
		currenttime.ParseDateTime(ogdavavector[i].datatimeStr);
		if (currenttime.GetHour()==0 && currenttime.GetMinute()==0 && currenttime.GetSecond()==0)
		{
			dIndex=i;
		}
	}//CString str;str.Format(_T("%d"),dIndex);MessageBox(str);
	//判断重力数据中0:0:0:所在行数
	
	for (int i = 0; i < gpsdatavector.size(); i++)
	{
		if(!currenttime.ParseDateTime(gpsdatavector[i].datatimeStr))
		{
			MessageBox(_T("日期格式有错误:")+gpsdatavector[i].datatimeStr+_T("\n请检查原始文件"));
			return false;
		}
		currentsecond=currenttime.GetHour()*3600+currenttime.GetMinute()*60+currenttime.GetSecond();
		GravIndex=currentsecond+dIndex;//
		if (GravIndex>=m_OGDataVector.size())
		{
			///MessageBox(_T("GPS时间索引超出GRAV向量范围")+gpsdatavector[i].datatimeStr);
			//CString str;str.Format(_T("重力向量%d  索引%d"),m_OGDataVector.size(),GravIndex);MessageBox(str);
			fclose(FPOUT);
			return true;
		}
		//正规重力文件应该是一整天的，00:00:00时刻的重力数据位向量的第二个元素，1s采样，以此类推
		//MessageBox(ogdavavector[currentsecond+1].datatimeStr+_T("*********")+gpsdatavector[i].datatimeStr);
		fprintf(FPOUT,"$DAT\t%s   \t%s\t%8.2f\t%8.2f\t%11.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%13.8f\t%13.8f\t%8.3f\t%8.3f\n",
			(PCSTR)(_bstr_t)(PCTSTR)m_OGDataVector[GravIndex].datatimeStr,(PCSTR)(_bstr_t)(PCTSTR)m_OGDataVector[GravIndex].day,
			m_OGDataVector[GravIndex].gravity,m_OGDataVector[GravIndex].spinttension,m_OGDataVector[GravIndex].beamposition,
			m_OGDataVector[GravIndex].VCC,m_OGDataVector[GravIndex].AL,m_OGDataVector[GravIndex].AX,m_OGDataVector[GravIndex].VE,
			m_OGDataVector[GravIndex].AX2,m_OGDataVector[GravIndex].XACC2,m_OGDataVector[GravIndex].LACC2,m_OGDataVector[GravIndex].CROSSACCEL,
			m_OGDataVector[GravIndex].LONGACCEL,0.0,gpsdatavector[i].longitude,gpsdatavector[i].latitude,gpsdatavector[i].velocity,gpsdatavector[i].Heading);
	}
	fclose(FPOUT);
	return true;
}


void C数据提取View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	ClearPointIndex();
}

//释放m_pointindex二维数组内存
int C数据提取View::ClearPointIndex(void)
{
	/*if (m_PointIndex)
	{
		for (int i = 0; i < m_Row_PointIndex; i++)
		{
			delete m_PointIndex[i];
		}
		delete m_PointIndex;
		m_PointIndex=NULL;
	}*/
	return 0;
}


void C数据提取View::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CView::OnMouseMove(nFlags, point);
}


void C数据提取View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//1. 绘制橡皮筋区域
	CRectTracker recttract;
	recttract.TrackRubberBand(this,point,TRUE);
	
	//2. 把tracker的区域作为放大区域
	m_SelectedRect=recttract.m_rect;
	m_SelectedRect.NormalizeRect();	//将选择区域正则化，鼠标托转方向会影响区域的坐标轴正负：会出现负的宽度和高度
	
	//3. 求交集
	m_SelectedRect.IntersectRect(m_SelectedRect,m_CourseFigureRect);

	//4. 初始化局部放大滑块控件
	CMainFrame* pMainFrm=(CMainFrame*)AfxGetApp()->GetMainWnd();
	CMFCRibbonSlider* PointNumber=DYNAMIC_DOWNCAST(CMFCRibbonSlider,pMainFrm->m_wndRibbonBar.FindByID(ID_SelectPointSlider));
	PointNumber->SetPos(0);
	m_CurrentSelectPointNumber=0;
		
	//5. 更新视图
	if (!m_SelectedRect.IsRectEmpty())
	{
		UpdateSelectRectFigure(m_SelectedRect);		//选择区域与绘图区域有非空交集
	}

	CView::OnLButtonDown(nFlags, point);
}


void C数据提取View::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	CView::OnLButtonUp(nFlags, point);
}


int C数据提取View::UpdateSelectRectFigure(CRect SelectedRect)
{
	if (!m_PointIndex)		//索引二维数组为空，则跳出
	{
		return 0;
	}
	//m_BitMapDC.Rectangle(SelectedRect);
	//遍历整个选择的区域像素点对应的数据点绘制情况，获取这个区域内绘制的数据点在数据向量中的索引
	vector<int> indexvector;
	int i1=SelectedRect.top-m_CourseFigureRect.top;
	int i2=SelectedRect.bottom-m_CourseFigureRect.top;
	int j1=SelectedRect.left-m_CourseFigureRect.left;
	int j2=SelectedRect.right-m_CourseFigureRect.left;
	for (int i = i1; i < i2; i++)
	{
		for (int j = j1; j < j2; j++)
		{
			if (m_PointIndex[i][j].PointExit)//该像素点有数据
			{
				indexvector.push_back(m_PointIndex[i][j].IndexInOGdataVector);
			}
		}
	}
	if (indexvector.size()<3)//如果选择区域内的数据个数小于3，则跳出
	{
		return 0;
	}
	//对找到的索引进行排序，找到最大值和最小值分别作为局部放大的起点和终点，差值作为局部放大范围
	int indexmin=indexvector[0],indexmax=indexvector[0];
	for (int i = 0; i < indexvector.size(); i++)
	{
		if (indexmax<indexvector[i])
		{
			indexmax=indexvector[i];
		}

		if (indexmin > indexvector[i])
		{
			indexmin=indexvector[i];
		}
	}
	if (indexmax < 0 || indexmax >= m_OGDataVector.size())
	{
		return 0;
	}
	//对局部放大范围赋值
	m_CurrentZoomUpRangeLeft=indexmin;
	m_ZoomUpRange=indexmax-indexmin;
	SetZoomUpSliderRange(m_ZoomUpRange);
	UpdateMainPlot();
	return 0;
}


int C数据提取View::SetZoomUpSliderRange(int range)
{
	CMainFrame* pMainFrm=(CMainFrame*)AfxGetApp()->GetMainWnd();
	CMFCRibbonSlider* PointNumber=DYNAMIC_DOWNCAST(CMFCRibbonSlider,pMainFrm->m_wndRibbonBar.FindByID(ID_SelectPointSlider));
	PointNumber->SetPos(0);
		PointNumber->SetRange(0,range-1);
	return 0;
}


void C数据提取View::ReadGPSDataFile_GPGGA(CString gpsdatafilename, vector<GPSSTRUCT>& GPSVECTOR)
{
	////读取文件
	CStdioFile fp;
	CString str_Line,str_Line_GPVTG,str_Line_GPZDA;
	if (fp.Open(gpsdatafilename,CFile::modeRead)==FALSE)
	{
		AfxMessageBox(_T("打开")+gpsdatafilename+_T("失败"));
		return;
	}
	//提取GPS信息
	GPSSTRUCT gpsdata;
	char headchar[7];
	CString headstr,head_gpvtg,head_gpzda;
	char comma;
	double temp;
	char *tempchar[100];
	char lonsign,latsign;
	const int headlength=7;
	const int headflaglength=6;
	double time1;
	int date1;
	int dd;
	double mm;
	int HH,MM,SS;
	int day,year,month;
	CString timestr,datestr;
	FILE* fp_error=NULL;
	CString errorfilename=gpsdatafilename+_T(".log");
	if ((fp_error=fopen((_bstr_t)errorfilename,"w"))==NULL)
	{
		AfxMessageBox(_T("打开")+errorfilename+_T("失败"));
		return;
	}
	while (fp.ReadString(str_Line))
	{
		headstr=str_Line.Left(headflaglength);//读取GPGGA行
		str_Line=str_Line.Right(str_Line.GetLength()-headlength);
		if (headstr==_T("$GPGGA") && str_Line.Mid(str_Line.GetLength()-3,1)==_T("*"))			//带星号说明这行是完整的，但是字段不一定一样长，因此这里读取数据时不能用Mid读取，应该格式化读取%lf等
		{
			fp.ReadString(str_Line_GPVTG);//读取gpvtg行
			head_gpvtg=str_Line_GPVTG.Left(headflaglength);
			str_Line_GPVTG=str_Line_GPVTG.Right(str_Line_GPVTG.GetLength()-headlength);
			if (head_gpvtg==_T("$GPVTG") && str_Line_GPVTG.Mid(str_Line_GPVTG.GetLength()-3,1)==_T("*"))
			{
				fp.ReadString(str_Line_GPZDA);//读取gpzda行
				fp.ReadString(str_Line_GPZDA);//读取gpzda行
				head_gpzda=str_Line_GPZDA.Left(headflaglength);
				str_Line_GPZDA=str_Line_GPZDA.Right(str_Line_GPZDA.GetLength()-headlength);
				if (head_gpzda==_T("$GPZDA") && str_Line_GPZDA.Mid(str_Line_GPZDA.GetLength()-3,1)==_T("*"))
				{
					//有的记录，虽然是PGRMC开头，末尾也有*，但是中间记录有错误或缺失导致不能正确读取，因此需要判断都好个数进一步确定正确的记录
					if (GetCharCountInCString(str_Line,_T(","))!=13)			//GPGGA正常的有13个逗号
					{
						fprintf(fp_error,"GPGGA逗号个数不是13：   %s\n",(PCSTR)(_bstr_t)(PCTSTR)str_Line);
					}else
					{
						if (GetCharCountInCString(str_Line_GPVTG,_T(","))!=8)
						{
							fprintf(fp_error,"GPVTG逗号个数不是8：   %s\n",(PCSTR)(_bstr_t)(PCTSTR)str_Line_GPVTG);
						}else
						{
							if (GetCharCountInCString(str_Line_GPZDA,_T(","))!=5)
							{
								fprintf(fp_error,"GPZDA逗号个数不是5：   %s\n",(PCSTR)(_bstr_t)(PCTSTR)str_Line_GPZDA);
							}else
							{
								//*****************************读取GPZDA（日期）********************************************************************
								sscanf(str_Line_GPZDA,"%lf%c%d%c%d%c%d",&time1,&comma,&day,&comma,&month,&comma,&year);
								//转换日期格式
								datestr.Format(_T("%d-%02d-%02d"),year,month,day);
								//*****************************读取GPGGA（经纬度）********************************************************************
								sscanf(str_Line,"%lf%c%lf%c%c%c%lf%c%c",&time1,&comma,&gpsdata.latitude,&comma,&latsign,&comma,&gpsdata.longitude,&comma,&lonsign);
								//转换时间格式
								HH=time1/10000;MM=(time1-HH*10000)/100;SS=(time1-HH*10000-MM*100);
								timestr.Format(_T("%02d:%02d:%02d"),HH,MM,SS);
								gpsdata.datatimeStr=datestr+_T(" ")+timestr;
								//转换经纬度(经度格式为：dddmm.mmmm;纬度)
								//纬度位数不够造成错误，怎么解决？？？？（放在检查改正那里统一改？？判断前后三个经纬度差值不能超过1度）
								dd=(int)gpsdata.latitude/100.0;			//纬度
								mm=gpsdata.latitude-dd*100;
								if (latsign=='S')
								{
									gpsdata.latitude=-((double)dd+mm/60.0);
								}else
								{
									gpsdata.latitude=(double)dd+mm/60.0;
								}
								dd=(int)gpsdata.longitude/100.0;			//经度
								mm=gpsdata.longitude-dd*100;
								if (lonsign=='W')
								{
									gpsdata.longitude=-((double)dd+mm/60.0);
								}else
								{
									gpsdata.longitude=(double)dd+mm/60.0;
								}
								
								//*****************************读取GPVTG(航向航速)********************************************************************
								sscanf(str_Line_GPVTG,"%lf%c%c%c%c%c%c%lf",&gpsdata.Heading,&comma,&comma,&comma,&comma,&comma,&comma,&gpsdata.velocity);
								//----------------------------------
								//判断经纬度范围，防止错误
								if (gpsdata.longitude >=-180 && gpsdata.longitude<=180 && gpsdata.latitude >=-90 && gpsdata.latitude<=90)
								{
									GPSVECTOR.push_back(gpsdata);
								}
							}
						}
					
					}
				}
				
			}
			
		}
		
	}fp.Close();
	fclose(fp_error);
}


void C数据提取View::OnCreatewordreport()
{
	// TODO: 在此添加命令处理程序代码
	//1 批量读取测线文件
	if (m_WriteToWordDlg.DoModal()!=IDOK)
	{
		return;
	}
	//获取GPS文件路径
	CString gpspathname=m_WriteToWordDlg.m_SurveyLinePath;
	vector<CString>filenamevector;
	CString extentname=m_WriteToWordDlg.m_ExtName;
	int filenum=GetFileCount(gpspathname,extentname,filenamevector);//读取所有测线文件名
	if (filenum==0)
	{
		MessageBox(_T("读取了0个有效测线"));
		return;
	}
	//文件指针数组
	CStdioFile* fp=new CStdioFile[filenum];
	CString errorinfoStr;
	for (int i = 0; i < filenum; i++)
	{
		if (fp[i].Open(filenamevector[i],CFile::modeRead)==FALSE)
		{
			errorinfoStr.Format(_T("读取第%d个测线失败，程序将退出"),i);
			MessageBox(errorinfoStr);
			return;
		}
	}

	
	//2 打开word，创建文档：文档表格格式参考【资料整编与汇交一般性要求】附表B.1[综合地球物理测线信息表]
	CreateWordDocuments();
	CreateWord();
	m_wdDocs=m_wdApp.get_Documents();
	m_wdDoc=m_wdApp.get_ActiveDocument();
	CFont0 font = m_wdSel.get_Font();					//设置word基本属性
           font.put_Name(_T("Times New Roman"));
		   m_wdSel.get_PageSetup();
	CPageSetup pgesetup=m_wdSel.get_PageSetup();
	pgesetup.put_Orientation(1);
	 
	CString RTITableTitle;RTITableTitle.Format(_T("%s航次第%s航段重力测线统计"),m_WriteToWordDlg.m_HangCi,m_WriteToWordDlg.m_HangDuan);
	m_wdSel.TypeParagraph();
	m_wdSel.put_Style(COleVariant(_T("标题")));			//标题样式
	font = m_wdSel.get_Font();//设置字体
           font.put_Name(_T("Times New Roman"));
	m_wdSel.TypeText(RTITableTitle);					//文档标题

	m_wdSel.TypeParagraph();							//新起一段
	m_wdSel.put_Style(COleVariant(_T("标题")));			//标题样式
	font = m_wdSel.get_Font();//设置字体
           font.put_Name(_T("Times New Roman"));
		   font.put_Size(11);
		   font.put_Bold(0);
	m_wdSel.TypeText(_T("单位/机构：")+m_WriteToWordDlg.m_DanWeiJiGou);
	m_wdSel.TypeText(_T("       调查船：")+m_WriteToWordDlg.m_DiaoChaChuan);
	m_wdSel.TypeText(_T("       航次编号：")+m_WriteToWordDlg.m_Title);

	//3. 表格部分*********************************************************
	int nRowSize = filenum+2,nColumnSize=9;				//表格的行数和列数
	m_wdSel.TypeParagraph();			//新起一段
	CParagraph paragraph=m_wdSel.get_ParagraphFormat();
	paragraph.put_Alignment(1);
	font = m_wdSel.get_Font();//设置字体
           font.put_Name(_T("Times New Roman"));
	CTables0 wordTables = m_wdDoc.get_Tables();
	CRange wordRange = m_wdSel.get_Range();
	COleVariant varTrue(short(1),VT_BOOL),vOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);  
    CComVariant Template(_T(""));    //没有使用WORD的文档模板  
    CComVariant NewTemplate(false),DocumentType(0),Visible;  
	CTable0 wordTable= wordTables.Add(wordRange, nRowSize , nColumnSize, &NewTemplate, varTrue);
	
	//边框
	CBorders tblBorders;
	tblBorders=wordTable.get_Borders();
	tblBorders.put_InsideLineStyle(1);//内边框，7表示中空的边框，0表示不画边框，1表示实边框
	tblBorders.put_OutsideLineStyle(1);//外边框
	
	
	// 填表
	CCell cell;
	CString firstline,lastline,templine;

	cell = wordTable.Cell(1, 1);cell.Select();m_wdSel.TypeText(_T("航次编号"));
	cell = wordTable.Cell(1, 2);cell.Select();m_wdSel.TypeText(_T("测线号"));
	cell = wordTable.Cell(1, 3);cell.Select();m_wdSel.TypeText(_T("起始时间\nyyyymmddHHMMSS"));
	cell = wordTable.Cell(1, 4);cell.Select();m_wdSel.TypeText(_T("终止时间\nyyyymmddHHMMSS"));
	cell = wordTable.Cell(1, 5);cell.Select();m_wdSel.TypeText(_T("起始经度\n°"));
	cell = wordTable.Cell(1, 6);cell.Select();m_wdSel.TypeText(_T("起始纬度\n°"));
	cell = wordTable.Cell(1, 7);cell.Select();m_wdSel.TypeText(_T("终止经度\n°"));
	cell = wordTable.Cell(1, 8);cell.Select();m_wdSel.TypeText(_T("终止纬度\n°"));
	cell = wordTable.Cell(1, 9);cell.Select();m_wdSel.TypeText(_T("测线长度\nkm"));
	double SumDistance=0;
	for (int i = 0; i < filenum; i++)
	{
		char tempchar[20],datechar[20],timechar[20];
		CString surveylinename,datestr,timestr,longitudestr,latitudestr,distancestr;
		double longitude,latitude,distance=0,headingtemp,velocitytemp;
		int linenumber=0;
		fp[i].ReadString(firstline);
		if (firstline.Left(4)!=_T("$DAT"))
		{
			MessageBox(_T("文件格式不正确"));
			return;
		}
		//判断第一行的字符串长度，决定用什么方法读取
		if (firstline.GetLength()==197)	//标准格式
		{
			while (fp[i].ReadString(templine))
			{
				lastline=templine;

				velocitytemp=atof(templine.Mid(180,8));

				distance+=velocitytemp*1.852/3600.0;//速度乘以时间=km
				linenumber++;
			}

			//起始信息
			datestr=firstline.Mid(5,10);
			timestr=firstline.Mid(16,8);
			datestr.Format(_T("%s %s"),datestr,timestr);
			longitude=atof(firstline.Mid(152,13));
			latitude=atof(firstline.Mid(166,13));
	
			longitudestr.Format(_T("%.8f"),longitude);
			latitudestr.Format(_T("%.8f"),latitude);
			COleDateTime date;
			date.ParseDateTime(datestr);
			datestr.Format(_T("%4d%02d%02d%02d%02d%02d"),date.GetYear(),date.GetMonth(),date.GetDay(),date.GetHour(),date.GetMinute(),date.GetSecond());
			//写入表格(起始)
			surveylinename.Format(_T("%s%02d"),m_WriteToWordDlg.m_surveylinename,i+1);
			cell = wordTable.Cell(i+2, 1);cell.Select();m_wdSel.TypeText(m_WriteToWordDlg.m_Title);
			cell = wordTable.Cell(i+2, 2);cell.Select();m_wdSel.TypeText(surveylinename);
			cell = wordTable.Cell(i+2, 3);cell.Select();m_wdSel.TypeText(datestr);
			cell = wordTable.Cell(i+2, 5);cell.Select();m_wdSel.TypeText(longitudestr);
			cell = wordTable.Cell(i+2, 6);cell.Select();m_wdSel.TypeText(latitudestr);

			//终止信息
			datestr=lastline.Mid(5,10);
			timestr=lastline.Mid(16,8);
			datestr.Format(_T("%s %s"),datestr,timestr);
			longitude=atof(lastline.Mid(152,13));
			latitude=atof(lastline.Mid(166,13));
	
			longitudestr.Format(_T("%.8f"),longitude);
			latitudestr.Format(_T("%.8f"),latitude);

			date.ParseDateTime(datestr);
			datestr.Format(_T("%4d%02d%02d%02d%02d%02d"),date.GetYear(),date.GetMonth(),date.GetDay(),date.GetHour(),date.GetMinute(),date.GetSecond());


			//写入表格(终止)
			cell = wordTable.Cell(i+2, 4);cell.Select();m_wdSel.TypeText(datestr);
			cell = wordTable.Cell(i+2, 7);cell.Select();m_wdSel.TypeText(longitudestr);
			cell = wordTable.Cell(i+2, 8);cell.Select();m_wdSel.TypeText(latitudestr);
			distancestr.Format(_T("%.1f"),distance);
			cell = wordTable.Cell(i+2, 9);cell.Select();m_wdSel.TypeText(distancestr);
			//cell = wordTable.Cell(i+2, 10);cell.Select();m_wdSel.TypeText(GetFileNameByPath(filenamevector[i]));//测线文件名
		}else
		{
			while (fp[i].ReadString(templine))
			{
				lastline=templine;
				sscanf(templine,"%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%lf%lf%lf%lf",tempchar,datechar,timechar,tempchar,tempchar,tempchar,
				tempchar,tempchar,tempchar,tempchar,tempchar,tempchar,tempchar,tempchar,tempchar,tempchar,tempchar,&longitude,&latitude,&velocitytemp,&headingtemp);
				distance+=velocitytemp*1.852/3600.0;//速度乘以时间=km
				linenumber++;
			}
			sscanf(firstline,"%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%lf%lf%lf%lf",tempchar,datechar,timechar,tempchar,tempchar,tempchar,
				tempchar,tempchar,tempchar,tempchar,tempchar,tempchar,tempchar,tempchar,tempchar,tempchar,tempchar,&longitude,&latitude,&velocitytemp,&headingtemp);			//起始
			longitudestr.Format(_T("%.8f"),longitude);
			latitudestr.Format(_T("%.8f"),latitude);
			datestr.Format(_T("%s %s"),datechar,timechar);
			COleDateTime date;
			date.ParseDateTime(datestr);
			datestr.Format(_T("%4d%02d%02d%02d%02d%02d"),date.GetYear(),date.GetMonth(),date.GetDay(),date.GetHour(),date.GetMinute(),date.GetSecond());
			//写入表格(起始)
			surveylinename.Format(_T("%s%02d"),m_WriteToWordDlg.m_surveylinename,i+1);
			cell = wordTable.Cell(i+2, 1);cell.Select();m_wdSel.TypeText(m_WriteToWordDlg.m_Title);
			cell = wordTable.Cell(i+2, 2);cell.Select();m_wdSel.TypeText(surveylinename);
			cell = wordTable.Cell(i+2, 3);cell.Select();m_wdSel.TypeText(datestr);
			cell = wordTable.Cell(i+2, 5);cell.Select();m_wdSel.TypeText(longitudestr);
			cell = wordTable.Cell(i+2, 6);cell.Select();m_wdSel.TypeText(latitudestr);

			sscanf(lastline,"%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%lf%lf%lf%lf",tempchar,datechar,timechar,tempchar,tempchar,tempchar,
				tempchar,tempchar,tempchar,tempchar,tempchar,tempchar,tempchar,tempchar,tempchar,tempchar,tempchar,&longitude,&latitude,&velocitytemp,&headingtemp);			//终止
			longitudestr.Format(_T("%.8f"),longitude);
			latitudestr.Format(_T("%.8f"),latitude);
			datestr.Format(_T("%s %s"),datechar,timechar);
			date.ParseDateTime(datestr);
			datestr.Format(_T("%4d%02d%02d%02d%02d%02d"),date.GetYear(),date.GetMonth(),date.GetDay(),date.GetHour(),date.GetMinute(),date.GetSecond());


			//写入表格(起始)
			cell = wordTable.Cell(i+2, 4);cell.Select();m_wdSel.TypeText(datestr);
			cell = wordTable.Cell(i+2, 7);cell.Select();m_wdSel.TypeText(longitudestr);
			cell = wordTable.Cell(i+2, 8);cell.Select();m_wdSel.TypeText(latitudestr);
			distancestr.Format(_T("%.1f"),distance);
			cell = wordTable.Cell(i+2, 9);cell.Select();m_wdSel.TypeText(distancestr);
			//cell = wordTable.Cell(i+2, 10);cell.Select();m_wdSel.TypeText(GetFileNameByPath(filenamevector[i],true));//测线文件名
		}
		fp[i].Close();
		//总航程
		SumDistance+=distance;
	}
	//最后一行
	CCell wordCell = wordTable.Cell(nRowSize, 1);		//起始单元格
	wordRange = wordCell.get_Range();			//起始单元格区域
	int iStart = wordRange.get_Start();			//起始单元格索引
	wordCell = wordTable.Cell(nRowSize, nColumnSize);			//末了单元格                
	wordRange = wordCell.get_Range();			//末了单元格区域
	int iEnd = wordRange.get_End();				//末了单元格索引
	wordRange.put_Start(iStart);				//获取待合并的单元格区域
	wordRange.put_End(iEnd);
	tblBorders=wordRange.get_Borders();
	tblBorders.put_InsideLineStyle(0);//内边框

	CString sumdistancestr;
	sumdistancestr.Format(_T("测线总长度（km）：%.1f"),SumDistance);
	MergeCell(wordTable,nRowSize,6,nRowSize,9,sumdistancestr);//总长度
	MergeCell(wordTable,nRowSize,4,nRowSize,5,_T(""));//
	MergeCell(wordTable,nRowSize,1,nRowSize,3,_T("整编人:")+m_WriteToWordDlg.m_person);//整编人
	//m_wdSel.EndOf(COleVariant((short)wdStory), COleVariant((short)wdMove)); // 结束表格编辑
	wordTable.put_AllowAutoFit(1);
	//wordTable.AutoFitBehavior(1);//根据内容自动调整
	//wordTable.AutoFitBehavior(2);//根据窗口自动调整
	////设置表格格式为与中心对齐
	wordCell = wordTable.Cell(1, 1);		//起始单元格
	wordRange = wordCell.get_Range();			//起始单元格区域
	iStart = wordRange.get_Start();			//起始单元格索引
	wordCell = wordTable.Cell(nRowSize-1, nColumnSize);			//末了单元格                
	wordRange = wordCell.get_Range();			//末了单元格区域
	iEnd = wordRange.get_End();				//末了单元格索引
	wordRange.put_Start(iStart);				//获取待合并的单元格区域
	wordRange.put_End(iEnd);
	CCells wordCells = wordRange.get_Cells();	//转换为单元格
	wordCells.put_VerticalAlignment(1);//布局：居中

	ShowWordApp();
	//释放资源
	//m_wdDocs.ReleaseDispatch();
	m_wdDoc.ReleaseDispatch();
	m_wdRange.ReleaseDispatch();
	m_wdSel.ReleaseDispatch();
	wordTable.ReleaseDispatch();
	m_wdDocs.ReleaseDispatch();
	m_wdApp.ReleaseDispatch();
    return ;  
}
//合并单元格
void C数据提取View::MergeCell(CTable0& wordTable,int row1,int col1,int row2,int col2,CString text)
{
	CRange wordRange;
	CCell wordCell = wordTable.Cell(row1, col1);		//起始单元格
	wordRange = wordCell.get_Range();			//起始单元格区域
	wordRange.put_Text(text);
	int iStart = wordRange.get_Start();			//起始单元格索引
	wordCell = wordTable.Cell(row2, col2);			//末了单元格                
	wordRange = wordCell.get_Range();			//末了单元格区域
	int iEnd = wordRange.get_End();				//末了单元格索引
	wordRange.put_Start(iStart);				//获取待合并的单元格区域
	wordRange.put_End(iEnd);
	CCells wordCells = wordRange.get_Cells();	//转换为单元格
	wordCells.Merge();							//合并
}

BOOL C数据提取View::CreateWordApp(void)
{
	COleException pe;  
    if (!m_wdApp.CreateDispatch(_T("Word.Application"), &pe))  
    {  
        AfxMessageBox("Application创建失败，请确保安装了word 2000或以上版本!", MB_OK|MB_ICONWARNING);  
        pe.ReportError();  
        throw &pe;  
        return FALSE;  
    }  
    return TRUE;  
}


BOOL C数据提取View::CreateWordDocuments(void)
{
	if (m_wdApp)
	{
		return false;
	}
	 if (FALSE == CreateWordApp())   
    {  
        return FALSE;  
    }  
    m_wdDocs.AttachDispatch(m_wdApp.get_Documents());  
    if (!m_wdDocs.m_lpDispatch)   
    {  
        AfxMessageBox("Documents创建失败!", MB_OK|MB_ICONWARNING);  
        return FALSE;  
    }  
    return TRUE;  
}


BOOL C数据提取View::CreateWordDocument(void)
{
	if (!m_wdDocs.m_lpDispatch)   
    {  
        AfxMessageBox("Documents为空!", MB_OK|MB_ICONWARNING);  
        return FALSE;  
    }  
  
    COleVariant varTrue(short(1),VT_BOOL),vOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);  
    CComVariant Template(_T(""));    //没有使用WORD的文档模板  
    CComVariant NewTemplate(false),DocumentType(0),Visible;  
  
    m_wdDocs.Add(&Template,&NewTemplate,&DocumentType,&Visible);      
  
    //得到document变量  
	m_wdDoc = m_wdApp.get_ActiveDocument();  
    if (!m_wdDoc.m_lpDispatch)   
    {  
        AfxMessageBox("Document获取失败!", MB_OK|MB_ICONWARNING);  
        return FALSE;  
    }  
    //得到selection变量  
	m_wdSel = m_wdApp.get_Selection();  
    if (!m_wdSel.m_lpDispatch)   
    {  
        AfxMessageBox("Select获取失败!", MB_OK|MB_ICONWARNING);  
        return FALSE;  
    }  
    //得到Range变量  
    m_wdRange = m_wdDoc.Range(vOptional,vOptional);  
    if(!m_wdRange.m_lpDispatch)  
    {  
        AfxMessageBox("Range获取失败!", MB_OK|MB_ICONWARNING);  
        return FALSE;  
    }  
  
    return TRUE;  
}


BOOL C数据提取View::CreateWord(void)
{
	 if (FALSE == CreateWordDocument())   
    {  
        return FALSE;  
    }  
    return true; 
}


void C数据提取View::ShowWordApp(void)
{
	m_wdApp.put_Visible(TRUE);	 
}


void C数据提取View::HideWordApp(void)
{
	m_wdApp.put_Visible(FALSE);  
}


int C数据提取View::WriteTextToWord(CString szText)
{
	m_wdSel.TypeText(szText);
	
	return 0;
}


void C数据提取View::CloseWordApp(void)
{
	COleVariant vTrue((short)TRUE),      
                vFalse((short)FALSE),  
                vOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);  
    m_wdDoc.Save();  
    m_wdApp.Quit(vFalse,    // SaveChanges.  
             vTrue,            // OriginalFormat.  
             vFalse            // RouteDocument.  
             );  
    //释放内存申请资源  
  
    m_wdRange.ReleaseDispatch();  
    m_wdSel.ReleaseDispatch();  
    m_wdDoc.ReleaseDispatch();  
    m_wdDocs.ReleaseDispatch();  
    m_wdApp.ReleaseDispatch(); 
}


void C数据提取View::OutPutData(CString filename,vector<OGDataStruct> gravdata)
{
	FILE *fpOut=NULL;
	if ((fpOut=fopen((_bstr_t)filename,"w"))==NULL)
	{
		AfxMessageBox(_T("打开")+filename+_T("失败"));
		return ;
	}
	//输出测线
	int Max=gravdata.size();
	int space=Max*m_ProgressSpace;
	m_ProgressBar->SetRange(0,Max);
	for (int i = 0; i <gravdata.size(); i++)
	{
		fprintf(fpOut,"$DAT\t%s   \t%s\t%8.2f\t%8.2f\t%11.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%13.8f\t%13.8f\t%8.3f\t%8.3f\n",
			(PCSTR)(_bstr_t)(PCTSTR)gravdata[i].datatimeStr,(PCSTR)(_bstr_t)(PCTSTR)gravdata[i].day,
			gravdata[i].gravity,gravdata[i].spinttension,
			gravdata[i].beamposition,gravdata[i].VCC,gravdata[i].AL,gravdata[i].AX,gravdata[i].VE,gravdata[i].AX2,gravdata[i].XACC2,
			gravdata[i].LACC2,gravdata[i].CROSSACCEL,gravdata[i].LONGACCEL,gravdata[i].EOTVOS,gravdata[i].LONGITUDE,gravdata[i].LATITUDE,
			gravdata[i].VELOCITY,gravdata[i].Heading);
		if ((i)%space==0)
		{
			m_ProgressBar->SetPos(i);
		}
	}
	fclose(fpOut);
	m_ProgressBar->SetPos(m_ProgressBar->GetRangeMin());
}


int C数据提取View::CheckUse(void)
{
	//*************************按使用天数限制***********************
	int UserDays=90;//限制使用天数
	COleDateTime date,currentdate;
	CTime tm; tm=CTime::GetCurrentTime();//获取系统当前日期和时间
	currentdate.ParseDateTime(tm.Format(_T("%Y-%m-%d")));
	CString datestr;
	char datechar[20];
	int useday;

	//获取系统盘符
	//************************************文件路径******************************************************************************
	char filename[300];
	//char path[MAX_PATH];
	//GetSystemDirectory(path,MAX_PATH);		//系统路径
	////GetCurrentDirectory(MAX_PATH,path);			//程序当前路径
	//CString pathstr=(PCTSTR)(_bstr_t)path;
	//
	//sprintf(filename,"%s\\MGDPS.info",pathstr.Left(2));/*CString str;str=(PCTSTR)(_bstr_t)filename;MessageBox(str);*/

	CHAR szPath[MAX_PATH] = {0};   
	SHGetSpecialFolderPath(NULL, szPath,CSIDL_PERSONAL, FALSE);    
	//MessageBox(szPath); 
	sprintf(filename,"%s\\MGDPS.info",szPath);
	//**********************************************************************************************************************
	FILE* fp;
	if (!PathFileExists(filename))
	{
		MessageBox(_T("您是首次使用，欢迎您使用MGDPS V2.1"));
		if ((fp=fopen(filename,"w"))==NULL)
		{
			//MessageBox(_T("打开失败"));
			return 0;
		}
		fprintf(fp,"%d-%d-%d",tm.GetYear(),tm.GetMonth(),tm.GetDay());
	}else
	{	//MessageBox(_T("修改使用次数"));
		if ((fp=fopen(filename,"r"))==NULL)
		{
			return 0;
		}
		fscanf(fp,"%s",datechar);
		datestr=(PCTSTR)(_bstr_t)datechar;
		date.ParseDateTime(datestr);//计算差值
		useday=(currentdate-date).GetDays();
		//显示在状态栏里
		CString strTitlePane1;strTitlePane1.Format(_T("已使用%d天"),useday);
		CMainFrame* pMainFrm=(CMainFrame*)AfxGetApp()->GetMainWnd();
		CMFCRibbonLabel *pStatus1 = (CMFCRibbonLabel*)pMainFrm->m_wndStatusBar.GetElement(0);
		pStatus1->SetText(strTitlePane1);
		pMainFrm->m_wndStatusBar.RedrawWindow();
		if (useday > UserDays)
		{
			//MessageBox(_T("软件已经过期，请联系开发者\nzhikuiguo@live.cn"));
			//exit(0);
		}
	}
	if(fp)fclose(fp);
	//设置文件为隐藏
		CFileStatus fs;
		CFile::GetStatus(filename,fs);
		fs.m_attribute = CFile::hidden;           //set hidden attribute
		CFile::SetStatus(filename,fs);
	//********按次数限制*******************************************
	//int UserTimes=20;//软件限用次数
	//char *filename="C:\\Windows\\MGDPS.info";
	//int userday;
	//FILE* fp,*fp2;
	//if (!PathFileExists(filename))
	//{
	//	MessageBox(_T("您是首次使用，欢迎您使用MGDPS V2.1"));
	//	if ((fp=fopen(filename,"w"))==NULL)
	//	{
	//		return 0;
	//	}
	//	fprintf(fp,"%d",UserTimes);
	//}else
	//{	//MessageBox(_T("修改使用次数"));
	//	if ((fp=fopen(filename,"r"))==NULL)
	//	{
	//		return 0;
	//	}
	//	fscanf(fp,"%d",&userday);
	//	if (userday<=0)
	//	{
	//		MessageBox(_T("软件已经过期，请联系开发者"));
	//		exit(0);
	//	}else
	//	{
	//		fclose(fp);fp=NULL;
	//		if ((fp2=fopen(filename,"w"))==NULL)
	//		{
	//			return 0;
	//		}
	//		fprintf(fp2,"%d",userday-1);
	//	}
	//}
	//if(fp)fclose(fp);
	//if(fp2)fclose(fp2);
	//******************************************************************
	return 0;
}


int C数据提取View::UpdataProgress(int pos,UINT ribbonelementID)
{
	CMainFrame* pMainFrm=(CMainFrame*)AfxGetApp()->GetMainWnd();
	CMFCRibbonProgressBar* prog = DYNAMIC_DOWNCAST(CMFCRibbonProgressBar, pMainFrm->m_wndStatusBar.FindElement(ribbonelementID));
	prog->SetPos(pos);
	
	return 0;
}
int C数据提取View::SetProgressRange(int min, int max,UINT ribbonelementID)
{
	CMainFrame* pMainFrm=(CMainFrame*)AfxGetApp()->GetMainWnd();
	CMFCRibbonProgressBar* prog = DYNAMIC_DOWNCAST(CMFCRibbonProgressBar, pMainFrm->m_wndStatusBar.FindElement(ribbonelementID));
	prog->SetRange(min,max);
	return 0;
}

//检查（修正完的）GPS文件第一行是不是0:0:0
bool C数据提取View::InspectGPSFile(CString gpsfilename)
{
	GPSSTRUCT gpsdata;
	CString datestr,timestr;
	CString firstdatestr;
	char* datechar[20],*timechar[20];
	FILE *fp=NULL;
	if ((fp=fopen((_bstr_t)gpsfilename,"r"))==NULL)
	{
		AfxMessageBox(_T("打开")+gpsfilename+_T("失败"));
		return false;
	}
	//读取文件
	fscanf(fp,"%s%s%lf%lf%lf%lf",datechar,timechar,&gpsdata.longitude,&gpsdata.latitude,&gpsdata.velocity,&gpsdata.Heading);
	datestr.Format(_T("%s"),datechar);timestr.Format(_T(" %s"),timechar);
	gpsdata.datatimeStr=datestr+timestr;

	COleDateTime currenttime;
	currenttime.ParseDateTime(gpsdata.datatimeStr);
	if (currenttime.GetHour()==0 && currenttime.GetMinute()==0 && currenttime.GetSecond()==0)
	{
		return true;
	}
	fclose(fp);
	return false;
}


CString C数据提取View::GetFileNameByPath(CString pathname,bool isWithFileSuffix)
{
	 int pos = pathname.ReverseFind('\\');
    CString fileName = pathname.Right(pathname.GetLength() - pos - 1);
    if (FALSE == isWithFileSuffix)
    {
        pos = fileName.Find('.');
        fileName = fileName.Left(pos); 
    } 
    return fileName;
}
