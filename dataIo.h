#ifndef DATAIO_H
#define DATAIO_H

#include "stdafx.h"
#include "utility.h"
#include "pointcloudprocess.h"
#include <pcl/point_cloud.h>  
#include <pcl/point_types.h>  
#include <pcl/io/pcd_io.h>  
#include <vector>
#include <liblas/liblas.hpp>
#include <liblas/version.hpp>
#include <liblas/point.hpp>
#include <boost/filesystem.hpp>
#include <boost/function.hpp>
#include <strstream>
#include <fstream>

//Ϊ�˲����Flann��ͻ����ΪOpenCV��PCL�ﶼ���ˣ��������importCV�����
using namespace std;
using namespace utility;
using namespace PCprocess;

class DataIo
{
public:
	
	struct Paralist{
		// Default value
		float intensity_scale = 8;
		float keypoint_nonmax_radius = 0.2;
		float geometric_consistency_tolerant = 0.3;
		float search_radius = 0.4;
		float tolerantMinOverlap_ratio = 0.7;
		float sideline_vector_distance = 3.5;

	};

	//Parameter List Related
	void readParalist(string paralistfile);
	void displayparameter(int datatype, int roadtype, int is_road_extracted);


	//Read from folder
	bool batchReadFileNamesInFolders(const std::string &folderName,//�ļ���·��;
		const std::string & extension,//Ŀ���ļ�����չ��;
		std::vector<std::string> &fileNames);//����Ŀ���ļ���·��+�ļ���+��չ��;

	bool batchReadFileNamesInFoldersAndSubFolders(const std::string &folderName,//�ļ���·��;
		const std::string & extension,//Ŀ���ļ�����չ��;
		std::vector<std::string> &fileNames);//����Ŀ���ļ���·��+�ļ���+��չ��;

	//pcd file
	bool readPcdFile(const string &fileName, const pcXYZRGBAPtr &pointCloud);
	bool writePcdFile(const string &fileName, const pcXYZIPtr &pointCloud);
	bool writePcdFile(const string &fileName, const pcXYZRGBPtr &pointCloud);
	bool writePcdAll(const string &foldername ,const string &fileName, const vector<pcXYZI> &pointClouds);
	bool writePcdAll(const string &foldername, const string &fileName, const pcXYZIPtr &NGCloud, const pcXYZIPtr &GCloud, const vector<pcXYZI> &pointClouds, const vector<RoadMarking> &roadmarkings); //���أ� ���� �ǵ����Ҳһ�����;
	bool writePcdAll(const string &foldername, const string &fileName, const vector<pcXYZI> &pointClouds, const vector<RoadMarking> &roadmarkings, double minX, double minY); //����  ������ߵ��ƾ������� (pcd�е�ë������;
	bool writePcdAll(const string &foldername, const string &fileName, const vector<pcXYZI> &pointClouds, const vector<RoadMarking> &roadmarkings); //����  ������ߵ����������;
	
	//las file  
	bool readLasFileHeader(const std::string &fileName, liblas::Header& header);
	bool readLasFile(const std::string &fileName, pcl::PointCloud<pcl::PointXYZI> &pointCloud); // ����bounding box
	bool readLasFile(const string &fileName, pcl::PointCloud<pcl::PointXYZI> &pointCloud, vector<double> & boundingbox, double & X_min, double & Y_min); // ���أ������Դ���bounding box ��������������˳�� Xmin, Ymin, Zmin, Xmax, Ymax, Zmax , �Լ�ƽ�ƺ������ԭ����Ϣ;
	bool writeLasFile(const string &fileName, pcl::PointCloud<pcl::PointXYZI> &pointCloud);
	bool writeLasFile(const std::string &fileName, pcl::PointCloud<pcl::PointXYZI> &pointCloud, liblas::Color lasColor); //����
	bool writeLasFile(const std::string &fileName, pcl::PointCloud<pcl::PointXYZRGB> &pointCloud); //����
	bool batchWriteLasFile(const std::string &folderName, const std::vector<std::string> &fileNames,std::vector<pcl::PointCloud<pcl::PointXYZI>> &pointClouds);
	bool batchWriteLasFile(const std::string &folderName, const std::vector<std::string> &fileNames,std::vector<pcl::PointCloud<pcl::PointXYZRGB>> &pointClouds);
	bool mergeLasFileHeaders(const std::vector<liblas::Header>& header, liblas::Header& mergeFileHeader);
	bool mergeLasFilesColoredByFile(const string &folderName);
	bool mergeLasFilesIntensity(const string &folderName);

	//ply file
	bool readPlyFile(const string &fileName, const pcXYZIPtr &pointCloud);
	
	//txt file
	bool writemarkVectTXT(const vector<RoadMarking> &roadmarkings);

	//dxf file
	bool writemarkVectDXF(const vector<RoadMarking> &roadmarkings);
	bool writemarkVectDXF(const vector<RoadMarking> &roadmarkings, double minX,double minY); //���أ� ��������
	bool writemarkVectDXF(const vector<RoadMarking> &roadmarkings, const vector<RoadMarking> &sideline_markings, double minX, double minY); // ���أ� �������� + �������л�

	//display
	void displayroad(const pcXYZIPtr &ngcloud, const pcXYZIPtr &gcloud);
	void displaymark(const vector<pcXYZI> &clouds);
	void displaymarkwithng(const vector<pcXYZI> &clouds, const pcXYZIPtr &ngcloud);
	void displaymarkbycategory(const vector<pcXYZI> &clouds, const vector<RoadMarking> &roadmarkings);
	void displaymarkVect(const vector<RoadMarking> &roadmarkings);
	void displaymarkVect(const vector<RoadMarking> &roadmarkings, const vector<RoadMarking> &sideline_markings); // ���أ� �������л�
	
	//Boudning Box
	struct Bounds
	{
		double minx;
		double miny;
		double minz;
		double maxx;
		double maxy;
		double maxz;
		Bounds()
		{
			minx = 0.0;
			miny = 0.0;
			minz = 0.0;
			maxx = 0.0;
			maxy = 0.0;
			maxz = 0.0;
		}
	};

	bool GetBoundaryOfPointCloud(pcl::PointCloud<pcl::PointXYZI> &pointCloud, Bounds &bound);
	bool GetBoundaryOfPointCloud(pcl::PointCloud<pcl::PointXYZRGB> &pointCloud, Bounds &bound);

	Paralist paralist;
private:

};

#endif 
