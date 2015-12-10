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

// ������ȡView.cpp : C������ȡView ���ʵ��


#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "������ȡ.h"
#endif


#include "������ȡDoc.h"
#include "������ȡView.h"
#include "MainFrm.h"
#include "CubicForward.h"
#include "io.h"



//��ȡ�����̺߳���
UINT ReadGravDataTreadFunc(LPVOID ThreadParam);//��ȡ���������ļ��̺߳���
UINT MergeGravityFileThreadFunc(LPVOID ThreadParam);//��ȡ��������ļ��̺߳���
UINT UpdateFigureThrFunAfterRead(LPVOID ThreadParam);//��ȡ�������������Ժ�ĸ���ͼ��
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// C������ȡView

IMPLEMENT_DYNCREATE(C������ȡView, CView)

BEGIN_MESSAGE_MAP(C������ȡView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &C������ȡView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_BUTTON3, &C������ȡView::OnButton3)
	ON_COMMAND(ID_RemoveData, &C������ȡView::OnRemovedata)
	ON_COMMAND(ID_RemovePathOfData, &C������ȡView::OnRemovepathofdata)
	ON_COMMAND(ID_EDIT3, &C������ȡView::OnEdit3)
	ON_COMMAND(ID_Clear, &C������ȡView::OnClear)
	ON_COMMAND(ID_DisplayVelocity, &C������ȡView::OnDisplayvelocity)
	ON_COMMAND(ID_DisplayGoodData, &C������ȡView::OnDisplaygooddata)
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_SmoothVelocity, &C������ȡView::OnSmoothvelocity)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_COMMAND(ID_Start, &C������ȡView::OnStart)
	ON_COMMAND(ID_Stop, &C������ȡView::OnStop)
	ON_COMMAND(ID_SelectPointSlider, &C������ȡView::OnSelectpointslider)
	ON_COMMAND(ID_StartPoint, &C������ȡView::OnStartpoint)
	ON_COMMAND(ID_Back, &C������ȡView::OnBack)
	ON_COMMAND(ID_FinishPoint, &C������ȡView::OnFinishpoint)
	ON_COMMAND(ID_OutDataLine, &C������ȡView::OnOutdataline)
	ON_COMMAND(ID_MainFigure, &C������ȡView::OnMainfigure)
	ON_COMMAND(ID_Advance, &C������ȡView::OnAdvance)
	ON_COMMAND(ID_DisplayGravData, &C������ȡView::OnDisplaygravdata)
	ON_COMMAND(ID_SelectStartData, &C������ȡView::OnSelectstartdata)
	ON_COMMAND(ID_SelectEndDate, &C������ȡView::OnSelectenddate)
	ON_COMMAND(ID_SmoothGravity, &C������ȡView::OnSmoothgravity)
	ON_COMMAND(ID_BUTTON2, &C������ȡView::OnButton2)
	ON_COMMAND(ID_BGCorrect, &C������ȡView::OnBgcorrect)
	ON_COMMAND(ID_FILE_SAVE_AS, &C������ȡView::OnFileSaveAs)
	ON_COMMAND(ID_FILE_SAVE, &C������ȡView::OnFileSave)
	ON_COMMAND(ID_GetGPUProperty, &C������ȡView::OnGetgpuproperty)
	ON_COMMAND(ID_TimeCorrect, &C������ȡView::OnTimecorrect)
	ON_COMMAND(ID_OutPutFGA, &C������ȡView::OnOutputfga)
	ON_COMMAND(ID_ExtractGPS, &C������ȡView::OnExtractgps)
	ON_COMMAND(ID_CheckAmend, &C������ȡView::OnCheckamend)
	ON_COMMAND(ID_MatchGPS_GRAV, &C������ȡView::OnMatchgpsGrav)
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_CreateWordReport, &C������ȡView::OnCreatewordreport)
	//�Զ�����Ϣӳ��
	ON_MESSAGE(WM_PROGRESS_MESSAGE, OnUpdateProgress )
	ON_MESSAGE(WM_PROGRESS_Range, OnSetProgressRange )
	ON_MESSAGE(WM_UpdatePar, OnUpdatePar )
	ON_MESSAGE(WM_GetDistance, OnGetDistance )
	ON_MESSAGE(WM_UpdateFigure, OnUpdateFigure )
END_MESSAGE_MAP()

// C������ȡView ����/����

C������ȡView::C������ȡView()
	: m_DataPathName(_T(""))
	, m_LineWidth(1)
	, m_GridLineWidth(1)
	, m_LineTitle(_T("����ͼʾ"))
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
	// TODO: �ڴ˴���ӹ������
	m_BkColor=RGB(0,0,0);
	m_LineColor=RGB(255,255,0);
	m_RemoveDataLineColor=RGB(255,0,255);
	m_TextColor=RGB(100,100,100);
	m_GridColor=RGB(20,20,20);
	m_AxisColor=RGB(100,100,100);
	m_ZoomColor=RGB(0,0,255);
	m_StartPointColor=RGB(0,255,0);
	m_FinishPointColor=RGB(255,0,255);
	m_Font.CreatePointFont(180,_T("����"));
	
	m_ZoomRangeMoveSpeed=m_ZoomUpRange/4*3;//�ƶ��ٶȣ��ƶ�����С������Χ��ȵ��ķ�֮��(* )
	m_SelectedRect=CRect(CPoint(0,0),CPoint(0,0));
	CheckUse();
}

C������ȡView::~C������ȡView()
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

//��ȡ��������ļ��̺߳���
UINT MergeGravityFileThreadFunc(LPVOID ThreadParam)
{
	 C������ȡView * pView = (C������ȡView *)ThreadParam;
	 pView->m_Flag_ReadMultiGravThread=true;//���������ȡ�̱߳�ռ��
	 
	 int filenum=pView->m_FileNameVector.size();
	pView->m_ProgressMin=0;pView->m_ProgressMax=filenum+1;					//��������Χ����
	pView->SendMessage( WM_PROGRESS_Range, 0, IDR_MAINPROGRESSBAR );	//���½�������Χ
	
	 //1. ��ȡ����ļ�
	for (int i = 0; i < filenum; i++)
	{
		pView->m_CurrentProgressPos=i;
		pView->SendMessage( WM_PROGRESS_MESSAGE, 0, IDR_MAINPROGRESSBAR );	//���½�����״̬
		while (pView->m_Flag_ReadGravThread)//��û����
		{

		}
		pView->m_Flag_ReadGravThread=true;
		pView->m_CurrentDataFileName=pView->m_FileNameVector[i];
		AfxBeginThread((AFX_THREADPROC)ReadGravDataTreadFunc,pView);
	}
	pView->m_CurrentProgressPos=filenum;
	pView->SendMessage( WM_PROGRESS_MESSAGE, 0, IDR_MAINPROGRESSBAR );	//���½�����״̬
	while (pView->m_Flag_ReadGravThread)//��û����
	{

	}
	//2. ����ϲ��ļ�
	FILE *fpOut=NULL;
	if ((fpOut=fopen((_bstr_t)pView->m_OutPutFileName,"w"))==NULL)
	{
		AfxMessageBox(_T("��")+pView->m_OutPutFileName+_T("ʧ��"));
		pView->m_Flag_ReadMultiGravThread=false;//���������ȡ�߳�������
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
	pView->SendMessage( WM_PROGRESS_MESSAGE, 0, IDR_MAINPROGRESSBAR );	//���½�����״̬
	//3. ����ϲ���־
	FILE *fplog=NULL;
	if ((fplog=fopen((_bstr_t)pView->m_LogFileName,"w"))==NULL)
	{
		AfxMessageBox(_T("��")+pView->m_LogFileName+_T("ʧ��"));
		pView->m_Flag_ReadMultiGravThread=true;//���������ȡ�߳�������
		return 0;
	} 
	for (int i = 0; i < filenum; i++)
	{
		fprintf(fplog,"%s\n",(PCSTR)(_bstr_t)(PCTSTR)pView->m_FileNameVector[i]/*.Right(m_DataFileNameVector[i].GetLength()-m_DataPathName.GetLength())*/);
	}
	fclose(fplog);
	
	CString str;str.Format(_T("��������%d���ļ�"),filenum);AfxMessageBox(str);

	pView->m_Flag_ReadMultiGravThread=true;//���������ȡ�߳�������
	pView->m_CurrentProgressPos=0;
	pView->SendMessage( WM_PROGRESS_MESSAGE, 0, IDR_MAINPROGRESSBAR );	//���½�����״̬
	pView->SendMessage( WM_PROGRESS_MESSAGE, 0, IDR_CHILDROGRESSBAR );	//���½�����״̬
	return 0;
}
//���ý�������ǰλ��
LRESULT C������ȡView::OnUpdateProgress( WPARAM wParam, LPARAM lParam )
{
	int ribbonelementID = lParam;
	CMainFrame* pMainFrm=(CMainFrame*)AfxGetApp()->GetMainWnd();
	CMFCRibbonProgressBar* prog = DYNAMIC_DOWNCAST(CMFCRibbonProgressBar, pMainFrm->m_wndStatusBar.FindElement(ribbonelementID));
	prog->SetPos(m_CurrentProgressPos);
	return NULL;
}
//���ý�������Χ
LRESULT C������ȡView::OnSetProgressRange( WPARAM wParam, LPARAM lParam )
{
	int ribbonelementID = lParam;
	CMainFrame* pMainFrm=(CMainFrame*)AfxGetApp()->GetMainWnd();
	CMFCRibbonProgressBar* prog = DYNAMIC_DOWNCAST(CMFCRibbonProgressBar, pMainFrm->m_wndStatusBar.FindElement(ribbonelementID));
	prog->SetRange(m_ProgressMin,m_ProgressMax);
	return NULL;
}
//���ø��»�ͼ����
LRESULT C������ȡView::OnUpdatePar( WPARAM wParam, LPARAM lParam )
{
	UpdateParameter();
	return NULL;
}
//��ȡ���߳���
LRESULT C������ȡView::OnGetDistance( WPARAM wParam, LPARAM lParam )
{
	GetDistance();
	return NULL;
}
//��ȡ���߳���
LRESULT C������ȡView::OnUpdateFigure( WPARAM wParam, LPARAM lParam )
{
	UpdateMainPlot();
	return NULL;
}

/*DWORD WINAPI C������ȡView::*/
UINT ReadGravDataTreadFunc(LPVOID ThreadParam)
{
	//HWND hWnd=(HWND)ThreadParam;
    C������ȡView * pView = (C������ȡView *)ThreadParam;
	pView->m_Flag_ReadGravThread=true;//������ȡ�̱߳�־��ʾ��ռ��
	//����S�����������˵���飨Micro-g LaCoste Air-Sea Gravity Meter �CSystemII ����73ҳ���ݸ���˵����ÿ����Ŀ����ʼλ�ú��п�
	//��ȡID
	CStdioFile fp;
	if (fp.Open(pView->m_CurrentDataFileName,CFile::modeRead)==FALSE)
	{
		AfxMessageBox(_T("��")+pView->m_CurrentDataFileName+_T("ʧ��"));
		pView->m_Flag_ReadGravThread=false;//������ȡ�߳�������
		return false;
	}
	
	long progressspace=fp.GetLength()/1990;//��׼�����ļ�ÿ����199���ֽڣ�Ȼ�������10�ȷ֣���ʾ������
	pView->m_ProgressMin=0;pView->m_ProgressMax=10;					//��������Χ����
	pView->SendMessage( WM_PROGRESS_Range, 0, IDR_CHILDROGRESSBAR );	//���½�������Χ
	pView->SendMessage( WM_PROGRESS_MESSAGE, 0, IDR_CHILDROGRESSBAR );	//���½�����״̬

	CString LineStr,IDSTR;
	OGDataStruct OceanGravityData;
	CString datestr,timestr;
	long linenum=0;
	char IDCHAR[10],DATECHAR[20],TIMECHAR[20],DAYCHAR[20];
	fp.ReadString(LineStr);
	IDSTR=LineStr.Left(4);
	if (IDSTR==_T("$DAT"))		//ԭʼ����
	{
		fp.SeekToBegin();
		if (LineStr.GetLength()==197)		//����׼��ʽ��ԭʼ���ݸ�ʽ���߸Ľ��汾��д���ʽ
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
					//UpdataProgress(linenum/progressspace,IDR_CHILDROGRESSBAR);//ˢ�½�����
					pView->m_CurrentProgressPos=linenum/progressspace;
					pView->SendMessage( WM_PROGRESS_MESSAGE, 0, IDR_CHILDROGRESSBAR );
				}
				
			}
			pView->m_DataInfo=datestr;
			pView->m_DataInfo=_T("����:")+pView->m_DataInfo;
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
					//UpdataProgress(linenum/progressspace,IDR_CHILDROGRESSBAR);//ˢ�½�����
					pView->m_CurrentProgressPos=linenum/progressspace;
					pView->SendMessage( WM_PROGRESS_MESSAGE, 0, IDR_CHILDROGRESSBAR );
				}
			}
			pView->m_DataInfo.Format(_T("%s"),DATECHAR);
			pView->m_DataInfo=_T("����:")+pView->m_DataInfo;
		}
		
		
	}
	else
	{
		AfxMessageBox(_T("�ļ���ʽ����ȷ,�������������н����ַ���ʽ����"));
		//pView->m_Flag_ReadGravThread=false;//������ȡ�߳�������
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
				//UpdataProgress(linenum/progressspace,IDR_CHILDROGRESSBAR);//ˢ�½�����
				pView->m_CurrentProgressPos = linenum / progressspace;
				pView->SendMessage(WM_PROGRESS_MESSAGE, 0, IDR_CHILDROGRESSBAR);
			}
		}
		pView->m_DataInfo.Format(_T("XXXXX"));
		AfxMessageBox(_T("��ȡ���"));
	}

	//�ر��ļ�
	fp.Close();
	
	pView->m_Flag_ReadGravThread=false;//������ȡ�߳�������
	return 0;
}
UINT UpdateFigureThrFunAfterRead(LPVOID ThreadParam)
{
	 C������ȡView * pView = (C������ȡView *)ThreadParam;
	 while (pView->m_Flag_ReadGravThread)//��ȡ�ļ��߳�û�н�����ִ��while��ѭ��
	 {

	 }
	pView->m_CurrentProgressPos=2;
	pView->SendMessage( WM_PROGRESS_MESSAGE, 0, IDR_CHILDROGRESSBAR );

	//ˢ����ͼ
	pView->SendMessage( WM_UpdatePar, 0, 0 );//���²���
	//1.1 ���㺽������
	pView->SendMessage( WM_GetDistance, 0, 0 );	
	pView->m_CurrentProgressPos=3;
	pView->SendMessage( WM_PROGRESS_MESSAGE, 0, IDR_CHILDROGRESSBAR );
	
	//1.2 ���û�ͼ����
	pView->SendMessage( WM_UpdateFigure, 0, 0 );	
	pView->m_CurrentProgressPos=4;
	pView->SendMessage( WM_PROGRESS_MESSAGE, 0, IDR_CHILDROGRESSBAR );
	
	pView->m_CurrentProgressPos=0;
	pView->SendMessage( WM_PROGRESS_MESSAGE, 0, IDR_CHILDROGRESSBAR );

	return 0;
}
BOOL C������ȡView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// C������ȡView ����

void C������ȡView::OnDraw(CDC* pDC)
{
	C������ȡDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
	CRect rc;GetClientRect(&rc);
	pDC->BitBlt(rc.left,rc.top,rc.Width(),rc.Height(),&m_BitMapDC,0,0,SRCCOPY);

}


// C������ȡView ��ӡ


void C������ȡView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL C������ȡView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void C������ȡView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void C������ȡView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void C������ȡView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void C������ȡView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// C������ȡView ���

#ifdef _DEBUG
void C������ȡView::AssertValid() const
{
	CView::AssertValid();
}

void C������ȡView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

C������ȡDoc* C������ȡView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(C������ȡDoc)));
	return (C������ȡDoc*)m_pDocument;
}
#endif //_DEBUG


// C������ȡView ��Ϣ�������


int CALLBACK BrowseCallBackFun3(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)  
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
void C������ȡView::OnButton3()
{
	//��·���л�ȡ�ļ���
	if (m_Flag_ReadGravThread || m_Flag_ReadMultiGravThread)
	{
		MessageBox(_T("��ȡ���������߳�����ʹ�ã��ȴ����"));
		return;
	}

	if (m_MergeSurveyLineDlg.DoModal()!=IDOK)
	{
		return;
	}
	vector<CString>filenamevector;
	int filenum=GetFileCount(m_MergeSurveyLineDlg.m_PathName,m_MergeSurveyLineDlg.m_ExtenFileName,filenamevector);

	//���˵��ô�����
	m_FileNameVector=filenamevector;//�ļ�������
	m_OutPutFileName=m_MergeSurveyLineDlg.m_PathName+_T("\\���ߺϲ�.dat");
	m_LogFileName=m_MergeSurveyLineDlg.m_PathName+_T("\\�ϲ���־.log");

	//���úϲ������̺߳���
	AfxBeginThread((AFX_THREADPROC)MergeGravityFileThreadFunc,this);

	/*SetProgressRange(0,3,IDR_MAINPROGRESSBAR);UpdataProgress(1,IDR_MAINPROGRESSBAR);
	SetProgressRange(0,filenum,IDR_CHILDROGRESSBAR);
	for (int i = 0; i < filenum; i++)
	{
		ReadGravData(filenamevector[i]);
		UpdataProgress(i,IDR_CHILDROGRESSBAR);
	}*/

	//����ϲ��ļ�
	//OnFileSave();
	//UpdataProgress(2,IDR_MAINPROGRESSBAR);
	//CString outputfilename=m_MergeSurveyLineDlg.m_PathName+_T("\\���ߺϲ�.dat");
	//OutPutData(m_OutPutFileName,m_OGDataVector);
	////����ϲ���־
	//CString logfilename=m_MergeSurveyLineDlg.m_PathName+_T("\\�ϲ���־.log");
	//FILE *fpOut=NULL;
	//if ((fpOut=fopen((_bstr_t)m_LogFileName,"w"))==NULL)
	//{
	//	AfxMessageBox(_T("��")+m_LogFileName+_T("ʧ��"));
	//	return ;
	//} 
	//UpdataProgress(3,IDR_MAINPROGRESSBAR);
	//for (int i = 0; i < filenum; i++)
	//{
	//	fprintf(fpOut,"%s\n",(PCSTR)(_bstr_t)(PCTSTR)filenamevector[i]/*.Right(m_DataFileNameVector[i].GetLength()-m_DataPathName.GetLength())*/);
	//}
	//fclose(fpOut);
	//UpdataProgress(4,IDR_MAINPROGRESSBAR);UpdataProgress(0,IDR_MAINPROGRESSBAR);
	//CString str;str.Format(_T("��������%d���ļ�"),filenum);MessageBox(str);

}

