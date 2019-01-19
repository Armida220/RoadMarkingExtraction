#include "stdafx.h"
#include "dataIo.h"
#include "utility.h"
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/io/ply_io.h>
#include <liblas/liblas.hpp>
#include <liblas/version.hpp>
#include <liblas/point.hpp>
#include <pcl/visualization/common/common.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <iosfwd>
#include <fstream>
#include <intsafe.h>
#include <dl_dxf.h>
#include <dl_entities.h>

using namespace  std;
using namespace  utility;
using namespace  boost::filesystem;


void DataIo::readParalist(string paralistfile)
{
	ifstream infile;   //������
	infile.open(paralistfile, ios::in);
	if (!infile.is_open()) cout << "Open file failure, use default parameters." << endl;

	/*
	float intensity_scale = 8;
	float keypoint_nonmax_radius = 0.2;
	float geometric_consistency_tolerant = 0.3;
	float search_radius = 0.4;
	float tolerantMinOverlap_ratio = 0.7;
	float sideline_vector_distance = 3.5;
	*/

	infile >> paralist.intensity_scale;
	infile >> paralist.keypoint_nonmax_radius;
	infile >> paralist.geometric_consistency_tolerant;
	infile >> paralist.search_radius;
	infile >> paralist.tolerantMinOverlap_ratio;	
	infile >> paralist.sideline_vector_distance;


}

void DataIo::displayparameter(int datatype, int roadtype, int is_road_extracted){
	cout << "--------------------------------------------------------" << endl;
	cout << "Parameter Specification:" << endl;
	
	if (datatype == 1) cout << "MLS/TLS\t";
	else cout << "ALS data\t";
		
	if (roadtype == 1)cout << "Highway\t";
	else cout << "City road\t";
	
	if (is_road_extracted == 1) cout << "Road unextracted\n";
	else cout << "Road extracted\n";

	cout << "Point Cloud Intensity scale:\t" << paralist.intensity_scale << endl;
	cout << "Model matching keypoint non-max suppression radius (m):\t" << paralist.keypoint_nonmax_radius << endl;
	cout << "Model matching geometric consistency tolerant distance (m):\t" << paralist.geometric_consistency_tolerant << endl;
	cout << "Model matching overlapping search radius (m):\t" << paralist.search_radius << endl;
	cout << "Model matching tolerant min overlapping ratio:\t" << paralist.tolerantMinOverlap_ratio << endl;
	cout << "Sideline vectorization distance (m):\t" << paralist.sideline_vector_distance << endl;

	cout << "--------------------------------------------------------" << endl;

}


bool DataIo::writeLasFile(const std::string &fileName, pcl::PointCloud<pcl::PointXYZI> &pointCloud)
{
	Bounds bound;
	GetBoundaryOfPointCloud(pointCloud, bound);

	ofstream ofs;
	ofs.open(fileName, std::ios::out | std::ios::binary);
	if (ofs.is_open())
	{
		liblas::Header header;
		header.SetDataFormatId(liblas::ePointFormat2);
		header.SetVersionMajor(1);
		header.SetVersionMinor(2);
		header.SetMin(bound.minx, bound.miny, bound.minz);
		header.SetMax(bound.maxx, bound.maxy, bound.maxz);
		header.SetOffset((bound.minx + bound.maxx) / 2.0, (bound.miny + bound.maxy) / 2.0, (bound.minz + bound.maxz) / 2.0);

		header.SetScale(0.01, 0.01, 0.01);
		header.SetPointRecordsCount(pointCloud.points.size());

		liblas::Writer writer(ofs, header);
		liblas::Point pt(&header);

		for (int i = 0; i < pointCloud.points.size(); i++)
		{
			pt.SetCoordinates(pointCloud.points[i].x, pointCloud.points[i].y, pointCloud.points[i].z);
			pt.SetIntensity(pointCloud.points[i].intensity);
			writer.WritePoint(pt);
		}
		ofs.flush();
		ofs.close();
	}

	return 1;
}

bool DataIo::writeLasFile(const std::string &fileName, pcl::PointCloud<pcl::PointXYZI> &pointCloud, liblas::Color lasColor)
{
	Bounds bound;
	GetBoundaryOfPointCloud(pointCloud, bound);

	ofstream ofs;
	ofs.open(fileName, std::ios::out | std::ios::binary);
	if (ofs.is_open())
	{
		liblas::Header header;
		header.SetDataFormatId(liblas::ePointFormat2);
		header.SetVersionMajor(1);
		header.SetVersionMinor(2);
		header.SetMin(bound.minx, bound.miny, bound.minz);
		header.SetMax(bound.maxx, bound.maxy, bound.maxz);
		header.SetOffset((bound.minx + bound.maxx) / 2.0, (bound.miny + bound.maxy) / 2.0, (bound.minz + bound.maxz) / 2.0);

		header.SetScale(0.01, 0.01, 0.01);
		header.SetPointRecordsCount(pointCloud.points.size());

		liblas::Writer writer(ofs, header);
		liblas::Point pt(&header);

		for (int i = 0; i < pointCloud.points.size(); i++)
		{
			pt.SetCoordinates(pointCloud.points[i].x, pointCloud.points[i].y, pointCloud.points[i].z);
			pt.SetIntensity(pointCloud.points[i].intensity);
			pt.SetColor(lasColor);
			writer.WritePoint(pt);
		}
		ofs.flush();
		ofs.close();
	}

	return 1;
}


bool DataIo::writeLasFile(const std::string &fileName, pcl::PointCloud<pcl::PointXYZRGB> &pointCloud)
{
	Bounds bound;
	GetBoundaryOfPointCloud(pointCloud, bound);

	ofstream ofs;
	ofs.open(fileName, std::ios::out | std::ios::binary);
	if (ofs.is_open())
	{
		liblas::Header header;
		header.SetDataFormatId(liblas::ePointFormat2);
		header.SetVersionMajor(1);
		header.SetVersionMinor(2);
		header.SetMin(bound.minx, bound.miny, bound.minz);
		header.SetMax(bound.maxx, bound.maxy, bound.maxz);
		header.SetOffset((bound.minx + bound.maxx) / 2.0, (bound.miny + bound.maxy) / 2.0, (bound.minz + bound.maxz) / 2.0);

		header.SetScale(0.0000001, 0.0000001, 0.0000001);
		header.SetPointRecordsCount(pointCloud.points.size());

		liblas::Writer writer(ofs, header);
		liblas::Point pt(&header);

		for (int i = 0; i < pointCloud.points.size(); i++)
		{
			if (bound.maxx < 10.0&&bound.maxy < 10.0&&bound.maxz < 10.0)
			{
				pt.SetCoordinates((double)(pointCloud.points[i].x*1000.0),
					(double)(pointCloud.points[i].y*1000.0),
					(double)(pointCloud.points[i].z*1000.0));
			}
			else
			{
				pt.SetCoordinates((double)pointCloud.points[i].x, (double)pointCloud.points[i].y,
					(double)pointCloud.points[i].z);
			}

			uint8_t red = pointCloud.points[i].r;
			uint8_t green = pointCloud.points[i].g;
			uint8_t blue = pointCloud.points[i].b;
			pt.SetColor(liblas::Color(red, green, blue));
			writer.WritePoint(pt);
		}
		ofs.flush();
		ofs.close();
	}

	return 1;
}


bool DataIo::batchWriteLasFile(const std::string &folderName, const std::vector<std::string> &fileNames,
	std::vector<pcl::PointCloud<pcl::PointXYZI>> &pointClouds)
{
	path folderPath(folderName);

	//�����ļ���;
	if (!exists(folderName))
	{
		create_directory(folderName);
	}

	if (fileNames.empty())
	{
		return 0;
	}
	else
	{
		for (size_t i = 0; i < fileNames.size(); ++i)
		{
			path dir(fileNames[i]);
			string fileName;
			//��ȡ������·���ͺ�׺���ļ���;
			fileName = dir.stem().string();
			fileName = folderName + "\\" + fileName + ".las";
			writeLasFile(fileName, pointClouds[i]);
		}
	}

	return 1;
}

bool DataIo::batchWriteLasFile(const std::string &folderName, const std::vector<std::string> &fileNames,
	std::vector<pcl::PointCloud<pcl::PointXYZRGB>> &pointClouds)
{
	path folderPath(folderName);

	//�����ļ���;
	if (!exists(folderName))
	{
		create_directory(folderName);
	}

	if (fileNames.empty())
	{
		return 0;
	}
	else
	{
		for (size_t i = 0; i < fileNames.size(); ++i)
		{
			path dir(fileNames[i]);
			string fileName;
			//��ȡ������·���ͺ�׺���ļ���;
			fileName = dir.stem().string();
			fileName = folderName + "\\" + fileName + ".las";
			writeLasFile(fileName, pointClouds[i]);
		}
	}

	return 1;
}

bool DataIo::batchReadFileNamesInFolders(const std::string &folderName, const std::string & extension,
	std::vector<std::string> &fileNames)
{
	//����boost�����ļ����ڵ��ļ�;
	if (!exists(folderName))
	{
		return 0;
	}
	else
	{
		//�������ļ����µ�����ply�ļ�,�����浽fileNames��;
		directory_iterator end_iter;
		for (directory_iterator iter(folderName); iter != end_iter; ++iter)
		{
			if (is_regular_file(iter->status()))
			{
				string fileName;
				fileName = iter->path().string();

				path dir(fileName);

				if (!dir.extension().string().empty())
				{
					if (!fileName.substr(fileName.rfind('.')).compare(extension))
					{
						fileNames.push_back(fileName);
					}
				}
			}
		}
	}
	return 1;
}

