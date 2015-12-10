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

// 数据提取Doc.cpp : C数据提取Doc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "数据提取.h"
#endif

#include "数据提取Doc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// C数据提取Doc

IMPLEMENT_DYNCREATE(C数据提取Doc, CDocument)

BEGIN_MESSAGE_MAP(C数据提取Doc, CDocument)
	ON_COMMAND(ID_BUTTON2, &C数据提取Doc::OnButton2)
END_MESSAGE_MAP()


// C数据提取Doc 构造/析构

C数据提取Doc::C数据提取Doc()
{
	// TODO: 在此添加一次性构造代码

}

C数据提取Doc::~C数据提取Doc()
{
}

BOOL C数据提取Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// C数据提取Doc 序列化

void C数据提取Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void C数据提取Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void C数据提取Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:  strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void C数据提取Doc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// C数据提取Doc 诊断

#ifdef _DEBUG
void C数据提取Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void C数据提取Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// C数据提取Doc 命令


void C数据提取Doc::OnButton2()
{
	// TODO: 在此添加命令处理程序代码
	TCHAR szFilter[] = _T("海洋重力数据 文件（*.DAT)|*.DAT|所有文件(*.*)|*.*||");
	CFileDialog FileDlg(TRUE, _T("DAT"),NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL);
	FileDlg.m_ofn.lpstrTitle=_T("打开海洋重力数据文件");

	/*TCHAR szBuf[MAX_PATH];
	ZeroMemory(szBuf,MAX_PATH);
	GetCurrentDirectory(MAX_PATH,szBuf);
	CString currentpath(szBuf);*/
	//设置对话框当前路径
	//FileDlg.GetOFN().lpstrInitialDir =currentpath;
	CString DataFileName,NewDataFileName;
	if (IDOK == FileDlg.DoModal())
	{   
		DataFileName = FileDlg.GetPathName();  
	}
	else
	{
		return;
	}
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
	char tempchar[200],symbol[20];
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
