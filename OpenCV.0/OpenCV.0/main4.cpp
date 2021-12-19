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

bool PixelEqual(Vec3i first, Vec3i second){
    return first[0] == second[0] && first[1] == second[1] && first[2] == second[2];
}

Mat GluePictures(Mat first, Mat second, int borderPosition)
{
    Mat result(second.rows, second.cols + borderPosition, CV_8UC3, Scalar(255, 255, 0));
    
    first(Rect(0,0,borderPosition, first.rows)).copyTo(result(Rect(0,0,borderPosition, first.rows)));
    imshow("temp result 1", result);
    
    second.copyTo(result(Rect(borderPosition, 0, second.cols, second.rows)));
    return result;
}

Mat VerySimpleСoncatenation(Mat first, Mat second) {
    
    int border = 0;
    bool flag = false;

    Vec3i secondPixel = second.at<Vec3b>(0,700);
    
    while (!flag && border < first.cols) {
        Vec3i firstPixel = first.at<Vec3b>(border, 700);
        
        if (PixelEqual(firstPixel, secondPixel))
        {
            if (
            PixelEqual(first.at<Vec3i>(border, 720), second.at<Vec3i>(0, 720)) &&
            PixelEqual(first.at<Vec3i>(border + 20, 700), second.at<Vec3i>(19, 700)) &&
            PixelEqual(first.at<Vec3i>(border + 20, 720), second.at<Vec3i>(19, 720)))
            {
                break;
            }
        }
        border++;
    }
    
    return GluePictures(first, second, border);
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
    
    waitKey(0);
    return 0;
}