bool DataIo::batchReadFileNamesInFoldersAndSubFolders(const std::string &folderName,//�ļ���·��;
	const std::string & extension,//Ŀ���ļ�����չ��;
	std::vector<std::string> &fileNames)//����Ŀ���ļ���·��+�ļ���+��չ��;
{
	boost::filesystem::path  fullpath(folderName);

	if (!exists(fullpath))
	{
		return false;
	}

	recursive_directory_iterator end_iter;
	for (recursive_directory_iterator iter(fullpath); iter != end_iter; iter++)
	{
		try
		{
			if (is_directory(*iter))
			{
			}
			else
			{
				std::string sFileName = iter->path().string();
				path dir(sFileName);

				if (!dir.extension().string().empty())
				{
					if (!sFileName.substr(sFileName.rfind('.')).compare(extension))
					{
						fileNames.push_back(sFileName);
					}
				}

			}
		}
		catch (const std::exception & ex)
		{
			std::cerr << ex.what() << std::endl;
			continue;
		}
	}
	return true;
}

bool DataIo::readPcdFile(const std::string &fileName, const pcXYZRGBAPtr &pointCloud)
{
	if (pcl::io::loadPCDFile(fileName, *pointCloud) == -1) //* load the file
	{
		PCL_ERROR("Couldn't read file\n");
		return false;
	}

	return true;
}

bool DataIo::writePcdFile(const std::string &fileName, const pcXYZIPtr &pointCloud)
{
	if (pcl::io::savePCDFileBinary<pcl::PointXYZI>(fileName, *pointCloud) == -1) //* load the file
	{
		PCL_ERROR("Couldn't read file\n");
		return false;
	}

	return true;
}

bool DataIo::writePcdFile(const string &fileName, const pcXYZRGBPtr &pointCloud)
{
	if (pcl::io::savePCDFileBinary<pcl::PointXYZRGB>(fileName, *pointCloud) == -1) //* load the file
	{
		PCL_ERROR("Couldn't read file\n");
		return false;
	}
	return true;
}

bool DataIo::writePcdAll(const string &foldername, const string &fileName, const pcXYZIPtr &NGCloud, const pcXYZIPtr &GCloud, const vector<pcXYZI> &pointClouds, const vector<RoadMarking> &roadmarkings)
{
	vector<pcXYZRGB> pointCloudsRGB;
	pointCloudsRGB.resize(pointClouds.size());

	//�����ɫ;
	for (size_t i = 0; i < pointClouds.size(); i++)
	{
		int R, G, B;
		
		switch (roadmarkings[i].category)
		{
		case 1:
			R = 255;
			G = 0;
			B = 0;
			break;
		case 2:
			R = 0;
			G = 255;
			B = 0;
			break;
		case 3:
			R = 0;
			G = 0;
			B = 255;
			break;
		case 4:
			R = 255;
			G = 0;
			B = 255;
			break;
		case 5:
			R = 0;
			G = 255;
			B = 255;
			break;
		default:
			R = 255;
			G = 255;
			B = 0;
			break;
		}
		for (size_t j = 0; j < pointClouds[i].size(); j++)
		{
			pcl::PointXYZRGB pt;
			pt.x = pointClouds[i].points[j].x;
			pt.y = pointClouds[i].points[j].y;
			pt.z = pointClouds[i].points[j].z;
			pt.r = R;
			pt.g = G;
			pt.b = B;
			pointCloudsRGB[i].push_back(pt);
		}
	}

	if (!boost::filesystem::exists(foldername))
	{
		boost::filesystem::create_directory(foldername);
	}

	for (size_t i = 0; i < pointClouds.size(); i++)
	{
		string  outputFileName;
		ostringstream oss;
		oss << i << "_" << fileName;
		outputFileName = foldername + "\\" + oss.str();

		writePcdFile(outputFileName, pointCloudsRGB[i].makeShared());
	}
	string NGCloud_Filename, GCloud_Filename;
	
	NGCloud_Filename = foldername + "\\Non_Ground_Cloud.pcd";
	GCloud_Filename = foldername + "\\Ground_Cloud.pcd";
	
	writePcdFile(NGCloud_Filename, NGCloud);
	writePcdFile(GCloud_Filename, GCloud);

	cout << "Output Done" << endl;
	return true;
}


bool DataIo::writePcdAll(const string &foldername, const string &fileName, const vector<pcXYZI> &pointClouds, const vector<RoadMarking> &roadmarkings, double minX, double minY)
{
	vector<pcXYZRGB> pointCloudsRGB;
	pointCloudsRGB.resize(pointClouds.size());

	//�����ɫ;
	for (size_t i = 0; i < pointClouds.size(); i++)
	{
		int R, G, B;

		switch (roadmarkings[i].category)
		{
		case 1:
			R = 255;
			G = 0;
			B = 0;
			break;
		case 2:
			R = 0;
			G = 255;
			B = 0;
			break;
		case 3:
			R = 0;
			G = 0;
			B = 255;
			break;
		case 4:
			R = 255;
			G = 0;
			B = 255;
			break;
		case 5:
			R = 0;
			G = 255;
			B = 255;
			break;
		default:
			R = 255;
			G = 255;
			B = 0;
			break;
		}
		for (size_t j = 0; j < pointClouds[i].size(); j++)
		{
			pcl::PointXYZRGB pt;
			pt.x = pointClouds[i].points[j].x+minX;
			pt.y = pointClouds[i].points[j].y+minY;
			pt.z = pointClouds[i].points[j].z;
			pt.r = R;
			pt.g = G;
			pt.b = B;
			pointCloudsRGB[i].push_back(pt);
		}
	}

	if (!boost::filesystem::exists(foldername))
	{
		boost::filesystem::create_directory(foldername);
	}

	for (size_t i = 0; i < pointClouds.size(); i++)
	{
		string  outputFileName;
		ostringstream oss;
		oss << i << "_" << fileName;
		outputFileName = foldername + "\\" + oss.str();

		writePcdFile(outputFileName, pointCloudsRGB[i].makeShared());
	}
	
	cout << "Output Done" << endl;
	return true;
}

bool DataIo::writePcdAll(const string &foldername, const string &fileName, const vector<pcXYZI> &pointClouds, const vector<RoadMarking> &roadmarkings)
{
	vector<pcXYZRGB> pointCloudsRGB;
	pointCloudsRGB.resize(pointClouds.size());

	//�����ɫ;
	for (size_t i = 0; i < pointClouds.size(); i++)
	{
		int R, G, B;

		switch (roadmarkings[i].category)
		{
		case 1:
			R = 0;
			G = 255;
			B = 0;
			break;
		case 2:
			R = 255;
			G = 0;
			B = 0;
			break;
		case 3:
			R = 0;
			G = 0;
			B = 255;
			break;
		case 4:
			R = 255;
			G = 0;
			B = 255;
			break;
		case 5:
			R = 0;
			G = 255;
			B = 255;
			break;
		default:
			R = 255;
			G = 255;
			B = 0;
			break;
		}
		for (size_t j = 0; j < pointClouds[i].size(); j++)
		{
			pcl::PointXYZRGB pt;
			pt.x = pointClouds[i].points[j].x ;
			pt.y = pointClouds[i].points[j].y ;
			pt.z = pointClouds[i].points[j].z;
			pt.r = R;
			pt.g = G;
			pt.b = B;
			pointCloudsRGB[i].push_back(pt);
		}
	}

	if (!boost::filesystem::exists(foldername))
	{
		boost::filesystem::create_directory(foldername);
	}

	for (size_t i = 0; i < pointClouds.size(); i++)
	{
		string  outputFileName;
		ostringstream oss;
		oss << i << "_" << fileName;
		outputFileName = foldername + "\\" + oss.str();

		writePcdFile(outputFileName, pointCloudsRGB[i].makeShared());
	}

	cout << "Output Done" << endl;
	return true;
}

bool DataIo::writePcdAll(const string &folderName, const string &fileName, const vector<pcXYZI> &pointClouds){

	//�����ļ���;
	/*if (boost::filesystem::exists(folderName))
	{
		directory_iterator end_iter;
		for (directory_iterator iter(folderName); iter != end_iter; ++iter)
		{
			if (is_regular_file(iter->status()))
			{
				string fileName;
				fileName = iter->path().string();
				boost::filesystem::remove(fileName);
			}
		}
	}*/

	if (!boost::filesystem::exists(folderName))
	{
		boost::filesystem::create_directory(folderName);
	}

	for (size_t i = 0; i < pointClouds.size(); i++)
	{
		string  outputFileName;
		ostringstream oss;
		oss << i << "_"<<fileName;	
		outputFileName = folderName + "\\" + oss.str();
		
		writePcdFile(outputFileName, pointClouds[i].makeShared());
	}

	cout << "Output Done" << endl;
	return true;

}

