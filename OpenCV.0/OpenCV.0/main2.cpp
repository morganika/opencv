//
//  main2.cpp
//  OpenCV.0
//
//  Created by Юлька on 11.12.2021.
//  Copyright © 2021 Юлька. All rights reserved.
//

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include "main2.hpp"

using namespace cv;
using namespace std;

Mat ApplyFilter(Mat image, vector<vector<double>> filter)
{
    int filterRow = (int)filter.size();
    int filterColumn = (int)filter[0].size();
    int halfFilterSize = filterColumn/2;
    
    
    int height, width;

    height=image.rows;
    width=image.cols;

    cout << "Width : " << width << endl;
    cout << "Height: " << height << endl;
    
    Mat result;
    image.copyTo(result);
    
    
    for (int i = halfFilterSize; i < height - halfFilterSize; i++)
        for (int j = halfFilterSize; j < width - halfFilterSize; j++)
            {
                double cr = 0.0, cg = 0.0, cb = 0.0;
                
                for (int wid = 0; wid < filterRow; wid++)
                    for (int heig = 0; heig < filterColumn; heig++) {
                        Vec3b pixel = image.at<Vec3b>(i - halfFilterSize + heig, j - halfFilterSize  + wid);
                        cr += pixel[2] * filter[wid][heig];
                        cg += pixel[1] * filter[wid][heig];
                        cb += pixel[0] * filter[wid][heig];
                    }
                
                result.at<Vec3b>(i, j)[0]=cb;
                result.at<Vec3b>(i, j)[1]=cg;
                result.at<Vec3b>(i, j)[2]=cr;
            }
    
    return result;
};

int main()
{
   // Лаба 2
    Mat cat = imread("/Users/mythings/Documents/С++/repocpp/OpenCV.0/OpenCV.0/cobake.jpg");
    imshow("showing something charming(1)", cat); //вывод оригинального изображения
    
//    const vector<vector<double>> hardBlurMatrix = {
//        {0.000789, 0.006581, 0.013347, 0.006581, 0.000789},
//        {0.006581, 0.054901, 0.111345, 0.054901, 0.006581},
//        {0.013347, 0.111345, 0.225821, 0.111345, 0.013347},
//        {0.006581, 0.054901, 0.111345, 0.054901, 0.006581},
//        {0.000789, 0.006581, 0.013347, 0.006581, 0.000789}
//    };
//
//    const vector<vector<double>> simpleBlurMatrix = {
//            { 0.1125, 0.1125, 0.1125},
//            { 0.1125, 0.1, 0.1125},
//            { 0.1125, 0.1125, 0.1125}
//        };
    
    const vector<vector<double>> mediumBlurMatrix = {
        {0.04, 0.04, 0.04, 0.04, 0.04},
        {0.04, 0.04, 0.04, 0.04, 0.04},
        {0.04, 0.04, 0.04, 0.04, 0.04},
        {0.04, 0.04, 0.04, 0.04, 0.04},
        {0.04, 0.04, 0.04, 0.04, 0.04}
    };
    
    Mat myBlur = ApplyFilter(cat, mediumBlurMatrix);
    imshow("Result 1:", myBlur); //вывод размытого изображения
    
    
    //  Лаба 2.2
    
    Mat im_gray = imread("/Users/mythings/Documents/С++/repocpp/OpenCV.0/OpenCV.0/cobake.jpg", IMREAD_GRAYSCALE);
    imshow("showing something charming(2)", im_gray); //вывод оригинального изображения
    
    const vector<vector<double>> gradNorthMatrix = {
        { 0, -1, 0},
        { 0, 1, 0},
        { 0, 0, 0}
    };
    
    
    Mat grayMethod = ApplyFilter(im_gray, gradNorthMatrix);
    imshow("Result 2:", grayMethod); //вывод градиент-изображения
    
    
    waitKey(0);
    return 0;
    
}

