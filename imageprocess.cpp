#include "imageprocess.h" 
#include <numeric>


using namespace image;
using namespace std;

void Imageprocess::pcgrid(const pcXYZIPtr &cloud, float resolution, int &nx, int &ny){ //nx,ny ������
	float minx, miny, maxx, maxy, lx, ly;
	int ix, iy;
	minx = miny = FLT_MAX;
	maxx = maxy = -FLT_MAX;
	for (size_t i = 0; i < cloud->points.size(); i++){
		if (minx > cloud->points[i].x) minx = cloud->points[i].x;
		if (miny > cloud->points[i].y) miny = cloud->points[i].y;
		if (maxx < cloud->points[i].x) maxx = cloud->points[i].x;
		if (maxy < cloud->points[i].y) maxy = cloud->points[i].y;
	}
	lx = maxx - minx;
	ly = maxy - miny;
	nx = lx / resolution + 1;
	ny = ly / resolution + 1;
	cout << "nx\t" << nx << "\tny\t" << ny << endl;
}

void Imageprocess:: pc2imgI(const pcXYZIPtr &cloud, float resolution, Mat &img){
	
	float minx, miny, maxx, maxy, maxi, mini, lx, ly;
	int ix, iy,nx,ny;
	minx = miny = mini = FLT_MAX;
	maxx = maxy = maxi = -FLT_MAX;

	for (size_t i = 0; i < cloud->points.size(); i++){
		if (minx > cloud->points[i].x) minx = cloud->points[i].x;
		if (miny > cloud->points[i].y) miny = cloud->points[i].y;
		if (maxx < cloud->points[i].x) maxx = cloud->points[i].x;
		if (maxy < cloud->points[i].y) maxy = cloud->points[i].y;
	}

	lx = maxx - minx;
	ly = maxy - miny;
	nx = lx / resolution + 1;
	ny = ly / resolution + 1;
	vector<vector<vector<float>>> matrixi;	
	vector<vector<float>> ave;
	matrixi.resize(nx);
	ave.resize(nx);

	for (size_t i = 0; i < nx; i++){
		matrixi[i].resize(ny);
		ave[i].resize(ny);
	}

	for (size_t i = 0; i < nx; i++){
		for (size_t j = 0; j < ny; j++){
			matrixi[i][j].push_back(0);
		}
	}
	
	for (size_t i = 0; i < cloud->points.size(); i++){
		ix = (cloud->points[i].x - minx) / resolution;
		iy = (cloud->points[i].y - miny) / resolution;
		matrixi[ix][iy].push_back(cloud->points[i].intensity);
	}
	//�Ȱѵ��ӵ�������ȥ
	
	for (int i = 0; i < nx; i++)
	{
		for (int j = 0; j < ny; j++)
		{
			ave[i][j] = (1 + accumulate(begin(matrixi[i][j]), end(matrixi[i][j]), 0.0)) / matrixi[i][j].size();
			//���������ȡƽ��
			
		}
	}
	
	for (int i = 0; i < nx; i++)
	{
		for (int j = 0; j < ny; j++)
		{
			maxi = max(maxi, ave[i][j]);
			mini = min(mini, ave[i][j]);
		}
	}

	for (int i = 0; i < nx; i++)
	{
		uchar *data = img.ptr<uchar>(i);
		for (int j = 0; j < ny; j++)
		{
			int out;
			if (ave[i][j] > mini){
				 out = (int)((ave[i][j] - mini) / (maxi - mini) * 255);
			}
			else{ out = 0; }

	         data[3 * j] = out;
	         data[3 * j + 1] = out;
	         data[3 * j + 2] = out;
	         //pixel ��ֵ
		}
	}

	//imshow("Projection Image", img);
	/*for (size_t i = 0; i < nx; i++){
		for (size_t j = 0; j < ny; j++)
		{

			img.at<uchar>(i, j) = 1;// accumulate(begin(matrixi[i][j]), end(matrixi[i][j]), 0.0) / matrixi[i][j].size();

			//img.at<uchar>(i,j)    �Ҷ�ͼ��
			//img.at<Vec3b>(i,j)    rgb ͼ��

	    }
	}*/

}