int C������ȡView::GetFileCount(CString PathName,CString ExtenName, vector<CString>& filenamevector)
{
	//������������
	DataFileNumber=0;
	CFileFind finder;
	//·��
	CString strWildcard(PathName); 
	strWildcard += _T("//*.*");

// ��ʼ���ҵ�һ���ļ�
	BOOL bWorking = finder.FindFile(strWildcard);
	filenamevector.clear();//ʹ������ǰ�����
	//�ж�ѭ����ֱ����һ��������
	while (bWorking) 
	{ 
		bWorking = finder.FindNextFile();

		if (finder.IsDots()) //�ж��Ƿ����ļ� 
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
int C������ȡView::GetFileCount(CString PathName)
{
	//������������
	DataFileNumber=0;
	CFileFind finder;
	//·��
	CString strWildcard(PathName); 
	strWildcard += _T("//*.*");

// ��ʼ���ҵ�һ���ļ�
	BOOL bWorking = finder.FindFile(strWildcard);
	m_DataFileNameVector.clear();//ʹ������ǰ�����
	//�ж�ѭ����ֱ����һ��������
	while (bWorking) 
	{ 
		bWorking = finder.FindNextFile();

		if (finder.IsDots()) //�ж��Ƿ����ļ� 
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
		//if (finder.IsDirectory()) //�ж��Ƿ����ļ��� //·�� 
		//{ 
		//	CString str = finder.GetFilePath(); 
		//	//���������·���µ��ļ����������Լ������øú���
		//	//GetFileCount(str);
		//}
	}
	finder.Close(); 

	return DataFileNumber;
}


void C������ȡView::CollectData(CString DataFileName)
{
	CString NewDataFileName;
	NewDataFileName=DataFileName.Left(DataFileName.GetLength()-4)+_T("-�����.DAT");
	//AfxMessageBox(NewDataFileName);
	//���ļ�
	FILE *fp=NULL,*fpout=NULL;
	if ((fp=fopen((_bstr_t)DataFileName,"r"))==NULL)
	{
		AfxMessageBox(_T("��")+DataFileName+_T("ʧ��"));
		return ;
	}
	if ((fpout=fopen((_bstr_t)NewDataFileName,"w"))==NULL)
	{
		AfxMessageBox(_T("��")+NewDataFileName+_T("ʧ��"));
		return ;
	}

	//��ȡ�ļ�
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
		//�����������ȥ������
		/*AfxMessageBox(tempstr);
		AfxMessageBox(IDStr);
		AfxMessageBox(DATEStr);
		AfxMessageBox(TimeStr);
		AfxMessageBox(YearStr);
		AfxMessageBox(GravStr);
		AfxMessageBox(SpringTensitonStr);*/
		//д�ļ�
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


void C������ȡView::SelectLanLonGrav(CString DataFileName)
{
	CString NewDataFileName;
	NewDataFileName=DataFileName.Left(DataFileName.GetLength()-4)+_T("-��ȡ����-��γ��-�ٶ�.DAT");
	//AfxMessageBox(NewDataFileName);
	//���ļ�
	FILE *fp=NULL,*fpout=NULL;
	if ((fp=fopen((_bstr_t)DataFileName,"r"))==NULL)
	{
		AfxMessageBox(_T("��")+DataFileName+_T("ʧ��"));
		return ;
	}
	if ((fpout=fopen((_bstr_t)NewDataFileName,"w"))==NULL)
	{
		AfxMessageBox(_T("��")+NewDataFileName+_T("ʧ��"));
		return ;
	}

	//��ȡ�ļ�
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
		//�����������ȥ������
		/*AfxMessageBox(tempstr);
		AfxMessageBox(IDStr);
		AfxMessageBox(DATEStr);
		AfxMessageBox(TimeStr);
		AfxMessageBox(YearStr);
		AfxMessageBox(GravStr);
		AfxMessageBox(SpringTensitonStr);*/
		//д�ļ�
		fprintf(fpout,"%.8f\t%.8f\t%s\t%.3f\n",Longtitude,Latitude,(PCSTR)(_bstr_t)(PCTSTR)GravStr,Velocity);
	}
	
	fclose(fp);fclose(fpout);
}


void C������ȡView::OnRemovedata()
{
	if (m_Flag_ReadGravThread || m_Flag_ReadMultiGravThread)
	{
		MessageBox(_T("��ȡ���������߳�����ʹ�ã��ȴ����"));
		return;
	}
	// TODO: �ڴ���������������
	//UpdateParameter();//���²���
	TCHAR szFilter[] = _T("������������ �ļ���*.DAT)|*.DAT|�����ļ�(*.*)|*.*||");
	CFileDialog FileDlg(TRUE, _T("DAT"),NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL);
	FileDlg.m_ofn.lpstrTitle=_T("�򿪺������������ļ�");

	/*TCHAR szBuf[MAX_PATH];
	ZeroMemory(szBuf,MAX_PATH);
	GetCurrentDirectory(MAX_PATH,szBuf);
	CString currentpath(szBuf);*/
	//���öԻ���ǰ·��
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
	
	if (m_CurrentDataFileName==DataFileName)	//�������ѡ��ͬһ���ļ������ǰ���������ݣ����»�������ļ��Ĵ�������������
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
	//�������̶߳�ȡ����,��ȡ���ݺ��������и���ͼ�����
	m_ReadGravDataThread=AfxBeginThread((AFX_THREADPROC)ReadGravDataTreadFunc,this);
	
	//�����ͼ�̣߳��ȴ���ȡ��ϸ�����ͼ
	AfxBeginThread((AFX_THREADPROC)UpdateFigureThrFunAfterRead,this);
	
	//ReadGravData(m_CurrentDataFileName);   UpdataProgress(2,IDR_MAINPROGRESSBAR);
	////UpdateParameter();//���²���
	////1.1 ���㺽������
	//GetDistance();		UpdataProgress(3,IDR_MAINPROGRESSBAR);
	////1.2 ���û�ͼ����
	//UpdateMainPlot();	UpdataProgress(4,IDR_MAINPROGRESSBAR);
	//UpdataProgress(0,IDR_MAINPROGRESSBAR);
}


void C������ȡView::OnRemovepathofdata()
{
	// TODO: �ڴ���������������
	TCHAR pszPath[MAX_PATH];  
    BROWSEINFO bi;   
    bi.hwndOwner      = this->GetSafeHwnd();  
    bi.pidlRoot       = NULL;  
    bi.pszDisplayName = NULL;   
    bi.lpszTitle      = NULL;   
    bi.ulFlags        = BIF_USENEWUI;  
    bi.lpfn           = BrowseCallBackFun3;     //�ص�����  
    bi.lParam         =(LPARAM)(LPCTSTR)m_DataPathName;  //�����ص������Ĳ���,����Ĭ��·��  
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

	//��·���л�ȡ�ļ���
	int filenum=GetFileCount(m_DataPathName);
	for(int i=0;i<filenum;i++)
	{
		ReadDat(m_DataFileNameVector[i]);
	}
}


bool C������ȡView::ReadDat(CString DataFileName)
{

	//���ļ�
	//m_ProgressBar->SetPos(m_ProgressBar->GetRangeMax()*0.4);		//������
	FILE *fp=NULL/*,*fpout=NULL,*fpRemove=NULL*/;
	if ((fp=fopen((_bstr_t)DataFileName,"r"))==NULL)
	{
		//AfxMessageBox(_T("��")+DataFileName+_T("ʧ��"));
		//m_ProgressBar->SetPos(m_ProgressBar->GetRangeMin());
		return false;
	}
	//m_ProgressBar->SetPos(m_ProgressBar->GetRangeMax());		//������
	ClearPointIndex();
	//��ȡ�ļ�
	char symbol;
	//double /*Gravity,SpringTension,*/BeamPosition,VCC,AL,AX,VE,AX2,XACC2,LACC2,CROSS,LONGACCEL,Eotvos,Longtitude,Latitude,Heading,Velocity;
	OGDataStruct OceanGravityData;
	CString tempstr,IDStr,DATE_year_Str,MonthStr,DayStr,HourStr,MinuteStr,SecondStr,YearStr,GravStr,SpringTensitonStr;

	////��ȡ����֮ǰ���������
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
			//�ж��Ƿ�����Ч���ַ���
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
			OceanGravityData.DataTime.ParseDateTime(OceanGravityData.date+_T(" ")+OceanGravityData.time);						//��ʱ�������ַ���ת��Ϊʱ�����ͱ���
			OceanGravityData.datatimeStr=OceanGravityData.DataTime.Format("%Y-%m-%d\t%H:%M:%S");									//�����ַ���
			fscanf(fp,"%lf%c%lf%c%lf%c%lf%c%lf%c%lf%c%lf%c%lf%c%lf%c%lf%c%lf%c%lf%c%lf%c%lf%c%lf%c",&OceanGravityData.beamposition,&symbol,&OceanGravityData.VCC,&symbol,&OceanGravityData.AL,
				&symbol,&OceanGravityData.AX,&symbol,&OceanGravityData.VE,&symbol,&OceanGravityData.AX2,&symbol,&OceanGravityData.XACC2,&symbol,&OceanGravityData.LACC2,&symbol,
				&OceanGravityData.CROSSACCEL,&symbol,&OceanGravityData.LONGACCEL,&symbol,&OceanGravityData.EOTVOS,&symbol,&OceanGravityData.LONGITUDE,&symbol,&OceanGravityData.LATITUDE,
				&symbol,&OceanGravityData.VELOCITY,&symbol,&OceanGravityData.Heading,&symbol);
			fgetc(fp);//��ȡ�س���
		
			//����ʱ����ٶȵ�������
			//m_TimeVector.push_back(timenum);timenum++;
			//���澭γ�ȵ��ڴ�
			m_OGDataVector.push_back(OceanGravityData);
			
			//if (m_OGDataVector.size()%space==0)		//������
			//{
			//	m_ProgressBar->SetPos(m_OGDataVector.size());
			//}
		}
		//m_ProgressBar->SetPos(m_ProgressBar->GetRangeMin());
		//m_DataInfo=_T("����:")+DATE_year_Str+_T("��")+MonthStr+_T("��")+DayStr+_T("��   ");
		m_DataInfo=OceanGravityData.DataTime.Format("%Y-%m-%d");
		m_DataInfo=_T("����:")+m_DataInfo;
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
			//�ж��Ƿ�����Ч���ַ���
			if (tempstr.Left(1)!=_T("$"))
			{
				break;
			}
			OceanGravityData.day=tempstr.Right(3);tempstr=tempstr.Left(tempstr.GetLength()-4);
			OceanGravityData.time=tempstr.Right(8);tempstr=tempstr.Left(tempstr.GetLength()-9);
			OceanGravityData.date=tempstr.Right(10);//MessageBox(OceanGravityData.date);
			OceanGravityData.DataTime.ParseDateTime(OceanGravityData.date+_T(" ")+OceanGravityData.time);						//��ʱ�������ַ���ת��Ϊʱ�����ͱ���
			OceanGravityData.datatimeStr=OceanGravityData.DataTime.Format("%Y-%m-%d\t%H:%M:%S");									//�����ַ���
			fscanf(fp,"%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf",&OceanGravityData.gravity,&OceanGravityData.spinttension,&OceanGravityData.beamposition,&OceanGravityData.VCC,&OceanGravityData.AL,
				&OceanGravityData.AX,&OceanGravityData.VE,&OceanGravityData.AX2,&OceanGravityData.XACC2,&OceanGravityData.LACC2,
				&OceanGravityData.CROSSACCEL,&OceanGravityData.LONGACCEL,&OceanGravityData.EOTVOS,&OceanGravityData.LONGITUDE,&OceanGravityData.LATITUDE,
				&OceanGravityData.VELOCITY,&OceanGravityData.Heading,symbol);
			fgetc(fp);//��ȡ�س���
		
			//����ʱ����ٶȵ�������
			//m_TimeVector.push_back(timenum);timenum++;
			//���澭γ�ȵ��ڴ�
			m_OGDataVector.push_back(OceanGravityData);
			/*if (m_OGDataVector.size()%space==0)
			{
				m_ProgressBar->SetPos(m_OGDataVector.size());
			}*/
		}
		//m_ProgressBar->SetPos(m_ProgressBar->GetRangeMax());
		//m_ProgressBar->SetPos(m_ProgressBar->GetRangeMin());
		m_DataInfo=OceanGravityData.DataTime.Format("%Y-%m-%d");
		m_DataInfo=_T("����:")+m_DataInfo;
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

bool C������ȡView::ReadGravData(CString DataFileName)
{
	//��ֹ���濨��
     // OutputDebugString("gdsgsd");
	//����S�����������˵���飨Micro-g LaCoste Air-Sea Gravity Meter �CSystemII ����73ҳ���ݸ���˵����ÿ����Ŀ����ʼλ�ú��п�
	//��ȡID
	CStdioFile fp;
	if (fp.Open(DataFileName,CFile::modeRead)==FALSE)
	{
		//AfxMessageBox(_T("��")+DataFileName+_T("ʧ��"));
		return false;
	}
	int progressspace=86400/100;
	SetProgressRange(0,100,IDR_CHILDROGRESSBAR);//���ý�������Χ
	CString LineStr,IDSTR;
	OGDataStruct OceanGravityData;
	CString datestr,timestr;
	int linenum=0;
	char IDCHAR[10],DATECHAR[20],TIMECHAR[20],DAYCHAR[20];
	fp.ReadString(LineStr);
	IDSTR=LineStr.Left(4);
	if (IDSTR==_T("$DAT"))		//ԭʼ����
	{
		fp.SeekToBegin();
		if (LineStr.GetLength()==197)		//����׼��ʽ��ԭʼ���ݸ�ʽ���߸Ľ��汾��д���ʽ
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
					UpdataProgress(linenum/progressspace,IDR_CHILDROGRESSBAR);//ˢ�½�����
				}
				
			}
			m_DataInfo=datestr;
			m_DataInfo=_T("����:")+m_DataInfo;
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
					UpdataProgress(linenum/progressspace,IDR_CHILDROGRESSBAR);//ˢ�½�����
				}
			}
			m_DataInfo.Format(_T("%s"),DATECHAR);
			m_DataInfo=_T("����:")+m_DataInfo;
		}
		
		
	}
	else
	{
		MessageBox(_T("�ļ���ʽ����ȷ"));
		return false;
	}

	//�ر��ļ�
	fp.Close();
}
//���ƺ���ͼ
int C������ȡView::PlotLine(CRect rc,CString Title,CString Title2,vector<OGDataStruct>OGDataVector,vector<double>RemoveXVector,vector<double>RemoveYVector)
{
	if (OGDataVector.size()<2 && RemoveXVector.size()<2)
	{
		return 0;
	}
	//1.1 �����ֵ��Сֵ
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
	
	//��ͼ��λͼ��
	
	//***********************
//	CClientDC dc(this);
	CPen linePen(PS_SOLID,m_LineWidth,m_LineColor);
	CPen RemoveDataPen(PS_SOLID,m_LineWidth,m_RemoveDataLineColor);
	CPen GridPen(PS_SOLID,m_GridLineWidth,m_GridColor);
	CPen AxisPen(PS_SOLID,m_GridLineWidth,m_AxisColor);
	
	CFont* oldFont=(CFont*)m_BitMapDC.SelectObject(&m_Font);
	
	//���Ʊ߿�
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
	
	//��������
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
	//x����10���̶ȣ�y��ע5��
	m_XGridSize=m_BitMapDC.GetTextExtent(xminStr).cx;
	m_YGridSize=m_Rect.Height()/10;
	int xgridnum=m_Rect.Width()/m_XGridSize;
	int ygridnum=m_Rect.Height()/m_YGridSize;
	int x,y;
	int textx,texty;
	CString MarkStr;
	//m_BitMapDC.SetTextColor(RGB(0,0,0));				//�����������ע������ɫ
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
	for (int i = 1; i < xgridnum; i++)			//X��
	{
		x=m_Rect.left+i*m_XGridSize;y=m_Rect.bottom;
		m_BitMapDC.MoveTo(x,y);
		y-=XMarkLength;
		m_BitMapDC.LineTo(x,y);

		MarkStr.Format(XLabelFormat,xmin+i*m_Xbeishu*m_XGridSize);		//��ע����
		textx=x-m_BitMapDC.GetTextExtent(MarkStr).cx/2;
		texty=m_Rect.bottom;
		m_BitMapDC.TextOut(textx,texty,MarkStr);
	}
	for (int i = 1; i < ygridnum; i++)		//Y��
	{
		x=m_Rect.left;y=m_Rect.bottom-i*m_YGridSize;
		m_BitMapDC.MoveTo(x,y);
		x+=YMarkLength;
		m_BitMapDC.LineTo(x,y);

		MarkStr.Format(YLabelFormat,ymin+i*m_Ybeishu*m_YGridSize);		//��ע����
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
	//�������������
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
				if (!((rows >= m_Row_PointIndex || rows<0) || (cols>=m_Col_PointIndex || cols<0)))//��֤���鲻Խ�磬�����ж�
				{
					m_PointIndex[rows][cols].PointExit=true;
					m_PointIndex[rows][cols].IndexInOGdataVector=i;
				}
				
			}
		}
	}
	//���ƾֲ���Χ��־
	space=m_OGDataVector.size()*m_ProgressSpace;
	CPen ZoomRangePen(PS_SOLID,3,m_ZoomColor);
	m_BitMapDC.SelectObject(&ZoomRangePen);
	for (int i = m_CurrentZoomUpRangeLeft; i < m_CurrentZoomUpRangeLeft+m_ZoomUpRange; i++)
	{
		int x=m_Rect.left+(m_OGDataVector[i].LONGITUDE-xmin)/m_Xbeishu;
		int y=m_Rect.bottom-(m_OGDataVector[i].LATITUDE-ymin)/m_Ybeishu;
		m_BitMapDC.Ellipse(x,y,x+2,y+2);
	}
	//�����������յ�
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
	//���ݷ�Χ
	DataRangeStr.Format(_T("����:")+XLabelFormat+_T("  γ��:")+YLabelFormat+_T("  "),xmax-xmin,ymax-ymin);
	//ͼ�����
	CSize m_StrSize;							//�ַ�����С����ߣ�
	m_StrSize=m_BitMapDC.GetTextExtent(DataRangeStr+Title2);
	int textX=rc.left+rc.Width()/2-m_StrSize.cx/2;		//���߱���
	m_BitMapDC.TextOut(textX,rc.top,DataRangeStr+Title2);
	
	int textY=m_StrSize.cy+m_TextSpaceY;
	textX=m_Rect.left+(m_Rect.Width()-m_BitMapDC.GetTextExtent(Title).cx)/2;
	m_BitMapDC.TextOut(textX,textY,Title);
	//�ָ��豸
	m_BitMapDC.SelectObject(oldpen);
	m_BitMapDC.SelectObject(oldFont);
	return 0;
}