void DataIo::displayroad(const pcXYZIPtr &ngcloud, const pcXYZIPtr &gcloud)
{

	boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer(new pcl::visualization::PCLVisualizer("3D Viewer"));
	viewer->setBackgroundColor(255, 255, 255);
	char t[256];
	string s;
	int n = 0;
	float maxi,maxvi,mini,maxz,minz;
	maxi = -FLT_MAX;
	mini = FLT_MAX;
	maxz = -FLT_MAX;
	minz = FLT_MAX;

	int startcolor[3];
	int endcolor[3];
	int rr, gg, bb;
	float kk;

	//cout << "Input the color scale that you want to render the point cloud." << endl;
	//cout << "Start Color R,G,B:" << endl;

	//cin >> startcolor[0] >> startcolor[1] >> startcolor[2];

	//cout << "End Color R,G,B:" << endl;
	//cin >> endcolor[0] >> endcolor[1] >> endcolor[2];
	

	startcolor[0] = 255; startcolor[1] = 0; startcolor[2] = 0;
	endcolor[0] = 0; endcolor[1] = 255; endcolor[2] = 0;

	rr = startcolor[0] - endcolor[0];
	gg = startcolor[1] - endcolor[1];
	bb = startcolor[2] - endcolor[2];

	//maxvi = 10000;
	pcXYZRGBPtr GC(new pcXYZRGB());
	pcXYZRGBPtr NGC(new pcXYZRGB());

	for (size_t i = 0; i < ngcloud->points.size(); ++i)
	{
		if (ngcloud->points[i].z > maxz) maxz = ngcloud->points[i].z;
		if (ngcloud->points[i].z < minz) minz = ngcloud->points[i].z;
	}


	for (size_t i = 0; i < gcloud->points.size(); ++i)
	{
		if (gcloud->points[i].intensity > maxi) maxi = gcloud->points[i].intensity;
		if (gcloud->points[i].intensity < mini) mini = gcloud->points[i].intensity;
	}


	//cout << "max intensity: " << maxi << "\tmin intensity: " << mini<<endl;

	for (size_t i = 0; i < ngcloud->points.size(); ++i)
	{
		pcl::PointXYZRGB pt;
		pt.x = ngcloud->points[i].x;
		pt.y = ngcloud->points[i].y;
		pt.z = ngcloud->points[i].z;
		
		kk = (ngcloud->points[i].z - minz) / (maxz - minz);
		pt.r = endcolor[0] + rr * kk;
		pt.g = endcolor[1] + gg * kk;
		pt.b = endcolor[2] + bb * kk;
		NGC->points.push_back(pt);
	}
	
	viewer->addPointCloud(NGC, "Non-ground");

	// Ground points are rendered in intensity
	for (size_t i = 0; i < gcloud->points.size(); ++i)
	{
		pcl::PointXYZRGB pt;
		pt.x = gcloud->points[i].x;
		pt.y = gcloud->points[i].y;
		pt.z = gcloud->points[i].z;
		/*if (gcloud->points[i].intensity<maxvi)
		{
			pt.r = 255;
			pt.g = 255;
			pt.b = 255;
		}
		else{
			pt.r = 0;
			pt.g = 0;
			pt.b = 0;
		}*/
		//pt.r = (int)((min(gcloud->points[i].intensity, maxvi) - mini) / (maxvi - mini) * 255);
		//pt.g = (int)((min(gcloud->points[i].intensity, maxvi) - mini) / (maxvi - mini) * 255);
		//pt.b = (int)((min(gcloud->points[i].intensity, maxvi) - mini) / (maxvi - mini) * 255);

		pt.r = 255 * (gcloud->points[i].intensity - mini) / (maxi - mini);
		pt.g = 255 * (gcloud->points[i].intensity - mini) / (maxi - mini);
		pt.b = 255 * (gcloud->points[i].intensity - mini) / (maxi - mini);
		GC->points.push_back(pt);
	}

	viewer->addPointCloud(GC, "Ground");

	cout << "Click X(close) to continue..." << endl;
	while (!viewer->wasStopped())
	{
		viewer->spinOnce(100);
		boost::this_thread::sleep(boost::posix_time::microseconds(100000));
	}
}

void DataIo::displaymark(const vector<pcXYZI> &clouds){
	
	boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer(new pcl::visualization::PCLVisualizer("3D Viewer"));
	viewer->setBackgroundColor(255, 255, 255);
	char t[256];
	string s;
	int n = 0;

	vector<pcXYZRGB>  colorclouds;
	colorclouds.resize(clouds.size());
	for (int i = 0; i <clouds.size(); ++i)
	{
		int r = 255 * (rand() / (1.0 + RAND_MAX));
		int g = 255 * (rand() / (1.0 + RAND_MAX));
		int b = 255 * (rand() / (1.0 + RAND_MAX));

		for (size_t j = 0; j < clouds[i].points.size(); ++j)
		{
			pcl::PointXYZRGB pt;
			pt.x = clouds[i].points[j].x;
			pt.y = clouds[i].points[j].y;
			pt.z = clouds[i].points[j].z; 
			//�����ɫ
			
		    pt.r = r;
			pt.g = g;
			pt.b = b;
			
			colorclouds[i].points.push_back(pt);

		}
		string  colorcloud;
		ostringstream oss;
		oss << i <<  "_colorcloud";
		colorcloud = oss.str();

		viewer->addPointCloud(colorclouds[i].makeShared(),colorcloud);

	}	

	cout << "Click X(close) to continue..." << endl;
	while (!viewer->wasStopped())
	{
		viewer->spinOnce(100);
		boost::this_thread::sleep(boost::posix_time::microseconds(100000));
	}
}

void DataIo::displaymarkwithng(const vector<pcXYZI> &clouds, const pcXYZIPtr &ngcloud)
{
	boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer(new pcl::visualization::PCLVisualizer("3D Viewer"));
	viewer->setBackgroundColor(255, 255, 255);
	char t[256];
	string s;
	int n = 0;

	vector<pcXYZRGB>  colorclouds;
	colorclouds.resize(clouds.size());
	for (int i = 0; i < clouds.size(); ++i)
	{
		int r = 255 * (rand() / (1.0 + RAND_MAX));
		int g = 255 * (rand() / (1.0 + RAND_MAX));
		int b = 255 * (rand() / (1.0 + RAND_MAX));

		for (size_t j = 0; j < clouds[i].points.size(); ++j)
		{
			pcl::PointXYZRGB pt;
			pt.x = clouds[i].points[j].x;
			pt.y = clouds[i].points[j].y;
			pt.z = clouds[i].points[j].z;
			//�����ɫ

			pt.r = r;
			pt.g = g;
			pt.b = b;

			colorclouds[i].points.push_back(pt);

		}
		string  colorcloud;
		ostringstream oss;
		oss << i << "_colorcloud";
		colorcloud = oss.str();

		viewer->addPointCloud(colorclouds[i].makeShared(), colorcloud);

	}

	float  maxz, minz;
	maxz = -FLT_MAX;
	minz = FLT_MAX;

	int startcolor[3];
	int endcolor[3];
	int rr, gg, bb;
	float kk;

	startcolor[0] = 255; startcolor[1] = 0; startcolor[2] = 0;
	endcolor[0] = 0; endcolor[1] = 255; endcolor[2] = 0;

	rr = startcolor[0] - endcolor[0];
	gg = startcolor[1] - endcolor[1];
	bb = startcolor[2] - endcolor[2];

	pcXYZRGBPtr NGC(new pcXYZRGB());

	for (size_t i = 0; i < ngcloud->points.size(); ++i)
	{
		if (ngcloud->points[i].z > maxz) maxz = ngcloud->points[i].z;
		if (ngcloud->points[i].z < minz) minz = ngcloud->points[i].z;
	}

	for (size_t i = 0; i < ngcloud->points.size(); ++i)
	{
		pcl::PointXYZRGB pt;
		pt.x = ngcloud->points[i].x;
		pt.y = ngcloud->points[i].y;
		pt.z = ngcloud->points[i].z;

		kk = (ngcloud->points[i].z - minz) / (maxz - minz);
		pt.r = endcolor[0] + rr * kk;
		pt.g = endcolor[1] + gg * kk;
		pt.b = endcolor[2] + bb * kk;
		NGC->points.push_back(pt);
	}

	viewer->addPointCloud(NGC, "Non-ground");

	cout << "Click X(close) to continue..." << endl;
	while (!viewer->wasStopped())
	{
		viewer->spinOnce(100);
		boost::this_thread::sleep(boost::posix_time::microseconds(100000));
	}
}