void Imageprocess::pc2imgZ(const pcXYZIPtr &cloud, float resolution, Mat &img){

	float minx, miny, maxx, maxy, maxz, minz, lx, ly;
	int ix, iy, nx, ny;
	minx = miny = minz = FLT_MAX;
	maxx = maxy = maxz = -FLT_MAX;

	for (size_t i = 0; i < cloud->points.size(); i++){
		if (minx > cloud->points[i].x) minx = cloud->points[i].x;
		if (miny > cloud->points[i].y) miny = cloud->points[i].y;
		if (maxx < cloud->points[i].x) maxx = cloud->points[i].x;
		if (maxy < cloud->points[i].y) maxy = cloud->points[i].y;
	}

	lx = maxx - minx;
	ly = maxy - miny;
	nx = lx / resolution + 1;
	ny = ly / resolution + 1;
	vector<vector<vector<float>>> matrixz;
	vector<vector<float>> ave;
	matrixz.resize(nx);
	ave.resize(nx);

	for (size_t i = 0; i < nx; i++){
		matrixz[i].resize(ny);
		ave[i].resize(ny);
	}

	for (size_t i = 0; i < nx; i++){
		for (size_t j = 0; j < ny; j++){
			matrixz[i][j].push_back(0);
		}
	}

	for (size_t i = 0; i < cloud->points.size(); i++){
		ix = (cloud->points[i].x - minx) / resolution;
		iy = (cloud->points[i].y - miny) / resolution;
		matrixz[ix][iy].push_back(cloud->points[i].z);
	}
	//�Ȱѵ��ӵ�������ȥ

	for (int i = 0; i < nx; i++)
	{
		for (int j = 0; j < ny; j++)
		{
			ave[i][j] = (1 + accumulate(begin(matrixz[i][j]), end(matrixz[i][j]), 0.0)) / matrixz[i][j].size();
			//���������ȡƽ��

		}
	}

	for (int i = 0; i < nx; i++)
	{
		for (int j = 0; j < ny; j++)
		{
			maxz = max(maxz, ave[i][j]);
			minz = min(minz, ave[i][j]);
		}
	}

	for (int i = 0; i < nx; i++)
	{
		uchar *data = img.ptr<uchar>(i);
		for (int j = 0; j < ny; j++)
		{
			int out;
			if (ave[i][j] > minz){
				out = (int)((ave[i][j] - minz) / (maxz - minz) * 255);
			}
			else{ out = 0; }

			data[3 * j] = out;
			data[3 * j + 1] = out;
			data[3 * j + 2] = out;
			//pixel ��ֵ
		}
	}

	//imshow("Projection Image", img);
	/*for (size_t i = 0; i < nx; i++){
	for (size_t j = 0; j < ny; j++)
	{

	img.at<uchar>(i, j) = 1;// accumulate(begin(matrixi[i][j]), end(matrixi[i][j]), 0.0) / matrixi[i][j].size();

	//img.at<uchar>(i,j)    �Ҷ�ͼ��
	//img.at<Vec3b>(i,j)    rgb ͼ��

	}
	}*/
}

Mat Imageprocess::Sobelboundary(Mat img0)
{
	//Using Sobel Operation
	Mat grayImage, grad_xg, grad_yg, abs_grad_xg, abs_grad_yg, dstg;

	//��ԭʼͼת��Ϊ�Ҷ�ͼ
	cvtColor(img0, grayImage, COLOR_BGR2GRAY);
	//��x�����ݶ�
	Sobel(grayImage, grad_xg, CV_16S, 1, 0, 3, 1, 1, BORDER_DEFAULT);
	convertScaleAbs(grad_xg, abs_grad_xg);
	//��y�����ݶ�
	Sobel(grayImage, grad_yg, CV_16S, 0, 1, 3, 1, 1, BORDER_DEFAULT);
	convertScaleAbs(grad_yg, abs_grad_yg);
	//�ϲ��ݶ�
	addWeighted(abs_grad_xg, 0.5, abs_grad_yg, 0.5, 0, dstg);
	return dstg;
}

