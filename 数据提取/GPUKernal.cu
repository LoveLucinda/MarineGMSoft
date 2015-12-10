
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include "stdafx.h"
#include "time.h"
#include "math.h"
/**
  * CUDA����˺���
  *
  */
/*===========================================================================================================
 �������ܣ�
 ������

����˵����GPU����˺�����cpu�޷�����
		  ÿһ���̺߳Ŷ�Ӧ�ڼ������ÿһ�����������˻�
		  #pragma unroll�����forѭ���Ǵ��еģ������ۼ���ͣ����д��id��Ӧ������Ԫ��
============================================================================================================*/
__global__ void
	DemCorrect(const double *demdata,const GRDRANGEINDEX* indexarray,double *CorrectData,int numberofelement,GRDSTRUCT datainfo)
{
	int id=blockDim.x*blockIdx.x+threadIdx.x;		//�̺߳ţ��൱��CPUѭ�����i��
	if(id>=numberofelement)return;

	double temp=0;
	double grav=0,density=1.64;
	double tempx1,tempx2,tempy1,tempy2,G122,G121,G112,G111,G222,G221,G212,G211;
	double p222,p221,p212,p211,p122,p121,p112,p111;		//��x��y��z��˳������
	double p2,p1;
	double z=-0.1;
	double x1=0,x2=0,y1=0,y2=0,z1=0,z2=0;
#pragma unroll							//��һ���̼߳���һ�����ĸ���ֵ�����ݣ�
	for (int i = indexarray[id].rowmin; i <indexarray[id].rowmax; i++)
	{
		for (int j = indexarray[id].colmin; j < indexarray[id].colmax; j++)
		{
			

		//ģ�Ͳ���
		/*x1=datainfo.xmin+j*datainfo.dx,x2=datainfo.xmin+(j+1)*datainfo.dx,y1=datainfo.ymin+i*datainfo.dy,y2=datainfo.ymin+(i+1)*datainfo.dy,
			z1=0-z,z2=demdata[j+i*datainfo.cols]-z;*/
		
		//�м����
		
		p2=z2*z2;	//X�������ʽ���ǳ���
		p1=z1*z1;

		tempx1=x1-indexarray[id].x0;		//���������
		tempx2=x2-indexarray[id].x0;
		
		tempy1=y1-indexarray[id].y0;
		tempy2=y2-indexarray[id].y0;
			
		p222=sqrt(pow(tempx2,2)+pow(tempy2,2)+p2);		//����
		p221=sqrt(pow(tempx2,2)+pow(tempy2,2)+p1);
		p212=sqrt(pow(tempx2,2)+pow(tempy1,2)+p2);
		p211=sqrt(pow(tempx2,2)+pow(tempy1,2)+p1);

		////
		p122=sqrt(pow(tempx1,2)+pow(tempy2,2)+p2);		//����
		p121=sqrt(pow(tempx1,2)+pow(tempy2,2)+p1);
		p112=sqrt(pow(tempx1,2)+pow(tempy1,2)+p2);
		p111=sqrt(pow(tempx1,2)+pow(tempy1,2)+p1);
		////	
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
		
	}
	CorrectData[id]=grav;
}

/*===========================================================================================================
 �������ܣ��������ϵĵ��θ���ֵ��������������У����
 ������
	
����˵����GPU����˺�����cpu�޷�����
		  ÿһ���̺߳Ŷ�Ӧ�ڼ������ÿһ�����������˻�
		  #pragma unroll�����forѭ���Ǵ��еģ������ۼ���ͣ����д��id��Ӧ������Ԫ��
============================================================================================================*/
extern "C"
void GPU_Correct(double* CorrectData,int CorrectDataNumber,double* DemData,GRDRANGEINDEX* IndexArray,GRDSTRUCT demdatainfo)
{
	//CUDA���������
	cudaError_t err=cudaSuccess;
	//����GPU��������
	size_t size=CorrectDataNumber*sizeof(double);
	double *d_CorrectData=NULL;
	err=cudaMalloc((void**)&d_CorrectData,size);
	if (err!=cudaSuccess){AfxMessageBox(_T("����GPU����d_CorrectDataʧ��\n"));exit(EXIT_FAILURE);}

	size_t size_indexarray=CorrectDataNumber*sizeof(GRDRANGEINDEX);
	GRDRANGEINDEX *d_IndexArray=NULL;
	err=cudaMalloc((void**)&d_IndexArray,size_indexarray);
	if (err!=cudaSuccess){AfxMessageBox(_T("����GPU����d_IndexArrayʧ��\n"));exit(EXIT_FAILURE);}

	size_t size_demdata=demdatainfo.rows*demdatainfo.cols*sizeof(double);
	double *d_DemData=NULL;
	err=cudaMalloc((void**)&d_DemData,size_demdata);
	if (err!=cudaSuccess){AfxMessageBox(_T("����GPU����d_DemDataʧ��\n"));exit(EXIT_FAILURE);}

	//��host�洢�ռ俽����GPU
	//AfxMessageBox(_T("host�洢�ռ俽����GPU\n"));
	err=cudaMemcpy(d_CorrectData,CorrectData,size,cudaMemcpyHostToDevice);
	if (err!=cudaSuccess){AfxMessageBox(_T("�����洢�ռ�CorrectData��deviceʧ��\n"));exit(EXIT_FAILURE);}

	err=cudaMemcpy(d_IndexArray,IndexArray,size_indexarray,cudaMemcpyHostToDevice);
	if (err!=cudaSuccess){AfxMessageBox(_T("�����洢�ռ�IndexArray��deviceʧ��\n"));exit(EXIT_FAILURE);}

	err=cudaMemcpy(d_DemData,DemData,size_demdata,cudaMemcpyHostToDevice);
	if (err!=cudaSuccess){AfxMessageBox(_T("�����洢�ռ�DemData��deviceʧ��\n"));exit(EXIT_FAILURE);}


	//����CUDA�˺���
	int threadsPerBlock=512;//��ȡ�豸����
	int blockPerGrid=(CorrectDataNumber+threadsPerBlock-1)/threadsPerBlock;
	//printf("CUDA�˺������� %d ��blocks��%d ���߳�\n",blockPerGrid,threadsPerBlock);
	DemCorrect<<<blockPerGrid,threadsPerBlock>>>(d_DemData,d_IndexArray,d_CorrectData,CorrectDataNumber,demdatainfo);		//GPU����˺���
	err=cudaGetLastError();
	if (err!=cudaSuccess){AfxMessageBox(_T("����CUDA�˺���ʧ�ܣ�������룺%s)��\n"));exit(EXIT_FAILURE);}

	
	//��GPU����������ݵ��ڴ�
	//printf("��GPU����������ݵ��ڴ�\n");
	err=cudaMemcpy(CorrectData,d_CorrectData,size,cudaMemcpyDeviceToHost);
	CString errorstr;errorstr.Format(_T("��GPU��������ʧ�ܣ�������룺%s)��\n"),cudaGetErrorString(err));
	if (err!=cudaSuccess){AfxMessageBox(errorstr);exit(EXIT_FAILURE);}

	//�ͷ��ڴ�
	err=cudaFree(d_CorrectData);
	if (err!=cudaSuccess){AfxMessageBox(_T("CUDA�ͷ�d_A�洢�ռ�ʧ�ܣ�������룺%s)��\n"));exit(EXIT_FAILURE);}
	err=cudaFree(d_IndexArray);
	if (err!=cudaSuccess){AfxMessageBox(_T("CUDA�ͷ�d_B�洢�ռ�ʧ�ܣ�������룺%s)��\n"));exit(EXIT_FAILURE);}
	err=cudaFree(d_DemData);
	if (err!=cudaSuccess){AfxMessageBox(_T("CUDA�ͷ�d_C�洢�ռ�ʧ�ܣ�������룺%s)��\n"));exit(EXIT_FAILURE);}

	//����device
	err=cudaDeviceReset();
	if (err!=cudaSuccess){AfxMessageBox(_T("����deviecʧ�ܣ�������룺%s)��\n",));exit(EXIT_FAILURE);}
}
/*===========================================================================================================
 �������ܣ���ȡ����ӡ�豸����
============================================================================================================*/
extern "C"
void GetGPUProperty()
{
	//1.1 ��ȡGPU����
	int deviceCount;
	cudaGetDeviceCount(&deviceCount);
	
	//1.2 ��ȡGPU�豸����
	int devID=0;			//�豸��
	cudaDeviceProp deviceProp;
	cudaGetDeviceProperties(&deviceProp,devID);//deviceProp.major ��deviceProp.minor �ֱ�ΪCUDA�豸�������������汾�źʹΰ汾�š�CPU��������ֵ������Ϊ9999

	//--1.2.1 �豸��ȫ�ִ洢����С
	int TotalGlobalMem=deviceProp.totalGlobalMem;//�豸�е�ȫ�ִ洢����С��global memory��������Ƕ����Կ������ǰ����Դ�Ĵ�С��
	//--1.2.2 SM����
	int SMcount=deviceProp.multiProcessorCount;//�豸�п��õ�����˴�����SM�ĸ�����ÿ��SM��8��SP,����SP�ĸ���Ϊ��8* deviceProp.multiProcessorCout
	int SPcount=SMcount*8;
	//--1.2.3 �����洢����С
	int ConstMem=deviceProp.totalConstMem;//�����洢���Ĵ�С
	//--1.2.4
	int ShareMem=deviceProp.sharedMemPerBlock;//ÿ��block�п��õĹ���洢����С
	//--1.2.5
	int RegsCount=deviceProp.regsPerBlock;//ÿ��block�п��õļĴ�������
	//--1.2.6 
	int ThreadCount=deviceProp.warpSize;//�߳���warp�ɶ��ٸ�thread���
	//--1.2.7
	int ThreadInBlock=deviceProp.maxThreadsPerBlock;//һ��block���������ж��ٸ�thread

	//1.3 ���GPU��Ϣ
	CString gpupropstr,tempstr;
	gpupropstr.Format(_T("����GPU��������: %d ��\n"),deviceCount);
	tempstr.Format(_T("ȫ�ִ洢����С(Mb):%d\nThread������%d\n"),TotalGlobalMem/1024/1024,ThreadCount);
	gpupropstr+=tempstr;
	tempstr.Format(_T("ÿ��block��Thread������%d\nÿ��block�п��üĴ���������%d\nSP������%d\n"),ThreadInBlock,RegsCount,SPcount);
	gpupropstr+=tempstr;
	AfxMessageBox(gpupropstr);
	printf("ÿ��block�п��ù���洢����С��%d\n",ShareMem);
	printf("����������С��%d\n",ConstMem);
	printf("SM������%d\n",SMcount);
}
