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
#include "main2.hpp"
#include <stdio.h>
#include <cmath>

using namespace cv;
using namespace std;

int main(){
    
    Mat first = imread("/Users/mythings/Documents/С++/repocpp/OpenCV.0/OpenCV.0/1.jpg");
    imshow("showing something charming(1)", first); //вывод оригинального изображения 1
    
    Mat second = imread("/Users/mythings/Documents/С++/repocpp/OpenCV.0/OpenCV.0/2.jpg");
    imshow("showing something charming(2)", second); //вывод оригинального изображения 2
    
    
    waitKey(0);
    return 0;
}