float Imageprocess::caculateCurrentEntropy(Mat hist, int threshold)
{
	float BackgroundSum = 0, targetSum = 0;
	const float* pDataHist = (float*)hist.ptr<float>(0);
	for (int i = 0; i < 256; i++)
	{
		//�ۼƱ���ֵ
		if (i < threshold)
		{
			BackgroundSum += pDataHist[i];
		}
		//�ۼ�ǰ��ֵ
		else
		{
			targetSum += pDataHist[i];
		}
	}
	float BackgroundEntropy = 0, targetEntropy = 0;
	for (int i = 0; i < 256; i++)
	{
		//���㱳����
		if (i < threshold)
		{
			if (pDataHist[i] == 0)
				continue;
			float ratio1 = pDataHist[i] / BackgroundSum;
			BackgroundEntropy += -ratio1*logf(ratio1);
		}
		else  //����ǰ����
		{
			if (pDataHist[i] == 0)
				continue;
			float ratio2 = pDataHist[i] / targetSum;
			targetEntropy += -ratio2*logf(ratio2);
		}
	}
	return (targetEntropy + BackgroundEntropy);  //�Ӻͣ��õ���ǰ��ֵ����
}


Mat Imageprocess::maxEntropySegMentation(Mat inputImage)
{   
	// Max Entropy Binarization 
	// Using the distribution of histogram to calculate the threshold leading to the max entropy.
	const int channels[1] = { 0 };
	const int histSize[1] = { 256 };
	float pranges[2] = { 0, 256 };
	const float* ranges[1] = { pranges };
	MatND hist;
	calcHist(&inputImage, 1, channels, Mat(), hist, 1, histSize, ranges);
	float maxentropy = 0;
	int max_index = 0;
	Mat result;
	for (int i = 0; i < 256; i++)  //����256��ֵ��Ϊ��ֵ�� ��ȡ�����ֵ
	{
		float cur_entropy = caculateCurrentEntropy(hist, i);
		if (cur_entropy > maxentropy)
		{
			maxentropy = cur_entropy;
			max_index = i;
		}
	}
	threshold(inputImage, result, max_index, 1, CV_THRESH_BINARY);  // > max_index assign as 1   < max_index assign as 0
	return result;
}

