#include "stdio.h"
#include "math.h"
#include "stdlib.h"
#include "vector"

using namespace std;
#ifndef FORWARD
#define FORWARD   

#define PI 3.141592653
#define GRAV_CONSTANT 6.667		//������������
//
struct CUBIC_FORWARD_INFO 
{
	//�۲�ϵͳ����(��ģ����������Ҳ��ͬ���ɹ���)
	double xmin,ymin,dx,dy;
	int xnumber,ynumber;			//X����ĵ�����Y����ĵ���
	double z;						//�۲���߶ȣ�һ��Ϊ������
	double angle_I0;				//�ش����
	double angle_Apie0;				//�ش�ƫ��
	//ģ�����ԡ����β����������˶��ģ����������
	vector<double>x1_vector,x2_vector,y1_vector,y2_vector,z1_vector,z2_vector;
	vector<double>m_vector,angle_I_vector,angle_Apie_vector;			//�ŵ����Բ���
	vector<double>m_density_vector;										//���������Բ���
};
//������Ϣ
struct MESHINFO
{
	int M,N,L;		//x,y,z�����ϵ��������
	double dx,dy,dz;	//x,y,z�����ϵ�������
	double initx,inity,initz;	//��ʼ����
};
double arctg(double v);

//������
void Cubic_Hax_Forward(CUBIC_FORWARD_INFO cubic_mag_info,double *Mag);
void Cubic_Hay_Forward(CUBIC_FORWARD_INFO cubic_mag_info,double *Mag);
void Cubic_Za_Forward(CUBIC_FORWARD_INFO cubic_mag_info,double *Mag);
void Cubic_daiertaT_Forward(CUBIC_FORWARD_INFO cubic_mag_info,double *Mag);
void Cubic_Modulus_T_Forward(CUBIC_FORWARD_INFO cubic_mag_info,double *Mag);
void WriteForwardDataToGrdFile(CUBIC_FORWARD_INFO datainfo,char* filename,double* mag);
void WriteMagneticForwardDataToGrdFile(CUBIC_FORWARD_INFO datainfo,char* filename,double* z);

//�������ݶ���������
void Cubic_Vz_Forward(CUBIC_FORWARD_INFO cubic_grav_info,double *grav);
void Cubic_Vxx_Forward(CUBIC_FORWARD_INFO cubic_grav_info,double *grav);
void Cubic_Vxy_Forward(CUBIC_FORWARD_INFO cubic_grav_info,double *grav);
void Cubic_Vxz_Forward(CUBIC_FORWARD_INFO cubic_grav_info,double *grav);
void Cubic_Vyy_Forward(CUBIC_FORWARD_INFO cubic_grav_info,double *grav);
void Cubic_Vyz_Forward(CUBIC_FORWARD_INFO cubic_grav_info,double *grav);
void Cubic_Vzz_Forward(CUBIC_FORWARD_INFO cubic_grav_info,double *grav);
void Cubic_GravityNSS_Forward(CUBIC_FORWARD_INFO cubic_grav_info,double *grav);//���㹫ʽ������������û�и㶨��μ���NSS������
void Cubic_AnalyticSignal_Forward(CUBIC_FORWARD_INFO cubic_grav_info,double **grav);
void WriteGrd(char* filename,double* data,int m,int n,double xmin,double xmax,double ymin,double ymax);
//������������
void GetMeshInfo(MESHINFO& meshinfo,char* filename);		//��ȡ������Ϣ�ļ���meshtool3dС�������ɵ��ļ���
void GetModel(char* filename,double* model,MESHINFO meshinfo);	//��ȡģ�Ͳ����ļ���meshtools3d�������ɣ�
void _g_GetKernalMat_fast(MESHINFO meshinfo,double** KernalMat_G);


double Cubic_Vz_Forward(CUBIC_FORWARD_INFO cubic_grav_info,double x,double y);//����ģ������x��y�㴦�������쳣ֵ
#endif
