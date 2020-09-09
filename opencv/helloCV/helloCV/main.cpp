//#include <stdio.h>
//#include <opencv2/opencv.hpp>
//#include <iostream>
//#include <string>
//#include <time.h>
//#include <math.h>
//
//using namespace std;
//
//int main(void)
//{
//	srand(time(NULL));
//
//	//-------------------------------------------------------------- make sample data  
//	/* random number in range 0 - 1 not including 1 */
//	float random = 0.f;
//	/* the white noise */
//	float noise = 0.f;
//	/* Setup constants */
//	const static int q = 15;
//	const static float c1 = (1 << q) - 1;
//	const static float c2 = ((int)(c1 / 3)) + 1;
//	const static float c3 = 1.f / c1;
//
//	double noise_sigma = 100;
//	double x[100];
//	double y[100];
//
//	double iA = 0.005;
//	double iB = 0.5;
//	double iC = 0;
//	for (int i = 0; i < 100; i++)
//	{
//		x[i] = i;
//		y[i] = iA * (x[i] * x[i]) + iB * x[i] + iC;
//
//#if 0
//		if (i > 50 && i < 70)
//		{
//			y[i] += 0.1*abs(x[i]);
//		}
//#endif
//		//y[i] = y[i] + noise_sigma*((rand()%100)+1) ;
//
//		random = ((float)rand() / (float)(RAND_MAX + 1));
//		noise = (2.f * ((random * c2) + (random * c2) + (random * c2)) - 3.f * (c2 - 1.f)) * c3;
//
//		int noise_scale = 2.0;
//		if (i > 50 && i < 70) noise_scale = 5.0;
//		y[i] = y[i] + noise * noise_scale;
//	}
//
//
//	//-------------------------------------------------------------- build matrix
//	cv::Mat A(100, 3, CV_64FC1);
//	cv::Mat B(100, 1, CV_64FC1);
//
//	for (int i = 0; i < 100; i++)
//	{
//		A.at<double>(i, 0) = x[i] * x[i];
//	}
//	for (int i = 0; i < 100; i++)
//	{
//		A.at<double>(i, 1) = x[i];
//	}
//	for (int i = 0; i < 100; i++)
//	{
//		A.at<double>(i, 2) = 1.0;
//	}
//
//	for (int i = 0; i < 100; i++)
//	{
//		B.at<double>(i, 0) = y[i];
//	}
//
//	//-------------------------------------------------------------- RANSAC fitting 
//	int n_data = 100;
//	int N = 100;	//iterations 
//	double T = 3 * noise_sigma;   // residual threshold
//
//	int n_sample = 3;
//	int max_cnt = 0;
//	cv::Mat best_model(3, 1, CV_64FC1);
//
//	for (int i = 0; i < N; i++)
//	{
//		//random sampling - 3 point  
//		int k[3] = { -1, };
//		k[0] = floor((rand() % 100 + 1)) + 1;
//
//		do
//		{
//			k[1] = floor((rand() % 100 + 1)) + 1;
//		} while (k[1] == k[0] || k[1] < 0);
//
//		do
//		{
//			k[2] = floor((rand() % 100 + 1)) + 1;
//		} while (k[2] == k[0] || k[2] == k[1] || k[2] < 0);
//
//		printf("random sample : %d %d %d\n", k[0], k[1], k[2]);
//
//		//model estimation
//		cv::Mat AA(3, 3, CV_64FC1);
//		cv::Mat BB(3, 1, CV_64FC1);
//		for (int j = 0; j < 3; j++)
//		{
//			AA.at<double>(j, 0) = x[k[j]] * x[k[j]];
//			AA.at<double>(j, 1) = x[k[j]];
//			AA.at<double>(j, 2) = 1.0;
//
//			BB.at<double>(j, 0) = y[k[j]];
//		}
//
//		cv::Mat AA_pinv(3, 3, CV_64FC1);
//		invert(AA, AA_pinv, cv::DECOMP_SVD);
//
//		cv::Mat X = AA_pinv * BB;
//
//		//evaluation 
//		cv::Mat residual(100, 1, CV_64FC1);
//		residual = cv::abs(B - A * X);
//		int cnt = 0;
//		for (int j = 0; j < 100; j++)
//		{
//			double data = residual.at<double>(j, 0);
//
//			if (data < T)
//			{
//				cnt++;
//			}
//		}
//
//		if (cnt > max_cnt)
//		{
//			best_model = X;
//			max_cnt = cnt;
//		}
//	}
//
//	//------------------------------------------------------------------- optional LS fitting 
//	cv::Mat residual = cv::abs(A*best_model - B);
//	std::vector<int> vec_index;
//	for (int i = 0; i < 100; i++)
//	{
//		double data = residual.at<double>(i, 0);
//		if (data < T)
//		{
//			vec_index.push_back(i);
//		}
//	}
//
//	cv::Mat A2(vec_index.size(), 3, CV_64FC1);
//	cv::Mat B2(vec_index.size(), 1, CV_64FC1);
//
//	for (int i = 0; i < vec_index.size(); i++)
//	{
//		A2.at<double>(i, 0) = x[vec_index[i]] * x[vec_index[i]];
//		A2.at<double>(i, 1) = x[vec_index[i]];
//		A2.at<double>(i, 2) = 1.0;
//
//		B2.at<double>(i, 0) = y[vec_index[i]];
//	}
//
//	cv::Mat A2_pinv(3, vec_index.size(), CV_64FC1);
//	invert(A2, A2_pinv, cv::DECOMP_SVD);
//
//	cv::Mat X = A2_pinv * B2;
//
//	//Drawing
//	cv::Mat F = A * X;
//	printf("matrix F : cols =%d, rows=%d\n", F.cols, F.rows);
//
//	int interval = 5;
//	cv::Mat imgResult(100 * interval, 100 * interval, CV_8UC3);
//	imgResult = cv::Scalar(0);
//	for (int iy = 0; iy < 100; iy++)
//	{
//		cv::circle(imgResult, cv::Point(x[iy] * interval, y[iy] * interval), 3, cv::Scalar(0, 0, 255));
//
//		double data = F.at<double>(iy, 0);
//
//		cv::circle(imgResult, cv::Point(x[iy] * interval, data*interval), 1, cv::Scalar(0, 255, 0));
//	}
//	cv::imshow("result", imgResult);
//	cv::waitKey(0);
//
//	return 0;
//}


