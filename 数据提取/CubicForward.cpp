#include "stdafx.h"
#include "CubicForward.h"

/*/////////////////////////////////////////////////////////////////////////////////////////////////////////
������ƣ���־ظ��2014.8���й����ʴ�ѧ���人), ��̽¥511

���ܣ����Cpp������Ҫ�����ݳ������������Vz��Vxx��Vxy��Vxz��Vyy��Vyz��Vzz���Լ��ų���Hax��Hay��Za����T��Taģ����

�������ݣ�ͨ�������ṹ�崫��ģ�������Բ����ͼ��β����Լ��۲�ϵͳ���������ݽ��ͨ��ָ���������
		����һ�μ�����ģ�͵��ӵ��������ͬģ����Ĳ���ͨ�������ṹ�������Ԫ�ش���

�ο����ף���־����2009���شų���ŷ���̽�����ʳ����磬120ҳ
		  ����Ȼ��2012�����������ݶ�������ά����Լ�������о���2.2��
/////////////////////////////////////////////////////////////////////////////////////////////////////////*/


double arctg(double v)
{
	double ang;
	if(v>=0)
	{
		ang=atan(v);
	}
	else if(v<0)
	{
		ang=atan(v)+3.1415926535897932384626433832795;
	}
	return ang;
}
//Hax���㹫ʽ���־����Ľ̲��ϵĲ�һ�£�arctan��ǰ��û�и��ţ����Ϲ�ʽ����
void Cubic_Hax_Forward(CUBIC_FORWARD_INFO cubic_mag_info,double *Mag)
{
	
	//�۲�ϵͳ����
	double dx=cubic_mag_info.dx,dy=cubic_mag_info.dy;
	int xnumber=cubic_mag_info.xnumber,ynumber=cubic_mag_info.ynumber;
	double z=cubic_mag_info.z;
	//��ʼ��Ϊ0
	for (int i = 0; i < xnumber*ynumber; i++)
	{
		Mag[i]=0.0;
	}
	for (int k = 0; k < cubic_mag_info.x1_vector.size(); k++)
	{
		//ģ�Ͳ���
		double x1=cubic_mag_info.x1_vector[k],x2=cubic_mag_info.x2_vector[k],y1=cubic_mag_info.y1_vector[k],y2=cubic_mag_info.y2_vector[k],z1=cubic_mag_info.z1_vector[k],z2=cubic_mag_info.z2_vector[k];
		double angle_I=cubic_mag_info.angle_I_vector[k];						//����ǣ��ȣ�
		double angle_Apie=cubic_mag_info.angle_Apie_vector[k];					//��ƫ�ǣ��ȣ�
		double m=cubic_mag_info.m_vector[k];									//�Ż�ǿ�ȣ�A/m��
		
		//��������
		angle_I=angle_I/180.0*PI;												//����Ϊ����
		angle_Apie=angle_Apie/180.0*PI;											//����Ϊ����
		double mx=m*cos(angle_I)*cos(angle_Apie);
		double my=m*cos(angle_I)*sin(angle_Apie);
		double mz=m*sin(angle_I);
		//�м����
		double tempx1,tempx2,tempy1,tempy2,G122,G121,G112,G111,G222,G221,G212,G211;
		double p222,p221,p212,p211,p122,p121,p112,p111;							//��x��y��z��˳������
		double p2,p1;
		p2=pow(z2-z,2);															//X�������ʱ���ǳ���
		p1=pow(z1-z,2);
	
		int i;
		double x,y;
		//ѭ������
		for (i=0;i<xnumber;i++)
		{
			x=cubic_mag_info.xmin+i*dx;
			for (int j=0;j<ynumber;j++)
			{
			
				y=cubic_mag_info.ymin+j*dy;
				tempx1=x1-x;													//���������
				tempx2=x2-x;
			
				tempy1=y1-y;
				tempy2=y2-y;
				//tempy1=-20;tempy2=20;
			
				p222=sqrt(pow(tempx2,2)+pow(tempy2,2)+p2);						//����
				p221=sqrt(pow(tempx2,2)+pow(tempy2,2)+p1);
				p212=sqrt(pow(tempx2,2)+pow(tempy1,2)+p2);
				p211=sqrt(pow(tempx2,2)+pow(tempy1,2)+p1);
			
				p122=sqrt(pow(tempx1,2)+pow(tempy2,2)+p2);						//����
				p121=sqrt(pow(tempx1,2)+pow(tempy2,2)+p1);
				p112=sqrt(pow(tempx1,2)+pow(tempy1,2)+p2);
				p111=sqrt(pow(tempx1,2)+pow(tempy1,2)+p1);
			
				G222=mx*atan2((x2-x)*(y2-y),pow((x2-x),2)+p222*(z2-z)+pow(z2-z,2))+my*log(p222+z2-z)+mz*log(p222+y2-y);		//������
				G221=mx*atan2((x2-x)*(y2-y),pow((x2-x),2)+p221*(z1-z)+pow(z1-z,2))+my*log(p221+z1-z)+mz*log(p221+y2-y);
				G212=mx*atan2((x2-x)*(y1-y),pow((x2-x),2)+p212*(z2-z)+pow(z2-z,2))+my*log(p212+z2-z)+mz*log(p212+y1-y);
				G211=mx*atan2((x2-x)*(y1-y),pow((x2-x),2)+p211*(z1-z)+pow(z1-z,2))+my*log(p211+z1-z)+mz*log(p211+y1-y);
			
				G122=mx*atan2((x1-x)*(y2-y),pow((x1-x),2)+p122*(z2-z)+pow(z2-z,2))+my*log(p122+z2-z)+mz*log(p122+y2-y);		//������
				G121=mx*atan2((x1-x)*(y2-y),pow((x1-x),2)+p121*(z1-z)+pow(z1-z,2))+my*log(p121+z1-z)+mz*log(p121+y2-y);
				G112=mx*atan2((x1-x)*(y1-y),pow((x1-x),2)+p112*(z2-z)+pow(z2-z,2))+my*log(p112+z2-z)+mz*log(p112+y1-y);
				G111=mx*atan2((x1-x)*(y1-y),pow((x1-x),2)+p111*(z1-z)+pow(z1-z,2))+my*log(p111+z1-z)+mz*log(p111+y1-y);
			
				Mag[j+i*ynumber]+=100*(G222+G211+G121+G112-G221-G212-G122-G111);			//nT
			}
		}
	}
}

