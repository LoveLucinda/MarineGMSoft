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

// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <afxdisp.h>        // MFC �Զ�����



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��


//�����������ݽṹ��
struct OGDataStruct
{
	CString time,day,date,datatimeStr,tempstr;
	double beamposition,VCC,AL,AX,VE,AX2,XACC2,LACC2,CROSSACCEL,LONGACCEL,EOTVOS,LONGITUDE,LATITUDE,VELOCITY,Heading;
	double gravity,spinttension;
	double FGA;//���ɿ��������쳣
	COleDateTime DataTime;
};

struct GRDSTRUCT
{
	int rows,cols;
	double xmin,xmax,ymin,ymax,zmin,zmax;
	double dx,dy;
};
struct GRDRANGEINDEX
{
	int rowmin,rowmax,colmin,colmax;	//�����Χ�ظķ�Χ�ڵ�����ڵ�����
	double x0,y0;						//�������
};

struct BASEPOINTDATASTRUCT
{
	double grav,longitude,latitude,longitude_degree,longitude_minute,latitude_degree,latitude_minute;//��������ֵ����γ��
	double reading,reading_corrected;					//�ȶԻ���ʱ������ԭʼ���������㵽�����Ķ���
	double height_r,height_l;		//���Ҽа嵽ˮ���
	double HeightToDeck;			//�����ǵ��Ӱ��
	double HeightToBasePoint;		//������������߶�
	double Heitht_BaseToSea;		//���㵽ˮ���
	double DistanceToBasePoint;		//�ȶԻ��㴦�����֮����ϱ������
	COleDateTime datetime;			//�ȶԻ��������ʱ��
	CString datatimestr;			//�����ַ���
	char datetimechar[50];			//����
};

struct GPSSTRUCT
{
	//COleDateTime DataTime;
	CString datatimeStr;
	//GPGAA��ȫ��λ����
	//double heightTosea;			//���ߵ���ƽ��ĸ߶ȣ�������ȷ���������뺣���˲ʱ���룩(m)
	//double GeoidalHeight;		//���ˮ׼���
	//GPVTG:�����ٶ���Ϣ
	
	//GPRMC�����䶨λ����
	double longitude,latitude;
	double Heading;							//���汱Ϊ��׼�ĺ���
	double velocity;						//�Խ�Ϊ��λ�ĺ���
	//char flag_GPSstatus;					//��λ�Ƿ���Ч��A��Ч��V��Ч
	//GPGLL�����������Ϣ
	//GPZDA��UTCʱ�������

};
//����ȷ��ͼ����һ�����������ݵ��������
struct POINTINDEXSTRUCT
{
	bool PointExit;			//ĳ�����ص����Ƿ���������ݵ㣬�����»���ͼ��ʱ��ֵ
	int IndexInOGdataVector;//�����ص���������������ݵ㣬�������������е�����
};

//�������Щ��Ϣ��Ŷ����ڶ��߳���ʹ��
#define WM_PROGRESS_MESSAGE WM_USER+1		//���½�����״̬
#define WM_PROGRESS_END		WM_USER+2		//
#define WM_PROGRESS_Range		WM_USER+3	//���ý�������Χ
#define WM_UpdatePar		WM_USER+4		//���²�������ȡ���ļ���ִ�У�
#define WM_GetDistance		WM_USER+5		//��ȡ���߳���
#define WM_UpdateFigure		WM_USER+6		//����ͼ��

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


