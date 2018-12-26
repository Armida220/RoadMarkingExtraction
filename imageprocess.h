#ifndef IMAGE_H
#define IMAGE_H
#include <opencv2/opencv.hpp>   
#include <opencv2/highgui/highgui.hpp>
#include <pcl/point_cloud.h>  
#include <pcl/point_types.h>  
#include <pcl/io/pcd_io.h> 
#include <array>
#include <algorithm>
#include "utility.h"

using namespace utility;
using namespace cv;

namespace image{

	class Imageprocess
	{
	  public:
		  void pcgrid(vector<double> &boundingbox, float resolution);                   //ͶӰ���Ƹ�����(����������ƣ�
		  void savepcgrid(vector<double> &boundingbox, float resolution, const pcXYZIPtr &c, const pcXYZIPtr &gc, const pcXYZIPtr &ngc);   //ͶӰ���Ƹ���������¼���
		  void pc2imgI(const pcXYZIPtr &cloud, int whatcloud,  Mat &img);               //����ת����ǿ��ͼ��  whatcloud[ 0: Original Cloud , 1: Ground Cloud , 2: Non-ground Cloud]
		  void pc2imgZ(const pcXYZIPtr &cloud, int whatcloud,  Mat &img);               //����ת�߳�ͼ��      whatcloud[ 0: Original Cloud , 1: Ground Cloud , 2: Non-ground Cloud]
		  void pc2imgD(const pcXYZIPtr &cloud, int whatcloud,  Mat &img);               //����ת���ܶ�ͼ��    whatcloud[ 0: Original Cloud , 1: Ground Cloud , 2: Non-ground Cloud]
		 
		  void img2pc(const Mat &img, const pcXYZIPtr &incloud, pcXYZIPtr & outcloud);  //����ͼ��תԭʼ�޹����������
		  void img2pclabel(const Mat &img, const pcXYZIPtr &incloud, vector<pcXYZI> &outclouds, double dZ);   //�ָ����ͼ��ת�ָ����

		  Mat Sobelboundary(Mat img0);                                                  //Sobel ͼ���Ե��ȡ
		  
		  float caculateCurrentEntropy(Mat hist, int threshold);                        //���㵱ǰ��ֵ��ǰ����
		  Mat maxEntropySegMentation(Mat inputImage);                                   //�������ֵ�ָ�
		  Mat ExtractRoadPixel(const Mat & _binI,const Mat & _binZ,const Mat & _binD);  //�ø߳��ݶ��˳��ǵ�·����

		  void RemoveSmallRegion(const Mat &_binImg, Mat &_binfilter, int AreaLimit);   //�˳����������ٵ���ͨ�� (�����е��Ч�ɣ�
		  void CcaByTwoPass(const Mat & _binfilterImg, Mat & _labelImg);                //����ɨ�跨��ͨ�ɷַ�����4����
		  void CcaBySeedFill(const Mat& _binfilterImg, Mat & _lableImg);                //������䷨��ͨ�ɷַ�����8����
		  
		  void ImgReverse(const Mat &img, Mat &img_reverse);                            //��ֵ��ͼ��ɫ
		  void ImgFilling(const Mat &img, Mat &img_fill);                               //�ն����

		  void LabelColor(const Mat & _labelImg, Mat & _colorImg);                      //��ͨ����ɫ
		  Scalar GetRandomColor();                                                      //���ȡɫ��

		  void DetectCornerHarris(const Mat & src, const Mat & colorlabel, Mat & cornershow, Mat & cornerwithimg, int threshold);                                //Harris�ǵ���
		  void DetectCornerShiTomasi(const Mat & src, const Mat & colorlabel, Mat & cornerwithimg, int minDistance, double mincorenerscore);                     //Shi-Tomasi�ǵ���

		  void Truncate(const Mat & Img, Mat & TImg);                                    //ͼ���0�����ȡ

		  //Save img
		  void saveimg(const Mat &ProjI, const Mat &ProjZ, const Mat &ProjD, const Mat &ProjImf, const Mat &GI, const Mat &GZ, const Mat &BZ, const Mat &BD, const Mat &GIR, const Mat &BI, const Mat &BIF, const Mat &Label/*const Mat &Corner*/);
	  protected:
	
	  private:
		  int nx, ny; //pixel number
		  int timin, tjmin; //truncated pixel no.
		  float minX, minY, minZ;  //bounding box minimum value
		  float res;  //resolution
		  vector<vector<vector<int>>> CMatrixIndice;    //cloud
		  vector<vector<vector<int>>> GCMatrixIndice;   //ground cloud
		  vector<vector<vector<int>>> NGCMatrixIndice;  //non-ground cloud
		  int totallabel; //���˺������ͨ����
		  vector<vector <int>>  labelx;   //���label������X����
		  vector<vector <int>>  labely;   //���label������Y����

	};

}

#endif