//Hay���㹫ʽ���־����Ľ̲��ϵĲ�һ�£�arctan��ǰ��û�и��ţ����Ϲ�ʽ����
void Cubic_Hay_Forward(CUBIC_FORWARD_INFO cubic_mag_info,double *Mag)
{
	//�۲�ϵͳ����
	double dx=cubic_mag_info.dx,dy=cubic_mag_info.dy;
	int xnumber=cubic_mag_info.xnumber,ynumber=cubic_mag_info.ynumber;
	double z=cubic_mag_info.z;
	//��ʼ��Ϊ0
	for (int i = 0; i < xnumber*ynumber; i++)
	{
		Mag[i]=0.0;
	}
	for (int k = 0; k < cubic_mag_info.x1_vector.size(); k++)
	{
		//ģ�Ͳ���
		double x1=cubic_mag_info.x1_vector[k],x2=cubic_mag_info.x2_vector[k],y1=cubic_mag_info.y1_vector[k],y2=cubic_mag_info.y2_vector[k],
			z1=cubic_mag_info.z1_vector[k],z2=cubic_mag_info.z2_vector[k];
		double angle_I=cubic_mag_info.angle_I_vector[k];						//����ǣ��ȣ�
		double angle_Apie=cubic_mag_info.angle_Apie_vector[k];					//��ƫ��
		double m=cubic_mag_info.m_vector[k];									//�Ż�ǿ��
		
		//��������
		angle_I=angle_I/180.0*PI;												//����Ϊ����
		angle_Apie=angle_Apie/180.0*PI;											//����Ϊ����
		double mx=m*cos(angle_I)*cos(angle_Apie);
		double my=m*cos(angle_I)*sin(angle_Apie);
		double mz=m*sin(angle_I);
		//�м����
		double tempx1,tempx2,tempy1,tempy2,G122,G121,G112,G111,G222,G221,G212,G211;
		double p222,p221,p212,p211,p122,p121,p112,p111;							//��x��y��z��˳������
		double p2,p1;
		p2=pow(z2-z,2);															//X�������ʱ���ǳ���
		p1=pow(z1-z,2);
	
		int i;
		double x,y;
		//ѭ������
		for (i=0;i<xnumber;i++)
		{
			x=cubic_mag_info.xmin+i*dx;
			for (int j=0;j<ynumber;j++)
			{
			
				y=cubic_mag_info.ymin+j*dy;
				tempx1=x1-x;													//���������
				tempx2=x2-x;
			
				tempy1=y1-y;
				tempy2=y2-y;
				//tempy1=-20;tempy2=20;
			
				p222=sqrt(pow(tempx2,2)+pow(tempy2,2)+p2);		//����
				p221=sqrt(pow(tempx2,2)+pow(tempy2,2)+p1);
				p212=sqrt(pow(tempx2,2)+pow(tempy1,2)+p2);
				p211=sqrt(pow(tempx2,2)+pow(tempy1,2)+p1);
			
				p122=sqrt(pow(tempx1,2)+pow(tempy2,2)+p2);		//����
				p121=sqrt(pow(tempx1,2)+pow(tempy2,2)+p1);
				p112=sqrt(pow(tempx1,2)+pow(tempy1,2)+p2);
				p111=sqrt(pow(tempx1,2)+pow(tempy1,2)+p1);
			
				G222=my*atan2((x2-x)*(y2-y),pow((y2-y),2)+p222*(z2-z)+pow(z2-z,2))+mx*log(p222+z2-z)+mz*log(p222+x2-x);		//������
				G221=my*atan2((x2-x)*(y2-y),pow((y2-y),2)+p221*(z1-z)+pow(z1-z,2))+mx*log(p221+z1-z)+mz*log(p221+x2-x);
				G212=my*atan2((x2-x)*(y1-y),pow((y1-y),2)+p212*(z2-z)+pow(z2-z,2))+mx*log(p212+z2-z)+mz*log(p212+x2-x);
				G211=my*atan2((x2-x)*(y1-y),pow((y1-y),2)+p211*(z1-z)+pow(z1-z,2))+mx*log(p211+z1-z)+mz*log(p211+x2-x);
			
				G122=my*atan2((x1-x)*(y2-y),pow((y2-y),2)+p122*(z2-z)+pow(z2-z,2))+mx*log(p122+z2-z)+mz*log(p122+x1-x);		//������
				G121=my*atan2((x1-x)*(y2-y),pow((y2-y),2)+p121*(z1-z)+pow(z1-z,2))+mx*log(p121+z1-z)+mz*log(p121+x1-x);
				G112=my*atan2((x1-x)*(y1-y),pow((y1-y),2)+p112*(z2-z)+pow(z2-z,2))+mx*log(p112+z2-z)+mz*log(p112+x1-x);
				G111=my*atan2((x1-x)*(y1-y),pow((y1-y),2)+p111*(z1-z)+pow(z1-z,2))+mx*log(p111+z1-z)+mz*log(p111+x1-x);
			
				Mag[j+i*ynumber]+=100*(G222+G211+G121+G112-G221-G212-G122-G111);			//nT
			}
		}
	}
}

//����Za
void Cubic_Za_Forward(CUBIC_FORWARD_INFO cubic_mag_info,double *Mag)
{
	//�۲�ϵͳ����
	double dx=cubic_mag_info.dx,dy=cubic_mag_info.dy;
	int xnumber=cubic_mag_info.xnumber,ynumber=cubic_mag_info.ynumber;
	double z=cubic_mag_info.z;
	//��ʼ��Ϊ0
	for (int i = 0; i < xnumber*ynumber; i++)
	{
		Mag[i]=0.0;
	}
	for (int k = 0; k < cubic_mag_info.x1_vector.size(); k++)
	{
		//ģ�Ͳ���
		double x1=cubic_mag_info.x1_vector[k],x2=cubic_mag_info.x2_vector[k],y1=cubic_mag_info.y1_vector[k],y2=cubic_mag_info.y2_vector[k],
			z1=cubic_mag_info.z1_vector[k],z2=cubic_mag_info.z2_vector[k];
		double angle_I=cubic_mag_info.angle_I_vector[k];						//����ǣ��ȣ�
		double angle_Apie=cubic_mag_info.angle_Apie_vector[k];				//��ƫ��
		double m=cubic_mag_info.m_vector[k];									//�Ż�ǿ��
		
		//��������
		angle_I=angle_I/180.0*PI;				//����Ϊ����
		angle_Apie=angle_Apie/180.0*PI;		//����Ϊ����
		double mx=m*cos(angle_I)*cos(angle_Apie);
		double my=m*cos(angle_I)*sin(angle_Apie);
		double mz=m*sin(angle_I);
		//�м����
		double tempx1,tempx2,tempy1,tempy2,G122,G121,G112,G111,G222,G221,G212,G211;
		double p222,p221,p212,p211,p122,p121,p112,p111;		//��x��y��z��˳������
		double p2,p1;
		p2=pow(z2-z,2);	//X�������ʱ���ǳ���
		p1=pow(z1-z,2);
	
		int i;
		double x,y;
		//ѭ������
		for (i=0;i<xnumber;i++)
		{
			x=cubic_mag_info.xmin+i*dx;
			for (int j=0;j<ynumber;j++)
			{
			
				y=cubic_mag_info.ymin+j*dy;
				tempx1=x1-x;		//���������
				tempx2=x2-x;
			
				tempy1=y1-y;
				tempy2=y2-y;
				//tempy1=-20;tempy2=20;
			
				p222=sqrt(pow(tempx2,2)+pow(tempy2,2)+p2);		//����
				p221=sqrt(pow(tempx2,2)+pow(tempy2,2)+p1);
				p212=sqrt(pow(tempx2,2)+pow(tempy1,2)+p2);
				p211=sqrt(pow(tempx2,2)+pow(tempy1,2)+p1);
			
				p122=sqrt(pow(tempx1,2)+pow(tempy2,2)+p2);		//����
				p121=sqrt(pow(tempx1,2)+pow(tempy2,2)+p1);
				p112=sqrt(pow(tempx1,2)+pow(tempy1,2)+p2);
				p111=sqrt(pow(tempx1,2)+pow(tempy1,2)+p1);
			
				G222=mx*log(p222+y2-y)+my*log(p222+x2-x)-mz*atan2((x2-x)*(y2-y),p222*(z2-z));		//������
				G221=mx*log(p221+y2-y)+my*log(p221+x2-x)-mz*atan2((x2-x)*(y2-y),p221*(z1-z));
				G212=mx*log(p212+y1-y)+my*log(p212+x2-x)-mz*atan2((x2-x)*(y1-y),p212*(z2-z));
				G211=mx*log(p211+y1-y)+my*log(p211+x2-x)-mz*atan2((x2-x)*(y1-y),p211*(z1-z));
			
				G122=mx*log(p122+y2-y)+my*log(p122+x1-x)-mz*atan2((x1-x)*(y2-y),p122*(z2-z));		//������
				G121=mx*log(p121+y2-y)+my*log(p121+x1-x)-mz*atan2((x1-x)*(y2-y),p121*(z1-z));
				G112=mx*log(p112+y1-y)+my*log(p112+x1-x)-mz*atan2((x1-x)*(y1-y),p112*(z2-z));
				G111=mx*log(p111+y1-y)+my*log(p111+x1-x)-mz*atan2((x1-x)*(y1-y),p111*(z1-z));
			
				Mag[j+i*ynumber]+=100*(G222+G211+G121+G112-G221-G212-G122-G111);			//nT
			}
		}
	}
}