Mat Imageprocess::ExtractRoadPixel(const Mat & _binI, const Mat & _binZ)
{
	Mat result;
	_binI.convertTo(result, CV_8UC1);
	int mini, minj, maxi, maxj;
	vector <int> arrayi, arrayj;
	for (int i = 0; i < _binI.rows; i++)
	{
		for (int j = 0; j < _binI.cols; j++)
		{
			if (_binZ.at<uchar>(i, j) == 1)
			{
				result.at<uchar>(i, j) = 0;
			}
			else{ result.at<uchar>(i, j) = _binI.at<uchar>(i, j); }
			
			/*if (result.at<uchar>(i, j) == 1)
			{
				arrayi.push_back(i);
				arrayj.push_back(j);
			}*/
		}
	}
	return result;
	 /*maxi = *(std::max_element(std::begin(arrayi), std::end(arrayi)));
	 mini = *(std::min_element(std::begin(arrayi), std::end(arrayi)));
	 maxj = *(std::max_element(std::begin(arrayj), std::end(arrayj)));
	 minj = *(std::min_element(std::begin(arrayj), std::end(arrayj)));

	 Rect rect(mini, minj, maxi-mini+1, maxj-minj+1);
	 Mat ROI = result(rect);
	 return ROI;*/
}
void Imageprocess::CcaByTwoPass(const Mat & _binImg, Mat & _labelImg)
{
	// connected component analysis (8-component)
	// use two-pass algorithm ����ɨ�跨
	// 1. first pass: label each foreground pixel with a label
	// 2. second pass: visit each labeled pixel and merge neighbor labels
	// 
	// foreground pixel: _binImg(x,y) = 1
	// background pixel: _binImg(x,y) = 0

	//  reference: https://blog.csdn.net/icvpr/article/details/10259577 


	if (_binImg.empty() ||
		_binImg.type() != CV_8UC1)
	{
		return;
	}

	// 1. first pass

	_labelImg.release();
	_binImg.convertTo(_labelImg, CV_32SC1);                             // _labelImg -> _binImg  32 Signed Ϊ���ܷ��㹻������label������������32λ��

	int label = 1;                                                      // start by 2
	vector<int> labelSet;                                               // ������label
	labelSet.push_back(0);                                              // background: 0
	labelSet.push_back(1);                                              // foreground: 1

	int rows = _binImg.rows - 1;
	int cols = _binImg.cols - 1;
	for (int i = 1; i < rows; i++)                                      // �б���
	{
		int* data_preRow = _labelImg.ptr<int>(i - 1);                   // ָ����һ��
		int* data_curRow = _labelImg.ptr<int>(i);                       // ָ����һ��
		for (int j = 1; j < cols; j++)                                  // �б���
		{
			if (data_curRow[j] == 1)                                    // �� ��ǰ�� ������ ��ǰ�� �����ǻ�û��label��
			{
				vector<int> neighborLabels;                             // ��������label����
				neighborLabels.reserve(2);                              // Ԥ���ռ�
				int leftPixel = data_curRow[j - 1];                     // ����������
				int upPixel = data_preRow[j];                           // ����������
				//int leftupPixel = data_preRow[j - 1];                   // ������������

				if (leftPixel > 1)                                      // �������������Լ���label
				{
					neighborLabels.push_back(leftPixel);                // ����label �������label�ӽ�ȥ
				}
				if (upPixel > 1)                                        // �������������Լ���label
				{
					neighborLabels.push_back(upPixel);                  // ����label �������label�ӽ�ȥ
				}
				/*if (leftupPixel > 1)                                    // ���������������Լ���label
				{
					neighborLabels.push_back(leftPixel);                // ����label ���������label�ӽ�ȥ
				}*/

				if (neighborLabels.empty())                             // �����򶼻�ûlabel 
				{
					labelSet.push_back(++label);                        // assign to a new label
					data_curRow[j] = label;                             // ��ǰ���ر�Ϊ��label
					labelSet[label] = label;                            
				}
				else                                                    // ��Ȼ �����������label�ˣ�
				{ 
					sort(neighborLabels.begin(), neighborLabels.end()); // ����label���� ��С������
					int smallestLabel = neighborLabels[0];              
					data_curRow[j] = smallestLabel;                     // ��ǰ���ظ���С��label

					// save equivalence
					for (size_t k = 1; k < neighborLabels.size(); k++)  // ��������label��
					{
						int tempLabel = neighborLabels[k];              // 
						int& oldSmallestLabel = labelSet[tempLabel];
						if (oldSmallestLabel > smallestLabel)
						{
							labelSet[oldSmallestLabel] = smallestLabel;
							oldSmallestLabel = smallestLabel;
						}
						else if (oldSmallestLabel < smallestLabel)
						{
							labelSet[smallestLabel] = oldSmallestLabel;
						}
					}
				}
			}
		}
	}

	// update equivalent labels 
	// assigned with the smallest label in each equivalent label set  
	// ��¼Neighbors�и���ֵ��label��֮�����ȹ�ϵ������Щֵ��label��ͬ��ͬһ����ͨ����  
	for (size_t i = 2; i < labelSet.size(); i++)  // 0,1 ����
	{
		int curLabel = labelSet[i];
		int preLabel = labelSet[curLabel];
		while (preLabel != curLabel)
		{
			curLabel = preLabel;
			preLabel = labelSet[preLabel];
		}
		labelSet[i] = curLabel;
	}


	// 2. second pass
	for (int i = 0; i < rows; i++)
	{
		int* data = _labelImg.ptr<int>(i);
		for (int j = 0; j < cols; j++)
		{
			int& pixelLabel = data[j];                                    
			pixelLabel = labelSet[pixelLabel];
		}
	}
}


