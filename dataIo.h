#ifndef DATAIO_H
#define DATAIO_H


#include "utility.h"
#include "segmentation.h"
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
using namespace segmentation;

class DataIo: public Cutility
{
public:
	void readPointXYZLC(const string &fileName, vector<PointXYZLC> &pts);
	
	//pcd file
	bool readPcdFile(const string &fileName, const pcXYZIPtr &pointCloud);
	bool writePcdFile(const string &fileName, const pcXYZIPtr &pointCloud);
	bool writePcdAll(const string &foldername ,const string &fileName, const vector<pcXYZI> &pointClouds);

	//las file  
	bool readLasFileHeader(const std::string &fileName, liblas::Header& header);
	bool readLasFile(const string &fileName, pcl::PointCloud<pcl::PointXYZI> &pointCloud, vector<double> & boundingbox); // �����Դ���bounding box ��������������˳�� Xmin, Ymin, Zmin, Xmax, Ymax, Zmax
	bool writeLasFile(const string &fileName, pcl::PointCloud<pcl::PointXYZI> &pointCloud);
	bool mergeLasFileHeaders(const std::vector<liblas::Header>& header, liblas::Header& mergeFileHeader);
	bool mergeLasFilesColoredByFile(const string &folderName);
	bool mergeLasFilesIntensity(const string &folderName);
	
	//txt file
	bool writemarkVectTXT(const vector<RoadMarking> &roadmarkings);

	//dxf file
	bool writemarkVectDXF(const vector<RoadMarking> &roadmarkings);

	//display
	void displayroad(const pcXYZIPtr &ngcloud, const pcXYZIPtr &gcloud);
	void displaymark(const vector<pcXYZI> &clouds);
	void displaymarkwithng(const vector<pcXYZI> &clouds, const pcXYZIPtr &ngcloud);
	void displaymarkVect(const vector<RoadMarking> &roadmarkings);


private:

};

#endif 