//����Taģ��
void Cubic_Modulus_T_Forward(CUBIC_FORWARD_INFO cubic_mag_info,double *Mag)
{
	double angle_I=cubic_mag_info.angle_I0;						//�ش���ǣ��ȣ�
	double angle_Apie=cubic_mag_info.angle_Apie0;				//�ش�ƫ�ǣ��ȣ�
	//��������
	angle_I=angle_I/180.0*PI;									//����Ϊ����
	angle_Apie=angle_Apie/180.0*PI;								//����Ϊ����
	//���붯̬�������쳣ֵ
	double *Hax=new double[cubic_mag_info.xnumber*cubic_mag_info.ynumber];
	double *Hay=new double[cubic_mag_info.xnumber*cubic_mag_info.ynumber];
	double *Za=new double[cubic_mag_info.xnumber*cubic_mag_info.ynumber];

	//����hax��hay��za�����ļ��㺯��
	Cubic_Hax_Forward(cubic_mag_info,Hax);
	Cubic_Hay_Forward(cubic_mag_info,Hay);
	Cubic_Za_Forward(cubic_mag_info,Za);
	//����Taģ��
	for (int i = 0; i < cubic_mag_info.xnumber*cubic_mag_info.ynumber; i++)
	{
		Mag[i]=sqrt(pow(Hax[i],2)+pow(Hay[i],2)+pow(Za[i],2));
	}

	//�ͷ���Դ
	delete Hax,Hay,Za;
}

//���ݡ�T
void Cubic_daiertaT_Forward(CUBIC_FORWARD_INFO cubic_mag_info,double *Mag)
{
	double angle_I=cubic_mag_info.angle_I0;						//�ش���ǣ��ȣ�
	double angle_Apie=cubic_mag_info.angle_Apie0;				//�ش�ƫ�ǣ��ȣ�
	//��������
	angle_I=angle_I/180.0*PI;									//����Ϊ����
	angle_Apie=angle_Apie/180.0*PI;								//����Ϊ����
	//���붯̬�������쳣ֵ
	double *Hax=new double[cubic_mag_info.xnumber*cubic_mag_info.ynumber];
	double *Hay=new double[cubic_mag_info.xnumber*cubic_mag_info.ynumber];
	double *Za=new double[cubic_mag_info.xnumber*cubic_mag_info.ynumber];

	//����hax��hay��za�����ļ��㺯��
	Cubic_Hax_Forward(cubic_mag_info,Hax);
	Cubic_Hay_Forward(cubic_mag_info,Hay);
	Cubic_Za_Forward(cubic_mag_info,Za);
	//������쳣
	for (int i = 0; i < cubic_mag_info.xnumber*cubic_mag_info.ynumber; i++)
	{
		Mag[i]=Hax[i]*cos(angle_I)*cos(angle_Apie)+Hay[i]*cos(angle_I)*sin(angle_Apie)+Za[i]*sin(angle_I);
	}

	//�ͷ���Դ
	delete Hax,Hay,Za;
}
//����ģ������x��y���������쳣ֵ
double Cubic_Vz_Forward(CUBIC_FORWARD_INFO cubic_grav_info,double x,double y)
{
	double grav=0;
	//�۲�ϵͳ����
	
	double z=cubic_grav_info.z;
	for (int k = 0; k < cubic_grav_info.x1_vector.size(); k++)
	{
		//ģ�Ͳ���
		double x1=cubic_grav_info.x1_vector[k],x2=cubic_grav_info.x2_vector[k],y1=cubic_grav_info.y1_vector[k],y2=cubic_grav_info.y2_vector[k],
			z1=cubic_grav_info.z1_vector[k]-z,z2=cubic_grav_info.z2_vector[k]-z;
		double density=cubic_grav_info.m_density_vector[k];
		//�м����
		double tempx1,tempx2,tempy1,tempy2,G122,G121,G112,G111,G222,G221,G212,G211;
		double p222,p221,p212,p211,p122,p121,p112,p111;		//��x��y��z��˳������
		double p2,p1;
		p2=z2*z2;	//X�������ʽ���ǳ���
		p1=z1*z1;

		tempx1=x1-x;		//���������
		tempx2=x2-x;
		
		tempy1=y1-y;
		tempy2=y2-y;
		//tempy1=-20;tempy2=20;
			
		p222=sqrt(pow(tempx2,2)+pow(tempy2,2)+p2);		//����
		p221=sqrt(pow(tempx2,2)+pow(tempy2,2)+p1);
		p212=sqrt(pow(tempx2,2)+pow(tempy1,2)+p2);
		p211=sqrt(pow(tempx2,2)+pow(tempy1,2)+p1);
			
		p122=sqrt(pow(tempx1,2)+pow(tempy2,2)+p2);		//����
		p121=sqrt(pow(tempx1,2)+pow(tempy2,2)+p1);
		p112=sqrt(pow(tempx1,2)+pow(tempy1,2)+p2);
		p111=sqrt(pow(tempx1,2)+pow(tempy1,2)+p1);
			
		G222=tempx2*log(tempy2+p222)+tempy2*log(tempx2+p222)+z2*atan2((z2*p222),(tempx2*tempy2));		//������
		G221=tempx2*log(tempy2+p221)+tempy2*log(tempx2+p221)+z1*atan2((z1*p221),(tempx2*tempy2));
		G212=tempx2*log(tempy1+p212)+tempy1*log(tempx2+p212)+z2*atan2((z2*p212),(tempx2*tempy1));
		G211=tempx2*log(tempy1+p211)+tempy1*log(tempx2+p211)+z1*atan2((z1*p211),(tempx2*tempy1));
			
		G122=tempx1*log(tempy2+p122)+tempy2*log(tempx1+p122)+z2*atan2((z2*p122),(tempx1*tempy2));
		G121=tempx1*log(tempy2+p121)+tempy2*log(tempx1+p121)+z1*atan2((z1*p121),(tempx1*tempy2));
		G112=tempx1*log(tempy1+p112)+tempy1*log(tempx1+p112)+z2*atan2((z2*p112),(tempx1*tempy1));
		G111=tempx1*log(tempy1+p111)+tempy1*log(tempx1+p111)+z1*atan2((z1*p111),(tempx1*tempy1));
			
		grav+=-6.67*0.001*density*(G222+G211+G121+G112-G221-G212-G122-G111);	//mGal
	}
	return grav;
}
//����Vz
void Cubic_Vz_Forward(CUBIC_FORWARD_INFO cubic_grav_info,double *grav)
{
	//�۲�ϵͳ����
	double dx=cubic_grav_info.dx,dy=cubic_grav_info.dy;
	int xnumber=cubic_grav_info.xnumber,ynumber=cubic_grav_info.ynumber;
	double z=cubic_grav_info.z;
	//��ʼ��Ϊ0
	for (int i = 0; i < xnumber*ynumber; i++)
	{
		grav[i]=0.0;
	}
	for (int k = 0; k < cubic_grav_info.x1_vector.size(); k++)
	{
		//ģ�Ͳ���
		double x1=cubic_grav_info.x1_vector[k],x2=cubic_grav_info.x2_vector[k],y1=cubic_grav_info.y1_vector[k],y2=cubic_grav_info.y2_vector[k],
			z1=cubic_grav_info.z1_vector[k]-z,z2=cubic_grav_info.z2_vector[k]-z;
		double density=cubic_grav_info.m_density_vector[k];
		//�м����
		double tempx1,tempx2,tempy1,tempy2,G122,G121,G112,G111,G222,G221,G212,G211;
		double p222,p221,p212,p211,p122,p121,p112,p111;		//��x��y��z��˳������
		double p2,p1;
		p2=z2*z2;	//X�������ʽ���ǳ���
		p1=z1*z1;
		int i;
		double x,y;
		for (i=0;i<ynumber;i++)
		{
			for (int j=0;j<xnumber;j++)
			{
				y=cubic_grav_info.ymin+i*dy;
				x=cubic_grav_info.xmin+j*dy;;
				tempx1=x1-x;		//���������
				tempx2=x2-x;
		
				tempy1=y1-y;
				tempy2=y2-y;
				//tempy1=-20;tempy2=20;
			
				p222=sqrt(pow(tempx2,2)+pow(tempy2,2)+p2);		//����
				p221=sqrt(pow(tempx2,2)+pow(tempy2,2)+p1);
				p212=sqrt(pow(tempx2,2)+pow(tempy1,2)+p2);
				p211=sqrt(pow(tempx2,2)+pow(tempy1,2)+p1);
			
				p122=sqrt(pow(tempx1,2)+pow(tempy2,2)+p2);		//����
				p121=sqrt(pow(tempx1,2)+pow(tempy2,2)+p1);
				p112=sqrt(pow(tempx1,2)+pow(tempy1,2)+p2);
				p111=sqrt(pow(tempx1,2)+pow(tempy1,2)+p1);
			
				G222=tempx2*log(tempy2+p222)+tempy2*log(tempx2+p222)+z2*atan2((z2*p222),(tempx2*tempy2));		//������
				G221=tempx2*log(tempy2+p221)+tempy2*log(tempx2+p221)+z1*atan2((z1*p221),(tempx2*tempy2));
				G212=tempx2*log(tempy1+p212)+tempy1*log(tempx2+p212)+z2*atan2((z2*p212),(tempx2*tempy1));
				G211=tempx2*log(tempy1+p211)+tempy1*log(tempx2+p211)+z1*atan2((z1*p211),(tempx2*tempy1));
			
				G122=tempx1*log(tempy2+p122)+tempy2*log(tempx1+p122)+z2*atan2((z2*p122),(tempx1*tempy2));
				G121=tempx1*log(tempy2+p121)+tempy2*log(tempx1+p121)+z1*atan2((z1*p121),(tempx1*tempy2));
				G112=tempx1*log(tempy1+p112)+tempy1*log(tempx1+p112)+z2*atan2((z2*p112),(tempx1*tempy1));
				G111=tempx1*log(tempy1+p111)+tempy1*log(tempx1+p111)+z1*atan2((z1*p111),(tempx1*tempy1));
			
				grav[j+i*xnumber]+=-6.67*0.001*density*(G222+G211+G121+G112-G221-G212-G122-G111);	//mGal
			}
		}
	}
}