void Imageprocess:: CcaBySeedFill(const Mat& _binImg, Mat& _lableImg)
{
	// connected component analysis (8-component)
	// use seed filling algorithm
	// 1. begin with a foreground pixel and push its foreground neighbors into a stack;
	// 2. pop the top pixel on the stack and label it with the same label until the stack is empty
	// 
	// foreground pixel: _binImg(x,y) = 1
	// background pixel: _binImg(x,y) = 0


	if (_binImg.empty() ||
		_binImg.type() != CV_8UC1)
	{
		return;
	}

	_lableImg.release();
	_binImg.convertTo(_lableImg, CV_32SC1);

	int label = 1;  // start by 2

	int rows = _binImg.rows - 1;
	int cols = _binImg.cols - 1;
	for (int i = 1; i < rows - 1; i++)
	{
		int* data = _lableImg.ptr<int>(i);
		for (int j = 1; j < cols - 1; j++)
		{
			if (data[j] == 1)
			{
				std::stack<std::pair<int, int>> neighborPixels;
				neighborPixels.push(std::pair<int, int>(i, j));     // pixel position: <i,j>
				++label;  // begin with a new label
				while (!neighborPixels.empty())
				{
					// get the top pixel on the stack and label it with the same label
					std::pair<int, int> curPixel = neighborPixels.top();
					int curX = curPixel.first;
					int curY = curPixel.second;
					_lableImg.at<int>(curX, curY) = label;

					// pop the top pixel
					neighborPixels.pop();

					// push the 8-neighbors (foreground pixels)
					if (_lableImg.at<int>(curX, curY - 1) == 1)
					{// left pixel
						neighborPixels.push(std::pair<int, int>(curX, curY - 1));
					}
					if (_lableImg.at<int>(curX, curY + 1) == 1)
					{// right pixel
						neighborPixels.push(std::pair<int, int>(curX, curY + 1));
					}
					if (_lableImg.at<int>(curX - 1, curY) == 1)
					{// up pixel
						neighborPixels.push(std::pair<int, int>(curX - 1, curY));
					}
					if (_lableImg.at<int>(curX + 1, curY) == 1)
					{// down pixel
						neighborPixels.push(std::pair<int, int>(curX + 1, curY));
					}
					if (_lableImg.at<int>(curX - 1, curY - 1) == 1)
					{// left up pixel
						neighborPixels.push(std::pair<int, int>(curX -1, curY - 1));
					}
					if (_lableImg.at<int>(curX - 1, curY + 1) == 1)
					{// left down pixel
						neighborPixels.push(std::pair<int, int>(curX - 1, curY + 1));
					}
					if (_lableImg.at<int>(curX + 1, curY - 1) == 1)
					{// right up pixel
						neighborPixels.push(std::pair<int, int>(curX + 1, curY - 1));
					}
					if (_lableImg.at<int>(curX + 1, curY + 1) == 1)
					{// right down pixel
						neighborPixels.push(std::pair<int, int>(curX + 1, curY + 1));
					}
				}
			}
		}
	}
}