void DataIo::displaymarkbycategory(const vector<pcXYZI> &clouds, const vector<RoadMarking> &roadmarkings)
{
	boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer(new pcl::visualization::PCLVisualizer("3D Viewer"));
	viewer->setBackgroundColor(0, 0, 0);
	char t[256];
	string s;
	int n = 0;

	vector<pcXYZRGB>  colorclouds;
	colorclouds.resize(clouds.size());
	for (int i = 0; i < clouds.size(); ++i)
	{
		int r, g, b;
		switch (roadmarkings[i].category)
		{
		case 1:  //������   //ǳ�� 135 206 250
			r = 0;
			g = 255;
			b = 0;
			break;
		
		case 2:  //������   //õ��� 255 48 48
			r = 255;
			g = 0;
			b = 0;
			break;

		case 3:  //ǰ���ͷ  //��ɫ 238 130 238
			r = 0;
			g = 0;
			b = 255;
			break;
		
		case 4:  //ǰ�����ͷ //ǳ�� 193 255 193
			r = 0;
			g = 255;
			b = 255;
			break;

		case 5:  //�����ͷ //�ۺ� 255 193 193
			r = 255;
			g = 0;
			b = 255;
			break;

		default:
			r = 255;
			g = 250;
			b = 250;
			break;
		}
		   
		for (size_t j = 0; j < clouds[i].points.size(); ++j)
		{
			pcl::PointXYZRGB pt;
			pt.x = clouds[i].points[j].x;
			pt.y = clouds[i].points[j].y;
			pt.z = clouds[i].points[j].z;

			pt.r = r;
			pt.g = g;
			pt.b = b;

			colorclouds[i].points.push_back(pt);

		}
		
		string  colorcloud;
		ostringstream oss;
		oss << i << "_colorcloud";
		colorcloud = oss.str();

		viewer->addPointCloud(colorclouds[i].makeShared(), colorcloud);
	}

	cout << "Click X(close) to continue..." << endl;
	while (!viewer->wasStopped())
	{
		viewer->spinOnce(100);
		boost::this_thread::sleep(boost::posix_time::microseconds(100000));
	}
}
void DataIo::displaymarkVect(const vector<RoadMarking> &roadmarkings)
{
	boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer(new pcl::visualization::PCLVisualizer("Vectorization Viewer"));
	viewer->setBackgroundColor(0, 0, 0);
	char t[256];
	string s;
	int n = 0;

	for (int i = 0; i < roadmarkings.size(); i++)
	{
		switch (roadmarkings[i].category)
		{
		case 1:  //������
			for (int j = 0; j < roadmarkings[i].polyline.size()-1; j++)
			{
				pcl::PointXYZ pt1;
				pt1.x = roadmarkings[i].polyline[j].x;
				pt1.y = roadmarkings[i].polyline[j].y;
				pt1.z = roadmarkings[i].polyline[j].z;
				sprintf(t, "%d", n);
				s = t;
				viewer->addSphere(pt1, 0.1, 0.0, 1.0, 0.0, s);
				n++;

				pcl::PointXYZ pt2;
		        pt2.x = roadmarkings[i].polyline[j + 1].x;
			    pt2.y = roadmarkings[i].polyline[j + 1].y;
			    pt2.z = roadmarkings[i].polyline[j + 1].z;
			
				sprintf(t, "%d", n);
				s = t;
				viewer->addSphere(pt2, 0.1, 0.0, 1.0, 0.0, s);
				n++;

				sprintf(t, "%d", n);
				s = t;
				viewer->addLine(pt1, pt2, 0.0, 1.0, 0.0, s);
				viewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_LINE_WIDTH, 1, s);
				n++;
			}

			break;
		case 2:
			for (int j = 0; j < 4; j++) //�������
			{
				pcl::PointXYZ pt1;
				pt1.x = roadmarkings[i].polyline[j].x;
				pt1.y = roadmarkings[i].polyline[j].y;
				pt1.z = roadmarkings[i].polyline[j].z;
				sprintf(t, "%d", n);
				s = t;
				viewer->addSphere(pt1, 0.1, 1.0, 0.0, 0.0, s);
				n++;


				pcl::PointXYZ pt2;
				if (j == 3){
					pt2.x = roadmarkings[i].polyline[0].x;
					pt2.y = roadmarkings[i].polyline[0].y;
					pt2.z = roadmarkings[i].polyline[0].z;
				}
				else{
					pt2.x = roadmarkings[i].polyline[j + 1].x;
					pt2.y = roadmarkings[i].polyline[j + 1].y;
					pt2.z = roadmarkings[i].polyline[j + 1].z;
				}
				sprintf(t, "%d", n);
				s = t;
				viewer->addSphere(pt2, 0.1, 1.0, 0.0, 0.0, s);
				n++;

				sprintf(t, "%d", n);
				s = t;
				viewer->addLine(pt1, pt2, 1.0, 0.0, 0.0, s);
				viewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_LINE_WIDTH, 1, s);
				n++;
			}

			break;

		case 3:
			for (int j = 0; j < 7; j++) //ֱ��ͷ
			{
				pcl::PointXYZ pt1;
				pt1.x = roadmarkings[i].polyline[j].x;
				pt1.y = roadmarkings[i].polyline[j].y;
				pt1.z = roadmarkings[i].polyline[j].z;
				sprintf(t, "%d", n);
				s = t;
				viewer->addSphere(pt1, 0.1, 0.0, 0.0, 1.0, s);
				n++;


				pcl::PointXYZ pt2;
				if (j == 6){
					pt2.x = roadmarkings[i].polyline[0].x;
					pt2.y = roadmarkings[i].polyline[0].y;
					pt2.z = roadmarkings[i].polyline[0].z;
				}
				else{
					pt2.x = roadmarkings[i].polyline[j + 1].x;
					pt2.y = roadmarkings[i].polyline[j + 1].y;
					pt2.z = roadmarkings[i].polyline[j + 1].z;
				}
				sprintf(t, "%d", n);
				s = t;
				viewer->addSphere(pt2, 0.1, 0.0, 0.0, 1.0, s);
				n++;

				sprintf(t, "%d", n);
				s = t;
				viewer->addLine(pt1, pt2, 0.0, 0.0, 1.0, s);
				viewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_LINE_WIDTH, 1, s);
				n++;
			}

			break;
		case 4:
			for (int j = 0; j < 14; j++) //ֱ�Ҽ�ͷ
			{
				pcl::PointXYZ pt1;
				pt1.x = roadmarkings[i].polyline[j].x;
				pt1.y = roadmarkings[i].polyline[j].y;
				pt1.z = roadmarkings[i].polyline[j].z;
				sprintf(t, "%d", n);
				s = t;
				viewer->addSphere(pt1, 0.1, 0.0, 1.0, 1.0, s);
				n++;


				pcl::PointXYZ pt2;
				if (j == 13){
					pt2.x = roadmarkings[i].polyline[0].x;
					pt2.y = roadmarkings[i].polyline[0].y;
					pt2.z = roadmarkings[i].polyline[0].z;
				}
				else{
					pt2.x = roadmarkings[i].polyline[j + 1].x;
					pt2.y = roadmarkings[i].polyline[j + 1].y;
					pt2.z = roadmarkings[i].polyline[j + 1].z;
				}
				sprintf(t, "%d", n);
				s = t;
				viewer->addSphere(pt2, 0.1, 0.0, 1.0, 1.0, s);
				n++;

				sprintf(t, "%d", n);
				s = t;
				viewer->addLine(pt1, pt2, 0.0, 1.0, 1.0, s);
				viewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_LINE_WIDTH, 1, s);
				n++;
			}

			break;
		case 5:
			for (int j = 0; j < 9; j++) //�Ҽ�ͷ
			{
				pcl::PointXYZ pt1;
				pt1.x = roadmarkings[i].polyline[j].x;
				pt1.y = roadmarkings[i].polyline[j].y;
				pt1.z = roadmarkings[i].polyline[j].z;
				sprintf(t, "%d", n);
				s = t;
				viewer->addSphere(pt1, 0.1, 1.0, 0.0, 1.0, s);
				n++;


				pcl::PointXYZ pt2;
				if (j == 8){
					pt2.x = roadmarkings[i].polyline[0].x;
					pt2.y = roadmarkings[i].polyline[0].y;
					pt2.z = roadmarkings[i].polyline[0].z;
				}
				else{
					pt2.x = roadmarkings[i].polyline[j + 1].x;
					pt2.y = roadmarkings[i].polyline[j + 1].y;
					pt2.z = roadmarkings[i].polyline[j + 1].z;
				}
				sprintf(t, "%d", n);
				s = t;
				viewer->addSphere(pt2, 0.1, 1.0, 0.0, 1.0, s);
				n++;

				sprintf(t, "%d", n);
				s = t;
				viewer->addLine(pt1, pt2, 1.0, 0.0, 1.0, s);
				viewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_LINE_WIDTH, 1, s);
				n++;
			}

			break;
		
		default:
			break;

		}
	}
	cout << "Click X(close) to continue..." << endl;
	while (!viewer->wasStopped())
	{
		viewer->spinOnce(100);
		boost::this_thread::sleep(boost::posix_time::microseconds(100000));
	}

}