//����Vxx
void Cubic_Vxx_Forward(CUBIC_FORWARD_INFO cubic_grav_info,double *grav)
{
	//�۲�ϵͳ����
	double dx=cubic_grav_info.dx,dy=cubic_grav_info.dy;
	int xnumber=cubic_grav_info.xnumber,ynumber=cubic_grav_info.ynumber;
	double z=cubic_grav_info.z;
	//��ʼ��Ϊ0
	for (int i = 0; i < xnumber*ynumber; i++)
	{
		grav[i]=0.0;
	}
	for (int k = 0; k < cubic_grav_info.x1_vector.size(); k++)
	{
		//ģ�Ͳ���
		double x1=cubic_grav_info.x1_vector[k],x2=cubic_grav_info.x2_vector[k],y1=cubic_grav_info.y1_vector[k],y2=cubic_grav_info.y2_vector[k],
			z1=cubic_grav_info.z1_vector[k],z2=cubic_grav_info.z2_vector[k];
		double density=cubic_grav_info.m_density_vector[k];
		//�м����
		double tempx1,tempx2,tempy1,tempy2,G122,G121,G112,G111,G222,G221,G212,G211;
		double p222,p221,p212,p211,p122,p121,p112,p111;		//��x��y��z��˳������
		double tempz1=z1-z,tempz2=z2-z;
		double p2,p1;
		p2=pow(tempz2,2.0);	//X�������ʽ���ǳ���
		p1=pow(tempz1,2.0);

		int i;
		double x,y;
		for (i=0;i<ynumber;i++)
		{
			for (int j=0;j<xnumber;j++)
			{
				y=cubic_grav_info.ymin+i*dy;
				x=cubic_grav_info.xmin+j*dy;;
				tempx1=x1-x;		//���������
				tempx2=x2-x;
		
				tempy1=y1-y;
				tempy2=y2-y;
				//tempy1=-20;tempy2=20;
			
				p222=sqrt(pow(tempx2,2)+pow(tempy2,2)+p2);		//����
				p221=sqrt(pow(tempx2,2)+pow(tempy2,2)+p1);
				p212=sqrt(pow(tempx2,2)+pow(tempy1,2)+p2);
				p211=sqrt(pow(tempx2,2)+pow(tempy1,2)+p1);
			
				p122=sqrt(pow(tempx1,2)+pow(tempy2,2)+p2);		//����
				p121=sqrt(pow(tempx1,2)+pow(tempy2,2)+p1);
				p112=sqrt(pow(tempx1,2)+pow(tempy1,2)+p2);
				p111=sqrt(pow(tempx1,2)+pow(tempy1,2)+p1);
			
				G222=atan2(tempx2*tempy2,pow(tempx2,2.0)+p222*tempz2+p2);		//������
				G221=atan2(tempx2*tempy2,pow(tempx2,2.0)+p221*tempz1+p1);
				G212=atan2(tempx2*tempy1,pow(tempx2,2.0)+p212*tempz2+p2);
				G211=atan2(tempx2*tempy1,pow(tempx2,2.0)+p211*tempz1+p1);

				G122=atan2(tempx1*tempy2,pow(tempx1,2.0)+p122*tempz2+p2);		//������
				G121=atan2(tempx1*tempy2,pow(tempx1,2.0)+p121*tempz1+p1);
				G112=atan2(tempx1*tempy1,pow(tempx1,2.0)+p112*tempz2+p2);
				G111=atan2(tempx1*tempy1,pow(tempx1,2.0)+p111*tempz1+p1);

				grav[j+i*xnumber]+=6.67*10*density*(G222+G211+G121+G112-G221-G212-G122-G111);	//E
			}
		}
	}
}

//����Vxy
void Cubic_Vxy_Forward(CUBIC_FORWARD_INFO cubic_grav_info,double *grav)
{
	//�۲�ϵͳ����
	double dx=cubic_grav_info.dx,dy=cubic_grav_info.dy;
	int xnumber=cubic_grav_info.xnumber,ynumber=cubic_grav_info.ynumber;
	double z=cubic_grav_info.z;
	//��ʼ��Ϊ0
	for (int i = 0; i < xnumber*ynumber; i++)
	{
		grav[i]=0.0;
	}
	for (int k = 0; k < cubic_grav_info.x1_vector.size(); k++)
	{
		//ģ�Ͳ���
		double x1=cubic_grav_info.x1_vector[k],x2=cubic_grav_info.x2_vector[k],y1=cubic_grav_info.y1_vector[k],y2=cubic_grav_info.y2_vector[k],
			z1=cubic_grav_info.z1_vector[k],z2=cubic_grav_info.z2_vector[k];
		double density=cubic_grav_info.m_density_vector[k];
		//�м����
		double tempx1,tempx2,tempy1,tempy2,G122,G121,G112,G111,G222,G221,G212,G211;
		double p222,p221,p212,p211,p122,p121,p112,p111;		//��x��y��z��˳������
		double tempz1=z1-z,tempz2=z2-z;
		double p2,p1;
		p2=pow(tempz2,2.0);	//X�������ʽ���ǳ���
		p1=pow(tempz1,2.0);

		int i;
		double x,y;
		for (i=0;i<ynumber;i++)
		{
			for (int j=0;j<xnumber;j++)
			{
				y=cubic_grav_info.ymin+i*dy;
				x=cubic_grav_info.xmin+j*dy;;
				tempx1=x1-x;		//���������
				tempx2=x2-x;
		
				tempy1=y1-y;
				tempy2=y2-y;
				//tempy1=-20;tempy2=20;
			
				p222=sqrt(pow(tempx2,2)+pow(tempy2,2)+p2);		//����
				p221=sqrt(pow(tempx2,2)+pow(tempy2,2)+p1);
				p212=sqrt(pow(tempx2,2)+pow(tempy1,2)+p2);
				p211=sqrt(pow(tempx2,2)+pow(tempy1,2)+p1);
			
				p122=sqrt(pow(tempx1,2)+pow(tempy2,2)+p2);		//����
				p121=sqrt(pow(tempx1,2)+pow(tempy2,2)+p1);
				p112=sqrt(pow(tempx1,2)+pow(tempy1,2)+p2);
				p111=sqrt(pow(tempx1,2)+pow(tempy1,2)+p1);
			
				G222=log(tempz2+p222);		//������
				G221=log(tempz1+p221);
				G212=log(tempz2+p212);
				G211=log(tempz1+p211);

				G122=log(tempz2+p122);		//������
				G121=log(tempz1+p121);
				G112=log(tempz2+p112);
				G111=log(tempz1+p111);

				grav[j+i*xnumber]+=6.67*10*density*(G222+G211+G121+G112-G221-G212-G122-G111);	//E
			}
		}
	}
}

