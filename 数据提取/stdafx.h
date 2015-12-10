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

// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持


//海洋重力数据结构体
struct OGDataStruct
{
	CString time,day,date,datatimeStr,tempstr;
	double beamposition,VCC,AL,AX,VE,AX2,XACC2,LACC2,CROSSACCEL,LONGACCEL,EOTVOS,LONGITUDE,LATITUDE,VELOCITY,Heading;
	double gravity,spinttension;
	double FGA;//自由空气重力异常
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
	int rowmin,rowmax,colmin,colmax;	//测点周围地改范围内的网格节点索引
	double x0,y0;						//测点坐标
};

struct BASEPOINTDATASTRUCT
{
	double grav,longitude,latitude,longitude_degree,longitude_minute,latitude_degree,latitude_minute;//绝对重力值，经纬度
	double reading,reading_corrected;					//比对基点时重力仪原始读数，归算到基点后的读数
	double height_r,height_l;		//左右夹板到水面高
	double HeightToDeck;			//重力仪到加班高
	double HeightToBasePoint;		//重力仪至基点高度
	double Heitht_BaseToSea;		//基点到水面高
	double DistanceToBasePoint;		//比对基点处与基点之间的南北向距离
	COleDateTime datetime;			//比对基点的日期时间
	CString datatimestr;			//日期字符串
	char datetimechar[50];			//日期
};

struct GPSSTRUCT
{
	//COleDateTime DataTime;
	CString datatimeStr;
	//GPGAA：全球定位数据
	//double heightTosea;			//天线到海平面的高度（可用于确定重力仪与海面的瞬时距离）(m)
	//double GeoidalHeight;		//大地水准面高
	//GPVTG:地面速度信息
	
	//GPRMC：运输定位数据
	double longitude,latitude;
	double Heading;							//以真北为基准的航向
	double velocity;						//以节为单位的航速
	//char flag_GPSstatus;					//定位是否有效：A有效，V无效
	//GPGLL：大地坐标信息
	//GPZDA：UTC时间和日期

};
//用于确定图像中一个区域内数据点的索引，
struct POINTINDEXSTRUCT
{
	bool PointExit;			//某个像素点上是否绘制了数据点，在重新绘制图像时幅值
	int IndexInOGdataVector;//此像素点上如果绘制了数据点，它在数据向量中的索引
};

//下面的这些消息编号都是在多线程中使用
#define WM_PROGRESS_MESSAGE WM_USER+1		//更新进度条状态
#define WM_PROGRESS_END		WM_USER+2		//
#define WM_PROGRESS_Range		WM_USER+3	//设置进度条范围
#define WM_UpdatePar		WM_USER+4		//更新参数（读取完文件后执行）
#define WM_GetDistance		WM_USER+5		//获取航线长度
#define WM_UpdateFigure		WM_USER+6		//更新图像

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