void DataIo::displaymarkVect(const vector<RoadMarking> &roadmarkings, const vector<RoadMarking> &sideline_markings)
{
	boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer(new pcl::visualization::PCLVisualizer("Vectorization Viewer"));
	viewer->setBackgroundColor(0, 0, 0);
	char t[256];
	string s;
	int n = 0;

	for (int i = 0; i < sideline_markings.size(); i++)//������
	{
		for (int j = 0; j <sideline_markings[i].polyline.size() - 1; j++)
		{
			pcl::PointXYZ pt1;
			pt1.x = sideline_markings[i].polyline[j].x;
			pt1.y = sideline_markings[i].polyline[j].y;
			pt1.z = sideline_markings[i].polyline[j].z;
			sprintf(t, "%d", n);
			s = t;
			viewer->addSphere(pt1, 0.1, 0.0, 1.0, 0.0, s);
			n++;

			pcl::PointXYZ pt2;
			pt2.x = sideline_markings[i].polyline[j + 1].x;
			pt2.y = sideline_markings[i].polyline[j + 1].y;
			pt2.z = sideline_markings[i].polyline[j + 1].z;

			sprintf(t, "%d", n);
			s = t;
			viewer->addSphere(pt2, 0.1, 0.0, 1.0, 0.0, s);
			n++;

			sprintf(t, "%d", n);
			s = t;
			viewer->addLine(pt1, pt2, 0.0, 1.0, 0.0, s);
			viewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_LINE_WIDTH, 1, s);
			n++;
		}
	}

	for (int i = 0; i < roadmarkings.size(); i++)
	{
		switch (roadmarkings[i].category)
		{
		case 2:
			for (int j = 0; j < 4; j++) //�������
			{
				pcl::PointXYZ pt1;
				pt1.x = roadmarkings[i].polyline[j].x;
				pt1.y = roadmarkings[i].polyline[j].y;
				pt1.z = roadmarkings[i].polyline[j].z;
				sprintf(t, "%d", n);
				s = t;
				viewer->addSphere(pt1, 0.1, 1.0, 0.0, 0.0, s);
				n++;


				pcl::PointXYZ pt2;
				if (j == 3){
					pt2.x = roadmarkings[i].polyline[0].x;
					pt2.y = roadmarkings[i].polyline[0].y;
					pt2.z = roadmarkings[i].polyline[0].z;
				}
				else{
					pt2.x = roadmarkings[i].polyline[j + 1].x;
					pt2.y = roadmarkings[i].polyline[j + 1].y;
					pt2.z = roadmarkings[i].polyline[j + 1].z;
				}
				sprintf(t, "%d", n);
				s = t;
				viewer->addSphere(pt2, 0.1, 1.0, 0.0, 0.0, s);
				n++;

				sprintf(t, "%d", n);
				s = t;
				viewer->addLine(pt1, pt2, 1.0, 0.0, 0.0, s);
				viewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_LINE_WIDTH, 1, s);
				n++;
			}

			break;

		case 3:
			for (int j = 0; j < 7; j++) //ֱ��ͷ
			{
				pcl::PointXYZ pt1;
				pt1.x = roadmarkings[i].polyline[j].x;
				pt1.y = roadmarkings[i].polyline[j].y;
				pt1.z = roadmarkings[i].polyline[j].z;
				sprintf(t, "%d", n);
				s = t;
				viewer->addSphere(pt1, 0.1, 0.0, 0.0, 1.0, s);
				n++;


				pcl::PointXYZ pt2;
				if (j == 6){
					pt2.x = roadmarkings[i].polyline[0].x;
					pt2.y = roadmarkings[i].polyline[0].y;
					pt2.z = roadmarkings[i].polyline[0].z;
				}
				else{
					pt2.x = roadmarkings[i].polyline[j + 1].x;
					pt2.y = roadmarkings[i].polyline[j + 1].y;
					pt2.z = roadmarkings[i].polyline[j + 1].z;
				}
				sprintf(t, "%d", n);
				s = t;
				viewer->addSphere(pt2, 0.1, 0.0, 0.0, 1.0, s);
				n++;

				sprintf(t, "%d", n);
				s = t;
				viewer->addLine(pt1, pt2, 0.0, 0.0, 1.0, s);
				viewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_LINE_WIDTH, 1, s);
				n++;
			}

			break;
		case 4:
			for (int j = 0; j < 14; j++) //ֱ�Ҽ�ͷ
			{
				pcl::PointXYZ pt1;
				pt1.x = roadmarkings[i].polyline[j].x;
				pt1.y = roadmarkings[i].polyline[j].y;
				pt1.z = roadmarkings[i].polyline[j].z;
				sprintf(t, "%d", n);
				s = t;
				viewer->addSphere(pt1, 0.1, 0.0, 1.0, 1.0, s);
				n++;


				pcl::PointXYZ pt2;
				if (j == 13){
					pt2.x = roadmarkings[i].polyline[0].x;
					pt2.y = roadmarkings[i].polyline[0].y;
					pt2.z = roadmarkings[i].polyline[0].z;
				}
				else{
					pt2.x = roadmarkings[i].polyline[j + 1].x;
					pt2.y = roadmarkings[i].polyline[j + 1].y;
					pt2.z = roadmarkings[i].polyline[j + 1].z;
				}
				sprintf(t, "%d", n);
				s = t;
				viewer->addSphere(pt2, 0.1, 0.0, 1.0, 1.0, s);
				n++;

				sprintf(t, "%d", n);
				s = t;
				viewer->addLine(pt1, pt2, 0.0, 1.0, 1.0, s);
				viewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_LINE_WIDTH, 1, s);
				n++;
			}

			break;
		case 5:
			for (int j = 0; j < 9; j++) //�Ҽ�ͷ
			{
				pcl::PointXYZ pt1;
				pt1.x = roadmarkings[i].polyline[j].x;
				pt1.y = roadmarkings[i].polyline[j].y;
				pt1.z = roadmarkings[i].polyline[j].z;
				sprintf(t, "%d", n);
				s = t;
				viewer->addSphere(pt1, 0.1, 1.0, 0.0, 1.0, s);
				n++;


				pcl::PointXYZ pt2;
				if (j == 8){
					pt2.x = roadmarkings[i].polyline[0].x;
					pt2.y = roadmarkings[i].polyline[0].y;
					pt2.z = roadmarkings[i].polyline[0].z;
				}
				else{
					pt2.x = roadmarkings[i].polyline[j + 1].x;
					pt2.y = roadmarkings[i].polyline[j + 1].y;
					pt2.z = roadmarkings[i].polyline[j + 1].z;
				}
				sprintf(t, "%d", n);
				s = t;
				viewer->addSphere(pt2, 0.1, 1.0, 0.0, 1.0, s);
				n++;

				sprintf(t, "%d", n);
				s = t;
				viewer->addLine(pt1, pt2, 1.0, 0.0, 1.0, s);
				viewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_LINE_WIDTH, 1, s);
				n++;
			}

			break;

		default:
			break;


		}
	}
	cout << "Click X(close) to continue..." << endl;
	while (!viewer->wasStopped())
	{
		viewer->spinOnce(100);
		boost::this_thread::sleep(boost::posix_time::microseconds(100000));
	}
}

bool DataIo::readLasFileHeader(const std::string &fileName, liblas::Header& header)
{
	if (fileName.substr(fileName.rfind('.')).compare(".las"))
	{
		return 0;
	}
	else
	{
		std::ifstream ifs;
		ifs.open(fileName, std::ios::in | std::ios::binary);
		if (ifs.bad())
		{
			return 0;
		}

		liblas::ReaderFactory f;
		liblas::Reader reader = f.CreateWithStream(ifs);

		header = reader.GetHeader();
	}

	return 1;
}

