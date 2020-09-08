#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

void drawPoint(Mat img, Point2f pt, Scalar color)
{
	Point2f outPt;
	outPt.x = img.rows / 2 - pt.y;
	outPt.y = img.cols - pt.x;
	circle(img, Point2f(outPt.x, outPt.y), 1, color);
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

	circle(image, Point2f(image.rows / 2, image.cols), 2, Scalar(255, 255, 255));

	Point2f leftPt[50];
	Point2f rightPt[50];
	for (int i = 0; i < 50; i++)
	{
		leftPt[i].y = i;
		leftPt[i].x = -10;

		rightPt[i].y = i;
		rightPt[i].x = 10;
	}

	//memcpy(dstLeftList, lidarPt, sizeof(Point2f)*50);
	
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