void C������ȡView::OnEdit3()
{
	// TODO: �ڴ���������������

}


void C������ȡView::OnClear()
{
	// TODO: �ڴ���������������
	if (m_Flag_ReadGravThread || m_Flag_ReadMultiGravThread)
	{
		MessageBox(_T("��ȡ���������߳�����ʹ�ã��ȴ����"));
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
	//�ֲ��Ŵ�����Ĳ���
	m_CurrentZoomUpRangeLeft=0;
	m_StartPointNumber=0;
	m_FinishPointNumber=0;
	CMainFrame* pMainFrm=(CMainFrame*)AfxGetApp()->GetMainWnd();
	CMFCRibbonSlider* PointNumber=DYNAMIC_DOWNCAST(CMFCRibbonSlider,pMainFrm->m_wndRibbonBar.FindByID(ID_SelectPointSlider));
	PointNumber->SetPos(0);
	m_CurrentSelectPointNumber=0;
	Invalidate(false);
}

//���ƾֲ��Ŵ�ͼ
int C������ȡView::PlotLine(CRect rc,CString Title,CString Title2,vector<double>xVector,vector<double>yVector)
{
	if (xVector.size()<2)
	{
		return 0;
	}
	//1.1 �����ֵ��Сֵ
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
	CSize m_StrSize;							//�ַ�����С����ߣ�
	m_StrSize=m_BitMapDC.GetTextExtent(Title2);
	int textX=rc.left+rc.Width()/2-m_StrSize.cx/2;		//���߱���
	m_BitMapDC.TextOut(textX,0,Title2);
	
	int textY=rc.top+m_StrSize.cy;
	textX=rc.left+(rc.Width()-m_BitMapDC.GetTextExtent(Title).cx)/2;
	m_BitMapDC.TextOut(textX,textY,Title);
	//���Ʊ߿�
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
	//��������
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
	//x����10���̶ȣ�y��ע5��
	m_XGridSize=m_BitMapDC.GetTextExtent(xminStr).cx;
	m_YGridSize=m_Rect.Height()/10;
	int xgridnum=m_Rect.Width()/m_XGridSize;
	int ygridnum=m_Rect.Height()/m_YGridSize;
	int x,y;
	int textx,texty;
	CString MarkStr;
	//m_BitMapDC.SetTextColor(RGB(0,0,0));				//�����������ע������ɫ
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
	for (int i = 1; i < xgridnum; i++)			//X��
	{
		x=m_Rect.left+i*m_XGridSize;y=m_Rect.bottom;
		m_BitMapDC.MoveTo(x,y);
		y-=XMarkLength;
		m_BitMapDC.LineTo(x,y);

		MarkStr.Format(XLabelFormat,xmin+i*m_Xbeishu*m_XGridSize);		//��ע����
		textx=x-m_BitMapDC.GetTextExtent(MarkStr).cx/2;
		texty=m_Rect.bottom;
		m_BitMapDC.TextOut(textx,texty,MarkStr);
	}
	for (int i = 1; i < ygridnum; i++)		//Y��
	{
		x=m_Rect.left;y=m_Rect.bottom-i*m_YGridSize;
		m_BitMapDC.MoveTo(x,y);
		x+=YMarkLength;
		m_BitMapDC.LineTo(x,y);

		MarkStr.Format(YLabelFormat,ymin+i*m_Ybeishu*m_YGridSize);		//��ע����
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
	//�������������
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
	//���ƾֲ��Ŵ�ͼ�е�ǰѡ���ĵ�
	CBrush SelectPointBrush(RGB(255,0,0));
	m_BitMapDC.SelectObject(&SelectPointBrush);
	x=m_Rect.left+(xVector[m_CurrentSelectPointNumber]-xmin)/m_Xbeishu;
	y=m_Rect.bottom-(yVector[m_CurrentSelectPointNumber]-ymin)/m_Ybeishu;
	m_BitMapDC.Ellipse(x-10,y-10,x+10,y+10);
	//�ָ��豸
	m_BitMapDC.SelectObject(oldpen);
	m_BitMapDC.SelectObject(oldFont);
	return 0;
}


int C������ȡView::PlotLine(CRect rc,CString Title,CString Title2,vector<double>yVector)
{
	if (yVector.size()<2)
	{
		return 0;
	}
	//1.1 �����ֵ��Сֵ
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
	CSize m_StrSize;							//�ַ�����С����ߣ�
	m_StrSize=m_BitMapDC.GetTextExtent(Title2);
	int textX=rc.Width()/2-m_StrSize.cx/2;		//���߱���
	//m_BitMapDC.TextOut(textX,rc.top,Title);
	
	int textY=rc.top+m_StrSize.cy+m_TextSpaceY;
	textX=rc.left+(rc.Width()-m_BitMapDC.GetTextExtent(Title).cx)/2;
	m_BitMapDC.TextOut(textX,textY,Title);
	//���Ʊ߿�
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
	//��������
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
	//x����10���̶ȣ�y��ע5��
	m_XGridSize=m_BitMapDC.GetTextExtent(xminStr).cx;
	m_YGridSize=m_Rect.Height()/10;
	int xgridnum=m_Rect.Width()/m_XGridSize;
	int ygridnum=m_Rect.Height()/m_YGridSize;
	int x,y;
	int textx,texty;
	CString MarkStr;
	//m_BitMapDC.SetTextColor(RGB(0,0,0));				//�����������ע������ɫ
	//����Ĭ�ϱ�עС��λ��
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
	for (int i = 1; i < xgridnum; i++)			//X��
	{
		x=m_Rect.left+i*m_XGridSize;y=m_Rect.bottom;
		m_BitMapDC.MoveTo(x,y);
		y-=XMarkLength;
		m_BitMapDC.LineTo(x,y);

		MarkStr.Format(XLabelFormat,xmin+i*m_Xbeishu*m_XGridSize);		//��ע����
		textx=x-m_BitMapDC.GetTextExtent(MarkStr).cx/2;
		texty=m_Rect.bottom;
		m_BitMapDC.TextOut(textx,texty,MarkStr);
	}
	for (int i = 1; i < ygridnum; i++)		//Y��
	{
		x=m_Rect.left;y=m_Rect.bottom-i*m_YGridSize;
		m_BitMapDC.MoveTo(x,y);
		x+=YMarkLength;
		m_BitMapDC.LineTo(x,y);

		MarkStr.Format(YLabelFormat,ymin+i*m_Ybeishu*m_YGridSize);		//��ע����
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
	//�������������
	if (yVector.size()!=0)
	{
		for (int i = 0; i < yVector.size()-1; i++)
		{
			int x=m_Rect.left+i/m_Xbeishu;
			int y=m_Rect.bottom-(yVector[i]-ymin)/m_Ybeishu;
			m_BitMapDC.Ellipse(x,y,x+2,y+2);
		}
	}
	//���ݷ�Χ
	//DataRangeStr.Format(_T("����:")+XLabelFormat+_T("  γ��:")+YLabelFormat+_T("  "),xmax-xmin,ymax-ymin);
	//�ָ��豸
	m_BitMapDC.SelectObject(oldpen);
	m_BitMapDC.SelectObject(oldFont);
	return 0;
}


void C������ȡView::OnDisplayvelocity()
{
	// TODO: �ڴ���������������
	//UpdateParameter();
	m_LineTitle=_T("�ٶ�ʱ��ͼ");
	InitBitMap();//��ʼ��λͼ
	CRect rc;GetClientRect(&rc);
	PlotLine(rc,m_LineTitle,m_DataInfo+m_CurrentDataFileName,m_VelocityVector);

	Invalidate();
}


void C������ȡView::OnDisplaygooddata()
{
	// TODO: �ڴ���������������
	m_LineTitle=_T("��Ч����");
	InitBitMap();//��ʼ��λͼ
	CRect rc;GetClientRect(&rc);
//	PlotLine(rc,m_LineTitle,m_DataInfo+m_CurrentDataFileName,m_xVector,m_yVector);
	Invalidate();
	
}


BOOL C������ȡView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CBrush backBrush(m_BkColor);

	//����ɵ�brush
	CBrush* pOldBrush = pDC->SelectObject(&backBrush);
	CRect rect;
	pDC->GetClipBox(&rect);

	//����Ҫ������
	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);

	pDC->SelectObject(pOldBrush);
	backBrush.DeleteObject();
	pOldBrush->DeleteObject();
	DeleteObject(&rect);
	return true;
	return CView::OnEraseBkgnd(pDC);
}


void C������ȡView::OnSmoothvelocity()
{
	// TODO: �ڴ���������������
	//���ٶȽ���1117��Բ������
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


void C������ȡView::PlotZoomUpLine(void)
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
	m_ZoomUpDataInfo=m_OGDataVector[index].time+_T("��");
	index=m_CurrentZoomUpRangeLeft+m_ZoomUpRange-1;
	m_ZoomUpDataInfo+=m_OGDataVector[index].time;
	CString LongLatStr;CString str;
	LongLatStr.Format(_T(" ����:%.6f γ��:%.6f"),m_ZoomUpXVector[m_CurrentSelectPointNumber],m_ZoomUpYVector[m_CurrentSelectPointNumber]);
	m_ZoomUpDataInfo+=LongLatStr;
	PlotLine(rc,_T("�ֲ�")+m_ZoomUpDataInfo,_T("  "),m_ZoomUpXVector,m_ZoomUpYVector);
}


int C������ȡView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pDC=new CClientDC(this);		//��ʼ��DC
		//����������
	//m_ProgressDlg=new CProgressDlg;
	//m_ProgressDlg->Create(IDD_ProgressDlg,this);
	//
	CMainFrame* pMainFrm=(CMainFrame*)AfxGetApp()->GetMainWnd();
	m_ProgressBar=DYNAMIC_DOWNCAST(CMFCRibbonProgressBar,pMainFrm->m_wndRibbonBar.FindByID(ID_PROGRESS1));
	return 0;
}

//��ʼ��λͼ
void C������ȡView::InitBitMap(void)
{
	CRect BitMapRect;GetClientRect(&BitMapRect);
	CBitmap memBitmap;
	m_BitMapDC.DeleteDC();//����֮ǰ�ͷ���ǰ��
	m_BitMapDC.CreateCompatibleDC(NULL);
	memBitmap.CreateCompatibleBitmap(m_pDC,BitMapRect.Width(),BitMapRect.Height());//����߽���
	m_BitMapDC.SelectObject(&memBitmap);
	CBrush newBrush;
	newBrush.CreateSolidBrush(m_BkColor);
	m_BitMapDC.SelectObject(newBrush);//����ɫ
	m_BitMapDC.FillRect(BitMapRect,&newBrush);
}


void C������ȡView::OnTimer(UINT_PTR nIDEvent)
{
	if (m_Flag_ReadGravThread || m_Flag_ReadMultiGravThread)
	{
		MessageBox(_T("��ȡ���������߳�����ʹ�ã��ȴ����"));
		return;
	}
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_CurrentZoomUpRangeLeft+=m_ZoomRangeMoveSpeed;	//ÿ��һ��ʱ���źţ��ֲ��Ŵ������±߽�����
	if (m_CurrentZoomUpRangeLeft+m_ZoomUpRange>m_OGDataVector.size())
	{
		KillTimer(1);
		m_CurrentZoomUpRangeLeft=0;
		return;
	}
	UpdateMainPlot();

	CView::OnTimer(nIDEvent);
}


void C������ȡView::OnStart()
{
	if (m_Flag_ReadGravThread || m_Flag_ReadMultiGravThread)
	{
		MessageBox(_T("��ȡ���������߳�����ʹ�ã��ȴ����"));
		return;
	}
	// TODO: �ڴ���������������
	//UpdateParameter();
	SetTimer(1,m_TimeSpace,NULL);
}


void C������ȡView::OnStop()
{
	// TODO: �ڴ���������������
	KillTimer(1);
}