bool DataIo::writemarkVectDXF(const vector<RoadMarking> &roadmarkings)
{
	
	DL_Dxf dxf;
	DL_WriterA* dw = dxf.out("DXFTest.dxf", DL_Codes::AC1015);

	// section header:
	dxf.writeHeader(*dw);
	dw->sectionEnd();

	// section tables:
	dw->sectionTables();

	// VPORT:
	dxf.writeVPort(*dw);

	// LTYPE:
	dw->tableLinetypes(3);
	dxf.writeLinetype(*dw, DL_LinetypeData("CONTINUOUS", "Continuous", 0, 0, 0.0));
	dxf.writeLinetype(*dw, DL_LinetypeData("BYLAYER", "", 0, 0, 0.0));
	dxf.writeLinetype(*dw, DL_LinetypeData("BYBLOCK", "", 0, 0, 0.0));
	dw->tableEnd();

	// LAYER:
	dw->tableLayers(4);
	
	dxf.writeLayer(
		*dw,
		DL_LayerData("0", 0),
		DL_Attributes("", 1, 0x00ff0000, 15, "CONTINUOUS")//15 �߿�  ��ɫ  ��׼ͼ��
		);
	

	dxf.writeLayer(
		*dw,
		DL_LayerData("Side Lines", 0),
		DL_Attributes("", 1, 0x0000ff00, 15, "CONTINUOUS") //15 �߿�  0x00RRGGBB  ��ɫ  ��16���ƣ� 0x0000ff00Ϊ��ɫ
		);

	dxf.writeLayer(
		*dw,
		DL_LayerData("Rectangle Road Markings", 0),
		DL_Attributes("", 1, 0x00ff0000, 15, "CONTINUOUS") // ��ɫ
		);

	dxf.writeLayer(
		*dw,
		DL_LayerData("Arrows", 0),
		DL_Attributes("", 1, 0x000000ff, 15, "CONTINUOUS") // ��ɫ
		);

	dw->tableEnd();

	// STYLE:
	dw->tableStyle(1);
	DL_StyleData style("Standard", 0, 0.0, 1.0, 0.0, 0, 2.5, "txt", "");
	style.bold = false;
	style.italic = false;
	dxf.writeStyle(*dw, style);
	dw->tableEnd();

	// VIEW:
	dxf.writeView(*dw);

	// UCS:
	dxf.writeUcs(*dw);

	// APPID:
	dw->tableAppid(1);
	dxf.writeAppid(*dw, "ACAD");
	dw->tableEnd();

	// DIMSTYLE:
	dxf.writeDimStyle(*dw, 2.5, 0.625, 0.625, 0.625, 2.5);

	// BLOCK_RECORD:
	dxf.writeBlockRecord(*dw);
	dw->tableEnd();

	dw->sectionEnd();

	// BLOCK:
	dw->sectionBlocks();
	dxf.writeBlock(*dw, DL_BlockData("*Model_Space", 0, 0.0, 0.0, 0.0));
	dxf.writeEndBlock(*dw, "*Model_Space");
	dxf.writeBlock(*dw, DL_BlockData("*Paper_Space", 0, 0.0, 0.0, 0.0));
	dxf.writeEndBlock(*dw, "*Paper_Space");
	dxf.writeBlock(*dw, DL_BlockData("*Paper_Space0", 0, 0.0, 0.0, 0.0));
	dxf.writeEndBlock(*dw, "*Paper_Space0");
	dw->sectionEnd();

	// ENTITIES:
	dw->sectionEntities();

	DL_Attributes attributes("0", 256, 0x00000000, 5, "BYLAYER");
	DL_Attributes attributes_side_line("Side Lines", 256, 0x0000ff00, 5, "BYLAYER"); //��
	DL_Attributes attributes_rectangle("Rectangle Road Markings", 256, 0x00ff0000, 5, "BYLAYER");  //��
	DL_Attributes attributes_arrow("Arrows", 256, 0x000000ff, 5, "BYLAYER");  //��
	/*
	// LINE:
	DL_LineData lineData(10, 5, 0, 30, 5, 0);
	dxf.writeLine(*dw, lineData, attributes);

	// CIRCLE
	DL_CircleData circleData(10, 10, 0, 4);
	dxf.writeCircle(*dw, circleData, attributes);
	*/
	bool plineGen = true;
	//bool Isclosed;
	for (int i = 0; i < roadmarkings.size(); i++)
	{
		if (roadmarkings[i].category >= 1){

			int count = roadmarkings[i].polyline.size();

			//�Ƿ�պϣ�1�೤���߲��ñպ�
			switch (roadmarkings[i].category)
			{
			case 1:
				dxf.writePolyline(
					*dw,
					DL_PolylineData(count,
					0, 0,
					false * 0x1 + plineGen * 0x80),
					attributes_side_line);
				break;
			case 2:
				dxf.writePolyline(
					*dw,
					DL_PolylineData(count,
					0, 0,
					true * 0x1 + plineGen * 0x80),
					attributes_rectangle);
				break;
			case 3:
				dxf.writePolyline(
					*dw,
					DL_PolylineData(count,
					0, 0,
					true * 0x1 + plineGen * 0x80),
					attributes_arrow);
				break;
			case 4:
				dxf.writePolyline(
					*dw,
					DL_PolylineData(count,
					0, 0,
					true * 0x1 + plineGen * 0x80),
					attributes_arrow);
				break;
			default:
				break;
			}
			
			
			//���긳ֵ
			for (int j = 0; j < count; j++) {
			
				double bulge = 0;
				dxf.writeVertex(*dw, DL_VertexData(roadmarkings[i].polyline[j].x, roadmarkings[i].polyline[j].y, roadmarkings[i].polyline[j].z, bulge));
			}

			dxf.writePolylineEnd(*dw);
		}
	}
	
	// end section ENTITIES:
	dw->sectionEnd();
	dxf.writeObjects(*dw, "MY_OBJECTS");
	dxf.writeObjectsEnd(*dw);

	dw->dxfEOF();
	dw->close();
	delete dw;

	//dxflib
    //���dxf�ֿ���Ϣ
	/*bool CBlockSegment::OutDxfSegInfo()
	{
		string OutDxfPath = srcPrjRootPath + "\\segInfo.dxf";
		dw = dxf.out(OutDxfPath.c_str(), DL_Codes::AC1015);
		if (dw == NULL)
		{
			printf("�޷��򿪴����ֿ���Ϣdxf�ļ�");
			return false;
		}
		// section header:
		dxf.writeHeader(*dw);
		dw->sectionEnd();

		// section tables:
		dw->sectionTables();

		// VPORT:
		dxf.writeVPort(*dw);

		// LTYPE:
		dw->tableLinetypes(1);
		dxf.writeLinetype(*dw, DL_LinetypeData("CONTINUOUS", "Continuous", 0, 0, 0.0));
		dxf.writeLinetype(*dw, DL_LinetypeData("BYLAYER", "", 0, 0, 0.0));
		dxf.writeLinetype(*dw, DL_LinetypeData("BYBLOCK", "", 0, 0, 0.0));
		dw->tableEnd();

		// LAYER:
		dw->tableLayers(1);
		dxf.writeLayer(
			*dw,
			DL_LayerData("0", 0),
			DL_Attributes("", 1, 0x00ff0000, 15, "CONTINUOUS")
			);
		dw->tableEnd();

		// STYLE:
		dw->tableStyle(1);
		DL_StyleData style("Standard", 0, 0.0, 1.0, 0.0, 0, 2.5, "txt", "");
		style.bold = false;
		style.italic = false;
		dxf.writeStyle(*dw, style);
		dw->tableEnd();

		// VIEW:
		dxf.writeView(*dw);

		// UCS:
		dxf.writeUcs(*dw);

		// APPID:
		dw->tableAppid(1);
		dxf.writeAppid(*dw, "ACAD");
		dw->tableEnd();

		// DIMSTYLE:
		dxf.writeDimStyle(*dw, 2.5, 0.625, 0.625, 0.625, 2.5);

		// BLOCK_RECORD:
		dxf.writeBlockRecord(*dw);
		dw->tableEnd();

		dw->sectionEnd();

		// BLOCK:
		dw->sectionBlocks();
		dxf.writeBlock(*dw, DL_BlockData("*Model_Space", 0, 0.0, 0.0, 0.0));
		dxf.writeEndBlock(*dw, "*Model_Space");
		dxf.writeBlock(*dw, DL_BlockData("*Paper_Space", 0, 0.0, 0.0, 0.0));
		dxf.writeEndBlock(*dw, "*Paper_Space");
		dxf.writeBlock(*dw, DL_BlockData("*Paper_Space0", 0, 0.0, 0.0, 0.0));
		dxf.writeEndBlock(*dw, "*Paper_Space0");
		dw->sectionEnd();

		// ENTITIES:
		dw->sectionEntities();

		DL_Attributes attributes("0", 256, 0x00ffff00, 5, "BYLAYER");
		string BlockName = "";
		Vector3d TextPos;
		vector<Vector3d> LineVertexs;

		for (int i = 1; i < gridRowNum + 1; i++)
		{
			for (int j = 1; j < gridColNum + 1; j++)
			{
				//�ֶ�����
				std::stringstream ss;
				ss << inttoFormatStr(i, 3) << inttoFormatStr(j, 3);
				ss >> BlockName;
				//�ֶ�λ��
				double nTextX = gridLowCorner[0] + j*m_gridSize - 0.5*m_gridSize;
				double nTextY = gridLowCorner[1] + i*m_gridSize - 0.5*m_gridSize;
				TextPos[0] = nTextX; TextPos[1] = nTextY; TextPos[2] = 0;
				WriteDxfText(BlockName.c_str(), TextPos, attributes);
				LineVertexs.clear();
				//�߿򶥵�
				Vector3d v1(gridLowCorner[0] + j*m_gridSize, gridLowCorner[1] + i*m_gridSize, 0);
				Vector3d v2(gridLowCorner[0] + (j - 1)*m_gridSize, gridLowCorner[1] + i*m_gridSize, 0);
				Vector3d v3(gridLowCorner[0] + (j - 1)*m_gridSize, gridLowCorner[1] + (i - 1)*m_gridSize, 0);
				Vector3d v4(gridLowCorner[0] + j*m_gridSize, gridLowCorner[1] + (i - 1)*m_gridSize, 0);
				LineVertexs.push_back(v1);
				LineVertexs.push_back(v2);
				LineVertexs.push_back(v3);
				LineVertexs.push_back(v4);
				WriteDxfPolyline(LineVertexs, attributes, true);
			}
		}
		//�켣
		DL_Attributes attributes1("0", 128, 0x00ff0000, 5, "BYLAYER");
		WriteDxfPos(posData, attributes1, false);
		
		// end section ENTITIES:
		dw->sectionEnd();
		dxf.writeObjects(*dw, "MY_OBJECTS");
		dxf.writeObjectsEnd(*dw);

		dw->dxfEOF();
		dw->close();
		delete dw;
		return true;
	}

	//д�ַ�����dxf
	void CBlockSegment::WriteDxfText(const char* TextName, Vector3d& textPos, DL_Attributes& attributes)
	{
		int TextSize = m_gridSize*0.1;
		const char* m_stylename = "textstyle0";
		dxf.writeMText(
			*dw,
			DL_MTextData(
			textPos[0],
			textPos[1],
			0,
			0.0,
			0.0,
			0.0,
			TextSize,
			TextSize,
			1,
			1,
			2,
			1,
			TextName,
			m_stylename,
			0),
			attributes);

	}
	//д����ߵ�dxf
	void CBlockSegment::WriteDxfPolyline(vector<Vector3d>& lineVecs, DL_Attributes& attributes, bool bIsClosed)
	{
		int count = lineVecs.size();
		bool plineGen = true;

		dxf.writePolyline(
			*dw,
			DL_PolylineData(count,
			0, 0,
			bIsClosed * 0x1 + plineGen * 0x80),
			attributes
			);

		for (int i = 0; i < count; i++) {
			Vector3d v = lineVecs[i];
			double bulge = 0;
			dxf.writeVertex(*dw, DL_VertexData(v[0], v[1], 0.0, bulge));
		}

		dxf.writePolylineEnd(*dw);
	}

	//д�켣��Ϣ��dxf
	void CBlockSegment::WriteDxfPos(vector<vector<posXYZ>>& pxyz, DL_Attributes& attributes, bool bIsClosed)
	{
		for (int m = 0; m < pxyz.size(); m++)
		{
			if (pxyz[m].empty())
			{
				continue;
			}
			int count = pxyz[m].size();
			bool plineGen = true;

			dxf.writePolyline(
				*dw,
				DL_PolylineData(count,
				0, 0,
				bIsClosed * 0x1 + plineGen * 0x80),
				attributes
				);

			for (int i = 0; i < count; i++) {
				posXYZ v = pxyz[m][i];
				double bulge = 0;
				dxf.writeVertex(*dw, DL_VertexData(v.x, v.y, 0.0, bulge));
			}
			dxf.writePolylineEnd(*dw);
		}

	}*/
cout << "Output DXF done" << endl;
return true;

}