//����Vxz
void Cubic_Vxz_Forward(CUBIC_FORWARD_INFO cubic_grav_info,double *grav)
{
	//�۲�ϵͳ����
	double dx=cubic_grav_info.dx,dy=cubic_grav_info.dy;
	int xnumber=cubic_grav_info.xnumber,ynumber=cubic_grav_info.ynumber;
	double z=cubic_grav_info.z;
	//��ʼ��Ϊ0
	for (int i = 0; i < xnumber*ynumber; i++)
	{
		grav[i]=0.0;
	}
	for (int k = 0; k < cubic_grav_info.x1_vector.size(); k++)
	{
		//ģ�Ͳ���
		double x1=cubic_grav_info.x1_vector[k],x2=cubic_grav_info.x2_vector[k],y1=cubic_grav_info.y1_vector[k],y2=cubic_grav_info.y2_vector[k],
			z1=cubic_grav_info.z1_vector[k],z2=cubic_grav_info.z2_vector[k];
		double density=cubic_grav_info.m_density_vector[k];
		//�м����
		double tempx1,tempx2,tempy1,tempy2,G122,G121,G112,G111,G222,G221,G212,G211;
		double p222,p221,p212,p211,p122,p121,p112,p111;		//��x��y��z��˳������
		double tempz1=z1-z,tempz2=z2-z;
		double p2,p1;
		p2=pow(tempz2,2.0);	//X�������ʽ���ǳ���
		p1=pow(tempz1,2.0);

		int i;
		double x,y;
		for (i=0;i<ynumber;i++)
		{
			for (int j=0;j<xnumber;j++)
			{
				y=cubic_grav_info.ymin+i*dy;
				x=cubic_grav_info.xmin+j*dy;;
				tempx1=x1-x;		//���������
				tempx2=x2-x;
		
				tempy1=y1-y;
				tempy2=y2-y;
				//tempy1=-20;tempy2=20;
			
				p222=sqrt(pow(tempx2,2)+pow(tempy2,2)+p2);		//����
				p221=sqrt(pow(tempx2,2)+pow(tempy2,2)+p1);
				p212=sqrt(pow(tempx2,2)+pow(tempy1,2)+p2);
				p211=sqrt(pow(tempx2,2)+pow(tempy1,2)+p1);
			
				p122=sqrt(pow(tempx1,2)+pow(tempy2,2)+p2);		//����
				p121=sqrt(pow(tempx1,2)+pow(tempy2,2)+p1);
				p112=sqrt(pow(tempx1,2)+pow(tempy1,2)+p2);
				p111=sqrt(pow(tempx1,2)+pow(tempy1,2)+p1);
			
				G222=log(tempy2+p222);		//������
				G221=log(tempy2+p221);
				G212=log(tempy1+p212);
				G211=log(tempy1+p211);

				G122=log(tempy2+p122);		//������
				G121=log(tempy2+p121);
				G112=log(tempy1+p112);
				G111=log(tempy1+p111);

				grav[j+i*xnumber]+=6.67*10*density*(G222+G211+G121+G112-G221-G212-G122-G111);	//E
			}
		}
	}
}

//����Vyy
void Cubic_Vyy_Forward(CUBIC_FORWARD_INFO cubic_grav_info,double *grav)
{
	//�۲�ϵͳ����
	double dx=cubic_grav_info.dx,dy=cubic_grav_info.dy;
	int xnumber=cubic_grav_info.xnumber,ynumber=cubic_grav_info.ynumber;
	double z=cubic_grav_info.z;
	//��ʼ��Ϊ0
	for (int i = 0; i < xnumber*ynumber; i++)
	{
		grav[i]=0.0;
	}
	for (int k = 0; k < cubic_grav_info.x1_vector.size(); k++)
	{
		//ģ�Ͳ���
		double x1=cubic_grav_info.x1_vector[k],x2=cubic_grav_info.x2_vector[k],y1=cubic_grav_info.y1_vector[k],y2=cubic_grav_info.y2_vector[k],
			z1=cubic_grav_info.z1_vector[k],z2=cubic_grav_info.z2_vector[k];
		double density=cubic_grav_info.m_density_vector[k];
		//�м����
		double tempx1,tempx2,tempy1,tempy2,G122,G121,G112,G111,G222,G221,G212,G211;
		double p222,p221,p212,p211,p122,p121,p112,p111;		//��x��y��z��˳������
		double tempz1=z1-z,tempz2=z2-z;
		double p2,p1;
		p2=pow(tempz2,2.0);	//X�������ʽ���ǳ���
		p1=pow(tempz1,2.0);

		int i;
		double x,y;
		for (i=0;i<ynumber;i++)
		{
			for (int j=0;j<xnumber;j++)
			{
				y=cubic_grav_info.ymin+i*dy;
				x=cubic_grav_info.xmin+j*dy;;
				tempx1=x1-x;		//���������
				tempx2=x2-x;
		
				tempy1=y1-y;
				tempy2=y2-y;
				//tempy1=-20;tempy2=20;
			
				p222=sqrt(pow(tempx2,2)+pow(tempy2,2)+p2);		//����
				p221=sqrt(pow(tempx2,2)+pow(tempy2,2)+p1);
				p212=sqrt(pow(tempx2,2)+pow(tempy1,2)+p2);
				p211=sqrt(pow(tempx2,2)+pow(tempy1,2)+p1);
			
				p122=sqrt(pow(tempx1,2)+pow(tempy2,2)+p2);		//����
				p121=sqrt(pow(tempx1,2)+pow(tempy2,2)+p1);
				p112=sqrt(pow(tempx1,2)+pow(tempy1,2)+p2);
				p111=sqrt(pow(tempx1,2)+pow(tempy1,2)+p1);
			
				G222=atan2(tempx2*tempy2,pow(tempy2,2.0)+p222*tempz2+p2);		//������
				G221=atan2(tempx2*tempy2,pow(tempy2,2.0)+p221*tempz1+p1);
				G212=atan2(tempx2*tempy1,pow(tempy1,2.0)+p212*tempz2+p2);
				G211=atan2(tempx2*tempy1,pow(tempy1,2.0)+p211*tempz1+p1);

				G122=atan2(tempx1*tempy2,pow(tempy2,2.0)+p122*tempz2+p2);		//������
				G121=atan2(tempx1*tempy2,pow(tempy2,2.0)+p121*tempz1+p1);
				G112=atan2(tempx1*tempy1,pow(tempy1,2.0)+p112*tempz2+p2);
				G111=atan2(tempx1*tempy1,pow(tempy1,2.0)+p111*tempz1+p1);

				grav[j+i*xnumber]+=6.67*10*density*(G222+G211+G121+G112-G221-G212-G122-G111);	//E
			}
		}
	}
}