int C������ȡView::PlotVelocity(CRect rc,CString Title,CString Title2,vector<OGDataStruct>OGDataVector,int currentNumber)
{
	if (OGDataVector.size()<2)
	{
		return 0;
	}
	//1.1 �����ֵ��Сֵ
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
	CSize m_StrSize;							//�ַ�����С����ߣ�
	m_StrSize=m_BitMapDC.GetTextExtent(Title2);
	int textX=rc.Width()/2-m_StrSize.cx/2;		//���߱���
	//m_BitMapDC.TextOut(textX,rc.top,Title);
	
	int textY=rc.top+m_StrSize.cy+m_TextSpaceY;
	textX=rc.left+(rc.Width()-m_BitMapDC.GetTextExtent(Title).cx)/2;
	m_BitMapDC.TextOut(textX,textY,Title);
	//���Ʊ߿�
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
	//��������
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
	//x����10���̶ȣ�y��ע5��
	m_XGridSize=m_BitMapDC.GetTextExtent(xminStr).cx;
	m_YGridSize=m_Rect.Height()/10;
	int xgridnum=m_Rect.Width()/m_XGridSize;
	int ygridnum=m_Rect.Height()/m_YGridSize;
	int x,y;
	int textx,texty;
	CString MarkStr;
	//m_BitMapDC.SetTextColor(RGB(0,0,0));				//�����������ע������ɫ
	//����Ĭ�ϱ�עС��λ��
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
	for (int i = 1; i < xgridnum; i++)			//X��
	{
		x=m_Rect.left+i*m_XGridSize;y=m_Rect.bottom;
		m_BitMapDC.MoveTo(x,y);
		y-=XMarkLength;
		m_BitMapDC.LineTo(x,y);

		MarkStr.Format(XLabelFormat,xmin+i*m_Xbeishu*m_XGridSize);		//��ע����
		textx=x-m_BitMapDC.GetTextExtent(MarkStr).cx/2;
		texty=m_Rect.bottom;
		m_BitMapDC.TextOut(textx,texty,MarkStr);
	}
	for (int i = 1; i < ygridnum; i++)		//Y��
	{
		x=m_Rect.left;y=m_Rect.bottom-i*m_YGridSize;
		m_BitMapDC.MoveTo(x,y);
		x+=YMarkLength;
		m_BitMapDC.LineTo(x,y);

		MarkStr.Format(YLabelFormat,ymin+i*m_Ybeishu*m_YGridSize);		//��ע����
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
	//�������������
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
	//���ݷ�Χ
	//DataRangeStr.Format(_T("����:")+XLabelFormat+_T("  γ��:")+YLabelFormat+_T("  "),xmax-xmin,ymax-ymin);
	//���Ƶ�ǰ�ֲ�λ��ָʾ��
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

	//�ָ��豸
	m_BitMapDC.SelectObject(oldpen);
	m_BitMapDC.SelectObject(oldFont);
	return 0;
}


void C������ȡView::OnSelectpointslider()
{
	// TODO: �ڴ���������������
	CMainFrame* pMainFrm=(CMainFrame*)AfxGetApp()->GetMainWnd();
	CMFCRibbonSlider* PointNumber=DYNAMIC_DOWNCAST(CMFCRibbonSlider,pMainFrm->m_wndRibbonBar.FindByID(ID_SelectPointSlider));
	m_CurrentSelectPointNumber=PointNumber->GetPos();
	UpdateMainPlot();
}


void C������ȡView::UpdateMainPlot(void)
{
	if (m_Flag_ReadGravThread || m_Flag_ReadMultiGravThread)
	{
		MessageBox(_T("��ȡ���������߳�����ʹ�ã��ȴ����"));
		return;
	}
	SetProgressRange(0,3,IDR_CHILDROGRESSBAR); UpdataProgress(1,IDR_CHILDROGRESSBAR);//������
	m_LineTitle=_T("����ͼʾ ")+m_Distance;
	InitBitMap();//��ʼ��λͼ
	CRect rc;GetClientRect(&rc);
	CString filename=_T("   ")+m_CurrentDataFileName.Mid(m_CurrentDataFileName.ReverseFind( '\\')+1,m_CurrentDataFileName.GetLength());
	PlotLine(rc,m_LineTitle,m_DataInfo+filename,m_OGDataVector,m_RemoveXVector,m_RemoveYVector);   UpdataProgress(2,IDR_CHILDROGRESSBAR);
	//UpdateSelectRectFigure(m_SelectedRect);
	PlotZoomUpLine();	UpdataProgress(3,IDR_CHILDROGRESSBAR);
	//�ٶ�ͼ
	m_LineTitle=_T("�ٶ�ʱ��ͼ");
	//InitBitMap();//��ʼ��λͼ
	CRect rc2;GetClientRect(&rc2);
	rc2.left=rc2.Width()/2;
	rc2.top=rc2.Height()/2;
	PlotVelocity(rc2,m_LineTitle,_T(""),m_OGDataVector,m_CurrentZoomUpRangeLeft);UpdataProgress(4,IDR_CHILDROGRESSBAR);
	//�������ѡ��ķ�������
	
	Invalidate();//�����Ƶ�λͼ���Ƶ���ͼ������
	UpdataProgress(0,IDR_CHILDROGRESSBAR);
}


void C������ȡView::OnStartpoint()
{
	if (m_Flag_ReadGravThread || m_Flag_ReadMultiGravThread)
	{
		MessageBox(_T("��ȡ���������߳�����ʹ�ã��ȴ����"));
		return;
	}
	// TODO: �ڴ���������������
	m_StartPointNumber=m_CurrentZoomUpRangeLeft+m_CurrentSelectPointNumber;
	m_FlagStartPoint=true;
	UpdateMainPlot();
}


void C������ȡView::OnBack()
{
	// TODO: �ڴ���������������
	//UpdateParameter();
	m_CurrentZoomUpRangeLeft-=m_ZoomRangeMoveSpeed;	//����

	if (m_CurrentZoomUpRangeLeft<=0)
	{
		m_CurrentZoomUpRangeLeft=m_OGDataVector.size()-m_ZoomUpRange;
	}
	UpdateMainPlot();

}


void C������ȡView::OnFinishpoint()
{
	// TODO: �ڴ���������������
	m_FinishPointNumber=m_CurrentZoomUpRangeLeft+m_CurrentSelectPointNumber;
	m_FlagFinishiPoint=true;
	UpdateMainPlot();
}

void C������ȡView::OnButton2()
{
	// TODO: �ڴ���������������
	if (m_DataRangeDlg.DoModal()!=IDOK)
	{
		return;
	}
	double xmin=m_DataRangeDlg.m_xmin,xmax=m_DataRangeDlg.m_xmax,ymin=m_DataRangeDlg.m_ymin,ymax=m_DataRangeDlg.m_ymax;
	

	CString OutFileName=m_CurrentDataFileName.Left(m_CurrentDataFileName.GetLength()-4)+_T("-����ѡ��.DAT");

	TCHAR szFilter[] = _T("���߰�����ѡ�� �ļ���*.DAT)|*.DAT|�����ļ�(*.*)|*.*||");
	CFileDialog FileDlg(FALSE, _T("DAT"),OutFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL);
	FileDlg.m_ofn.lpstrTitle=_T("���������ѡ������ļ�");
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
		AfxMessageBox(_T("��")+OutFileName+_T("ʧ��"));
		return ;
	}
	//�������
	//int progspace=m_OGDataVector.size()/100;
	//SetProgressRange(0,100,IDR_MAINPROGRESSBAR);//������
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
void C������ȡView::OnOutdataline()
{
	// TODO: �ڴ���������������
	if (!m_FlagStartPoint)
	{
		MessageBox(_T("��ѡ��������"));
		return;
	}
	if (!m_FlagFinishiPoint)
	{
		MessageBox(_T("��ѡ������յ�"));
		return;
	}
	if (m_StartPointNumber>=m_FinishPointNumber)
	{
		MessageBox(_T("��㲻�����յ�֮��"));
		return;
	}
	CString OutFileName=m_CurrentDataFileName.Left(m_CurrentDataFileName.GetLength()-4)+_T("-��ȡ����.DAT");

	TCHAR szFilter[] = _T("������ȡ �ļ���*.DAT)|*.DAT|�����ļ�(*.*)|*.*||");
	CFileDialog FileDlg(FALSE, _T("DAT"),OutFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL);
	FileDlg.m_ofn.lpstrTitle=_T("��������ļ�");
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
		AfxMessageBox(_T("��")+OutFileName+_T("ʧ��"));
		return ;
	}
	//�������
	int Max=m_FinishPointNumber-m_StartPointNumber;
	int space=Max*m_ProgressSpace;
	int progspace=(m_FinishPointNumber-m_StartPointNumber)/100;
	for (int i = m_StartPointNumber; i <= m_FinishPointNumber; i++)
	{
		if (m_OGDataVector[0].tempstr==_T(""))//�Ǳ�׼�ĺ���������ʽ
		{
			fprintf(fpOut, "$DAT\t%s   \t%s\t%8.2f\t%8.2f\t%11.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%13.8f\t%13.8f\t%8.3f\t%8.3f\n",
				(PCSTR)(_bstr_t)(PCTSTR)m_OGDataVector[i].datatimeStr, (PCSTR)(_bstr_t)(PCTSTR)m_OGDataVector[i].day,
				m_OGDataVector[i].gravity, m_OGDataVector[i].spinttension,
				m_OGDataVector[i].beamposition, m_OGDataVector[i].VCC, m_OGDataVector[i].AL, m_OGDataVector[i].AX, m_OGDataVector[i].VE, m_OGDataVector[i].AX2, m_OGDataVector[i].XACC2,
				m_OGDataVector[i].LACC2, m_OGDataVector[i].CROSSACCEL, m_OGDataVector[i].LONGACCEL, m_OGDataVector[i].EOTVOS, m_OGDataVector[i].LONGITUDE, m_OGDataVector[i].LATITUDE,
				m_OGDataVector[i].VELOCITY, m_OGDataVector[i].Heading);
		}
		else //һ������ݿ��ӻ���ȡ
		{
			fprintf(fpOut, "%s\n",(PCSTR)(_bstr_t)(PCTSTR)m_OGDataVector[i].tempstr);
		}
		
	}
	fclose(fpOut);
	//UpdataProgress(0,IDR_MAINFRAME);
}


void C������ȡView::OnMainfigure()
{
	// TODO: �ڴ���������������
	if (m_Flag_ReadGravThread || m_Flag_ReadMultiGravThread)
	{
		MessageBox(_T("��ȡ���������߳�����ʹ�ã��ȴ����"));
		return;
	}
	UpdateMainPlot();
}


void C������ȡView::OnAdvance()
{
	// TODO: �ڴ���������������
	//UpdateParameter();
	m_CurrentZoomUpRangeLeft+=m_ZoomRangeMoveSpeed;	//����
	if (m_CurrentZoomUpRangeLeft+m_ZoomUpRange>m_OGDataVector.size())
	{
		//MessageBox(_T("�Ѿ������ߵ��յ�"));
		m_CurrentZoomUpRangeLeft=0;
		//return;
	}
	
	UpdateMainPlot();
}


int C������ȡView::UpdateParameter(void)
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
	//���������С
	PropValue=pMainFrm->m_wndProperties.m_wndPropList.GetProperty(2)->GetSubItem(1)->GetValue();
	int fontsize=atoi((CString)PropValue);
	m_Font.DeleteObject();
	m_Font.CreatePointFont(fontsize,_T("����"));
	return 0;
}


int C������ȡView::ShowProgressBar(int CurrentPos, int MaxPos)
{
	if (m_ProgressDlg!=NULL)
	{
		m_ProgressDlg->SendMessage(WM_CLOSE);
	}
	//����������
	m_ProgressDlg=new CProgressDlg;
	m_ProgressDlg->Create(IDD_ProgressDlg,this);
	m_ProgressDlg->ShowWindow(SW_SHOW);
	CProgressCtrl *pProgCtrl=(CProgressCtrl*)m_ProgressDlg->GetDlgItem(IDC_PROGRESS1);
	pProgCtrl->SetBarColor(RGB(255,0,0));
	pProgCtrl->SetRange(0,MaxPos);
	pProgCtrl->SetPos(CurrentPos);
	return 0;
}


void C������ȡView::SelectLanLonGravToFile(CString DataFileName,FILE *fpout)
{
	/*CString NewDataFileName;
	NewDataFileName=DataFileName.Left(DataFileName.GetLength()-4)+_T("-��ȡ����-��γ��-�ٶ�.DAT");*/
	//AfxMessageBox(NewDataFileName);
	//���ļ�
	FILE *fp=NULL/*,*fpout=NULL*/;
	if ((fp=fopen((_bstr_t)DataFileName,"r"))==NULL)
	{
		AfxMessageBox(_T("��")+DataFileName+_T("ʧ��"));
		return ;
	}
	//if ((fpout=fopen((_bstr_t)NewDataFileName,"w"))==NULL)
	//{
	//	AfxMessageBox(_T("��")+NewDataFileName+_T("ʧ��"));
	//	return ;
	//}

	//��ȡ�ļ�
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
		//�����������ȥ������
		/*AfxMessageBox(tempstr);
		AfxMessageBox(IDStr);
		AfxMessageBox(DATEStr);
		AfxMessageBox(TimeStr);
		AfxMessageBox(YearStr);
		AfxMessageBox(GravStr);
		AfxMessageBox(SpringTensitonStr);*/
		//д�ļ�
		//fprintf(fpout,"%.8f\t%.8f\t%s\t%.3f\n",Longtitude,Latitude,(PCSTR)(_bstr_t)(PCTSTR)GravStr,Velocity);
		fprintf(fpout,"%.8f\t%.8f\n",Longtitude,Latitude);
	}
	
	fclose(fp);//fclose(fpout);
}


void C������ȡView::OnDisplaygravdata()
{
	// TODO: �ڴ���������������
	
	InitBitMap();//��ʼ��λͼ
	PlotGrav();
	Invalidate();
}


int C������ȡView::PlotGrav(void)
{
	if (m_OGDataVector.size()<2)
	{
		return 0;
	}
	CString Title2=_T("����ͼʾ");
	CString Title=m_CurrentDataFileName;
	CRect rc;
	GetClientRect(&rc);
	//1.1 �����ֵ��Сֵ
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
	CSize m_StrSize;							//�ַ�����С����ߣ�
	m_StrSize=m_BitMapDC.GetTextExtent(Title2);
	int textX=rc.Width()/2-m_StrSize.cx/2;		//���߱���
	//m_BitMapDC.TextOut(textX,rc.top,Title);
	
	int textY=rc.top+m_StrSize.cy+m_TextSpaceY;
	textX=rc.left+(rc.Width()-m_BitMapDC.GetTextExtent(Title).cx)/2;
	m_BitMapDC.TextOut(textX,textY,Title);
	//���Ʊ߿�
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
	//��������
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
	//x����10���̶ȣ�y��ע5��
	m_XGridSize=m_BitMapDC.GetTextExtent(xminStr).cx;
	m_YGridSize=m_Rect.Height()/10;
	int xgridnum=m_Rect.Width()/m_XGridSize;
	int ygridnum=m_Rect.Height()/m_YGridSize;
	int x,y;
	int textx,texty;
	CString MarkStr;
	//m_BitMapDC.SetTextColor(RGB(0,0,0));				//�����������ע������ɫ
	//����Ĭ�ϱ�עС��λ��
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
	for (int i = 1; i < xgridnum; i++)			//X��
	{
		x=m_Rect.left+i*m_XGridSize;y=m_Rect.bottom;
		m_BitMapDC.MoveTo(x,y);
		y-=XMarkLength;
		m_BitMapDC.LineTo(x,y);

		MarkStr.Format(XLabelFormat,xmin+i*m_Xbeishu*m_XGridSize);		//��ע����
		textx=x-m_BitMapDC.GetTextExtent(MarkStr).cx/2;
		texty=m_Rect.bottom;
		m_BitMapDC.TextOut(textx,texty,MarkStr);
	}
	for (int i = 1; i < ygridnum; i++)		//Y��
	{
		x=m_Rect.left;y=m_Rect.bottom-i*m_YGridSize;
		m_BitMapDC.MoveTo(x,y);
		x+=YMarkLength;
		m_BitMapDC.LineTo(x,y);

		MarkStr.Format(YLabelFormat,ymin+i*m_Ybeishu*m_YGridSize);		//��ע����
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
	//�������������
	if (m_OGDataVector.size()!=0)
	{
		for (int i = 0; i < m_OGDataVector.size()-1; i++)
		{
			int x=m_Rect.left+i/m_Xbeishu;
			int y=m_Rect.bottom-(m_OGDataVector[i].gravity-ymin)/m_Ybeishu;
			m_BitMapDC.Ellipse(x,y,x+2,y+2);
		}
	}
	//���ݷ�Χ
	//DataRangeStr.Format(_T("����:")+XLabelFormat+_T("  γ��:")+YLabelFormat+_T("  "),xmax-xmin,ymax-ymin);
	//�ָ��豸
	m_BitMapDC.SelectObject(oldpen);
	m_BitMapDC.SelectObject(oldFont);
	return 0;
}


int C������ȡView::GetDistance(void)
{
	//�ɴ��ټ��㺽��,��һ����ΪһС�Σ�΢Ԫ��
	const int timespace=60;		//һ��΢Ԫ�Ƕ�����
	double distance=0;
	int datanum=m_OGDataVector.size();
	int SegmentNum=datanum/timespace;			//΢Ԫ��
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
	//����ʣ��Ĳ���һ��΢Ԫ��һС��
	AverageVelocity=0;
	for (int i=SegmentNum*timespace ; i < datanum; i++)
	{
		AverageVelocity+=m_OGDataVector[i].VELOCITY;
	}
	AverageVelocity=AverageVelocity/timespace;
	distance+=AverageVelocity/60.0;

	m_Distance.Format(_T("����:%.2f km"),distance*1.852);
	return 0;
}


void C������ȡView::OnSelectstartdata()
{
	// TODO: �ڴ���������������
	if (m_OGDataVector.size()>0)
	{
		m_DateTimeDlg.m_RangeStr=_T("��Χ:")+m_OGDataVector[0].datatimeStr+_T(" �� ")+m_OGDataVector[m_OGDataVector.size()-1].datatimeStr;
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
					//MessageBox(_T("ƥ����ȷ"));
					m_StartPointNumber=i;
					m_FlagStartPoint=true;
					UpdateMainPlot();
					break;
				}else
				{
					//MessageBox(_T("���ڲ�ƥ��"));
				}
				
			}
		}
		if (i>=m_OGDataVector.size())
		{
			MessageBox(_T("δ�ҵ�ƥ����"));
		}
	}
	
}


void C������ȡView::OnSelectenddate()
{
	// TODO: �ڴ���������������
	if (m_OGDataVector.size()>0)
	{
		m_DateTimeDlg.m_RangeStr=_T("��Χ:")+m_OGDataVector[0].datatimeStr+_T(" �� ")+m_OGDataVector[m_OGDataVector.size()-1].datatimeStr;
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
					//MessageBox(_T("ƥ����ȷ"));
					m_FinishPointNumber=i;
					m_FlagFinishiPoint=true;
					UpdateMainPlot();
					break;
				}else
				{
					//MessageBox(_T("���ڲ�ƥ��"));
				}
				
			}
		}
		if (i>=m_OGDataVector.size())
		{
			MessageBox(_T("δ�ҵ�ƥ����"));
		}
	}
}


void C������ȡView::OnSmoothgravity()
{
	// TODO: �ڴ���������������
	//���ٶȽ���1117��Բ������
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
	
	InitBitMap();//��ʼ��λͼ
	PlotGrav();
	Invalidate();
}