bool DataIo::writemarkVectDXF(const vector<RoadMarking> &roadmarkings, double minX, double minY)
{
	cout << "Draft Origin is: ( " << minX << " , " << minY << " )" << endl;

	DL_Dxf dxf;
	DL_WriterA* dw = dxf.out("Output_DXF.dxf", DL_Codes::AC1015);

	// section header:
	dxf.writeHeader(*dw);
	dw->sectionEnd();

	// section tables:
	dw->sectionTables();

	// VPORT:
	dxf.writeVPort(*dw);

	// LTYPE:
	dw->tableLinetypes(3);
	dxf.writeLinetype(*dw, DL_LinetypeData("CONTINUOUS", "Continuous", 0, 0, 0.0));
	dxf.writeLinetype(*dw, DL_LinetypeData("BYLAYER", "", 0, 0, 0.0));
	dxf.writeLinetype(*dw, DL_LinetypeData("BYBLOCK", "", 0, 0, 0.0));
	dw->tableEnd();

	// LAYER:
	dw->tableLayers(4);

	dxf.writeLayer(
		*dw,
		DL_LayerData("0", 0),
		DL_Attributes("", 1, 0x00ff0000, 15, "CONTINUOUS")//15 �߿�  ��ɫ  ��׼ͼ��
		);


	dxf.writeLayer(
		*dw,
		DL_LayerData("Side Lines", 0),
		DL_Attributes("", 1, 0x0000ff00, 15, "CONTINUOUS") //15 �߿�  0x00RRGGBB  ��ɫ  ��16���ƣ� 0x0000ff00Ϊ��ɫ
		);

	dxf.writeLayer(
		*dw,
		DL_LayerData("Rectangle Road Markings", 0),
		DL_Attributes("", 1, 0x00ff0000, 15, "CONTINUOUS") // ��ɫ
		);

	dxf.writeLayer(
		*dw,
		DL_LayerData("Arrows", 0),
		DL_Attributes("", 1, 0x000000ff, 15, "CONTINUOUS") // ��ɫ
		);

	dw->tableEnd();

	// STYLE:
	dw->tableStyle(1);
	DL_StyleData style("Standard", 0, 0.0, 1.0, 0.0, 0, 2.5, "txt", "");
	style.bold = false;
	style.italic = false;
	dxf.writeStyle(*dw, style);
	dw->tableEnd();

	// VIEW:
	dxf.writeView(*dw);

	// UCS:
	dxf.writeUcs(*dw);

	// APPID:
	dw->tableAppid(1);
	dxf.writeAppid(*dw, "ACAD");
	dw->tableEnd();

	// DIMSTYLE:
	dxf.writeDimStyle(*dw, 2.5, 0.625, 0.625, 0.625, 2.5);

	// BLOCK_RECORD:
	dxf.writeBlockRecord(*dw);
	dw->tableEnd();

	dw->sectionEnd();

	// BLOCK:
	dw->sectionBlocks();
	dxf.writeBlock(*dw, DL_BlockData("*Model_Space", 0, 0.0, 0.0, 0.0));
	dxf.writeEndBlock(*dw, "*Model_Space");
	dxf.writeBlock(*dw, DL_BlockData("*Paper_Space", 0, 0.0, 0.0, 0.0));
	dxf.writeEndBlock(*dw, "*Paper_Space");
	dxf.writeBlock(*dw, DL_BlockData("*Paper_Space0", 0, 0.0, 0.0, 0.0));
	dxf.writeEndBlock(*dw, "*Paper_Space0");
	dw->sectionEnd();

	// ENTITIES:
	dw->sectionEntities();

	DL_Attributes attributes("0", 256, 0x00000000, 5, "BYLAYER");
	DL_Attributes attributes_side_line("Side Lines", 256, 0x0000ff00, 5, "BYLAYER"); //��
	DL_Attributes attributes_rectangle("Rectangle Road Markings", 256, 0x00ff0000, 5, "BYLAYER");  //��
	DL_Attributes attributes_arrow("Arrows", 256, 0x000000ff, 5, "BYLAYER");  //��
	/*
	// LINE:
	DL_LineData lineData(10, 5, 0, 30, 5, 0);
	dxf.writeLine(*dw, lineData, attributes);

	// CIRCLE
	DL_CircleData circleData(10, 10, 0, 4);
	dxf.writeCircle(*dw, circleData, attributes);
	*/
	bool plineGen = true;
	//bool Isclosed;
	for (int i = 0; i < roadmarkings.size(); i++)
	{
		if (roadmarkings[i].category >= 1){

			int count = roadmarkings[i].polyline.size();

			//�Ƿ�պϣ�1�೤���߲��ñպ�
			switch (roadmarkings[i].category)
			{
			case 1:
				dxf.writePolyline(
					*dw,
					DL_PolylineData(count,
					0, 0,
					false * 0x1 + plineGen * 0x80),
					attributes_side_line);
				break;
			case 2:
				dxf.writePolyline(
					*dw,
					DL_PolylineData(count,
					0, 0,
					true * 0x1 + plineGen * 0x80),
					attributes_rectangle);
				break;
			case 3:
				dxf.writePolyline(
					*dw,
					DL_PolylineData(count,
					0, 0,
					true * 0x1 + plineGen * 0x80),
					attributes_arrow);
				break;
			case 4:
				dxf.writePolyline(
					*dw,
					DL_PolylineData(count,
					0, 0,
					true * 0x1 + plineGen * 0x80),
					attributes_arrow);
				break;
			default:
				break;
			}

			
			//���긳ֵ
			for (int j = 0; j < count; j++) {

				double bulge = 0;
				dxf.writeVertex(*dw, DL_VertexData(roadmarkings[i].polyline[j].x+minX, roadmarkings[i].polyline[j].y+minY, roadmarkings[i].polyline[j].z, bulge));
			}

			dxf.writePolylineEnd(*dw);
		}
	}

	// end section ENTITIES:
	dw->sectionEnd();
	dxf.writeObjects(*dw, "MY_OBJECTS");
	dxf.writeObjectsEnd(*dw);

	dw->dxfEOF();
	dw->close();
	delete dw;
	cout << "Output DXF done" << endl;
	return true;

}

bool DataIo::writemarkVectDXF(const vector<RoadMarking> &roadmarkings, const vector<RoadMarking> &sideline_markings, double minX, double minY)
{
	//cout << "Draft Origin is: ( " << minX << " , " << minY << " )" << endl;

	DL_Dxf dxf;
	DL_WriterA* dw = dxf.out("Output_DXF.dxf", DL_Codes::AC1015);

	// section header:
	dxf.writeHeader(*dw);
	dw->sectionEnd();

	// section tables:
	dw->sectionTables();

	// VPORT:
	dxf.writeVPort(*dw);

	// LTYPE:
	dw->tableLinetypes(3);
	dxf.writeLinetype(*dw, DL_LinetypeData("CONTINUOUS", "Continuous", 0, 0, 0.0));
	dxf.writeLinetype(*dw, DL_LinetypeData("BYLAYER", "", 0, 0, 0.0));
	dxf.writeLinetype(*dw, DL_LinetypeData("BYBLOCK", "", 0, 0, 0.0));
	dw->tableEnd();

	// LAYER:
	dw->tableLayers(4);

	dxf.writeLayer(
		*dw,
		DL_LayerData("0", 0),
		DL_Attributes("", 1, 0x00ff0000, 15, "CONTINUOUS")//15 �߿�  ��ɫ  ��׼ͼ��
		);


	dxf.writeLayer(
		*dw,
		DL_LayerData("Side Lines", 0),
		DL_Attributes("", 1, 0x0000ff00, 15, "CONTINUOUS") //15 �߿�  0x00RRGGBB  ��ɫ  ��16���ƣ� 0x0000ff00Ϊ��ɫ
		);

	dxf.writeLayer(
		*dw,
		DL_LayerData("Rectangle Road Markings", 0),
		DL_Attributes("", 1, 0x00ff0000, 15, "CONTINUOUS") // ��ɫ
		);

	dxf.writeLayer(
		*dw,
		DL_LayerData("Arrows", 0),
		DL_Attributes("", 1, 0x000000ff, 15, "CONTINUOUS") // ��ɫ
		);

	dw->tableEnd();

	// STYLE:
	dw->tableStyle(1);
	DL_StyleData style("Standard", 0, 0.0, 1.0, 0.0, 0, 2.5, "txt", "");
	style.bold = false;
	style.italic = false;
	dxf.writeStyle(*dw, style);
	dw->tableEnd();

	// VIEW:
	dxf.writeView(*dw);

	// UCS:
	dxf.writeUcs(*dw);

	// APPID:
	dw->tableAppid(1);
	dxf.writeAppid(*dw, "ACAD");
	dw->tableEnd();

	// DIMSTYLE:
	dxf.writeDimStyle(*dw, 2.5, 0.625, 0.625, 0.625, 2.5);

	// BLOCK_RECORD:
	dxf.writeBlockRecord(*dw);
	dw->tableEnd();

	dw->sectionEnd();

	// BLOCK:
	dw->sectionBlocks();
	dxf.writeBlock(*dw, DL_BlockData("*Model_Space", 0, 0.0, 0.0, 0.0));
	dxf.writeEndBlock(*dw, "*Model_Space");
	dxf.writeBlock(*dw, DL_BlockData("*Paper_Space", 0, 0.0, 0.0, 0.0));
	dxf.writeEndBlock(*dw, "*Paper_Space");
	dxf.writeBlock(*dw, DL_BlockData("*Paper_Space0", 0, 0.0, 0.0, 0.0));
	dxf.writeEndBlock(*dw, "*Paper_Space0");
	dw->sectionEnd();

	// ENTITIES:
	dw->sectionEntities();

	DL_Attributes attributes("0", 256, 0x00000000, 5, "BYLAYER");
	DL_Attributes attributes_side_line("Side Lines", 256, 0x0000ff00, 5, "BYLAYER"); //��
	DL_Attributes attributes_rectangle("Rectangle Road Markings", 256, 0x00ff0000, 5, "BYLAYER");  //��
	DL_Attributes attributes_arrow("Arrows", 256, 0x000000ff, 5, "BYLAYER");  //��
	/*
	// LINE:
	DL_LineData lineData(10, 5, 0, 30, 5, 0);
	dxf.writeLine(*dw, lineData, attributes);

	// CIRCLE
	DL_CircleData circleData(10, 10, 0, 4);
	dxf.writeCircle(*dw, circleData, attributes);
	*/
	bool plineGen = true;
	//bool Isclosed;
	
	for (int i = 0; i < sideline_markings.size(); i++)
	{
		
		dxf.writePolyline(
			*dw,
			DL_PolylineData(sideline_markings[i].polyline.size(), 0, 0, false * 0x1 + plineGen * 0x80),
			attributes_side_line);


		for (int j = 0; j < sideline_markings[i].polyline.size(); j++) {

			double bulge = 0;
			dxf.writeVertex(*dw, DL_VertexData(sideline_markings[i].polyline[j].x + minX, sideline_markings[i].polyline[j].y + minY, sideline_markings[i].polyline[j].z, bulge));
		}

		dxf.writePolylineEnd(*dw);
	}

	for (int i = 0; i < roadmarkings.size(); i++)
	{
		if (roadmarkings[i].category >= 2){

			int count = roadmarkings[i].polyline.size();

			//�Ƿ�պϣ�1�೤���߲��ñպ�
			switch (roadmarkings[i].category)
			{
			case 2:
				dxf.writePolyline(
					*dw,
					DL_PolylineData(count,
					0, 0,
					true * 0x1 + plineGen * 0x80),
					attributes_rectangle);
				break;
			case 3:
				dxf.writePolyline(
					*dw,
					DL_PolylineData(count,
					0, 0,
					true * 0x1 + plineGen * 0x80),
					attributes_arrow);
				break;
			case 4:
				dxf.writePolyline(
					*dw,
					DL_PolylineData(count,
					0, 0,
					true * 0x1 + plineGen * 0x80),
					attributes_arrow);
				break;
			case 5:
				dxf.writePolyline(
					*dw,
					DL_PolylineData(count,
					0, 0,
					true * 0x1 + plineGen * 0x80),
					attributes_arrow);
				break;
			default:
				break;
			}


			//���긳ֵ
			for (int j = 0; j < count; j++) {

				double bulge = 0;
				dxf.writeVertex(*dw, DL_VertexData(roadmarkings[i].polyline[j].x + minX, roadmarkings[i].polyline[j].y + minY, roadmarkings[i].polyline[j].z, bulge));
			}

			dxf.writePolylineEnd(*dw);
		}
	}

	// end section ENTITIES:
	dw->sectionEnd();
	dxf.writeObjects(*dw, "MY_OBJECTS");
	dxf.writeObjectsEnd(*dw);

	dw->dxfEOF();
	dw->close();
	delete dw;
	cout << "Output DXF done" << endl;
	return true;

}


