#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include "stdafx.h"
#include "utility.h"
#include "KMeans.h"
#include <pcl/ModelCoefficients.h>
#include <pcl/filters/project_inliers.h>
#include <opencv2/opencv.hpp>   
#include <opencv2/highgui/highgui.hpp>

using namespace utility;
using namespace std;

namespace PCprocess
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


	class Csegmentation
	{
	public:
		
		void GroundFilter_PMF(const pcXYZIPtr &cloud, pcXYZIPtr &gcloud, pcXYZIPtr &ngcloud);   // PMF �������˲�;
		void GroundFilter_PMF(const pcXYZIPtr &cloud, pcXYZIPtr &gcloud, pcXYZIPtr &ngcloud, int max_window_size, float slope, float initial_distance, float max_distance);   // PMF �������˲� ��������;
		
		pcXYZIPtr planesegRansac(const pcXYZIPtr &cloud, float threshold);   //Ransac ƽ�����;
		pcXYZIPtr groundprojection(const pcXYZIPtr &cloud);                  //���ƽ�ά���(ͶӰ��;
		void cloudFilter(const vector<pcXYZI> &inclouds, vector<pcXYZI> &outclouds, int N, int MeanK, double std);  //Otsu Intensity Thresholding + SOR Filter 
		void SORFilter(const pcXYZI &inclouds, pcXYZI &outclouds, int MeanK, double std); //SOR ��Statisics Outliers Remover) ���ҲӦ��д�ɶ����е��ƵĴ���ȽϺ�;
		void NFilter(const vector<pcXYZI> &inclouds, vector<pcXYZI> &outclouds, int K); //���ܵ�������һ�Σ�С��K�����ɾȥ

		void BoundingInformation(const vector<pcXYZI> &clouds, vector<vector<pcl::PointXYZI>> & boundingdatas); //bounding 4 points ��ȡ;
		void BoundingFeatureCalculation(const vector<vector<pcl::PointXYZI>> & boundingdatas, vector<BoundingFeature> & boundingfeatures); //bounding 4 points ������������;
		void BoundaryExtraction(const vector<pcXYZI> &clouds, vector<pcXYZI> &boundaryclouds);                 //����ȡ�߽��;
		void CornerExtraction(const vector<pcXYZI> &boundaryclouds, vector<pcXYZI> &cornerclouds, bool UseRadius, int K, float radius, float dis_threshold, float maxcos);     //����ȡ�߽��ϵĽǵ�;
		
		void CategoryJudgementBox_highway(const vector<BoundingFeature> & boundingfeatures, vector<RoadMarking> & roadmarkings); //���ٹ�· ��·���ߴַ���(����bounding box��Ϣ��;
		void CategoryJudgementBox_cityroad(const vector<BoundingFeature> & boundingfeatures, vector<RoadMarking> & roadmarkings); //���е�· ��·���ߴַ���(����bounding box��Ϣ��;

		void CombineSideLines(const vector<RoadMarking> & roadmarkings, double combine_length, vector <RoadMarking> & combine_sideline_markings);  // ��·�������л�����;
		void Find_head2tail(int head_index, const vector<vector<double>> & d_head_tail, vector<bool> & line_used, vector<int> & combineline, double combine_length);   //�ݹ麯��
		void Find_tail2head(int tail_index, const vector<vector<double>> & d_tail_head, vector<bool> & line_used, vector<int> & combineline, double combine_length);   //�ݹ麯��
		
		void MarkingVectorization_highway(const vector<pcXYZI> &clouds, const vector<vector<pcl::PointXYZI>> &boundingdatas, const vector<vector<pcl::PointXYZI>> &modeldatas, vector<RoadMarking> & roadmarkings, const vector<bool> & is_rights, double line_sample_dl, double ambiguousRatio);  //��·���߷��༰ʸ���� //line_sample_dl �߲�������   // ambiguousRatio ģ���������� //0.2
		void MarkingVectorization_cityroad(const vector<pcXYZI> &clouds, const vector<vector<pcl::PointXYZI>> &boundingdatas, const vector<vector<pcl::PointXYZI>> &modeldatas, vector<RoadMarking> & roadmarkings, const vector<bool> & is_rights, double line_sample_dl, double ambiguousRatio);  //��·���߷��༰ʸ���� //line_sample_dl �߲�������   // ambiguousRatio ģ���������� //0.2

		pcXYZI alphashape(const pcXYZI &cloud, float alpha_value);   //Concave Hull Generation
		
		pcXYZI CornerpointKNN(const pcXYZI &boundarycloud, int K, float disthreshold, float maxcos);                   //KNN corner point extraction 
		pcXYZI CornerpointRadius(const pcXYZI &boundarycloud, float radius, float disthreshold, float maxcos);         //Radius corner point extraction 

		pcXYZI CornerClusteringKMeans(const pcXYZI &cornercloud, int K);


	protected:
	
	private:
		
		
	};
}
#endif