void C������ȡView::OnBgcorrect()
{
	// TODO: �ڴ���������������

	//1.1 ˮ�����ݣ�grd��
//	m_StartTime=clock();
//	TCHAR szFilter[] = _T("ˮ������ �ļ���*.grd)|*.grd|�����ļ�(*.*)|*.*||");
//	CFileDialog FileDlg(TRUE, _T("DAT"),NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL);
//	FileDlg.m_ofn.lpstrTitle=_T("��ˮ������-���ڲ���У��");
//	//���öԻ���ǰ·��
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
//	//��ȡˮ������
//	GRDSTRUCT DemDataInfo;
//	FILE *fp=NULL/*,*fpout=NULL,*fpRemove=NULL*/;
//	if ((fp=fopen((_bstr_t)DataFileName,"r"))==NULL)
//	{
//		AfxMessageBox(_T("��")+DataFileName+_T("ʧ��"));
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
//	//1.2 ��ȡ�������Ĳ�������ļ�
//
//	TCHAR szFilter2[] = _T("������꣨*.dat)|*.dat|������꣨*.txt)|*.txt|�����ļ�(*.*)|*.*||");
//	CFileDialog FileDlg2(TRUE, _T("DAT"),NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter2, NULL);
//	FileDlg2.m_ofn.lpstrTitle=_T("�򿪲�������ļ���������꣩-���ڲ���У��");
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
//		AfxMessageBox(_T("��")+DataFileName+_T("ʧ��"));
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
//	//1.3 ���ݲ���������У����Χ
//	double dx=(DemDataInfo.xmax-DemDataInfo.xmin)/(DemDataInfo.cols-1);
//	double dy=(DemDataInfo.ymax-DemDataInfo.ymin)/(DemDataInfo.rows-1);
//	DemDataInfo.dx=dx,DemDataInfo.dy=dy;
//	double Distance_min=sqrt(dx*dx+dy*dy)/2.0;
//	double Distance,x0,y0;
//	int row0,col0;																		//���������������ڵ�����
//	double range=166000;																//������Χ��166km
//	int Xgrdnum_half=int(range/dx/2)+1,Ygrdnum_half=int(range/dy/2)+1;					//������Χ��x��y�����ϵ���������һ��
//	double rowmin,rowmax,colmin,colmax;
//	double *CorrectData=new double[x_vector.size()];									//���θ���ֵ����
//	GRDRANGEINDEX *IndexArray=new GRDRANGEINDEX[x_vector.size()];										//�����Χ����ǵ�����ֵ����
//	m_ProgressBar->SetRange(0,100);
//	int temp=x_vector.size()/100.0+1;
//	
//	for (int i = 0; i < x_vector.size(); i++)
//	{
//		x0=x_vector[i],y0=y_vector[i];													//�������
//		if (SearchIndex_ij(DemDataInfo,Distance_min,x0,y0,row0,col0))
//		{
//			//��������Χ������Χ�����ĸ��ǵ������
//			rowmin=((row0-Ygrdnum_half)>0?row0-Ygrdnum_half:0);
//			colmin=((col0-Xgrdnum_half)>0?col0-Xgrdnum_half:0);
//			rowmax=((row0+Ygrdnum_half)<DemDataInfo.rows?row0+Ygrdnum_half:DemDataInfo.rows);
//			colmax=((col0+Xgrdnum_half)<DemDataInfo.cols?col0+Xgrdnum_half:DemDataInfo.cols);
//			//���������Χ�ڵĵ������ݼ��㺯��
//			IndexArray[i].colmin=colmin;IndexArray[i].colmax=colmax;IndexArray[i].rowmax=rowmax;IndexArray[i].rowmin=rowmin;
//			IndexArray[i].x0=x0;IndexArray[i].y0=y0;
//			//CorrectData[i]=Correct_CubicForward(DemDataInfo,rowmin,rowmax,colmin,colmax,demdata,x0,y0);		//��i�����ĵ��θ���ֵ
//		}else
//		{
//			MessageBox(_T("��㲻��ˮ�����ݷ�Χ�ڣ���������˳�"));
//			return;
//		}
//		if (i%temp==0)
//		{
//			m_ProgressBar->SetPos(i/temp);
//		}
//	}
//	//GPU_Correct(CorrectData,x_vector.size(),demdata,IndexArray,DemDataInfo);								//����GPU�������õ��ظ�����CorrectData
//	//Correct_CubicForward(DemDataInfo,x_vector.size(),IndexArray,demdata,CorrectData);
//	m_FinishTime=clock();
////	CString str;str.Format(_T("��ʱ��%d ����    %d    %d"),m_FinishTime-m_StartTime,row0,col0);MessageBox(str);
//	//str.Format(_T("%lf    %lf"),CorrectData[0],CorrectData[100]);MessageBox(str);
//	
//	//1.4 ������θ���ֵ
//	CString OutFileName=DataFileName+_T("-���θ���.DAT");
//
//	TCHAR szFilter3[] = _T("���θ��� �ļ���*.DAT)|*.DAT|�����ļ�(*.*)|*.*||");
//	CFileDialog OutFileDlg(FALSE, _T("DAT"),OutFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter3, NULL);
//	OutFileDlg.m_ofn.lpstrTitle=_T("������θ�������");
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
//		AfxMessageBox(_T("��")+OutFileName+_T("ʧ��"));
//		return ;
//	}
//	for (int i = 0; i < x_vector.size(); i++)
//	{
//		fprintf(fpOut,"%lf\t%lf\t%lf\n",x_vector[i],y_vector[i],CorrectData[i]);
//	}
//	fclose(fpOut);
//	//�ͷ���Դ
//	delete CorrectData;

	//CString str;str.Format(_T("%lf    %lf"),dx,dy);MessageBox(str);
}


bool C������ȡView::SearchIndex_ij(GRDSTRUCT datainfo, double distance0,double x0, double y0, int& row, int& col)
{
	//ȫ������
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

	//ֱ���ж�
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

void  C������ȡView::Correct_CubicForward(GRDSTRUCT datainfo,int correctnumber, GRDRANGEINDEX* indexarray, double* demdata,double* CorrectData)
{
	//��������Χ�ظķ�Χ�ڵ�����
	//FILE *fp=NULL/*,*fpout=NULL,*fpRemove=NULL*/;
	//if ((fp=fopen("�ظķ�Χ.txt","w"))==NULL)
	//{
	//	AfxMessageBox(_T("�򿪵ظķ�Χ�ļ�ʧ��"));
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
		cubic_grav_info.z=-0.001;									//�۲����ڵ���
		//��ģ���������ֵ
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
		//1.1 �������ݺ���
		CorrectData[k]=Cubic_Vz_Forward(cubic_grav_info,indexarray[k].x0,indexarray[k].y0);
		if (k%temp==0)
		{
			m_ProgressBar->SetPos(k/temp);
		}
	}
	
}


void C������ȡView::OnFileSaveAs()
{
	// TODO: �ڴ���������������
	
}


void C������ȡView::OnFileSave()
{

	// TODO: �ڴ���������������
	CString OutFileName=m_CurrentDataFileName.Left(m_CurrentDataFileName.GetLength()-4)+_T("�������ݸ�ʽ��.DAT");

	TCHAR szFilter[] = _T("������ȡ �ļ���*.DAT)|*.DAT|�����ļ�(*.*)|*.*||");
	CFileDialog FileDlg(FALSE, _T("DAT"),OutFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL);
	FileDlg.m_ofn.lpstrTitle=_T("��������ļ�");
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
		AfxMessageBox(_T("��")+OutFileName+_T("ʧ��"));
		return ;
	}
	//�������
	int progspace=m_OGDataVector.size()/100;
	SetProgressRange(0,100,IDR_CHILDROGRESSBAR);//������
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


void C������ȡView::OnGetgpuproperty()
{
	// TODO: �ڴ���������������
//	GetGPUProperty();
}


void C������ȡView::OnTimecorrect()
{
	// TODO: �ڴ���������������
	if (m_TimeCorrectDlg.DoModal()!=IDOK)
	{
		return;
	}
	int timedelay=m_TimeCorrectDlg.m_TimeDelay;
	int filecount=GetFileCount(m_TimeCorrectDlg.m_DataFilePath);


	//����ϲ���־
	CString logfilename;
	TCHAR szFilter[] = _T("ʱ��У�� ��־��*.log)|*.log|�����ļ�(*.*)|*.*||");
	CFileDialog FileDlg(FALSE, _T("DAT"),"ʱ��У��", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL);
	FileDlg.m_ofn.lpstrTitle=_T("ʱ��У����־�ļ�");
	if (IDOK == FileDlg.DoModal())
	{   
		logfilename = FileDlg.GetPathName();  
		FILE *fpOut=NULL;
		if ((fpOut=fopen((_bstr_t)logfilename,"w"))==NULL)
		{
			AfxMessageBox(_T("��")+logfilename+_T("ʧ��"));
			return ;
		}
		for (int i = 0; i < filecount; i++)
		{
			fprintf(fpOut,"%s\n",(PCSTR)(_bstr_t)(PCTSTR)m_DataFileNameVector[i]/*.Right(m_DataFileNameVector[i].GetLength()-m_DataPathName.GetLength())*/);
		}
		fclose(fpOut);
	}
	
	CString str;str.Format(_T("��������%d���ļ�"),filecount);MessageBox(str);
}


void C������ȡView::OnOutputfga()
{
	// TODO: �ڴ���������������
	if (m_OGDataVector.size()==0)
	{
		MessageBox(_T("����ͼ����û�ж����κκ�����������\n�ȶ���ԭʼ����"));
		return;
	}
	//��ȡ��������
	CString BasePointFileName1,BasePointFileName2;
	TCHAR szFilter[] = _T("����ȶ� �ļ���*.inf)|*.inf|�����ļ�(*.*)|*.*||");
	CFileDialog FileDlg1(TRUE, _T("inf"),NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL);
	FileDlg1.m_ofn.lpstrTitle=_T("�򿪲���ǰ����ȶ������ļ�");

	if (IDOK == FileDlg1.DoModal())
	{   
		BasePointFileName1 = FileDlg1.GetPathName();  
	}
	else
	{
		return;
	}
	CFileDialog FileDlg2(TRUE, _T("inf"),NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL);
	FileDlg2.m_ofn.lpstrTitle=_T("�򿪲��������ȶ������ļ�");

	if (IDOK == FileDlg2.DoModal())
	{   
		BasePointFileName2 = FileDlg2.GetPathName();  
	}
	else
	{
		return;
	}
	CString OutFileName=m_CurrentDataFileName.Left(m_CurrentDataFileName.GetLength()-4)+_T("-���ɿռ������쳣.dat");
	TCHAR szFilter2[] = _T("���ɿռ������쳣 �ļ���*.dat)|*.dat|�����ļ�(*.*)|*.*||");
	CFileDialog OutputFileDlg(FALSE, _T("DAT"),OutFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter2, NULL);
	OutputFileDlg.m_ofn.lpstrTitle=_T("������ɿռ������쳣");
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
		AfxMessageBox(_T("��")+BasePointFileName1+_T("ʧ��"));
		return ;
	}
	if ((fp2=fopen((_bstr_t)BasePointFileName2,"r"))==NULL)
	{
		AfxMessageBox(_T("��")+BasePointFileName2+_T("ʧ��"));
		fclose(fp1);
		return ;
	}
	FILE *fpOut=NULL;
	if ((fpOut=fopen((_bstr_t)OutFileName,"w"))==NULL)//��������ɿռ������쳣�ļ�
	{
		AfxMessageBox(_T("��")+OutFileName+_T("ʧ��"));
		fclose(fp1);
		fclose(fp2);
		return ;
	}
	char tempchar[50];
	fscanf(fp1,"%s%lf%s%s%s%lf%lf%s%lf%lf%s%lf%s%lf%s%lf%s%lf%s%lf%s%lf",tempchar,&basedata1.grav,tempchar,basedata1.datetimechar,tempchar,
		&basedata1.longitude_degree,&basedata1.longitude_minute,tempchar,&basedata1.latitude_degree,&basedata1.latitude_minute,
		tempchar,&basedata1.reading,tempchar,&basedata1.Heitht_BaseToSea,tempchar,&basedata1.height_l,tempchar,&basedata1.height_r,tempchar,
		&basedata1.HeightToDeck,tempchar,&basedata1.DistanceToBasePoint);
	basedata1.longitude=basedata1.longitude_degree+basedata1.longitude_minute/60.0;			//��γ���ɶȷ�ת��Ϊ��
	basedata1.latitude=basedata1.latitude_degree+basedata1.latitude_minute/60.0;
	basedata1.datetime.ParseDateTime((PCTSTR)(_bstr_t)basedata1.datetimechar);			//�ȶ�����
	fscanf(fp2,"%s%lf%s%s%s%lf%lf%s%lf%lf%s%lf%s%lf%s%lf%s%lf%s%lf%s%lf",tempchar,&basedata2.grav,tempchar,basedata2.datetimechar,tempchar,
		&basedata2.longitude_degree,&basedata2.longitude_minute,tempchar,&basedata2.latitude_degree,&basedata2.latitude_minute,
		tempchar,&basedata2.reading,tempchar,&basedata2.Heitht_BaseToSea,tempchar,&basedata2.height_l,tempchar,&basedata2.height_r,tempchar,
		&basedata2.HeightToDeck,tempchar,&basedata2.DistanceToBasePoint);
	basedata2.longitude=basedata2.longitude_degree+basedata2.longitude_minute/60.0;			//��γ���ɶȷ�ת��Ϊ��
	basedata2.latitude=basedata2.latitude_degree+basedata2.latitude_minute/60.0;
	basedata2.datetime.ParseDateTime((PCTSTR)(_bstr_t)basedata2.datetimechar);		//�ȶ�����
	//CString str;str.Format(_T("%lf"),basedata2.DistanceToBasePoint);MessageBox(str);
	fclose(fp1);fclose(fp2);

	//2. ��������ȶ�
	double dgB=4.741636224*(0.01060488*sin(basedata1.latitude)*cos(basedata1.latitude)-0.0000234*sin(2.0*basedata1.latitude)*cos(2.0*basedata1.latitude))*basedata1.DistanceToBasePoint;
	basedata1.HeightToBasePoint=basedata1.Heitht_BaseToSea-(basedata1.height_l+basedata1.height_r)/2.0+basedata1.HeightToDeck;
	basedata1.reading_corrected=basedata1.reading-0.3086*basedata1.HeightToBasePoint;

	dgB=4.741636224*(0.01060488*sin(basedata2.latitude)*cos(basedata2.latitude)-0.0000234*sin(2.0*basedata2.latitude)*cos(2.0*basedata2.latitude))*basedata2.DistanceToBasePoint;
	basedata2.HeightToBasePoint=basedata2.Heitht_BaseToSea-(basedata2.height_l+basedata2.height_r)/2.0+basedata2.HeightToDeck;
	basedata2.reading_corrected=basedata2.reading-0.3086*basedata2.HeightToBasePoint;

	double K_gravmetor=0.998;//������ֵ
	double H=0.31;			//�����ǵ���ϵͳ��ƽ����ƽ��ĸ߶ȣ�m����ȫ������һ��ֵ������Ӧ��Ӧ�ô�GPS����������ȡ���ߵ���ƽ��߶�Ȼ��������ֵ�����������ֵ�����㲻ͬ�����
	double K_zeropoint=K_gravmetor*(basedata2.reading_corrected-basedata1.reading_corrected)/(basedata2.datetime.GetDay()-basedata1.datetime.GetDay());//ÿ���Ư����
	double K_draft=0.3086*(basedata2.HeightToBasePoint-basedata1.HeightToBasePoint)/(basedata2.datetime.GetDay()-basedata1.datetime.GetDay());//��ˮ����ϵ��
	double *dGravity=new double[m_OGDataVector.size()];		//�����ۼӸ��ָ���ֵ
	double dgravityTemp=0;
	double d_day;//�������𺽵�ʱ���죩
	//3. ���ɿռ������쳣����
	for (int i = 0; i < m_OGDataVector.size(); i++)
	{
		dgravityTemp=0;
		if (m_OGDataVector[i].DataTime < basedata1.datetime || m_OGDataVector[i].DataTime > basedata2.datetime)
		{
			CString errorinfo;errorinfo.Format(_T("������ڲ���ǰ�����ȶԷ�Χ��\n�����%d����������"),i+1);
			MessageBox(errorinfo);
			return;
		}
		d_day=m_OGDataVector[i].DataTime.GetDay()-basedata1.datetime.GetDay();
		//0. ��������Ķ�����
		//dgravityTemp+=K_gravmetor*(m_OGDataVector[i].gravity-basedata1.reading_corrected);//�������Ը�ֵ����������������ֵ֮��Ĳ�ֵ

		//1. ��Ư����
		//dgravityTemp+=K_zeropoint*(d_day);	//��Ư������

		//2. ��ˮ����
		//dgravityTemp+=K_draft*(d_day);		//��ˮ����ֵ

		//3. ���ظ�˹����
		dgravityTemp+=7.499*m_OGDataVector[i].VELOCITY*sin(m_OGDataVector[i].Heading)*cos(m_OGDataVector[i].LATITUDE)+0.004*pow(m_OGDataVector[i].VELOCITY,2.0);

		//4. ����������
		//dgravityTemp+=-978032.67714*(1+0.00193185138639*pow(sin(m_OGDataVector[i].Heading),2.0))/sqrt(1-0.00669437999013*pow(sin(m_OGDataVector[i].Heading),2.0));

		//5. ���ɿռ����
		//dgravityTemp+=0.3086*H;

		dGravity[i]=dgravityTemp;
	}
	//���ľ�������ֵ���ϸ������ֵ��Ϊ�����쳣
	//4. ���ɿռ������쳣���
	for (int i = 0; i < m_OGDataVector.size(); i++)
	{
		fprintf(fpOut,"%s\t%13.8f\t%13.8f\t%8.2f\t%8.2f\n",(PCSTR)(_bstr_t)(PCTSTR)m_OGDataVector[i].datatimeStr,m_OGDataVector[i].LONGITUDE,m_OGDataVector[i].LATITUDE,
			m_OGDataVector[i].gravity,/*basedata1.grav+*/dGravity[i]);
	}

	//�ͷŶ�̬������Դ
	delete dGravity;
	fclose(fpOut);
}


