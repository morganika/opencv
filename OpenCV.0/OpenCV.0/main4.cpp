//
//  main4.cpp
//  OpenCV.0
//
//  Created by Юлька on 18.12.2021.
//  Copyright © 2021 Юлька. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include <stdio.h>
#include <cmath>

using namespace cv;
using namespace std;

void DrawPoint(Mat* image, int xcoord, int ycoord, Vec3i bgr) {

    for (int i = xcoord-2; i < xcoord+2; i++) {
        for (int j = ycoord-2; j < ycoord+2; j++) {
            (*image).at<Vec3b>(j, i)[0] = bgr[0];
            (*image).at<Vec3b>(j, i)[1] = bgr[1];
            (*image).at<Vec3b>(j, i)[2] = bgr[2];
        }
    }
}

Mat GluePictures(Mat first, Mat second, int borderPosition)
{
    Mat result(second.rows, second.cols + borderPosition, CV_8UC3, Scalar(255, 255, 0));
    
    first(Rect(0,0,borderPosition, first.rows)).copyTo(result(Rect(0,0,borderPosition, first.rows)));
    second.copyTo(result(Rect(borderPosition, 0, second.cols, second.rows)));
    return result;
}

vector<Point2i> FindMainPoints(Mat cannyImage){
    std::vector<std::vector<cv::Point2i>> contours;
    cv::findContours(cannyImage, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    int biggestContourIdx = -1;
    double biggestContourArea = 0;
    for (int i = 0; i < contours.size(); ++i)
    {
        auto area = cv::contourArea(contours[i]);
        if (area > biggestContourArea)
        {
            biggestContourArea = area;
            biggestContourIdx = i;
        }
    }
    
    //first solution:
    std::vector<cv::Point2i> approx;
    cv::approxPolyDP(contours[biggestContourIdx], approx, 30, true);

    return approx;
}

Mat MainPointConcatenation(Mat first, Mat second)
{
    Mat cannyFirst, cannySecond;
    Canny(first, cannyFirst, 60, 300, 3 );
    Canny(second, cannySecond, 60, 300, 3  );
    
    imshow("First canny", cannyFirst);
    imshow("Second canny", cannySecond);
    
    vector<Point2i> firstPoints = FindMainPoints(cannyFirst);
    vector<Point2i> secondPoints = FindMainPoints(cannySecond);
    
    
    for (int i = 0 ; i < firstPoints.size(); i++){
        DrawPoint(&first, firstPoints[i].x, firstPoints[i].y, Vec3i(0,0,255));
    }
    
    for (int i = 0 ; i < secondPoints.size(); i++){
        DrawPoint(&second, secondPoints[i].x, secondPoints[i].y, Vec3i(0,0,255));
    }
    
    imshow("vertex", first);
    imshow("vertex 2", second);
    
    
    
    return GluePictures(first, second, first.cols-135);
}

int main(){
    
    Mat first = imread("/Users/mythings/Documents/С++/repocpp/OpenCV.0/OpenCV.0/1.jpg");
    imshow("first part", first); //вывод оригинального изображения 1
    
    Mat second = imread("/Users/mythings/Documents/С++/repocpp/OpenCV.0/OpenCV.0/2.jpg");
    imshow("second part", second); //вывод оригинального изображения 2
    
    //imshow("test", GluePictures(first, second, first.cols-135));
    
    
    // Not work
    /*
    Mat simpleRes = VerySimpleСoncatenation(first, second);
    imshow("simpleRes", simpleRes);*/
    
    imshow("Result", MainPointConcatenation(first, second));
    
    waitKey(0);
    return 0;
}
