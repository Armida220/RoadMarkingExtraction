#ifndef GROUNDEXTRACTION
#define GROUNDEXTRACTION

#include <pcl/point_types.h>
#include <pcl/point_cloud.h>
#include <pcl/features/normal_3d.h>
#include <pcl/ModelCoefficients.h>
#include <pcl/filters/project_inliers.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/visualization/cloud_viewer.h>

#include <vector>

#include <opencv/cv.h>
#include <opencv/highgui.h>

namespace groundExtraction
{   
		struct CenterPoint
		{
			double x;
			double y;
			double z;
			CenterPoint(double x = 0, double y = 0, double z = 0) :
				x(x), y(y), z(z)
			{
				z = 0.0;
				x = y = 0.0;
			}

		};

		struct Bounds
		{
			double min_x;
			double min_y;
			double min_z;
			double max_x;
			double max_y;
			double max_z;
			Bounds()
			{
				min_x = min_y = min_z = max_x = max_y = max_z = 0.0;
			}
		};



		struct Grid
		{
			bool is_empty;
			Grid()
			{
				is_empty = true;
			}
		};

		struct Voxel
		{
			vector<int>point_id;
			float min_z;
			float max_z;
			float dertaz;
			float min_z_x;//������͵��X����;
			float min_z_y;//������͵��y����;
			float NeighborMin_z;
			int PointsNumber;
			float mean_z;

			Voxel()
			{
				min_z = min_z_x = min_z_y = NeighborMin_z = mean_z = 0.f;
				PointsNumber = 1;
				dertaz = 0.0;
			}
		};

		struct SimplifiedVoxel
		{
			vector<int>point_id;
			float max_curvature;
			int max_curvature_point_id;
			bool has_keypoint;
			SimplifiedVoxel()
			{
				has_keypoint = false;
			}
		};


		class StructOperator
		{
		public:
			//��ȡ����
			void getCloudCenterPoint(const pcl::PointCloud<pcl::PointXYZ> & cloud, CenterPoint & centerPoint)
			{
				double cx = 0, cy = 0, cz = 0;

				for (int i = 0; i < cloud.size(); i++)
				{
					cx += cloud.points[i].x / cloud.size();
					cy += cloud.points[i].y / cloud.size();
					cz += cloud.points[i].z / cloud.size();
				}
				centerPoint.x = cx;
				centerPoint.y = cy;
				centerPoint.z = cz;
			}
			//��ȡ�߽�
			void getCloudBound(const pcl::PointCloud<pcl::PointXYZ> & cloud, Bounds & bound)
			{
				double min_x = cloud[0].x;
				double min_y = cloud[0].y;
				double min_z = cloud[0].z;
				double max_x = cloud[0].x;
				double max_y = cloud[0].y;
				double max_z = cloud[0].z;

				for (int i = 0; i<cloud.size(); i++)
				{
					//��ȡ�߽�
					if (min_x>cloud.points[i].x)
						min_x = cloud.points[i].x;
					if (min_y > cloud.points[i].y)
						min_y = cloud.points[i].y;
					if (min_z > cloud.points[i].z)
						min_z = cloud.points[i].z;
					if (max_x < cloud.points[i].x)
						max_x = cloud.points[i].x;
					if (max_y < cloud.points[i].y)
						max_y = cloud.points[i].y;
					if (max_z < cloud.points[i].z)
						max_z = cloud.points[i].z;
				}
				bound.min_x = min_x;
				bound.max_x = max_x;
				bound.min_y = min_y;
				bound.max_y = max_y;
				bound.min_z = min_z;
				bound.max_z = max_z;
			}

			void getCloudBound(const pcl::PointCloud<pcl::PointXYZI> & cloud, Bounds & bound)
			{
				double min_x = cloud[0].x;
				double min_y = cloud[0].y;
				double min_z = cloud[0].z;
				double max_x = cloud[0].x;
				double max_y = cloud[0].y;
				double max_z = cloud[0].z;

				for (int i = 0; i<cloud.size(); i++)
				{
					//��ȡ�߽�
					if (min_x>cloud.points[i].x)
						min_x = cloud.points[i].x;
					if (min_y > cloud.points[i].y)
						min_y = cloud.points[i].y;
					if (min_z > cloud.points[i].z)
						min_z = cloud.points[i].z;
					if (max_x < cloud.points[i].x)
						max_x = cloud.points[i].x;
					if (max_y < cloud.points[i].y)
						max_y = cloud.points[i].y;
					if (max_z < cloud.points[i].z)
						max_z = cloud.points[i].z;
				}
				bound.min_x = min_x;
				bound.max_x = max_x;
				bound.min_y = min_y;
				bound.max_y = max_y;
				bound.min_z = min_z;
				bound.max_z = max_z;
			}

