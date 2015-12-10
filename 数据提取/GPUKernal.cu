
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include "stdafx.h"
#include "time.h"
#include "math.h"
/**
  * CUDA计算核函数
  *
  */
/*===========================================================================================================
 函数功能：
 参数：

函数说明：GPU计算核函数，cpu无法调用
		  每一个线程号对应于计算矩阵每一行与列向量乘积
		  #pragma unroll下面的for循环是串行的，用于累加求和，结果写入id对应的数组元素
============================================================================================================*/
__global__ void
	DemCorrect(const double *demdata,const GRDRANGEINDEX* indexarray,double *CorrectData,int numberofelement,GRDSTRUCT datainfo)
{
	int id=blockDim.x*blockIdx.x+threadIdx.x;		//线程号（相当于CPU循环里的i）
	if(id>=numberofelement)return;

	double temp=0;
	double grav=0,density=1.64;
	double tempx1,tempx2,tempy1,tempy2,G122,G121,G112,G111,G222,G221,G212,G211;
	double p222,p221,p212,p211,p122,p121,p112,p111;		//按x，y，z的顺序排列
	double p2,p1;
	double z=-0.1;
	double x1=0,x2=0,y1=0,y2=0,z1=0,z2=0;
#pragma unroll							//用一个线程计算一个测点的改正值（正演）
	for (int i = indexarray[id].rowmin; i <indexarray[id].rowmax; i++)
	{
		for (int j = indexarray[id].colmin; j < indexarray[id].colmax; j++)
		{
			

		//模型参数
		/*x1=datainfo.xmin+j*datainfo.dx,x2=datainfo.xmin+(j+1)*datainfo.dx,y1=datainfo.ymin+i*datainfo.dy,y2=datainfo.ymin+(i+1)*datainfo.dy,
			z1=0-z,z2=demdata[j+i*datainfo.cols]-z;*/
		
		//中间变量
		
		p2=z2*z2;	//X方向计算式这是常数
		p1=z1*z1;

		tempx1=x1-indexarray[id].x0;		//计算积分限
		tempx2=x2-indexarray[id].x0;
		
		tempy1=y1-indexarray[id].y0;
		tempy2=y2-indexarray[id].y0;
			
		p222=sqrt(pow(tempx2,2)+pow(tempy2,2)+p2);		//距离
		p221=sqrt(pow(tempx2,2)+pow(tempy2,2)+p1);
		p212=sqrt(pow(tempx2,2)+pow(tempy1,2)+p2);
		p211=sqrt(pow(tempx2,2)+pow(tempy1,2)+p1);

		////
		p122=sqrt(pow(tempx1,2)+pow(tempy2,2)+p2);		//距离
		p121=sqrt(pow(tempx1,2)+pow(tempy2,2)+p1);
		p112=sqrt(pow(tempx1,2)+pow(tempy1,2)+p2);
		p111=sqrt(pow(tempx1,2)+pow(tempy1,2)+p1);
		////	
		G222=tempx2*log(tempy2+p222)+tempy2*log(tempx2+p222)+z2*atan2((z2*p222),(tempx2*tempy2));		//积分项
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
 函数功能：计算测点上的地形改正值（海洋重力布格校正）
 参数：
	
函数说明：GPU计算核函数，cpu无法调用
		  每一个线程号对应于计算矩阵每一行与列向量乘积
		  #pragma unroll下面的for循环是串行的，用于累加求和，结果写入id对应的数组元素
============================================================================================================*/
extern "C"
void GPU_Correct(double* CorrectData,int CorrectDataNumber,double* DemData,GRDRANGEINDEX* IndexArray,GRDSTRUCT demdatainfo)
{
	//CUDA错误检查变量
	cudaError_t err=cudaSuccess;
	//申请GPU输入数组
	size_t size=CorrectDataNumber*sizeof(double);
	double *d_CorrectData=NULL;
	err=cudaMalloc((void**)&d_CorrectData,size);
	if (err!=cudaSuccess){AfxMessageBox(_T("申请GPU数组d_CorrectData失败\n"));exit(EXIT_FAILURE);}

	size_t size_indexarray=CorrectDataNumber*sizeof(GRDRANGEINDEX);
	GRDRANGEINDEX *d_IndexArray=NULL;
	err=cudaMalloc((void**)&d_IndexArray,size_indexarray);
	if (err!=cudaSuccess){AfxMessageBox(_T("申请GPU数组d_IndexArray失败\n"));exit(EXIT_FAILURE);}

	size_t size_demdata=demdatainfo.rows*demdatainfo.cols*sizeof(double);
	double *d_DemData=NULL;
	err=cudaMalloc((void**)&d_DemData,size_demdata);
	if (err!=cudaSuccess){AfxMessageBox(_T("申请GPU数组d_DemData失败\n"));exit(EXIT_FAILURE);}

	//将host存储空间拷贝到GPU
	//AfxMessageBox(_T("host存储空间拷贝到GPU\n"));
	err=cudaMemcpy(d_CorrectData,CorrectData,size,cudaMemcpyHostToDevice);
	if (err!=cudaSuccess){AfxMessageBox(_T("拷贝存储空间CorrectData到device失败\n"));exit(EXIT_FAILURE);}

	err=cudaMemcpy(d_IndexArray,IndexArray,size_indexarray,cudaMemcpyHostToDevice);
	if (err!=cudaSuccess){AfxMessageBox(_T("拷贝存储空间IndexArray到device失败\n"));exit(EXIT_FAILURE);}

	err=cudaMemcpy(d_DemData,DemData,size_demdata,cudaMemcpyHostToDevice);
	if (err!=cudaSuccess){AfxMessageBox(_T("拷贝存储空间DemData到device失败\n"));exit(EXIT_FAILURE);}


	//调用CUDA核函数
	int threadsPerBlock=512;//获取设备参数
	int blockPerGrid=(CorrectDataNumber+threadsPerBlock-1)/threadsPerBlock;
	//printf("CUDA核函数调用 %d 个blocks，%d 个线程\n",blockPerGrid,threadsPerBlock);
	DemCorrect<<<blockPerGrid,threadsPerBlock>>>(d_DemData,d_IndexArray,d_CorrectData,CorrectDataNumber,demdatainfo);		//GPU计算核函数
	err=cudaGetLastError();
	if (err!=cudaSuccess){AfxMessageBox(_T("调用CUDA核函数失败（错误代码：%s)！\n"));exit(EXIT_FAILURE);}

	
	//从GPU拷贝结果数据到内存
	//printf("从GPU拷贝结果数据到内存\n");
	err=cudaMemcpy(CorrectData,d_CorrectData,size,cudaMemcpyDeviceToHost);
	CString errorstr;errorstr.Format(_T("从GPU拷贝数据失败（错误代码：%s)！\n"),cudaGetErrorString(err));
	if (err!=cudaSuccess){AfxMessageBox(errorstr);exit(EXIT_FAILURE);}

	//释放内存
	err=cudaFree(d_CorrectData);
	if (err!=cudaSuccess){AfxMessageBox(_T("CUDA释放d_A存储空间失败（错误代码：%s)！\n"));exit(EXIT_FAILURE);}
	err=cudaFree(d_IndexArray);
	if (err!=cudaSuccess){AfxMessageBox(_T("CUDA释放d_B存储空间失败（错误代码：%s)！\n"));exit(EXIT_FAILURE);}
	err=cudaFree(d_DemData);
	if (err!=cudaSuccess){AfxMessageBox(_T("CUDA释放d_C存储空间失败（错误代码：%s)！\n"));exit(EXIT_FAILURE);}

	//重置device
	err=cudaDeviceReset();
	if (err!=cudaSuccess){AfxMessageBox(_T("重置deviec失败（错误代码：%s)！\n",));exit(EXIT_FAILURE);}
}
/*===========================================================================================================
 函数功能：获取并打印设备参数
============================================================================================================*/
extern "C"
void GetGPUProperty()
{
	//1.1 获取GPU个数
	int deviceCount;
	cudaGetDeviceCount(&deviceCount);
	
	//1.2 获取GPU设备参数
	int devID=0;			//设备号
	cudaDeviceProp deviceProp;
	cudaGetDeviceProperties(&deviceProp,devID);//deviceProp.major 和deviceProp.minor 分别为CUDA设备计算能力的主版本号和次版本号。CPU的这两个值都被设为9999

	//--1.2.1 设备中全局存储器大小
	int TotalGlobalMem=deviceProp.totalGlobalMem;//设备中的全局存储器大小（global memory）。如果是独立显卡，就是板载显存的大小。
	//--1.2.2 SM个数
	int SMcount=deviceProp.multiProcessorCount;//设备中可用的流多核处理器SM的个数。每个SM中8个SP,所以SP的个数为：8* deviceProp.multiProcessorCout
	int SPcount=SMcount*8;
	//--1.2.3 常量存储器大小
	int ConstMem=deviceProp.totalConstMem;//常量存储器的大小
	//--1.2.4
	int ShareMem=deviceProp.sharedMemPerBlock;//每个block中可用的共享存储器大小
	//--1.2.5
	int RegsCount=deviceProp.regsPerBlock;//每个block中可用的寄存器个数
	//--1.2.6 
	int ThreadCount=deviceProp.warpSize;//线程束warp由多少个thread组成
	//--1.2.7
	int ThreadInBlock=deviceProp.maxThreadsPerBlock;//一个block中最多可用有多少个thread

	//1.3 输出GPU信息
	CString gpupropstr,tempstr;
	gpupropstr.Format(_T("您的GPU个数共有: %d 个\n"),deviceCount);
	tempstr.Format(_T("全局存储器大小(Mb):%d\nThread总数：%d\n"),TotalGlobalMem/1024/1024,ThreadCount);
	gpupropstr+=tempstr;
	tempstr.Format(_T("每个block中Thread个数：%d\n每个block中可用寄存器个数：%d\nSP个数：%d\n"),ThreadInBlock,RegsCount,SPcount);
	gpupropstr+=tempstr;
	AfxMessageBox(gpupropstr);
	printf("每个block中可用共享存储器大小：%d\n",ShareMem);
	printf("常量储器大小：%d\n",ConstMem);
	printf("SM个数：%d\n",SMcount);
}
