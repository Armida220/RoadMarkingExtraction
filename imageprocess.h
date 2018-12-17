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
		  void pcgrid(const pcXYZIPtr &cloud, float resolution, int &nx, int &ny);      //ͶӰ���Ƹ�����
		  void pc2imgI(const pcXYZIPtr &cloud, float resolution, Mat &img);             //����ת����ǿ��ͼ��
		  void pc2imgZ(const pcXYZIPtr &cloud, float resolution, Mat &img);             //����ת�߳�ͼ��

		  Mat Sobelboundary(Mat img0);                                                  //Sobel ͼ���Ե��ȡ
		  
		  float caculateCurrentEntropy(Mat hist, int threshold);                        //���㵱ǰ��ֵ��ǰ����
		  Mat maxEntropySegMentation(Mat inputImage);                                   //�������ֵ�ָ�
		  Mat ExtractRoadPixel(const Mat & _binI, const Mat & _binZ);                   //�ø߳��ݶ��˳��ǵ�·����

		  void RemoveSmallRegion(const Mat &_binImg, Mat &_binfilter, int AreaLimit);   //�˳����������ٵ���ͨ�� (�����е��Ч�ɣ�
		  void CcaByTwoPass(const Mat & _binfilterImg, Mat & _labelImg);                //����ɨ�跨��ͨ�ɷַ�����4����
		  void CcaBySeedFill(const Mat& _binfilterImg, Mat & _lableImg);                //������䷨��ͨ�ɷַ�����8����
		  
		  void LabelColor(const Mat & _labelImg, Mat & _colorImg);                      //��ͨ����ɫ
		  Scalar GetRandomColor();                                                      //���ȡɫ��

		  Mat Truncate(const Mat & Img);                                  //ͼ���0�����ȡ

		  //Save img
		  void saveimg(const Mat &ProjI, const Mat &ProjZ, const Mat &GI, const Mat &GZ, const Mat &BI, const Mat &BZ, const Mat &BI2, const Mat &Label);
	  protected:
	
	  private:
	};
}

#endif