bool DataIo::readLasFile(const std::string &fileName, pcl::PointCloud<pcl::PointXYZI> &pointCloud)
{
	if (fileName.substr(fileName.rfind('.')).compare(".las"))
	{
		return 0;
	}

	std::ifstream ifs;
	ifs.open(fileName, std::ios::in | std::ios::binary);
	if (ifs.bad())
	{
		cout << "δ����ƥ����" << endl;
	}
	liblas::ReaderFactory f;
	liblas::Reader reader = f.CreateWithStream(ifs);
	liblas::Header const& header = reader.GetHeader();

	//header�����ֱ����bounding box ����
	double Xmin, Ymin, Zmin, Xmax, Ymax, Zmax;
	Xmin = header.GetMinX();
	Ymin = header.GetMinY();
	Zmin = header.GetMinZ();
	Xmax = header.GetMaxX();
	Ymax = header.GetMaxY();
	Zmax = header.GetMaxZ();



	//int i = 0;
	/*whileѭ���б������еĵ�;*/
	while (reader.ReadNextPoint())
	{
		const liblas::Point& p = reader.GetPoint();
		//i++;
		pcl::PointXYZI  pt;
		/*�����Ļ���������ǿ��ֵ��ֵ��PCL�еĵ�;*/
		/*��һ��ƽ�ƣ�������WGS84 �µĵ�����̫���ˣ�����ɾ�����ʧ�� ��Ϊlas�Ķ�ȡ��������double�ģ���pcd��int��*/
		pt.x = p.GetX() - Xmin;
		pt.y = p.GetY() - Ymin;
		pt.z = p.GetZ();
		pt.intensity = p.GetIntensity();
		//if (i % 100 == 0) { cout << p.GetPointSourceID() << "\t" << p.GetTime() << endl; } //��ЩPointSourceID �� Time ��Ϣ������ScanLine Profile ����ɨ���ߣ��������·�棬����Ч���е㿰��
		pointCloud.points.push_back(pt);
	}

	return 1;
}

bool DataIo::readLasFile(const std::string &fileName, pcl::PointCloud<pcl::PointXYZI> &pointCloud, vector<double> & boundingbox,double & X_min,double & Y_min)
{
	if (fileName.substr(fileName.rfind('.')).compare(".las"))
	{
		return 0;
	}

	std::ifstream ifs;
	ifs.open(fileName, std::ios::in | std::ios::binary);
	if (ifs.bad())
	{
		cout << "δ����ƥ����" << endl;
	}
	liblas::ReaderFactory f;
	liblas::Reader reader = f.CreateWithStream(ifs);
	liblas::Header const& header = reader.GetHeader();
	
	//header�����ֱ����bounding box ����
	double Xmin, Ymin, Zmin, Xmax, Ymax,Zmax;
	Xmin = header.GetMinX();
	Ymin = header.GetMinY();
	Zmin = header.GetMinZ();
	Xmax = header.GetMaxX();
	Ymax = header.GetMaxY();
	Zmax = header.GetMaxZ();

	

	//int i = 0;
	/*whileѭ���б������еĵ�;*/
	while (reader.ReadNextPoint())
	{
		const liblas::Point& p = reader.GetPoint();
		//i++;
		pcl::PointXYZI  pt;
		/*�����Ļ���������ǿ��ֵ��ֵ��PCL�еĵ�;*/
		/*��һ��ƽ�ƣ�������WGS84 �µĵ�����̫���ˣ�����ɾ�����ʧ�� ��Ϊlas�Ķ�ȡ��������double�ģ���pcd��int��*/
		pt.x = p.GetX()-Xmin;
		pt.y = p.GetY()-Ymin;  
		pt.z = p.GetZ();
		pt.intensity = p.GetIntensity();
		//if (i % 100 == 0) { cout << p.GetPointSourceID() << "\t" << p.GetTime() << endl; } //��ЩPointSourceID �� Time ��Ϣ������ScanLine Profile ����ɨ���ߣ��������·�棬����Ч���е㿰��
		pointCloud.points.push_back(pt);
	}

	//Bounding Box after translation 
	boundingbox.push_back(0);
	boundingbox.push_back(0);
	boundingbox.push_back(Zmin);
	boundingbox.push_back(Xmax-Xmin);
	boundingbox.push_back(Ymax-Ymin);
	boundingbox.push_back(Zmax);
	
	// ƽ�ƺ�����ԭ��
	X_min = Xmin;
	Y_min = Ymin;

	return 1;
}


bool DataIo::GetBoundaryOfPointCloud(pcl::PointCloud<pcl::PointXYZI> &pointCloud, Bounds &bound)

{
	if (pointCloud.points.empty())
	{
		return 0;
	}
	else
	{
		double min_x = pointCloud.points[0].x;
		double min_y = pointCloud.points[0].y;
		double min_z = pointCloud.points[0].z;
		double max_x = pointCloud.points[0].x;
		double max_y = pointCloud.points[0].y;
		double max_z = pointCloud.points[0].z;

		for (int i = 0; i < pointCloud.points.size(); i++)
		{
			//��ȡ�߽�
			if (min_x > pointCloud.points[i].x)
				min_x = pointCloud.points[i].x;
			if (min_y > pointCloud.points[i].y)
				min_y = pointCloud.points[i].y;
			if (min_z > pointCloud.points[i].z)
				min_z = pointCloud.points[i].z;
			if (max_x < pointCloud.points[i].x)
				max_x = pointCloud.points[i].x;
			if (max_y < pointCloud.points[i].y)
				max_y = pointCloud.points[i].y;
			if (max_z < pointCloud.points[i].z)
				max_z = pointCloud.points[i].z;
		}
		bound.minx = min_x;
		bound.maxx = max_x;
		bound.miny = min_y;
		bound.maxy = max_y;
		bound.minz = min_z;
		bound.maxz = max_z;
	}
	return 1;
}


bool DataIo::GetBoundaryOfPointCloud(pcl::PointCloud<pcl::PointXYZRGB> &pointCloud, Bounds &bound)

{
	if (pointCloud.points.empty())
	{
		return 0;
	}
	else
	{
		double min_x = pointCloud.points[0].x;
		double min_y = pointCloud.points[0].y;
		double min_z = pointCloud.points[0].z;
		double max_x = pointCloud.points[0].x;
		double max_y = pointCloud.points[0].y;
		double max_z = pointCloud.points[0].z;

		for (int i = 0; i < pointCloud.points.size(); i++)
		{
			//��ȡ�߽�
			if (min_x > pointCloud.points[i].x)
				min_x = pointCloud.points[i].x;
			if (min_y > pointCloud.points[i].y)
				min_y = pointCloud.points[i].y;
			if (min_z > pointCloud.points[i].z)
				min_z = pointCloud.points[i].z;
			if (max_x < pointCloud.points[i].x)
				max_x = pointCloud.points[i].x;
			if (max_y < pointCloud.points[i].y)
				max_y = pointCloud.points[i].y;
			if (max_z < pointCloud.points[i].z)
				max_z = pointCloud.points[i].z;
		}
		bound.minx = min_x;
		bound.maxx = max_x;
		bound.miny = min_y;
		bound.maxy = max_y;
		bound.minz = min_z;
		bound.maxz = max_z;
	}
	return 1;
}

bool DataIo::readPlyFile(const string &fileName, const pcXYZIPtr &pointCloud)
{
	if (pcl::io::loadPLYFile<pcl::PointXYZI>(fileName, *pointCloud) == -1) //* load the file 
	{
		PCL_ERROR("Couldn't read file \n");
		return (-1);
	}
}
