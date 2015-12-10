#include "stdio.h"
#include "math.h"
#include "stdlib.h"
#include "vector"

using namespace std;
#ifndef FORWARD
#define FORWARD   

#define PI 3.141592653
#define GRAV_CONSTANT 6.667		//万有引力常数
//
struct CUBIC_FORWARD_INFO 
{
	//观测系统参数(多模型体的情况下也相同，可共用)
	double xmin,ymin,dx,dy;
	int xnumber,ynumber;			//X方向的点数，Y方向的点数
	double z;						//观测面高度（一般为负数）
	double angle_I0;				//地磁倾角
	double angle_Apie0;				//地磁偏角
	//模型物性、几何参数（考虑了多个模型体的情况）
	vector<double>x1_vector,x2_vector,y1_vector,y2_vector,z1_vector,z2_vector;
	vector<double>m_vector,angle_I_vector,angle_Apie_vector;			//磁的物性参数
	vector<double>m_density_vector;										//重力的物性参数
};
//网格信息
struct MESHINFO
{
	int M,N,L;		//x,y,z方向上的网格个数
	double dx,dy,dz;	//x,y,z方向上的网格宽度
	double initx,inity,initz;	//起始坐标
};
double arctg(double v);

//磁正演
void Cubic_Hax_Forward(CUBIC_FORWARD_INFO cubic_mag_info,double *Mag);
void Cubic_Hay_Forward(CUBIC_FORWARD_INFO cubic_mag_info,double *Mag);
void Cubic_Za_Forward(CUBIC_FORWARD_INFO cubic_mag_info,double *Mag);
void Cubic_daiertaT_Forward(CUBIC_FORWARD_INFO cubic_mag_info,double *Mag);
void Cubic_Modulus_T_Forward(CUBIC_FORWARD_INFO cubic_mag_info,double *Mag);
void WriteForwardDataToGrdFile(CUBIC_FORWARD_INFO datainfo,char* filename,double* mag);
void WriteMagneticForwardDataToGrdFile(CUBIC_FORWARD_INFO datainfo,char* filename,double* z);

//重力及梯度张量正演
void Cubic_Vz_Forward(CUBIC_FORWARD_INFO cubic_grav_info,double *grav);
void Cubic_Vxx_Forward(CUBIC_FORWARD_INFO cubic_grav_info,double *grav);
void Cubic_Vxy_Forward(CUBIC_FORWARD_INFO cubic_grav_info,double *grav);
void Cubic_Vxz_Forward(CUBIC_FORWARD_INFO cubic_grav_info,double *grav);
void Cubic_Vyy_Forward(CUBIC_FORWARD_INFO cubic_grav_info,double *grav);
void Cubic_Vyz_Forward(CUBIC_FORWARD_INFO cubic_grav_info,double *grav);
void Cubic_Vzz_Forward(CUBIC_FORWARD_INFO cubic_grav_info,double *grav);
void Cubic_GravityNSS_Forward(CUBIC_FORWARD_INFO cubic_grav_info,double *grav);//计算公式里有虚数，还没有搞定如何计算NSS！！！
void Cubic_AnalyticSignal_Forward(CUBIC_FORWARD_INFO cubic_grav_info,double **grav);
void WriteGrd(char* filename,double* data,int m,int n,double xmin,double xmax,double ymin,double ymax);
//网格重力正演
void GetMeshInfo(MESHINFO& meshinfo,char* filename);		//读取网格信息文件（meshtool3d小程序生成的文件）
void GetModel(char* filename,double* model,MESHINFO meshinfo);	//读取模型参数文件（meshtools3d程序生成）
void _g_GetKernalMat_fast(MESHINFO meshinfo,double** KernalMat_G);


double Cubic_Vz_Forward(CUBIC_FORWARD_INFO cubic_grav_info,double x,double y);//计算模型体在x，y点处的重力异常值
#endif