#include "opencv2/opencv.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <math.h>

using namespace std;
using namespace cv;

// init origin
Scalar white(255, 255, 255);
Scalar red(0, 0, 255);
Scalar blue(255, 0, 0);
Scalar green(0, 255, 0);


#if 1

void drawPoint(Mat img, Point2f pt, Scalar color)
{
	Point2f outPt;
	outPt.x = pt.x*10;
	outPt.y = pt.y*10;
	circle(img, Point2f(outPt.x, outPt.y), 1, color, 2);
}
int main(void)
{
	Mat image(1000, 1000, CV_8UC3, Scalar(0, 0, 0));
	if (image.empty())
		return -1;

	Point2f point[50];
	
	for(int i=0; i < 50; i++)
	{
		point[i].x = i+25;
		point[i].y = (point[i].x-50)*(point[i].x-50)*(0.01) + (point[i].x-50) * 0.02 + 20;// point[i].x*point[i].x + 2 * point[i].x ; // +rand() % 5;
		drawPoint(image, point[i], white);
	}

	// develop algorithm
	// RANSAC fitting
	int n_data = 50;
	int N = 100;
	int noise_sigma = 100;
	int T = 3 * noise_sigma;
	int n_sample = 3;
	int max_cnt = 0;
	float best_model[3] = { 0, 0, 0 };

	// build matrix
	float A[50][3], B[50];
	for (int i = 0; i < 50; i++)
	{
		A[i][0] = point[i].x * point[i].x;
		A[i][1] = point[i].x;
		A[i][2] = 1.0f;
		
		B[i] = point[i].y;
	}


	srand((unsigned int)time(NULL));
	for (int i = 0; i < N; i++)
	{
		// random sampling

		//extract 3 points
		int k[3] = { -1, };
		k[0] = floor(rand() % 50 + 1) + 1;
		do
		{
			k[1] = floor((rand() % 50 + 1)) + 1;
		} while (k[1] == k[0] || k[1] < 0);
		do 
		{
			k[2] = floor((rand() % 50 + 1)) + 1;
		} while (k[2] == k[0] || k[2] == k[1] || k[2] < 0);
	
		printf("random samples : %d %d %d\n", k[0], k[1], k[2]);

		float aa[3][3];
		float bb[3];

		for (int j = 0; j < 3; j++)
		{
			aa[j][0] = point[k[j]].x * point[k[j]].x;
			aa[j][1] = point[k[j]].x;
			aa[j][2] = 1;

			bb[j] = point[k[j]].y;
		}

		// inverse
		float det = aa[0][0] * aa[1][1] * aa[2][2] 
			+ aa[0][1] * aa[1][2] * aa[2][0] 
			+ aa[0][2] * aa[1][0] * aa[2][1]
			- (aa[0][0] * aa[1][2] * aa[2][1] 
				+ aa[0][1] * aa[1][0] * aa[2][2] 
				+ aa[0][2] * aa[1][1] * aa[2][1]);
		float t_aa[3][3];
		for (int j = 0; j < 3; j++)
		{
			for (int jj = 0; jj < 3; jj++)
			{
				t_aa[j][jj] = aa[jj][j];
			}
		}

		float inv_aa[3][3];
		inv_aa[0][0] = (1 / det)*(t_aa[1][1] * t_aa[2][2] - t_aa[1][2] * t_aa[2][1]);
		inv_aa[0][1] = (1 / det)*(t_aa[1][0] * t_aa[2][2] - t_aa[1][2] * t_aa[2][0]) * (-1);
		inv_aa[0][2] = (1 / det)*(t_aa[1][0] * t_aa[2][1] - t_aa[1][1] * t_aa[2][0]);
		
		inv_aa[1][0] = (1 / det)*(t_aa[0][1] * t_aa[2][2] - t_aa[0][2] * t_aa[2][1]) * (-1);
		inv_aa[1][1] = (1 / det)*(t_aa[0][0] * t_aa[2][2] - t_aa[0][2] * t_aa[2][0]);
		inv_aa[1][2] = (1 / det)*(t_aa[0][0] * t_aa[2][1] - t_aa[0][1] * t_aa[2][0]) * (-1);
		
		inv_aa[2][0] = (1 / det)*(t_aa[0][1] * t_aa[1][2] - t_aa[0][2] * t_aa[1][1]);
		inv_aa[2][1] = (1 / det)*(t_aa[0][0] * t_aa[1][2] - t_aa[0][2] * t_aa[1][0]) * (-1);
		inv_aa[2][2] = (1 / det)*(t_aa[0][0] * t_aa[1][1] - t_aa[0][1] * t_aa[1][0]);
		
		// x = AA_pinv * BB;
		float xx[3];
		xx[0] = inv_aa[0][0] * bb[0] + inv_aa[0][1] * bb[1] + inv_aa[0][2] * bb[2];
		xx[1] = inv_aa[1][0] * bb[0] + inv_aa[1][1] * bb[1] + inv_aa[1][2] * bb[2];
		xx[2] = inv_aa[2][0] * bb[0] + inv_aa[2][1] * bb[1] + inv_aa[2][2] * bb[2];

		// evaluation
		float residual[50]

	}



	//

	imshow("TEST", image);

	waitKey();

	return 0;
}