			//��ȡ���ĺͱ߽�
			void getBoundAndCenter(const pcl::PointCloud<pcl::PointXYZI> & cloud, Bounds & bound, CenterPoint& centerPoint)
			{
				double min_x = cloud[0].x;
				double min_y = cloud[0].y;
				double min_z = cloud[0].z;
				double max_x = cloud[0].x;
				double max_y = cloud[0].y;
				double max_z = cloud[0].z;

				double cx = 0, cy = 0, cz = 0;

				for (int i = 0; i<cloud.size(); i++)
				{
					//��ȡ�߽�
					if (min_x>cloud.points[i].x)
						min_x = cloud.points[i].x;
					if (min_y > cloud.points[i].y)
						min_y = cloud.points[i].y;
					if (min_z > cloud.points[i].z)
						min_z = cloud.points[i].z;
					if (max_x < cloud.points[i].x)
						max_x = cloud.points[i].x;
					if (max_y < cloud.points[i].y)
						max_y = cloud.points[i].y;
					if (max_z < cloud.points[i].z)
						max_z = cloud.points[i].z;


					cx += cloud.points[i].x / cloud.size();
					cy += cloud.points[i].y / cloud.size();
					cz += cloud.points[i].z / cloud.size();
				}
				bound.min_x = min_x;
				bound.max_x = max_x;
				bound.min_y = min_y;
				bound.max_y = max_y;
				bound.min_z = min_z;
				bound.max_z = max_z;


				centerPoint.x = cx;
				centerPoint.y = cy;
				centerPoint.z = cz;
			}

			void GetSubsetBoundary(pcl::PointCloud<pcl::PointXYZ>::Ptr & plane_wall_cloud,
				vector<int> & index, Bounds & bound)
			{
				//��������
				pcl::PointCloud<pcl::PointXYZ>::Ptr temp_cloud(new pcl::PointCloud<pcl::PointXYZ>);
				for (int i = 0; i < index.size(); i++)
				{
					temp_cloud->push_back(plane_wall_cloud->points[index[i]]);
				}
				getCloudBound(*temp_cloud, bound);
			}

	protected:

	private:
	};

	class CGroundExtraction
	{
	public:

		CGroundExtraction();
		~CGroundExtraction();

		/*�������ȡ,�ú�������ʶ������ͷǵ����;*/
		void ExtractGroundPoint(pcl::PointCloud<pcl::PointXYZI>::Ptr cloud,
			pcl::PointCloud<pcl::PointXYZI>::Ptr ground_cloud,
			pcl::PointCloud<pcl::PointXYZI>::Ptr no_ground_cloud,
			Bounds bounds, CenterPoint center_pt);

		void SetMaxHeightDifference(float max_height_difference){ max_height_difference_ = max_height_difference; }
		void SetMinPointNumInGrid(int min_pt_num_in_grid){ min_pt_num_in_grid_ = min_pt_num_in_grid; }
		void SetGridResolution(float grid_resolution){ grid_resolution_ = grid_resolution; }

		

	protected:

	private:

	

		/*��������е���͵�߳�;*/
		void CalculateGridAndMinZ(pcl::PointCloud<pcl::PointXYZI>::Ptr cloud,
			double max_x, double max_y, double min_x, double min_y,
			int row, int list, int num_voxel, Voxel* grid);

		/*���ݵ��Grid����͵�ĸ̲߳�жϵ����ͷǵ����;*/
		void JudgeGroundAndNonGroundPoints(pcl::PointCloud<pcl::PointXYZI>::Ptr cloud,
			pcl::PointCloud<pcl::PointXYZI>::Ptr ground_cloud,
			pcl::PointCloud<pcl::PointXYZI>::Ptr no_ground_cloud,
			Voxel* grid, int num_voxel);


		/*�Ե�����һ���жϣ�����õ���������Χ�����ܶ࣬���ж�Ϊ�ǵ����;*/
		void FilterNotGroundPoint(pcl::PointCloud<pcl::PointXYZI>::Ptr cloud,
			pcl::PointCloud<pcl::PointXYZI>::Ptr ground_cloud,
			pcl::PointCloud<pcl::PointXYZI>::Ptr no_ground_cloud);

		float grid_resolution_;//�����ķֱ���;
		int   min_pt_num_in_grid_;//��������С�ĵ������С�ڸõ�������Ϊ�ø���Ϊ������;
		float max_height_difference_;//�����ڵĵ����������͵�ĸ̲߳�,���ڸ�ֵ����Ϊ�Ƿǵ����;


	};
}
#endif