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

// ������ȡDoc.cpp : C������ȡDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "������ȡ.h"
#endif

#include "������ȡDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// C������ȡDoc

IMPLEMENT_DYNCREATE(C������ȡDoc, CDocument)

BEGIN_MESSAGE_MAP(C������ȡDoc, CDocument)
	ON_COMMAND(ID_BUTTON2, &C������ȡDoc::OnButton2)
END_MESSAGE_MAP()


// C������ȡDoc ����/����

C������ȡDoc::C������ȡDoc()
{
	// TODO: �ڴ����һ���Թ������

}

C������ȡDoc::~C������ȡDoc()
{
}

BOOL C������ȡDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// C������ȡDoc ���л�

void C������ȡDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void C������ȡDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �޸Ĵ˴����Ի����ĵ�����
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

// ������������֧��
void C������ȡDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ���ĵ����������������ݡ�
	// ���ݲ���Ӧ�ɡ�;���ָ�

	// ����:  strSearchContent = _T("point;rectangle;circle;ole object;")��
	SetSearchContent(strSearchContent);
}

void C������ȡDoc::SetSearchContent(const CString& value)
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

// C������ȡDoc ���

#ifdef _DEBUG
void C������ȡDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void C������ȡDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// C������ȡDoc ����


void C������ȡDoc::OnButton2()
{
	// TODO: �ڴ���������������
	TCHAR szFilter[] = _T("������������ �ļ���*.DAT)|*.DAT|�����ļ�(*.*)|*.*||");
	CFileDialog FileDlg(TRUE, _T("DAT"),NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL);
	FileDlg.m_ofn.lpstrTitle=_T("�򿪺������������ļ�");

	/*TCHAR szBuf[MAX_PATH];
	ZeroMemory(szBuf,MAX_PATH);
	GetCurrentDirectory(MAX_PATH,szBuf);
	CString currentpath(szBuf);*/
	//���öԻ���ǰ·��
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