//����Vyz
void Cubic_Vyz_Forward(CUBIC_FORWARD_INFO cubic_grav_info,double *grav)
{
	//�۲�ϵͳ����
	double dx=cubic_grav_info.dx,dy=cubic_grav_info.dy;
	int xnumber=cubic_grav_info.xnumber,ynumber=cubic_grav_info.ynumber;
	double z=cubic_grav_info.z;
	//��ʼ��Ϊ0
	for (int i = 0; i < xnumber*ynumber; i++)
	{
		grav[i]=0.0;
	}
	for (int k = 0; k < cubic_grav_info.x1_vector.size(); k++)
	{
		//ģ�Ͳ���
		double x1=cubic_grav_info.x1_vector[k],x2=cubic_grav_info.x2_vector[k],y1=cubic_grav_info.y1_vector[k],y2=cubic_grav_info.y2_vector[k],
			z1=cubic_grav_info.z1_vector[k],z2=cubic_grav_info.z2_vector[k];
		double density=cubic_grav_info.m_density_vector[k];
		//�м����
		double tempx1,tempx2,tempy1,tempy2,G122,G121,G112,G111,G222,G221,G212,G211;
		double p222,p221,p212,p211,p122,p121,p112,p111;		//��x��y��z��˳������
		double tempz1=z1-z,tempz2=z2-z;
		double p2,p1;
		p2=pow(tempz2,2.0);	//X�������ʽ���ǳ���
		p1=pow(tempz1,2.0);

		int i;
		double x,y;
		for (i=0;i<ynumber;i++)
		{
			for (int j=0;j<xnumber;j++)
			{
				y=cubic_grav_info.ymin+i*dy;
				x=cubic_grav_info.xmin+j*dy;;
				tempx1=x1-x;		//���������
				tempx2=x2-x;
		
				tempy1=y1-y;
				tempy2=y2-y;
				//tempy1=-20;tempy2=20;
			
				p222=sqrt(pow(tempx2,2)+pow(tempy2,2)+p2);		//����
				p221=sqrt(pow(tempx2,2)+pow(tempy2,2)+p1);
				p212=sqrt(pow(tempx2,2)+pow(tempy1,2)+p2);
				p211=sqrt(pow(tempx2,2)+pow(tempy1,2)+p1);
			
				p122=sqrt(pow(tempx1,2)+pow(tempy2,2)+p2);		//����
				p121=sqrt(pow(tempx1,2)+pow(tempy2,2)+p1);
				p112=sqrt(pow(tempx1,2)+pow(tempy1,2)+p2);
				p111=sqrt(pow(tempx1,2)+pow(tempy1,2)+p1);
			
				G222=log(tempx2+p222);		//������
				G221=log(tempx2+p221);
				G212=log(tempx2+p212);
				G211=log(tempx2+p211);

				G122=log(tempx1+p122);		//������
				G121=log(tempx1+p121);
				G112=log(tempx1+p112);
				G111=log(tempx1+p111);

				grav[j+i*xnumber]+=6.67*10*density*(G222+G211+G121+G112-G221-G212-G122-G111);	//E
			}
		}
	}
}

//����Vzz
void Cubic_Vzz_Forward(CUBIC_FORWARD_INFO cubic_grav_info,double *grav)
{
	//�۲�ϵͳ����
	double dx=cubic_grav_info.dx,dy=cubic_grav_info.dy;
	int xnumber=cubic_grav_info.xnumber,ynumber=cubic_grav_info.ynumber;
	double z=cubic_grav_info.z;
	//��ʼ��Ϊ0
	for (int i = 0; i < xnumber*ynumber; i++)
	{
		grav[i]=0.0;
	}
	for (int k = 0; k < cubic_grav_info.x1_vector.size(); k++)
	{
		//ģ�Ͳ���
		double x1=cubic_grav_info.x1_vector[k],x2=cubic_grav_info.x2_vector[k],y1=cubic_grav_info.y1_vector[k],y2=cubic_grav_info.y2_vector[k],
			z1=cubic_grav_info.z1_vector[k],z2=cubic_grav_info.z2_vector[k];
		double density=cubic_grav_info.m_density_vector[k];
		//�м����
		double tempx1,tempx2,tempy1,tempy2,G122,G121,G112,G111,G222,G221,G212,G211;
		double p222,p221,p212,p211,p122,p121,p112,p111;		//��x��y��z��˳������
		double tempz1=z1-z,tempz2=z2-z;
		double p2,p1;
		p2=pow(tempz2,2.0);	//X�������ʽ���ǳ���
		p1=pow(tempz1,2.0);

		int i;
		double x,y;
		for (i=0;i<ynumber;i++)
		{
			for (int j=0;j<xnumber;j++)
			{
				y=cubic_grav_info.ymin+i*dy;
				x=cubic_grav_info.xmin+j*dy;;
				tempx1=x1-x;		//���������
				tempx2=x2-x;
		
				tempy1=y1-y;
				tempy2=y2-y;
				//tempy1=-20;tempy2=20;
			
				p222=sqrt(pow(tempx2,2)+pow(tempy2,2)+p2);		//����
				p221=sqrt(pow(tempx2,2)+pow(tempy2,2)+p1);
				p212=sqrt(pow(tempx2,2)+pow(tempy1,2)+p2);
				p211=sqrt(pow(tempx2,2)+pow(tempy1,2)+p1);
			
				p122=sqrt(pow(tempx1,2)+pow(tempy2,2)+p2);		//����
				p121=sqrt(pow(tempx1,2)+pow(tempy2,2)+p1);
				p112=sqrt(pow(tempx1,2)+pow(tempy1,2)+p2);
				p111=sqrt(pow(tempx1,2)+pow(tempy1,2)+p1);
			
				G222=atan2(tempx2*tempy2,p222*tempz2);		//������
				G221=atan2(tempx2*tempy2,p221*tempz1);
				G212=atan2(tempx2*tempy1,p212*tempz2);
				G211=atan2(tempx2*tempy1,p211*tempz1);

				G122=atan2(tempx1*tempy2,p122*tempz2);		//������
				G121=atan2(tempx1*tempy2,p121*tempz1);
				G112=atan2(tempx1*tempy1,p112*tempz2);
				G111=atan2(tempx1*tempy1,p111*tempz1);

				grav[j+i*xnumber]+=-6.67*10*density*(G222+G211+G121+G112-G221-G212-G122-G111);	//E
			}
		}
	}
}

//��������NSS:�������������ֵ��̫�԰�������Beiki��2011�������NSS�㷨�����¿������Ǹ�����why������
void Cubic_GravityNSS_Forward(CUBIC_FORWARD_INFO cubic_grav_info,double *grav)
{
	int datanum=cubic_grav_info.xnumber*cubic_grav_info.ynumber;
	//���붯̬�������쳣ֵ
	double *Vz=new double[datanum];
	double *Vxx=new double[datanum];
	double *Vxy=new double[datanum];
	double *Vxz=new double[datanum];
	double *Vyy=new double[datanum];
	double *Vyz=new double[datanum];
	double *Vzz=new double[datanum];
	double *NSS=new double[datanum];
	
	//1.1 ���������ݶ��������ݺ���
	Cubic_Vxx_Forward(cubic_grav_info,Vxx);
	Cubic_Vxy_Forward(cubic_grav_info,Vxy);
	Cubic_Vxz_Forward(cubic_grav_info,Vxz);
	Cubic_Vyy_Forward(cubic_grav_info,Vyy);
	Cubic_Vyz_Forward(cubic_grav_info,Vyz);
	Cubic_Vzz_Forward(cubic_grav_info,Vzz);

	//1.2 ����NSS
	double *p=new double[datanum],*q=new double[datanum];
	double *lamda1=new double[datanum],*lamda2=new double[datanum],*lamda3=new double[datanum];
	double I1,I2,Bij,p0,p1,Amplitude,Angle,p0q,p1q;
	double Bxx,Byy,Bzz,Bxy,Bxz,Byz;
	for (int i = 0; i < datanum; i++)
	{
		Bxx=Vxx[i];Bxy=Vxy[i];Bxz=Vxz[i];Byy=Vyy[i];Byz=Vyz[i];Bzz=Vzz[i];
		I1=Bxx*Byy+Byy*Bzz+Bzz*Bxx-Bxy*Bxy-Byz*Byz-Bxz*Bxz;
		// �ݶ������ĵ�2������
		I2=Bxx*(Byy*Bzz-Byz*Byz)+Bxy*(Byz*Bxz-Bxy*Bzz)+Bxz*(Bxy*Byz-Bxz*Byy);
		// �ݶ�����������ģ
		Bij=sqrt(Bxx*Bxx+Bxy*Bxy+Bxz*Bxz+Bxy*Bxy+Byy*Byy+Byz*Byz+Bxz*Bxz+Byz*Byz+Bzz*Bzz);
		// ���򻯴�Դǿ��NSS
		p0=I2/2.0;
		p1=sqrt(-1.0*(I2*I2/4.0+I1*I1*I1/27.0));
		Amplitude=pow(sqrt(p0*p0+p1*p1),1.0/3.0);
		Angle=atan2(p1,p0)/3.0;
		p0q=2.0*Amplitude*cos(Angle);
		p1q=2.0*Amplitude*sin(Angle);
		lamda1[i]=p0q;
		lamda2[i]=-p0q/2.0+p1q/2.0*sqrt(3);
		lamda3[i]=-p0q/2.0-p1q/2.0*sqrt(3);
		grav[i]=sqrt(-lamda2[i]*lamda2[i]-lamda1[i]*lamda3[i]);

	}

	//�ͷ���Դ
	delete Vz,Vxx,Vxy,Vxz,Vyy,Vyz,Vzz,I1,I2,p,q,lamda1,lamda2,lamda3;

}