void C������ȡView::OnExtractgps()
{
	// TODO: �ڴ���������������

	if (m_ExtractGPSDataDlg.DoModal()!=IDOK)
	{
		return;
	}

	vector<GPSSTRUCT> gpsdatavector;
	if (m_ExtractGPSDataDlg.m_SingleOrMulti==0)		//�����ļ���ȡ
	{
		CString GPSfilename;
		TCHAR szFilter[] = _T("GPS �ļ���*.txt)|*.txt|��*.dat)|*.dat|�����ļ�(*.*)|*.*||");
		CFileDialog FileDlg(TRUE, m_ExtractGPSDataDlg.m_ExtentName,NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL);
		FileDlg.m_ofn.lpstrTitle=_T("�򿪲��������ȶ������ļ�");

		if (IDOK == FileDlg.DoModal())
		{   
			GPSfilename = FileDlg.GetPathName();  
		}
		else
		{
			return;
		}
		//��ȡgps�ļ�
		switch (m_ExtractGPSDataDlg.m_GPSFormat)
		{
		case 0:
			ReadGPSDataFile(GPSfilename,gpsdatavector);			//��ȡGPRMC��
			break;
		case 1:
			ReadGPSDataFile_GPGGA(GPSfilename,gpsdatavector);	//��ʱ��ԭʼ�ļ�����û��GPRMC����ȡGPGGA��GPVTG����ϵõ���γ�Ⱥ��ٺ���
			break;
		}
		
		if (gpsdatavector.size()==0)
		{
			MessageBox("����ȡ��0���ļ�");
			return;
		}
		////���GPS���ݵ��ļ�
		CString OutFileName=gpsdatavector[0].datatimeStr.Left(10)+_T("-GPS.DAT");
		TCHAR szFilter2[] = _T("��ȡ��GPS �ļ���*.DAT)|*.DAT|�����ļ�(*.*)|*.*||");
		CFileDialog FileDlg2(FALSE, _T("DAT"),OutFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter2, NULL);
		FileDlg2.m_ofn.lpstrTitle=_T("���������GPS����");
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
			AfxMessageBox(_T("��")+OutFileName+_T("ʧ��"));
			return ;
		}

		for (int i = 0; i < gpsdatavector.size(); i++)
		{
			fprintf(fpOut,"%s\t%13.8f\t%13.8f\t%8.1f\t%8.2f\n",(PCSTR)(_bstr_t)(PCTSTR)gpsdatavector[i].datatimeStr,gpsdatavector[i].longitude,
				gpsdatavector[i].latitude,gpsdatavector[i].velocity,gpsdatavector[i].Heading);
		}
		fclose(fpOut);
	}else if(m_ExtractGPSDataDlg.m_SingleOrMulti==1)		//������ȡ
	{
		//��ȡGPS�ļ�·��
		CString gpspathname=m_ExtractGPSDataDlg.m_PathName;
		//1. ��·���л�ȡ�ļ���
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
		//2. ����gps�ļ���ȡ����
		m_ProgressBar->SetRange(0,filenamevector.size());
		
		for (int i = 0; i < filenamevector.size(); i++)
		{
			switch (m_ExtractGPSDataDlg.m_GPSFormat)
			{
			case 0:
				ReadGPSDataFile(filenamevector[i],gpsdatavector);			//��ȡGPRMC��
				break;
			case 1:
				ReadGPSDataFile_GPGGA(filenamevector[i],gpsdatavector);	//��ʱ��ԭʼ�ļ�����û��GPRMC����ȡGPGGA��GPVTG����ϵõ���γ�Ⱥ��ٺ���
				break;
			}
			
			m_ProgressBar->SetPos(i);
		}
		
		//3. ���gps���ݵ��ļ��������������ļ���ÿ��һ���ļ���
		OutPutGPSData(gpspathname,gpsdatavector);
	}
	
	
	

	
}
void C������ȡView::OutPutGPSData(CString pathname, vector<GPSSTRUCT> GPSDATAVECTOR)
{
	if (GPSDATAVECTOR.size()==0)
	{
		MessageBox("����ȡ��0���ļ�");
		return;
	}
	//���GPS���ݵ��ļ�
	CString OutFileName=GPSDATAVECTOR[0].datatimeStr.Left(10)+_T("-GPS.DAT");

	OutFileName=pathname+_T("\\")+OutFileName;
	//������Ҫ�������ļ���������β����������ֵ+1��
	int numberoffile=0;
	COleDateTime firstdate,currentdate,lastdate;
	if(!firstdate.ParseDateTime(GPSDATAVECTOR[0].datatimeStr))
	{
		MessageBox(_T("��һ�����ڸ�ʽ�д���:")+GPSDATAVECTOR[0].datatimeStr+_T("\n����ԭʼ�ļ�"));
		return;
	}
	if (!lastdate.ParseDateTime(GPSDATAVECTOR[GPSDATAVECTOR.size()-1].datatimeStr))
	{
		MessageBox(_T("���һ�����ڸ�ʽ�д���:")+GPSDATAVECTOR[GPSDATAVECTOR.size()-1].datatimeStr+_T("\n����ԭʼ�ļ�"));
		return;
	}
	COleDateTimeSpan tt;
	tt=lastdate-firstdate;
	numberoffile=tt.GetDays()+1;
	m_ProgressBar->SetRange(0,numberoffile);
	vector<FILE*> fpvector;fpvector.push_back(NULL);
	int errordatanumber=0;
	FILE* fp_error=NULL;					//���󱨸��ļ�
	if ((fp_error=fopen((_bstr_t)pathname+_T("\\���󱨸�.log"),"w"))==NULL)
	{
		AfxMessageBox(_T("��")+pathname+_T("\\���󱨸�.log ʧ��"));
		return ;
	}fprintf(fp_error,"�������ݵ�����\t�������ݵ�ǰһ����¼\n");
	int indexoffile=0;
	if ((fpvector[indexoffile]=fopen((_bstr_t)OutFileName,"w"))==NULL)
	{
		AfxMessageBox(_T("��")+OutFileName+_T("ʧ��"));
		return ;
	}
	CString firstdatestr=GPSDATAVECTOR[0].datatimeStr.Left(10);
	for (int i = 0; i < GPSDATAVECTOR.size(); i++)
	{
		if(!currentdate.ParseDateTime(GPSDATAVECTOR[i].datatimeStr))			//��Ϊԭʼ���ݿ����д��󣬵��¶�ȡ�Ĳ����д��������ж������ַ����Ƿ��Ǻ��������
		{
			fprintf(fp_error,"%s---------%s\n",(PCSTR)(_bstr_t)(PCTSTR)GPSDATAVECTOR[i].datatimeStr,(PCSTR)(_bstr_t)(PCTSTR)GPSDATAVECTOR[i-1].datatimeStr);
			errordatanumber++;
			continue;
		}
		tt=currentdate-firstdate;
		
		if (currentdate.GetDay()!=firstdate.GetDay())	//gps�ļ���������¼�ģ������������ڲ�ֵ��Ӧ�ô�����β���ڲ�ֵ
		{
			indexoffile++;m_ProgressBar->SetPos(indexoffile);
			firstdate=currentdate;
			OutFileName=GPSDATAVECTOR[i].datatimeStr.Left(10)+_T("-GPS.DAT");
			OutFileName=pathname+_T("\\")+OutFileName;
			/*if (_access((_bstr_t)OutFileName,00)==-1)
			{
				MessageBox(OutFileName+_T("�ļ��Ѿ�����"));
				OutFileName=OutFileName+_T(".1");
			}*/
			fpvector.push_back(NULL);
			if ((fpvector[indexoffile]=fopen((_bstr_t)OutFileName,"w"))==NULL)
			{
				AfxMessageBox(_T("��")+OutFileName+_T("ʧ��"));
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
	infostr.Format(_T("GPS����������ϣ��������%d���ļ�\n%s--%s\n�������ݣ�%d �������󱨸�.log��"),numberoffile,GPSDATAVECTOR[0].datatimeStr.Left(10),GPSDATAVECTOR[GPSDATAVECTOR.size()-1].datatimeStr.Left(10),errordatanumber);
	MessageBox(infostr);
}

void C������ȡView::ReadGPSDataFile(CString gpsdatafilename, vector<GPSSTRUCT>& GPSVECTOR)
{
	////��ȡ�ļ�
	CStdioFile fp;
	CString str_Line;
	if (fp.Open(gpsdatafilename,CFile::modeRead)==FALSE)
	{
		AfxMessageBox(_T("��")+gpsdatafilename+_T("ʧ��"));
		return;
	}
	//��ȡGPS��Ϣ
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
		AfxMessageBox(_T("��")+errorfilename+_T("ʧ��"));
		return;
	}
	while (fp.ReadString(str_Line))
	{
		//str_Line=str_Line.Left(str_Line.GetLength()-23);//��ȡ22���δŲ�GPSʱ��ÿ�к��涼��23��ʱ�������ַ�����ȥ����
		headstr=str_Line.Left(headflaglength);
		str_Line=str_Line.Right(str_Line.GetLength()-headlength);
		if (headstr==_T("$GPRMC") && str_Line.Mid(str_Line.GetLength()-3,1)==_T("*"))			//���Ǻ�˵�������������ģ������ֶβ�һ��һ��������������ȡ����ʱ������Mid��ȡ��Ӧ�ø�ʽ����ȡ%lf��
		{
			//�еļ�¼����Ȼ��PGRMC��ͷ��ĩβҲ��*�������м��¼�д����ȱʧ���²�����ȷ��ȡ�������Ҫ�ж϶��ø�����һ��ȷ����ȷ�ļ�¼
			if (GetCharCountInCString(str_Line,_T(","))!=11)
			{
				fprintf(fp_error,"���Ÿ�������11��   %s\n",(PCSTR)(_bstr_t)(PCTSTR)str_Line);
			}else
			{
				sscanf(str_Line,"%lf%c%c%c%lf%c%c%c%lf%c%c%c%lf%c%lf%c%d",&time1,&comma,&comma,&comma,&gpsdata.latitude,
				&comma,&latsign,&comma,&gpsdata.longitude,&comma,&lonsign,&comma,&gpsdata.velocity,&comma,&gpsdata.Heading,&comma,&date1);
				//ת�����ڸ�ʽ
				HH=time1/10000;MM=(time1-HH*10000)/100;SS=(time1-HH*10000-MM*100);
				timestr.Format(_T("%02d:%02d:%02d"),HH,MM,SS);
				day=date1/10000;month=(date1-day*10000)/100;year=date1-day*10000-month*100;
				datestr.Format(_T("20%02d-%02d-%02d"),year,month,day);
				gpsdata.datatimeStr=datestr+_T(" ")+timestr;
				//ת����γ��(���ȸ�ʽΪ��dddmm.mmmm;γ��)
				//γ��λ��������ɴ�����ô����������������ڼ���������ͳһ�ģ����ж�ǰ��������γ�Ȳ�ֵ���ܳ���1�ȣ�
				dd=(int)gpsdata.latitude/100.0;			//γ��
				mm=gpsdata.latitude-dd*100;
				if (latsign=='S')
				{
					gpsdata.latitude=-((double)dd+mm/60.0);
				}else
				{
					gpsdata.latitude=(double)dd+mm/60.0;
				}
				dd=(int)gpsdata.longitude/100.0;			//����
				mm=gpsdata.longitude-dd*100;
				if (lonsign=='W')
				{
					gpsdata.longitude=-((double)dd+mm/60.0);
				}else
				{
					gpsdata.longitude=(double)dd+mm/60.0;
				}
				//----------------------------------
				//�жϾ�γ�ȷ�Χ����ֹ����
				if (gpsdata.longitude >=-180 && gpsdata.longitude<=180 && gpsdata.latitude >=-90 && gpsdata.latitude<=90)
				{
					GPSVECTOR.push_back(gpsdata);
				}
			}
		}
		
	}fp.Close();fclose(fp_error);
}



int C������ȡView::GetCharCountInCString(CString str, CString str0)
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


void C������ȡView::OnCheckamend()
{
	// TODO: �ڴ���������������
	if (m_ExtractGPSDataDlg.DoModal()!=IDOK)
	{
		return;
	}

	vector<GPSSTRUCT> gpsdatavector;
	if (m_ExtractGPSDataDlg.m_SingleOrMulti==0)		//�����ļ���ȡ
	{
		CString GPSfilename;
		TCHAR szFilter[] = _T("GPS �ļ���*.dat)|*.dat|��*.txt)|*.txt|�����ļ�(*.*)|*.*||");
		CFileDialog FileDlg(TRUE, m_ExtractGPSDataDlg.m_ExtentName,NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL);
		FileDlg.m_ofn.lpstrTitle=_T("�򿪲��������ȶ������ļ�");

		if (IDOK == FileDlg.DoModal())
		{   
			GPSfilename = FileDlg.GetPathName();  
		}
		else
		{
			return;
		}
		//��ȡgps�ļ�
		ReadExtractedGPSFile(GPSfilename,gpsdatavector);
		if (gpsdatavector.size()==0)
		{
			MessageBox("�������0���ļ�");
			return;
		}
		////���GPS���ݵ��ļ�
		CString OutFileName=gpsdatavector[0].datatimeStr.Left(10)+_T("-GPS.DAT");
		TCHAR szFilter2[] = _T("��ȡ��GPS �ļ���*.DAT)|*.DAT|�����ļ�(*.*)|*.*||");
		CFileDialog FileDlg2(FALSE, _T("DAT"),OutFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter2, NULL);
		FileDlg2.m_ofn.lpstrTitle=_T("���������GPS����");
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
			AfxMessageBox(_T("��")+OutFileName+_T("ʧ��"));
			return ;
		}

		for (int i = 0; i < gpsdatavector.size(); i++)
		{
			fprintf(fpOut,"%s\t%13.8f\t%13.8f\t%8.1f\t%8.2f\n",(PCSTR)(_bstr_t)(PCTSTR)gpsdatavector[i].datatimeStr,gpsdatavector[i].longitude,
				gpsdatavector[i].latitude,gpsdatavector[i].velocity,gpsdatavector[i].Heading);
		}
		fclose(fpOut);
	}else if(m_ExtractGPSDataDlg.m_SingleOrMulti==1)		//������ȡ
	{
		//��ȡGPS�ļ�·��
		CString gpspathname=m_ExtractGPSDataDlg.m_PathName;
		//1. ��·���л�ȡ�ļ���
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
			MessageBox(_T("�ļ�����С��3!�ֶ�����"));
			return;
		}
		AmendGPSTimeDate(gpspathname,filenamevector);//����gpsʱ�������ڴ�λ������(��֤ÿ���ļ���һ����0:0:0)
		//��ѯÿ���ļ�������ļ���һ�в���0��0:0���ļ���������������
		FILE* fp;
		if ((fp=fopen((_bstr_t)(gpspathname+_T("\\��鱨��.log")),"w"))==NULL)
		{
			MessageBox(_T("�򿪼�鱨���ļ�ʧ��"));
			return ;
		}
		fprintf(fp,"��һ�в���00:00:00��GPS�ļ��У�\n");
		for (int i = 0; i < filenamevector.size(); i++)
		{
			if (!InspectGPSFile(filenamevector[i]))
			{
				fprintf(fp,"%s\n",(PCSTR)(_bstr_t)(PCTSTR)filenamevector[i]);
			}
		}
		fclose(fp);
	}
	
	MessageBox(_T("�������"));
}
void C������ȡView::CorrectGPSTimeData(CString pathname,int& row1,vector<GPSSTRUCT>firstGPSvector,vector<GPSSTRUCT>secondGPSvector)
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
void C������ȡView::AmendGPSTimeDate(CString pathname,vector<CString> filenamevector)
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
	//���ȴӵڶ����ļ����������ڵ�һ���ļ������ڣ������ݣ���д���һ���ļ�
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
	WriteExtractedGPSData(pathname,firstGPSvector,0);//��һ���ļ��޸���ɣ�д��
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
	//�����������
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