#else
void drawPoint(Mat img, Point2f pt, Scalar color)
{
	Point2f outPt;
	outPt.x = img.rows / 2 - pt.y;
	outPt.y = img.cols - pt.x;
	circle(img, Point2f(outPt.x, outPt.y), 1, color, 2);
}
int main(void)
{
	Mat image(100, 100, CV_8UC3, Scalar(0, 0, 0));
	if (image.empty())
		return -1;

	// init origin
	Scalar white(255, 255, 255);
	Scalar red(0, 0, 255);
	Scalar blue(255, 0, 0);
	Scalar green(0, 255, 0);

	srand((unsigned int)time(NULL));

	circle(image, Point2f(image.rows / 2, image.cols), 2, Scalar(255, 255, 255));

	Point2f leftPt[50];
	Point2f rightPt[50];
	for (int i = 0; i < 50; i++)
	{
		leftPt[i].y = i;
		leftPt[i].x = -10 + rand() % 2;

		rightPt[i].y = i;
		rightPt[i].x = 10 + rand() % 2;
	}

	// Init drawing
	Point2f dstLeftList[50];
	Point2f dstRightList[50];
	// Draw left
	for (int i = 0; i < 50; i++)
	{
		dstLeftList[i].x = leftPt[i].y;
		dstLeftList[i].y = -leftPt[i].x;
		drawPoint(image, dstLeftList[i], red);
	}

	// Draw right
	for (int i = 0; i < 50; i++)
	{
		dstRightList[i].x = rightPt[i].y;
		dstRightList[i].y = -rightPt[i].x;
		drawPoint(image, dstRightList[i], green);
	}

	Mat dstimage;
	resize(image, dstimage, Size(500, 500));
	imshow("TEST", dstimage);

	waitKey();

	return 0;
}
#endif