//����GGT�Ľ����źţ�Ax��Ay��Az��Axz��Ayz��Azz��
//�ο����ף�Beiki, Majid��2010��Analytic signals of gravity gradient tensor and their application to estimate source location: Geophysics
//�ܹ����������������������źź����Ӧ��z�����������grav��һ��6��datanum�еĶ�ά����
void Cubic_AnalyticSignal_Forward(CUBIC_FORWARD_INFO cubic_grav_info,double **grav)
{
	int datanum=cubic_grav_info.xnumber*cubic_grav_info.ynumber;
	//���붯̬�������쳣ֵ
	double *Vz=new double[datanum];
	double *Vxx=new double[datanum];
	double *Vxy=new double[datanum];
	double *Vxz=new double[datanum];
	double *Vyy=new double[datanum];
	double *Vyz=new double[datanum];
	double *Vzz=new double[datanum];
	double *NSS=new double[datanum];
	
	//1.1 ���������ݶ��������ݺ���
	Cubic_Vxx_Forward(cubic_grav_info,Vxx);
	Cubic_Vxy_Forward(cubic_grav_info,Vxy);
	Cubic_Vxz_Forward(cubic_grav_info,Vxz);
	Cubic_Vyy_Forward(cubic_grav_info,Vyy);
	Cubic_Vyz_Forward(cubic_grav_info,Vyz);
	Cubic_Vzz_Forward(cubic_grav_info,Vzz);

	//1.2 ��������ź��Լ�z������
	for (int i = 0; i < datanum; i++)
	{
		grav[0][i]=sqrt(Vxx[i]*Vxx[i]+Vxy[i]*Vxy[i]+Vxz[i]*Vxz[i]);
		grav[1][i]=sqrt(Vxy[i]*Vxy[i]+Vyy[i]*Vyy[i]+Vyz[i]*Vyz[i]);
		grav[2][i]=sqrt(Vxz[i]*Vxz[i]+Vyz[i]*Vyz[i]+Vzz[i]*Vzz[i]);
	}
	
	
	//�ͷ���Դ
	delete Vz,Vxx,Vxy,Vxz,Vyy,Vyz,Vzz;
}
//���ݳ���дgrd�ļ�ʱһ�㶼�����ݲ���д��grd���棬����������ר�õ�дgrd�ļ��ĺ���
void WriteForwardDataToGrdFile(CUBIC_FORWARD_INFO datainfo,char* filename,double* z)
{
	int i;
	//�۲�ϵͳ����
	double dx=datainfo.dx;						//��ࣨm��
	double dy=datainfo.dy;						//�߾ࣨm��
	int xnumber=datainfo.xnumber;							//������(������
	int ynumber=datainfo.ynumber;							//ÿ�������ϵĲ������������
	//�����쳣ֵ�������Сֵ
	double zmax=z[0],zmin=z[0];
	double xmin=datainfo.xmin,xmax=datainfo.xmin+(datainfo.xnumber-1)*datainfo.dx,
		ymin=datainfo.ymin,ymax=datainfo.ymin+(datainfo.ynumber-1)*datainfo.dy;
	for (i=0;i<xnumber*ynumber;i++)
	{
		if (zmax<z[i])
		{
			zmax=z[i];
		}
		if (zmin>z[i])
		{
			zmin=z[i];
		}
	}
	
	//���Ϊgrd��ʽ
	FILE *fp=NULL;
	if ((fp=fopen(filename,"w"))==NULL)
	{
		printf("�� %s ʧ��\n",filename);
		exit(0);
	}
	fprintf(fp,"DSAA\n%d\t%d\n%lf\t%lf\n%lf\t%lf\n%lf\t%lf\n",xnumber,ynumber,xmin,xmax,ymin,ymax,zmin,zmax);		//��ӡ�ļ�ͷ
	//��ӡ����
	for (i=0;i<ynumber;i++)
	{
		for (int j=0;j<xnumber;j++)
		{
			fprintf(fp,"%lf\t",z[i*xnumber+j]);
		}
		fprintf(fp,"\n");
	}
	//�������ģ�Ͳ���
	fprintf(fp,"�۲�ϵͳ����:\n");
	fprintf(fp,"\t\txmin:%lf\tymin:%lf\tdx:%lf\tdy:%lf\txnumber(������:%d\tynumber(����):%d\n",
		datainfo.xmin,datainfo.ymin,datainfo.dx,datainfo.dy,datainfo.xnumber,datainfo.ynumber);
	
	//fprintf(fp,"���β���:\n");
	for (int i = 0; i < datainfo.x1_vector.size(); i++)
	{
		fprintf(fp,"��%d��ģ��\tx1:%lf\tx2:%lf\ty1:%lf\ty2:%lf\tz1:%lf\tz2:%lf\n",i+1,
			datainfo.x1_vector[i],datainfo.x2_vector[i],datainfo.y1_vector[i],
			datainfo.y2_vector[i],datainfo.z1_vector[i],datainfo.z2_vector[i]);
		if (datainfo.angle_I_vector.size()!=0)
		{
			fprintf(fp,"\t\t�Ż�ǿ��(A/m):%lf\t�Ż����(��):%lf\t�Ż�ƫ��(��):%lf\t�ش����(��):%lf\t�ش�ƫ��(��):%lf\n",
				datainfo.m_vector[i],datainfo.angle_I_vector[i],datainfo.angle_Apie_vector[i],
				datainfo.angle_I0,datainfo.angle_Apie0);
		}
		if (datainfo.m_density_vector.size()!=0)
		{
			fprintf(fp,"\t\t�ܶ�(g/cm3):%lf\n",datainfo.m_density_vector[i]);
		}
	}
	fclose(fp);

}