void Imageprocess::RemoveSmallRegion(const Mat &Src, Mat &Dst, int AreaLimit)
	{
		int RemoveCount = 0;
		//�½�һ����ǩͼ���ʼ��Ϊ0���ص㣬Ϊ�˼�¼ÿ�����ص����״̬�ı�ǩ��0����δ��飬1�������ڼ��,2�����鲻�ϸ���Ҫ��ת��ɫ����3������ϸ������   
		//��ʼ����ͼ��ȫ��Ϊ0��δ���  
		Src.convertTo(Dst, CV_8UC1);
		int CheckMode = 1; //��ɫ��Ϊ��ͨ��
		int NeihborMode = 1; //8����
		Mat PointLabel = Mat::zeros(Src.size(), CV_8UC1);
		if (CheckMode == 1)//ȥ��С��ͨ����İ�ɫ��  
		{
			//cout << "ȥ��С��ͨ��.";
			for (int i = 0; i < Src.rows; i++)
			{
				for (int j = 0; j < Src.cols; j++)
				{
					if (Src.at<uchar>(i, j) < 1)
					{
						PointLabel.at<uchar>(i, j) = 3;//��������ɫ����Ϊ�ϸ�����Ϊ3     
					}
				}
			}
		}
		else//ȥ���׶�����ɫ������  
		{
			//cout << "ȥ���׶�";
			for (int i = 0; i < Src.rows; i++)
			{
				for (int j = 0; j < Src.cols; j++)
				{
					if (Src.at<uchar>(i, j) > 10)
					{
						PointLabel.at<uchar>(i, j) = 3;//���ԭͼ�ǰ�ɫ���򣬱��Ϊ�ϸ�����Ϊ3  
					}
				}
			}
		}


		vector<Point2i>NeihborPos;//������ѹ������  
		NeihborPos.push_back(Point2i(-1, 0));
		NeihborPos.push_back(Point2i(1, 0));
		NeihborPos.push_back(Point2i(0, -1));
		NeihborPos.push_back(Point2i(0, 1));
		if (NeihborMode == 1)
		{
			//cout << "Neighbor mode: 8����." << endl;
			NeihborPos.push_back(Point2i(-1, -1));
			NeihborPos.push_back(Point2i(-1, 1));
			NeihborPos.push_back(Point2i(1, -1));
			NeihborPos.push_back(Point2i(1, 1));
		}
		else int a = 0;//cout << "Neighbor mode: 4����." << endl;
		int NeihborCount = 4 + 4 * NeihborMode;
		int CurrX = 0, CurrY = 0;
		//��ʼ���  
		for (int i = 0; i < Src.rows; i++)
		{
			for (int j = 0; j < Src.cols; j++)
			{
				if (PointLabel.at<uchar>(i, j) == 0)//��ǩͼ�����ص�Ϊ0����ʾ��δ���Ĳ��ϸ��  
				{   //��ʼ���  
					vector<Point2i>GrowBuffer;//��¼������ص�ĸ���  
					GrowBuffer.push_back(Point2i(j, i));
					PointLabel.at<uchar>(i, j) = 1;//���Ϊ���ڼ��  
					int CheckResult = 0;

					for (int z = 0; z < GrowBuffer.size(); z++)
					{
						for (int q = 0; q < NeihborCount; q++)
						{
							CurrX = GrowBuffer.at(z).x + NeihborPos.at(q).x;
							CurrY = GrowBuffer.at(z).y + NeihborPos.at(q).y;
							if (CurrX >= 0 && CurrX < Src.cols&&CurrY >= 0 && CurrY < Src.rows)  //��ֹԽ��    
							{
								if (PointLabel.at<uchar>(CurrY, CurrX) == 0)
								{
									GrowBuffer.push_back(Point2i(CurrX, CurrY));      //��������buffer    
									PointLabel.at<uchar>(CurrY, CurrX) = 1;           //���������ļ���ǩ�������ظ����    
								}
							}
						}
					}
					if (GrowBuffer.size() > AreaLimit) //�жϽ�����Ƿ񳬳��޶��Ĵ�С����1Ϊδ������2Ϊ����    
						CheckResult = 2;
					else
					{
						CheckResult = 1;
						RemoveCount++;//��¼�ж�������ȥ��  
					}

					for (int z = 0; z < GrowBuffer.size(); z++)
					{
						CurrX = GrowBuffer.at(z).x;
						CurrY = GrowBuffer.at(z).y;
						PointLabel.at<uchar>(CurrY, CurrX) += CheckResult;//��ǲ��ϸ�����ص㣬����ֵΪ2  
					}
					//********�����õ㴦�ļ��**********    
				}
			}
		}
		CheckMode = 255 * (1 - CheckMode);
		//��ʼ��ת�����С������    
		for (int i = 0; i < Src.rows; ++i)
		{
			for (int j = 0; j < Src.cols; ++j)
			{
				if (PointLabel.at<uchar>(i, j) == 2)
				{
					Dst.at<uchar>(i, j) = 0;
				}
				//else if (PointLabel.at<uchar>(i, j) == 3)
				else
                {
					Dst.at<uchar>(i, j) = Src.at<uchar>(i, j);

				}
			}
		}
		//cout << RemoveCount << " objects removed." << endl;
}



