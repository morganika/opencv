//
//  main.cpp
//  OpenCV.0
//
//  Created by Юлька on 02.11.2021.
//  Copyright © 2021 Юлька. All rights reserved.
//
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <string>
using namespace cv;
using namespace std;

int main()
{

    cout << "Hello, World!\n";

    Mat cat= imread("/Users/mythings/Documents/С++/repocpp/OpenCV.0/OpenCV.0/cobake.jpg");
    imshow("showing something charming(1)", cat); //вывод оригинального изображения
    
    Mat im_gray = imread("/Users/mythings/Documents/С++/repocpp/OpenCV.0/OpenCV.0/cobake.jpg", IMREAD_GRAYSCALE);
    imshow("showing something charming(2)", im_gray); //вывод чб изображения
    
    Mat huecat;
    cvtColor(cat, huecat, COLOR_HSV2BGR);
    imshow("showing something charming(3)",huecat); //вывод изображения с изменёнными отенками (HUE)
    
    Mat gaussecobake;
    GaussianBlur(cat, gaussecobake, Size(11, 11), 0, 0, BORDER_DEFAULT);
    imshow("showing something charming(4)",gaussecobake); //вывод изображения размытого по Гауссу
    
    Mat cannycat;
    Canny( cat,cannycat, 10, 70, 3 );
    imshow("showing something charming(5)",cannycat); //вывод изображения краёв
    
    Mat myvision;
    pyrUp(cat, myvision);
    imshow("showing something charming(6)", myvision); //вывод увеличенного изображения
           
    waitKey(0);
    return 0;
    
}