//�ŷ����ݵ������ԣ���Ҫ������ר�õ������������Ϊ�ŷ���x�����Ǳ���surfer��ͼʱ����ѡ��y���������Ҫ��һ������grd��ȣ�x��yҪ����
//���ݳ���дgrd�ļ�ʱһ�㶼�����ݲ���д��grd���棬����������ר�õ�дgrd�ļ��ĺ���
void WriteMagneticForwardDataToGrdFile(CUBIC_FORWARD_INFO datainfo,char* filename,double* z)
{
	int i;
	//�۲�ϵͳ����
	double dx=datainfo.dx;						//��ࣨm��
	double dy=datainfo.dy;						//�߾ࣨm��
	int xnumber=datainfo.xnumber;							//������(������
	int ynumber=datainfo.ynumber;							//ÿ�������ϵĲ������������
	//�����쳣ֵ�������Сֵ
	double zmax=z[0],zmin=z[0];
	double xmin=datainfo.xmin,xmax=datainfo.xmin+(datainfo.xnumber-1)*datainfo.dx,
		ymin=datainfo.ymin,ymax=datainfo.ymin+(datainfo.ynumber-1)*datainfo.dy;
	for (i=0;i<xnumber*ynumber;i++)
	{
		if (zmax<z[i])
		{
			zmax=z[i];
		}
		if (zmin>z[i])
		{
			zmin=z[i];
		}
	}
	
	//���Ϊgrd��ʽ
	FILE *fp=NULL;
	if ((fp=fopen(filename,"w"))==NULL)
	{
		printf("�� %s ʧ��\n",filename);
		exit(0);
	}
	fprintf(fp,"DSAA\n%d\t%d\n%lf\t%lf\n%lf\t%lf\n%lf\t%lf\n",ynumber,xnumber,ymin,ymax,xmin,xmax,zmin,zmax);		//��ӡ�ļ�ͷ
	//��ӡ����
	for (i=0;i<xnumber;i++)
	{
		for (int j=0;j<ynumber;j++)
		{
			fprintf(fp,"%lf\t",z[i*ynumber+j]);
		}
		fprintf(fp,"\n");
	}
	//�������ģ�Ͳ���
	fprintf(fp,"�۲�ϵͳ����:\n");
	fprintf(fp,"\t\txmin:%lf\tymin:%lf\tdx:%lf\tdy:%lf\txnumber(������:%d\tynumber(����):%d\n",
		datainfo.xmin,datainfo.ymin,datainfo.dx,datainfo.dy,datainfo.xnumber,datainfo.ynumber);
	
	//fprintf(fp,"���β���:\n");
	for (int i = 0; i < datainfo.x1_vector.size(); i++)
	{
		fprintf(fp,"��%d��ģ��\tx1:%lf\tx2:%lf\ty1:%lf\ty2:%lf\tz1:%lf\tz2:%lf\n",i+1,
			datainfo.x1_vector[i],datainfo.x2_vector[i],datainfo.y1_vector[i],
			datainfo.y2_vector[i],datainfo.z1_vector[i],datainfo.z2_vector[i]);
		if (datainfo.angle_I_vector.size()!=0)
		{
			fprintf(fp,"\t\t�Ż�ǿ��(A/m):%lf\t�Ż����(��):%lf\t�Ż�ƫ��(��):%lf\t�ش����(��):%lf\t�ش�ƫ��(��):%lf\n",
				datainfo.m_vector[i],datainfo.angle_I_vector[i],datainfo.angle_Apie_vector[i],
				datainfo.angle_I0,datainfo.angle_Apie0);
		}
		if (datainfo.m_density_vector.size()!=0)
		{
			fprintf(fp,"\t\t�ܶ�(g/cm3):%lf\n",datainfo.m_density_vector[i]);
		}
	}
	fclose(fp);

}


void GetMeshInfo(MESHINFO& meshinfo,char* filename)
{
	FILE* FP;
	if ((FP=fopen(filename,"r"))==NULL)
	{
		printf("�� %s ʧ��\n",filename);
		return ;
	}
	fscanf(FP,"%d%d%d%lf%lf%lf",&meshinfo.M,&meshinfo.N,&meshinfo.L,&meshinfo.initx,&meshinfo.inity,&meshinfo.initz);
	double temp;
	fscanf(FP,"%lf*%lf%lf*%lf%lf*%lf",&temp,&meshinfo.dx,&temp,&meshinfo.dy,&temp,&meshinfo.dz);
	fclose(FP);
}


void GetModel(char* filename,double* model,MESHINFO meshinfo)
{
	FILE* FP;
	if ((FP=fopen(filename,"r"))==NULL)
	{
		printf("�� %s ʧ��\n",filename);
		return ;
	}
	for (int i = 0; i < meshinfo.M*meshinfo.N*meshinfo.L; i++)
	{
		fscanf(FP,"%lf",&model[i]);
	}fclose(FP);
}

void WriteGrd(char* filename,double* data,int m,int n,double xmin,double xmax,double ymin,double ymax)
{
	FILE* fp=NULL;
	if ((fopen_s(&fp,filename,"w"))!=0)
	{
		printf("��%sʧ��!",filename);
		return;
	}
	double zmin=data[0],zmax=data[0];
	for (int i = 0; i < m*n; i++)
	{
		if (zmin>data[i])
		{
			zmin=data[i];
		}
		if (zmax<data[i])
		{
			zmax=data[i];
		}
	}
	fprintf(fp,"DSAA\n%d\t%d\n%lf\t%lf\n%lf\t%lf\n%lf\t%lf\n",m,n,xmin,xmax,ymin,ymax,zmin,zmax);
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			fprintf(fp,"%lf\t",data[i*n+j]);
		}
	}
	fclose(fp);
}


//�����˾�����
void _g_GetKernalMat_fast(MESHINFO meshinfo,double** KernalMat_G)
{
	//======================������========================================
	double R222,R122,R212,R112,R221,R121,R211,R111,x1,x2,y1,y2,z1,z2;
	double G222,G122,G212,G112,G221,G121,G211,G111;
	double x,y,z=meshinfo.initz;
	int datanum=meshinfo.M*meshinfo.N;
	double gridX_half=meshinfo.dx/2.0,gridY_half=meshinfo.dy/2.0,gridZ_half=meshinfo.dz/2.0;
	for (int j=0; j<meshinfo.L; j++)//����˾���
	{
		z1=j*meshinfo.dz-z;
		z2=z1+meshinfo.dz;
		for (int i=0; i<datanum; i++)
		{
			x=(i%meshinfo.M)*meshinfo.dx+gridX_half;//ȷ�������λ��
			y=(int(i/meshinfo.M))*meshinfo.dy+gridY_half;
			x1=-x;
			x2=x1+meshinfo.dx;
			y1=-y;
			y2=y1+meshinfo.dy;

			R111=sqrt(x1*x1+y1*y1+z1*z1);
			R112=sqrt(x1*x1+y1*y1+z2*z2);
			R121=sqrt(x1*x1+y2*y2+z1*z1);
			R122=sqrt(x1*x1+y2*y2+z2*z2);
			R211=sqrt(x2*x2+y1*y1+z1*z1);
			R212=sqrt(x2*x2+y1*y1+z2*z2);
			R221=sqrt(x2*x2+y2*y2+z1*z1);
			R222=sqrt(x2*x2+y2*y2+z2*z2);
			
			G222=x2*log(y2+R222)+y2*log(x2+R222)+z2*arctg(z2*R222/x2/y2);
			G122=x1*log(y2+R122)+y2*log(x1+R122)+z2*arctg(z2*R122/x1/y2);
			G212=x2*log(y1+R212)+y1*log(x2+R212)+z2*arctg(z2*R212/x2/y1);
			G112=x1*log(y1+R112)+y1*log(x1+R112)+z2*arctg(z2*R112/x1/y1);
			G221=x2*log(y2+R221)+y2*log(x2+R221)+z1*arctg(z1*R221/x2/y2);
			G121=x1*log(y2+R121)+y2*log(x1+R121)+z1*arctg(z1*R121/x1/y2);
			G211=x2*log(y1+R211)+y1*log(x2+R211)+z1*arctg(z1*R211/x2/y1);
			G111=x1*log(y1+R111)+y1*log(x1+R111)+z1*arctg(z1*R111/x1/y1);
			
			//KernalMat_G[j][i]=-(G222-G122-G212+G112-G221+G121+G211-G111);//����ϵ��
			KernalMat_G[j][i]=-GRAV_CONSTANT*pow(10,-3)*(G222-G122-G212+G112-G221+G121+G211-G111);//����ϵ��
		}
	}
	//=============================================================================
}