Scalar Imageprocess::GetRandomColor()
{
	uchar r = 255 * (rand() / (1.0 + RAND_MAX));   // rand() / (1.0+ RAND_MAX) : a random float number between 0 and 1 (can't be equal to 1)
	uchar g = 255 * (rand() / (1.0 + RAND_MAX));   // rand() 0 ~ 0x7fff ��32767��  RAND_MAX = 32767
	uchar b = 255 * (rand() / (1.0 + RAND_MAX));
	return Scalar(b, g, r);
}


void Imageprocess::LabelColor(const Mat & _labelImg, Mat & _colorLabelImg)
{
	if (_labelImg.empty() ||
		_labelImg.type() != CV_32SC1)
	{
		return;
	}

	std::map<int, Scalar> colors;

	int rows = _labelImg.rows;
	int cols = _labelImg.cols;

	_colorLabelImg.release();
	_colorLabelImg.create(rows, cols, CV_8UC3);
	_colorLabelImg = Scalar::all(0);

	for (int i = 0; i < rows; i++)
	{
		const int* data_src = (int*)_labelImg.ptr<int>(i);
		uchar* data_dst = _colorLabelImg.ptr<uchar>(i);
		for (int j = 0; j < cols; j++)
		{
			int pixelValue = data_src[j];
			if (pixelValue > 1)
			{
				if (colors.count(pixelValue) <= 0)
				{
					colors[pixelValue] = GetRandomColor();
				}
				Scalar color = colors[pixelValue];
				*data_dst++ = color[0];
				*data_dst++ = color[1];
				*data_dst++ = color[2];
			}
			else
			{
				data_dst++;
				data_dst++;
				data_dst++;
			}

		}
	}

}

Mat Imageprocess::Truncate(const Mat & Img)
{
	Mat result0;
	Mat result;
	int mini, minj, maxi, maxj;
	mini = INT_MAX; minj = INT_MAX;
	maxi = 0; maxj = 0;
	for (int i = 0; i < Img.rows; i++)
	{
		for (int j = 0; j < Img.cols; j++)
		{
			if (Img.at<uchar>(i, j) != 0)
			{
				mini = min(i, mini);
				minj = min(j, minj);
				maxi = max(i, maxi);
				maxj = max(i, maxj);
			}
		}
	}
	//cout << *maxj - *minj << endl << *maxi - *mini << endl;
	Rect rect(minj, mini, maxj - minj, maxi - mini);
	result = Img(rect);
	return result;
}

void Imageprocess::saveimg(const Mat &ProjI, const Mat &ProjZ, const Mat &GI, const Mat &GZ, const Mat &BI, const Mat &BZ, const Mat &BI2, const Mat &Label)
{
imwrite("1_Intensity Projection Image.jpg", ProjI);
imwrite("2_Elevation Projection Image.jpg", ProjZ);
imwrite("3_Intensity Gradient Image.jpg", GI);
imwrite("4_Slope Image.jpg", GZ);
imwrite("5_Intensity Binary Image.jpg", 255*BI);
imwrite("6_Slope Binary Image.jpg", 255*BZ);
imwrite("7_Road Intensity Binary Image.jpg", 255*BI2);
imwrite("8_RoadMarkings.jpg", Label);

cout << "Image Output Done." << endl;
}


