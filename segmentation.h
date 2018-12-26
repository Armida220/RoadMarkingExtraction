#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include "utility.h"
#include "stereoBinaryFeature.h"
#include <pcl/ModelCoefficients.h>
#include <pcl/filters/project_inliers.h>
#include <opencv2/opencv.hpp>   
#include <opencv2/highgui/highgui.hpp>

using namespace utility;
using namespace std;

namespace segmentation
{
	typedef std::vector<float> BagOfVisualWords;
	typedef std::vector<float> VisualWordsHistogram;

	struct RoadMarking  
	{
		int category;  //Category,from 1 to 5
		
		vector<pcl::PointXYZI> polyline; //For vectorization

		//Other semantic or topological information
	};

	struct BoundingFeature
	{
		double corner; //�Ƕ���
		vector<double> sortingEdges; //������������ ���������У�
	};

	struct SegMetric
	{
		size_t detectedNum;
		size_t groundTruthNum;
		size_t correctNum;
		float averageRatio;
		float precision;
		float recall;
		float f1;
	};

	struct Object
	{
		size_t objectLabel;
		size_t objectClass;
		int topic;
		PointCloudBound boundingBox;
		std::vector<size_t> ptIndexes;
		std::vector<size_t> kptIndexes;
		std::vector<int> vwIndexes;
		std::vector<int> vw2Indexes;
		BagOfVisualWords bovw;
		VisualWordsHistogram vwh;
		float difference;
		bool isChanged;
		Object()
		{
			objectLabel = objectClass = 0;
			isChanged = true;
		}

	};

	typedef std::vector<Object> OneSegmentation;
	typedef std::vector<OneSegmentation> multiSegmentation;


	class Csegmentation:public Cutility
	{
	public:
		pcXYZIPtr planesegRansac(const pcXYZIPtr &cloud, float threshold);   //Ransac ƽ�����
		pcXYZIPtr groundprojection(const pcXYZIPtr &cloud);                  //���ƽ�ά���
		void cloudFilter(const vector<pcXYZI> &inclouds, vector<pcXYZI> &outclouds, int N, int MeanK, double std);  //Otsu Intensity Thresholding + SOR Filter
		void SORFilter(const pcXYZI &inclouds, pcXYZI &outclouds, int MeanK, double std); //SOR ��Statisics Outliers Remover) ���ҲӦ��д�ɶ����е��ƵĴ���ȽϺ�
		void NFilter(const vector<pcXYZI> &inclouds, vector<pcXYZI> &outclouds, int K); //���ܵ�������һ�Σ�С��K�����ɾȥ

		void BoundingInformation(const vector<pcXYZI> &clouds, vector<vector<pcl::PointXYZI>> & boundingdatas); //bounding 4 points ��ȡ
		void BoundingFeatureCalculation(const vector<vector<pcl::PointXYZI>> & boundingdatas, vector<BoundingFeature> & boundingfeatures); //bounding 4 points ������������
		void CategoryJudgement(const vector<BoundingFeature> & boundingfeatures, vector<RoadMarking> & roadmarkings); //��·���ߴַ���
		void MarkingVectorization(const vector<pcXYZI> &clouds, vector<RoadMarking> & roadmarkings, double line_sample_dl);  //��·���߷��༰ʸ���� //line_sample_dl �߲�������

		void getVisualWordsIndex(const vectorSBF &bscFeats, multiSegmentation &multiObjects);  //�Ӿ����ʼ���
		void  getGroundTruth(const VePointXYZLC &ptlcs, vector<Object> &groundTruth);

	protected:
		void  getObjectBound(const VePointXYZLC &ptlcs, Object &object);
		bool  isOverlapping(const Object &object1, const Object &object2);
	private:
		
		void  findOverlappingGT(const Object &object, const vector<Object> &groundTruth, vector<Object> &overlappingGT);
		float findMaxRatio(const Object &object, const vector<Object> &overlappingGT);
		size_t calculateIntersection(const std::vector<size_t> &indexes1, const std::vector<size_t> &indexes2);
		size_t calculateUnion(const std::vector<size_t> &indexes1, const std::vector<size_t> &indexes2);
	};
}
#endif