void C������ȡView::WriteExtractedGPSData(CString pathname, vector<GPSSTRUCT> GPSDATAVECTOR,int startrow,bool IsCheck)
{
	CString OutFileName=pathname+_T("\\")+GPSDATAVECTOR[0].datatimeStr.Left(10)+_T("-GPS.DAT");
	FILE *fpOut=NULL;
	if ((fpOut=fopen((_bstr_t)OutFileName,"w"))==NULL)
	{
		AfxMessageBox(_T("��")+OutFileName+_T("ʧ��"));
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
void C������ȡView::ReadExtractedGPSFile(CString filename, vector<GPSSTRUCT>& gpsdatavector)
{
	GPSSTRUCT gpsdata;
	CString datestr,timestr;
	CString firstdatestr;
	char* datechar[20],*timechar[20];
	FILE *fp=NULL;
	if ((fp=fopen((_bstr_t)filename,"r"))==NULL)
	{
		AfxMessageBox(_T("��")+filename+_T("ʧ��"));
		return ;
	}
	//��ȡ�ļ�
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

void C������ȡView::OnMatchgpsGrav()
{
	// TODO: �ڴ���������������
	if (m_MatchGPSGravDlg.DoModal()!=IDOK)
	{
		return;
	}
	CString gravpath=m_MatchGPSGravDlg.m_GravDataPath,gpspath=m_MatchGPSGravDlg.m_GPSDataPath,matchedgravpath=m_MatchGPSGravDlg.m_MatchedGravDataPath;
	//�Ȼ�ȡGPS�ļ�����
	vector<CString>gpsfilenamevector;
	vector<GPSSTRUCT>gpsdatavector;
	GetFileCount(gpspath,_T("DAT"),gpsfilenamevector);
	int gpsfilenum=gpsfilenamevector.size();
	CString gravfilename;			//����gravpath��gps�ļ��������ȷ����ƥ������������ļ���
	COleDateTime currentdate;
	int matchedfilenum=0;
	m_ProgressBar->SetRange(1,gpsfilenum);
	FILE* fp_log;
	CString filename;
	filename.Format(_T("%s\\ƥ�䱨��.log.dat"),matchedgravpath);
	if ((fp_log=fopen((_bstr_t)filename,"w"))==NULL)
	{
		AfxMessageBox(_T("��")+filename+_T("ʧ��"));
		return ;
	}
	fprintf(fp_log,"GPS���ݹ�%d��\nȱʧ�������ݵ�����:\n",gpsfilenum);
	for (int i = 0; i < gpsfilenum; i++)
	{
		gpsdatavector.clear();
		ReadExtractedGPSFile(gpsfilenamevector[i],gpsdatavector);
		//��ȡ����
		if(!currentdate.ParseDateTime(gpsdatavector[0].datatimeStr))
		{
			MessageBox(gpsfilenamevector[i]+_T("\n��һ�����ڸ�ʽ�д���:")+gpsdatavector[0].datatimeStr+_T("\n����ԭʼ�ļ�"));
			return;
		}
		//currentdate.GetDayOfYear();		//��������
		gravfilename.Format(_T("%s\\%d_%03d.%s.DAT"),gravpath,currentdate.GetYear(),currentdate.GetDayOfYear(),m_MatchGPSGravDlg.m_GravimeterVersion);
		//����������
		m_OGDataVector.clear();//��ȡ��������֮ǰ���������
		if (!ReadGravData(gravfilename))
		{
			fprintf(fp_log,"%s\n",(PCSTR)(_bstr_t)(PCTSTR)gpsdatavector[0].datatimeStr.Left(10));
			continue;
		}
		     
		//ƥ��������gps�����
		if (WriteGRAVGPSData(matchedgravpath,m_OGDataVector,gpsdatavector))
		{
			matchedfilenum++;
		}
		m_ProgressBar->SetPos(i);
	}
	fclose(fp_log);
	CString strinfo;
	strinfo.Format(_T("��ƥ��%d�������ļ�,��ƥ�䱨��"),matchedfilenum);
	MessageBox(strinfo);
}


bool C������ȡView::WriteGRAVGPSData(CString pathname,vector<OGDataStruct> ogdavavector, vector<GPSSTRUCT> gpsdatavector)
{
	if (ogdavavector.size()==0 || gpsdatavector.size()==0)
	{
		MessageBox(_T("GPS������������û�ж�ȡ"));
		return false;
	}
	FILE* FPOUT=NULL;
	CString filename;
	filename.Format(_T("%s\\%s_GravGPS.dat"),pathname,gpsdatavector[0].datatimeStr.Left(10));
	if ((FPOUT=fopen((_bstr_t)filename,"w"))==NULL)
	{
		AfxMessageBox(_T("��")+filename+_T("ʧ��"));
		return false;
	}
	COleDateTime currenttime;
	int currentsecond;//��ǰʱ�����00:00:00��������
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
	//�ж�����������0:0:0:��������
	
	for (int i = 0; i < gpsdatavector.size(); i++)
	{
		if(!currenttime.ParseDateTime(gpsdatavector[i].datatimeStr))
		{
			MessageBox(_T("���ڸ�ʽ�д���:")+gpsdatavector[i].datatimeStr+_T("\n����ԭʼ�ļ�"));
			return false;
		}
		currentsecond=currenttime.GetHour()*3600+currenttime.GetMinute()*60+currenttime.GetSecond();
		GravIndex=currentsecond+dIndex;//
		if (GravIndex>=m_OGDataVector.size())
		{
			///MessageBox(_T("GPSʱ����������GRAV������Χ")+gpsdatavector[i].datatimeStr);
			//CString str;str.Format(_T("��������%d  ����%d"),m_OGDataVector.size(),GravIndex);MessageBox(str);
			fclose(FPOUT);
			return true;
		}
		//���������ļ�Ӧ����һ����ģ�00:00:00ʱ�̵���������λ�����ĵڶ���Ԫ�أ�1s�������Դ�����
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


void C������ȡView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	ClearPointIndex();
}

//�ͷ�m_pointindex��ά�����ڴ�
int C������ȡView::ClearPointIndex(void)
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


void C������ȡView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CView::OnMouseMove(nFlags, point);
}


void C������ȡView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//1. ������Ƥ������
	CRectTracker recttract;
	recttract.TrackRubberBand(this,point,TRUE);
	
	//2. ��tracker��������Ϊ�Ŵ�����
	m_SelectedRect=recttract.m_rect;
	m_SelectedRect.NormalizeRect();	//��ѡ���������򻯣������ת�����Ӱ�����������������������ָ��Ŀ�Ⱥ͸߶�
	
	//3. �󽻼�
	m_SelectedRect.IntersectRect(m_SelectedRect,m_CourseFigureRect);

	//4. ��ʼ���ֲ��Ŵ󻬿�ؼ�
	CMainFrame* pMainFrm=(CMainFrame*)AfxGetApp()->GetMainWnd();
	CMFCRibbonSlider* PointNumber=DYNAMIC_DOWNCAST(CMFCRibbonSlider,pMainFrm->m_wndRibbonBar.FindByID(ID_SelectPointSlider));
	PointNumber->SetPos(0);
	m_CurrentSelectPointNumber=0;
		
	//5. ������ͼ
	if (!m_SelectedRect.IsRectEmpty())
	{
		UpdateSelectRectFigure(m_SelectedRect);		//ѡ���������ͼ�����зǿս���
	}

	CView::OnLButtonDown(nFlags, point);
}


void C������ȡView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	
	CView::OnLButtonUp(nFlags, point);
}


int C������ȡView::UpdateSelectRectFigure(CRect SelectedRect)
{
	if (!m_PointIndex)		//������ά����Ϊ�գ�������
	{
		return 0;
	}
	//m_BitMapDC.Rectangle(SelectedRect);
	//��������ѡ����������ص��Ӧ�����ݵ�����������ȡ��������ڻ��Ƶ����ݵ������������е�����
	vector<int> indexvector;
	int i1=SelectedRect.top-m_CourseFigureRect.top;
	int i2=SelectedRect.bottom-m_CourseFigureRect.top;
	int j1=SelectedRect.left-m_CourseFigureRect.left;
	int j2=SelectedRect.right-m_CourseFigureRect.left;
	for (int i = i1; i < i2; i++)
	{
		for (int j = j1; j < j2; j++)
		{
			if (m_PointIndex[i][j].PointExit)//�����ص�������
			{
				indexvector.push_back(m_PointIndex[i][j].IndexInOGdataVector);
			}
		}
	}
	if (indexvector.size()<3)//���ѡ�������ڵ����ݸ���С��3��������
	{
		return 0;
	}
	//���ҵ����������������ҵ����ֵ����Сֵ�ֱ���Ϊ�ֲ��Ŵ�������յ㣬��ֵ��Ϊ�ֲ��Ŵ�Χ
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
	//�Ծֲ��Ŵ�Χ��ֵ
	m_CurrentZoomUpRangeLeft=indexmin;
	m_ZoomUpRange=indexmax-indexmin;
	SetZoomUpSliderRange(m_ZoomUpRange);
	UpdateMainPlot();
	return 0;
}


int C������ȡView::SetZoomUpSliderRange(int range)
{
	CMainFrame* pMainFrm=(CMainFrame*)AfxGetApp()->GetMainWnd();
	CMFCRibbonSlider* PointNumber=DYNAMIC_DOWNCAST(CMFCRibbonSlider,pMainFrm->m_wndRibbonBar.FindByID(ID_SelectPointSlider));
	PointNumber->SetPos(0);
		PointNumber->SetRange(0,range-1);
	return 0;
}


void C������ȡView::ReadGPSDataFile_GPGGA(CString gpsdatafilename, vector<GPSSTRUCT>& GPSVECTOR)
{
	////��ȡ�ļ�
	CStdioFile fp;
	CString str_Line,str_Line_GPVTG,str_Line_GPZDA;
	if (fp.Open(gpsdatafilename,CFile::modeRead)==FALSE)
	{
		AfxMessageBox(_T("��")+gpsdatafilename+_T("ʧ��"));
		return;
	}
	//��ȡGPS��Ϣ
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
		AfxMessageBox(_T("��")+errorfilename+_T("ʧ��"));
		return;
	}
	while (fp.ReadString(str_Line))
	{
		headstr=str_Line.Left(headflaglength);//��ȡGPGGA��
		str_Line=str_Line.Right(str_Line.GetLength()-headlength);
		if (headstr==_T("$GPGGA") && str_Line.Mid(str_Line.GetLength()-3,1)==_T("*"))			//���Ǻ�˵�������������ģ������ֶβ�һ��һ��������������ȡ����ʱ������Mid��ȡ��Ӧ�ø�ʽ����ȡ%lf��
		{
			fp.ReadString(str_Line_GPVTG);//��ȡgpvtg��
			head_gpvtg=str_Line_GPVTG.Left(headflaglength);
			str_Line_GPVTG=str_Line_GPVTG.Right(str_Line_GPVTG.GetLength()-headlength);
			if (head_gpvtg==_T("$GPVTG") && str_Line_GPVTG.Mid(str_Line_GPVTG.GetLength()-3,1)==_T("*"))
			{
				fp.ReadString(str_Line_GPZDA);//��ȡgpzda��
				fp.ReadString(str_Line_GPZDA);//��ȡgpzda��
				head_gpzda=str_Line_GPZDA.Left(headflaglength);
				str_Line_GPZDA=str_Line_GPZDA.Right(str_Line_GPZDA.GetLength()-headlength);
				if (head_gpzda==_T("$GPZDA") && str_Line_GPZDA.Mid(str_Line_GPZDA.GetLength()-3,1)==_T("*"))
				{
					//�еļ�¼����Ȼ��PGRMC��ͷ��ĩβҲ��*�������м��¼�д����ȱʧ���²�����ȷ��ȡ�������Ҫ�ж϶��ø�����һ��ȷ����ȷ�ļ�¼
					if (GetCharCountInCString(str_Line,_T(","))!=13)			//GPGGA��������13������
					{
						fprintf(fp_error,"GPGGA���Ÿ�������13��   %s\n",(PCSTR)(_bstr_t)(PCTSTR)str_Line);
					}else
					{
						if (GetCharCountInCString(str_Line_GPVTG,_T(","))!=8)
						{
							fprintf(fp_error,"GPVTG���Ÿ�������8��   %s\n",(PCSTR)(_bstr_t)(PCTSTR)str_Line_GPVTG);
						}else
						{
							if (GetCharCountInCString(str_Line_GPZDA,_T(","))!=5)
							{
								fprintf(fp_error,"GPZDA���Ÿ�������5��   %s\n",(PCSTR)(_bstr_t)(PCTSTR)str_Line_GPZDA);
							}else
							{
								//*****************************��ȡGPZDA�����ڣ�********************************************************************
								sscanf(str_Line_GPZDA,"%lf%c%d%c%d%c%d",&time1,&comma,&day,&comma,&month,&comma,&year);
								//ת�����ڸ�ʽ
								datestr.Format(_T("%d-%02d-%02d"),year,month,day);
								//*****************************��ȡGPGGA����γ�ȣ�********************************************************************
								sscanf(str_Line,"%lf%c%lf%c%c%c%lf%c%c",&time1,&comma,&gpsdata.latitude,&comma,&latsign,&comma,&gpsdata.longitude,&comma,&lonsign);
								//ת��ʱ���ʽ
								HH=time1/10000;MM=(time1-HH*10000)/100;SS=(time1-HH*10000-MM*100);
								timestr.Format(_T("%02d:%02d:%02d"),HH,MM,SS);
								gpsdata.datatimeStr=datestr+_T(" ")+timestr;
								//ת����γ��(���ȸ�ʽΪ��dddmm.mmmm;γ��)
								//γ��λ��������ɴ�����ô����������������ڼ���������ͳһ�ģ����ж�ǰ��������γ�Ȳ�ֵ���ܳ���1�ȣ�
								dd=(int)gpsdata.latitude/100.0;			//γ��
								mm=gpsdata.latitude-dd*100;
								if (latsign=='S')
								{
									gpsdata.latitude=-((double)dd+mm/60.0);
								}else
								{
									gpsdata.latitude=(double)dd+mm/60.0;
								}
								dd=(int)gpsdata.longitude/100.0;			//����
								mm=gpsdata.longitude-dd*100;
								if (lonsign=='W')
								{
									gpsdata.longitude=-((double)dd+mm/60.0);
								}else
								{
									gpsdata.longitude=(double)dd+mm/60.0;
								}
								
								//*****************************��ȡGPVTG(������)********************************************************************
								sscanf(str_Line_GPVTG,"%lf%c%c%c%c%c%c%lf",&gpsdata.Heading,&comma,&comma,&comma,&comma,&comma,&comma,&gpsdata.velocity);
								//----------------------------------
								//�жϾ�γ�ȷ�Χ����ֹ����
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


void C������ȡView::OnCreatewordreport()
{
	// TODO: �ڴ���������������
	//1 ������ȡ�����ļ�
	if (m_WriteToWordDlg.DoModal()!=IDOK)
	{
		return;
	}
	//��ȡGPS�ļ�·��
	CString gpspathname=m_WriteToWordDlg.m_SurveyLinePath;
	vector<CString>filenamevector;
	CString extentname=m_WriteToWordDlg.m_ExtName;
	int filenum=GetFileCount(gpspathname,extentname,filenamevector);//��ȡ���в����ļ���
	if (filenum==0)
	{
		MessageBox(_T("��ȡ��0����Ч����"));
		return;
	}
	//�ļ�ָ������
	CStdioFile* fp=new CStdioFile[filenum];
	CString errorinfoStr;
	for (int i = 0; i < filenum; i++)
	{
		if (fp[i].Open(filenamevector[i],CFile::modeRead)==FALSE)
		{
			errorinfoStr.Format(_T("��ȡ��%d������ʧ�ܣ������˳�"),i);
			MessageBox(errorinfoStr);
			return;
		}
	}

	
	//2 ��word�������ĵ����ĵ�����ʽ�ο�������������㽻һ����Ҫ�󡿸���B.1[�ۺϵ������������Ϣ��]
	CreateWordDocuments();
	CreateWord();
	m_wdDocs=m_wdApp.get_Documents();
	m_wdDoc=m_wdApp.get_ActiveDocument();
	CFont0 font = m_wdSel.get_Font();					//����word��������
           font.put_Name(_T("Times New Roman"));
		   m_wdSel.get_PageSetup();
	CPageSetup pgesetup=m_wdSel.get_PageSetup();
	pgesetup.put_Orientation(1);
	 
	CString RTITableTitle;RTITableTitle.Format(_T("%s���ε�%s������������ͳ��"),m_WriteToWordDlg.m_HangCi,m_WriteToWordDlg.m_HangDuan);
	m_wdSel.TypeParagraph();
	m_wdSel.put_Style(COleVariant(_T("����")));			//������ʽ
	font = m_wdSel.get_Font();//��������
           font.put_Name(_T("Times New Roman"));
	m_wdSel.TypeText(RTITableTitle);					//�ĵ�����

	m_wdSel.TypeParagraph();							//����һ��
	m_wdSel.put_Style(COleVariant(_T("����")));			//������ʽ
	font = m_wdSel.get_Font();//��������
           font.put_Name(_T("Times New Roman"));
		   font.put_Size(11);
		   font.put_Bold(0);
	m_wdSel.TypeText(_T("��λ/������")+m_WriteToWordDlg.m_DanWeiJiGou);
	m_wdSel.TypeText(_T("       ���鴬��")+m_WriteToWordDlg.m_DiaoChaChuan);
	m_wdSel.TypeText(_T("       ���α�ţ�")+m_WriteToWordDlg.m_Title);

	//3. ��񲿷�*********************************************************
	int nRowSize = filenum+2,nColumnSize=9;				//��������������
	m_wdSel.TypeParagraph();			//����һ��
	CParagraph paragraph=m_wdSel.get_ParagraphFormat();
	paragraph.put_Alignment(1);
	font = m_wdSel.get_Font();//��������
           font.put_Name(_T("Times New Roman"));
	CTables0 wordTables = m_wdDoc.get_Tables();
	CRange wordRange = m_wdSel.get_Range();
	COleVariant varTrue(short(1),VT_BOOL),vOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);  
    CComVariant Template(_T(""));    //û��ʹ��WORD���ĵ�ģ��  
    CComVariant NewTemplate(false),DocumentType(0),Visible;  
	CTable0 wordTable= wordTables.Add(wordRange, nRowSize , nColumnSize, &NewTemplate, varTrue);
	
	//�߿�
	CBorders tblBorders;
	tblBorders=wordTable.get_Borders();
	tblBorders.put_InsideLineStyle(1);//�ڱ߿�7��ʾ�пյı߿�0��ʾ�����߿�1��ʾʵ�߿�
	tblBorders.put_OutsideLineStyle(1);//��߿�
	
	
	// ���
	CCell cell;
	CString firstline,lastline,templine;

	cell = wordTable.Cell(1, 1);cell.Select();m_wdSel.TypeText(_T("���α��"));
	cell = wordTable.Cell(1, 2);cell.Select();m_wdSel.TypeText(_T("���ߺ�"));
	cell = wordTable.Cell(1, 3);cell.Select();m_wdSel.TypeText(_T("��ʼʱ��\nyyyymmddHHMMSS"));
	cell = wordTable.Cell(1, 4);cell.Select();m_wdSel.TypeText(_T("��ֹʱ��\nyyyymmddHHMMSS"));
	cell = wordTable.Cell(1, 5);cell.Select();m_wdSel.TypeText(_T("��ʼ����\n��"));
	cell = wordTable.Cell(1, 6);cell.Select();m_wdSel.TypeText(_T("��ʼγ��\n��"));
	cell = wordTable.Cell(1, 7);cell.Select();m_wdSel.TypeText(_T("��ֹ����\n��"));
	cell = wordTable.Cell(1, 8);cell.Select();m_wdSel.TypeText(_T("��ֹγ��\n��"));
	cell = wordTable.Cell(1, 9);cell.Select();m_wdSel.TypeText(_T("���߳���\nkm"));
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
			MessageBox(_T("�ļ���ʽ����ȷ"));
			return;
		}
		//�жϵ�һ�е��ַ������ȣ�������ʲô������ȡ
		if (firstline.GetLength()==197)	//��׼��ʽ
		{
			while (fp[i].ReadString(templine))
			{
				lastline=templine;

				velocitytemp=atof(templine.Mid(180,8));

				distance+=velocitytemp*1.852/3600.0;//�ٶȳ���ʱ��=km
				linenumber++;
			}

			//��ʼ��Ϣ
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
			//д����(��ʼ)
			surveylinename.Format(_T("%s%02d"),m_WriteToWordDlg.m_surveylinename,i+1);
			cell = wordTable.Cell(i+2, 1);cell.Select();m_wdSel.TypeText(m_WriteToWordDlg.m_Title);
			cell = wordTable.Cell(i+2, 2);cell.Select();m_wdSel.TypeText(surveylinename);
			cell = wordTable.Cell(i+2, 3);cell.Select();m_wdSel.TypeText(datestr);
			cell = wordTable.Cell(i+2, 5);cell.Select();m_wdSel.TypeText(longitudestr);
			cell = wordTable.Cell(i+2, 6);cell.Select();m_wdSel.TypeText(latitudestr);

			//��ֹ��Ϣ
			datestr=lastline.Mid(5,10);
			timestr=lastline.Mid(16,8);
			datestr.Format(_T("%s %s"),datestr,timestr);
			longitude=atof(lastline.Mid(152,13));
			latitude=atof(lastline.Mid(166,13));
	
			longitudestr.Format(_T("%.8f"),longitude);
			latitudestr.Format(_T("%.8f"),latitude);

			date.ParseDateTime(datestr);
			datestr.Format(_T("%4d%02d%02d%02d%02d%02d"),date.GetYear(),date.GetMonth(),date.GetDay(),date.GetHour(),date.GetMinute(),date.GetSecond());


			//д����(��ֹ)
			cell = wordTable.Cell(i+2, 4);cell.Select();m_wdSel.TypeText(datestr);
			cell = wordTable.Cell(i+2, 7);cell.Select();m_wdSel.TypeText(longitudestr);
			cell = wordTable.Cell(i+2, 8);cell.Select();m_wdSel.TypeText(latitudestr);
			distancestr.Format(_T("%.1f"),distance);
			cell = wordTable.Cell(i+2, 9);cell.Select();m_wdSel.TypeText(distancestr);
			//cell = wordTable.Cell(i+2, 10);cell.Select();m_wdSel.TypeText(GetFileNameByPath(filenamevector[i]));//�����ļ���
		}else
		{
			while (fp[i].ReadString(templine))
			{
				lastline=templine;
				sscanf(templine,"%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%lf%lf%lf%lf",tempchar,datechar,timechar,tempchar,tempchar,tempchar,
				tempchar,tempchar,tempchar,tempchar,tempchar,tempchar,tempchar,tempchar,tempchar,tempchar,tempchar,&longitude,&latitude,&velocitytemp,&headingtemp);
				distance+=velocitytemp*1.852/3600.0;//�ٶȳ���ʱ��=km
				linenumber++;
			}
			sscanf(firstline,"%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%lf%lf%lf%lf",tempchar,datechar,timechar,tempchar,tempchar,tempchar,
				tempchar,tempchar,tempchar,tempchar,tempchar,tempchar,tempchar,tempchar,tempchar,tempchar,tempchar,&longitude,&latitude,&velocitytemp,&headingtemp);			//��ʼ
			longitudestr.Format(_T("%.8f"),longitude);
			latitudestr.Format(_T("%.8f"),latitude);
			datestr.Format(_T("%s %s"),datechar,timechar);
			COleDateTime date;
			date.ParseDateTime(datestr);
			datestr.Format(_T("%4d%02d%02d%02d%02d%02d"),date.GetYear(),date.GetMonth(),date.GetDay(),date.GetHour(),date.GetMinute(),date.GetSecond());
			//д����(��ʼ)
			surveylinename.Format(_T("%s%02d"),m_WriteToWordDlg.m_surveylinename,i+1);
			cell = wordTable.Cell(i+2, 1);cell.Select();m_wdSel.TypeText(m_WriteToWordDlg.m_Title);
			cell = wordTable.Cell(i+2, 2);cell.Select();m_wdSel.TypeText(surveylinename);
			cell = wordTable.Cell(i+2, 3);cell.Select();m_wdSel.TypeText(datestr);
			cell = wordTable.Cell(i+2, 5);cell.Select();m_wdSel.TypeText(longitudestr);
			cell = wordTable.Cell(i+2, 6);cell.Select();m_wdSel.TypeText(latitudestr);

			sscanf(lastline,"%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%lf%lf%lf%lf",tempchar,datechar,timechar,tempchar,tempchar,tempchar,
				tempchar,tempchar,tempchar,tempchar,tempchar,tempchar,tempchar,tempchar,tempchar,tempchar,tempchar,&longitude,&latitude,&velocitytemp,&headingtemp);			//��ֹ
			longitudestr.Format(_T("%.8f"),longitude);
			latitudestr.Format(_T("%.8f"),latitude);
			datestr.Format(_T("%s %s"),datechar,timechar);
			date.ParseDateTime(datestr);
			datestr.Format(_T("%4d%02d%02d%02d%02d%02d"),date.GetYear(),date.GetMonth(),date.GetDay(),date.GetHour(),date.GetMinute(),date.GetSecond());


			//д����(��ʼ)
			cell = wordTable.Cell(i+2, 4);cell.Select();m_wdSel.TypeText(datestr);
			cell = wordTable.Cell(i+2, 7);cell.Select();m_wdSel.TypeText(longitudestr);
			cell = wordTable.Cell(i+2, 8);cell.Select();m_wdSel.TypeText(latitudestr);
			distancestr.Format(_T("%.1f"),distance);
			cell = wordTable.Cell(i+2, 9);cell.Select();m_wdSel.TypeText(distancestr);
			//cell = wordTable.Cell(i+2, 10);cell.Select();m_wdSel.TypeText(GetFileNameByPath(filenamevector[i],true));//�����ļ���
		}
		fp[i].Close();
		//�ܺ���
		SumDistance+=distance;
	}
	//���һ��
	CCell wordCell = wordTable.Cell(nRowSize, 1);		//��ʼ��Ԫ��
	wordRange = wordCell.get_Range();			//��ʼ��Ԫ������
	int iStart = wordRange.get_Start();			//��ʼ��Ԫ������
	wordCell = wordTable.Cell(nRowSize, nColumnSize);			//ĩ�˵�Ԫ��                
	wordRange = wordCell.get_Range();			//ĩ�˵�Ԫ������
	int iEnd = wordRange.get_End();				//ĩ�˵�Ԫ������
	wordRange.put_Start(iStart);				//��ȡ���ϲ��ĵ�Ԫ������
	wordRange.put_End(iEnd);
	tblBorders=wordRange.get_Borders();
	tblBorders.put_InsideLineStyle(0);//�ڱ߿�

	CString sumdistancestr;
	sumdistancestr.Format(_T("�����ܳ��ȣ�km����%.1f"),SumDistance);
	MergeCell(wordTable,nRowSize,6,nRowSize,9,sumdistancestr);//�ܳ���
	MergeCell(wordTable,nRowSize,4,nRowSize,5,_T(""));//
	MergeCell(wordTable,nRowSize,1,nRowSize,3,_T("������:")+m_WriteToWordDlg.m_person);//������
	//m_wdSel.EndOf(COleVariant((short)wdStory), COleVariant((short)wdMove)); // �������༭
	wordTable.put_AllowAutoFit(1);
	//wordTable.AutoFitBehavior(1);//���������Զ�����
	//wordTable.AutoFitBehavior(2);//���ݴ����Զ�����
	////���ñ���ʽΪ�����Ķ���
	wordCell = wordTable.Cell(1, 1);		//��ʼ��Ԫ��
	wordRange = wordCell.get_Range();			//��ʼ��Ԫ������
	iStart = wordRange.get_Start();			//��ʼ��Ԫ������
	wordCell = wordTable.Cell(nRowSize-1, nColumnSize);			//ĩ�˵�Ԫ��                
	wordRange = wordCell.get_Range();			//ĩ�˵�Ԫ������
	iEnd = wordRange.get_End();				//ĩ�˵�Ԫ������
	wordRange.put_Start(iStart);				//��ȡ���ϲ��ĵ�Ԫ������
	wordRange.put_End(iEnd);
	CCells wordCells = wordRange.get_Cells();	//ת��Ϊ��Ԫ��
	wordCells.put_VerticalAlignment(1);//���֣�����

	ShowWordApp();
	//�ͷ���Դ
	//m_wdDocs.ReleaseDispatch();
	m_wdDoc.ReleaseDispatch();
	m_wdRange.ReleaseDispatch();
	m_wdSel.ReleaseDispatch();
	wordTable.ReleaseDispatch();
	m_wdDocs.ReleaseDispatch();
	m_wdApp.ReleaseDispatch();
    return ;  
}
//�ϲ���Ԫ��
void C������ȡView::MergeCell(CTable0& wordTable,int row1,int col1,int row2,int col2,CString text)
{
	CRange wordRange;
	CCell wordCell = wordTable.Cell(row1, col1);		//��ʼ��Ԫ��
	wordRange = wordCell.get_Range();			//��ʼ��Ԫ������
	wordRange.put_Text(text);
	int iStart = wordRange.get_Start();			//��ʼ��Ԫ������
	wordCell = wordTable.Cell(row2, col2);			//ĩ�˵�Ԫ��                
	wordRange = wordCell.get_Range();			//ĩ�˵�Ԫ������
	int iEnd = wordRange.get_End();				//ĩ�˵�Ԫ������
	wordRange.put_Start(iStart);				//��ȡ���ϲ��ĵ�Ԫ������
	wordRange.put_End(iEnd);
	CCells wordCells = wordRange.get_Cells();	//ת��Ϊ��Ԫ��
	wordCells.Merge();							//�ϲ�
}

BOOL C������ȡView::CreateWordApp(void)
{
	COleException pe;  
    if (!m_wdApp.CreateDispatch(_T("Word.Application"), &pe))  
    {  
        AfxMessageBox("Application����ʧ�ܣ���ȷ����װ��word 2000�����ϰ汾!", MB_OK|MB_ICONWARNING);  
        pe.ReportError();  
        throw &pe;  
        return FALSE;  
    }  
    return TRUE;  
}


BOOL C������ȡView::CreateWordDocuments(void)
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
        AfxMessageBox("Documents����ʧ��!", MB_OK|MB_ICONWARNING);  
        return FALSE;  
    }  
    return TRUE;  
}


BOOL C������ȡView::CreateWordDocument(void)
{
	if (!m_wdDocs.m_lpDispatch)   
    {  
        AfxMessageBox("DocumentsΪ��!", MB_OK|MB_ICONWARNING);  
        return FALSE;  
    }  
  
    COleVariant varTrue(short(1),VT_BOOL),vOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);  
    CComVariant Template(_T(""));    //û��ʹ��WORD���ĵ�ģ��  
    CComVariant NewTemplate(false),DocumentType(0),Visible;  
  
    m_wdDocs.Add(&Template,&NewTemplate,&DocumentType,&Visible);      
  
    //�õ�document����  
	m_wdDoc = m_wdApp.get_ActiveDocument();  
    if (!m_wdDoc.m_lpDispatch)   
    {  
        AfxMessageBox("Document��ȡʧ��!", MB_OK|MB_ICONWARNING);  
        return FALSE;  
    }  
    //�õ�selection����  
	m_wdSel = m_wdApp.get_Selection();  
    if (!m_wdSel.m_lpDispatch)   
    {  
        AfxMessageBox("Select��ȡʧ��!", MB_OK|MB_ICONWARNING);  
        return FALSE;  
    }  
    //�õ�Range����  
    m_wdRange = m_wdDoc.Range(vOptional,vOptional);  
    if(!m_wdRange.m_lpDispatch)  
    {  
        AfxMessageBox("Range��ȡʧ��!", MB_OK|MB_ICONWARNING);  
        return FALSE;  
    }  
  
    return TRUE;  
}


BOOL C������ȡView::CreateWord(void)
{
	 if (FALSE == CreateWordDocument())   
    {  
        return FALSE;  
    }  
    return true; 
}


void C������ȡView::ShowWordApp(void)
{
	m_wdApp.put_Visible(TRUE);	 
}


void C������ȡView::HideWordApp(void)
{
	m_wdApp.put_Visible(FALSE);  
}


int C������ȡView::WriteTextToWord(CString szText)
{
	m_wdSel.TypeText(szText);
	
	return 0;
}


void C������ȡView::CloseWordApp(void)
{
	COleVariant vTrue((short)TRUE),      
                vFalse((short)FALSE),  
                vOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);  
    m_wdDoc.Save();  
    m_wdApp.Quit(vFalse,    // SaveChanges.  
             vTrue,            // OriginalFormat.  
             vFalse            // RouteDocument.  
             );  
    //�ͷ��ڴ�������Դ  
  
    m_wdRange.ReleaseDispatch();  
    m_wdSel.ReleaseDispatch();  
    m_wdDoc.ReleaseDispatch();  
    m_wdDocs.ReleaseDispatch();  
    m_wdApp.ReleaseDispatch(); 
}


void C������ȡView::OutPutData(CString filename,vector<OGDataStruct> gravdata)
{
	FILE *fpOut=NULL;
	if ((fpOut=fopen((_bstr_t)filename,"w"))==NULL)
	{
		AfxMessageBox(_T("��")+filename+_T("ʧ��"));
		return ;
	}
	//�������
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


int C������ȡView::CheckUse(void)
{
	//*************************��ʹ����������***********************
	int UserDays=90;//����ʹ������
	COleDateTime date,currentdate;
	CTime tm; tm=CTime::GetCurrentTime();//��ȡϵͳ��ǰ���ں�ʱ��
	currentdate.ParseDateTime(tm.Format(_T("%Y-%m-%d")));
	CString datestr;
	char datechar[20];
	int useday;

	//��ȡϵͳ�̷�
	//************************************�ļ�·��******************************************************************************
	char filename[300];
	//char path[MAX_PATH];
	//GetSystemDirectory(path,MAX_PATH);		//ϵͳ·��
	////GetCurrentDirectory(MAX_PATH,path);			//����ǰ·��
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
		MessageBox(_T("�����״�ʹ�ã���ӭ��ʹ��MGDPS V2.1"));
		if ((fp=fopen(filename,"w"))==NULL)
		{
			//MessageBox(_T("��ʧ��"));
			return 0;
		}
		fprintf(fp,"%d-%d-%d",tm.GetYear(),tm.GetMonth(),tm.GetDay());
	}else
	{	//MessageBox(_T("�޸�ʹ�ô���"));
		if ((fp=fopen(filename,"r"))==NULL)
		{
			return 0;
		}
		fscanf(fp,"%s",datechar);
		datestr=(PCTSTR)(_bstr_t)datechar;
		date.ParseDateTime(datestr);//�����ֵ
		useday=(currentdate-date).GetDays();
		//��ʾ��״̬����
		CString strTitlePane1;strTitlePane1.Format(_T("��ʹ��%d��"),useday);
		CMainFrame* pMainFrm=(CMainFrame*)AfxGetApp()->GetMainWnd();
		CMFCRibbonLabel *pStatus1 = (CMFCRibbonLabel*)pMainFrm->m_wndStatusBar.GetElement(0);
		pStatus1->SetText(strTitlePane1);
		pMainFrm->m_wndStatusBar.RedrawWindow();
		if (useday > UserDays)
		{
			//MessageBox(_T("����Ѿ����ڣ�����ϵ������\nzhikuiguo@live.cn"));
			//exit(0);
		}
	}
	if(fp)fclose(fp);
	//�����ļ�Ϊ����
		CFileStatus fs;
		CFile::GetStatus(filename,fs);
		fs.m_attribute = CFile::hidden;           //set hidden attribute
		CFile::SetStatus(filename,fs);
	//********����������*******************************************
	//int UserTimes=20;//������ô���
	//char *filename="C:\\Windows\\MGDPS.info";
	//int userday;
	//FILE* fp,*fp2;
	//if (!PathFileExists(filename))
	//{
	//	MessageBox(_T("�����״�ʹ�ã���ӭ��ʹ��MGDPS V2.1"));
	//	if ((fp=fopen(filename,"w"))==NULL)
	//	{
	//		return 0;
	//	}
	//	fprintf(fp,"%d",UserTimes);
	//}else
	//{	//MessageBox(_T("�޸�ʹ�ô���"));
	//	if ((fp=fopen(filename,"r"))==NULL)
	//	{
	//		return 0;
	//	}
	//	fscanf(fp,"%d",&userday);
	//	if (userday<=0)
	//	{
	//		MessageBox(_T("����Ѿ����ڣ�����ϵ������"));
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


int C������ȡView::UpdataProgress(int pos,UINT ribbonelementID)
{
	CMainFrame* pMainFrm=(CMainFrame*)AfxGetApp()->GetMainWnd();
	CMFCRibbonProgressBar* prog = DYNAMIC_DOWNCAST(CMFCRibbonProgressBar, pMainFrm->m_wndStatusBar.FindElement(ribbonelementID));
	prog->SetPos(pos);
	
	return 0;
}
int C������ȡView::SetProgressRange(int min, int max,UINT ribbonelementID)
{
	CMainFrame* pMainFrm=(CMainFrame*)AfxGetApp()->GetMainWnd();
	CMFCRibbonProgressBar* prog = DYNAMIC_DOWNCAST(CMFCRibbonProgressBar, pMainFrm->m_wndStatusBar.FindElement(ribbonelementID));
	prog->SetRange(min,max);
	return 0;
}

//��飨������ģ�GPS�ļ���һ���ǲ���0:0:0
bool C������ȡView::InspectGPSFile(CString gpsfilename)
{
	GPSSTRUCT gpsdata;
	CString datestr,timestr;
	CString firstdatestr;
	char* datechar[20],*timechar[20];
	FILE *fp=NULL;
	if ((fp=fopen((_bstr_t)gpsfilename,"r"))==NULL)
	{
		AfxMessageBox(_T("��")+gpsfilename+_T("ʧ��"));
		return false;
	}
	//��ȡ�ļ�
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


CString C������ȡView::GetFileNameByPath(CString pathname,bool isWithFileSuffix)
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
