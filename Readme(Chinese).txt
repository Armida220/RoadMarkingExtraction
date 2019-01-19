帮助文档（中文版）
程序名：道路标线提取程序
功能：实现多平台道路点云标线自动提取，分类，矢量化
应用场景：高速公路，城市道路
适用点云采集方式：MLS（推荐），TLS，ALS

1.开发环境
在VS2013下使用C++开发

2.模块介绍
本项目主要包括6个模块：Utility、DataIO、GroundExtraction、ImageProcession、PointCloudProcession、ModelMatching
1）Utility：提供点云数据结构，包围盒计算等功能 （utility.h)
2）DataIO：提供点云读写，显示，矢量化输出以及调参接口 （dataIo.h/cpp, [dxflib] dl_dxf.cpp, dl_writer_ascii.cpp)
3）GroundExtraction：提供网格高程差阈值法与渐进形态学滤波法两种地面分割算法的实现 (ground_extraction.h/cpp)
4）ImageProcession：提供点云投影为图像，图像回溯点云，及各类图像处理算法的实现 (imageprocess.h/cpp)
5）PointCloudProcession：提供各类点云滤波，标线点云边缘提取，分类，序列化，矢量化等算法的实现 (pointcloudprocess.h/cpp)
6）ModelMatching：提供道路标线点云的模板匹配算法的实现，包含点云关键点提取，特征提取，特征匹配，几何一致性匹配对筛选，配准等多个子模块 (modelmatch.h/cpp, keypointDetection.h/cpp, KMeans.h/cpp, principleComponentAnalysis.h/cpp, binaryFeatureExtraction.h/cpp, stereoBinaryFeature.h/cpp, objectRecognition.h/cpp) 
[模型库文件夹：高速公路标线模板：Models_Highway、 城市道路表现模板：Models_Cityroad]

3.编译指南
1）必选第三方库
Boost、dxflib、eigen3、flann、liblas、OpenCV (>2.4)、pcl (>1.8)
2）可选第三方库
CGAL、Qhull
3）编译器：VS 12 2013 x64

4.使用说明
输入： I.  点云文件（las）
       II. 参数文件（txt），或键入任意字符，使用默认参数
       III.数据类型选择 : 1. MLS/TLS   2.ALS 
       IV. 道路类型选择 : 1. Highway   2.City Road
       V.  道路路面是否已经被提取: 1.No  2.Yes 
       VI. 输入道路点云大致点密度（每平方米内的点数）

输出:  I.  投影图片 (jpg) [包括反射强度投影，反射强度梯度投影，反射强度梯度二值化投影，连通域标注等图像]
       II. 所有的分割后标线点云，按类别赋色[非绝对坐标](pcd) [或绝对坐标点云(las)]: Classified Road Markings 文件夹
       III.标线矢量化结果[绝对坐标](dxf):Output_DXF.dxf

显示:  I.  地面，非地面点分割结果
       II. 分割后标线点云（随机赋色）
       III.分类后标线点云（按类别赋色）[长边线：绿；长方形标线：红；向前箭头：蓝；向前向右（左）箭头：青；向右（左）箭头：紫；其他：白]
       IV. 矢量化结果（按类别赋色）[长边线：绿；长方形标线：红；各类箭头：蓝]


5.主要参数介绍：
intensity_scale 该参数控制点反射强度的取值范围，一般该值越小，所找到的标线越多，相应的找到的噪点也越多 （推荐值 6-12）
keypoint_nonmax_radius  该参数控制模板匹配时的关键点稀疏程度 (单位 m)，该值越大，关键点越少，匹配耗时越短，也越不精确  （推荐值 0.1-0.25）
geometric_consistency_tolerant 该参数控制模板匹配时的几何一致性对应点对筛选的几何一致性允许距离 （单位 m )，该值越大，几何一致性筛选越松，越容易找到匹配结果，也更可能出现误匹配 (推荐值0.2-0.5)
search_radius 该参数控制模板匹配重叠度计算时的近邻点搜索半径 (单位 m)，该值越大，重叠度一般越高，一般与tolerantMinOverlap_ratio联合调参 （推荐值0.2-0.5）
tolerantMinOverlap_ratio 该参数控制认为模板匹配成功的判断标准，即模板与场景点的重叠度，该值越大，越不容易匹配成功，相应的匹配正确率越高 （推荐值0.65-0.75）
sideline_vector_distance 该参数控制长边线矢量化时的距离阈值 (单位 m)，端点距离小于该阈值的相邻边线会被连成一段；此外，矢量化的采样点也以该值作为采样间隔 （推荐值2-5） 

6.算法主要逻辑流程：
1)数据输入
2)基于地面分割与点云坡度、密度投影的道路面提取
3)点云反射强度投影，图像处理，提取分割后道路标线像素
4)分割后道路标线像素回溯分割后点云，点云滤波
5)基于几何限制条件的道路标线点云粗分类
6)基于模板匹配的道路箭头型标线点云分类
7)道路标线点云序列化，矢量化
8)结果输出

7.项目成员
潘越

8.版本介绍
V1.0.0：初始版本，功能：车载激光点云高速公路或城市道路点云标线自动提取，分类与矢量化


引用及参考
1.Zhen D , Bisheng Y , Fuxun L , et al. Hierarchical registration of unordered TLS point clouds based on binary shape context descriptor[J]. ISPRS Journal of Photogrammetry and Remote Sensing, 2018, 144:61-79.
2.Dong Z , Yang B , Liu Y , et al. A novel binary shape context for 3D local surface description[J]. ISPRS Journal of Photogrammetry and Remote Sensing, 2017, 130:431-452.
3.Li Y , Hua L , Junxiang T , et al. Scan Line Based Road Marking Extraction from Mobile LiDAR Point Clouds[J]. Sensors, 2016, 16(6):903.
4.Guan H , LI, Jonathan, et al. Using mobile laser scanning data for automated extraction of road markings[J]. Isprs Journal of Photogrammetry & Remote Sensing, 2014, 87(1):93-107.
5.Yang B , Huang R , Dong Z , et al. Two-step adaptive extraction method for ground points and breaklines from lidar point clouds[J]. ISPRS Journal of Photogrammetry and Remote Sensing, 2016, 119:373-389.
