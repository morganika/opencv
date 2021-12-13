//
//  main3.cpp
//  OpenCV.0
//
//  Created by Юлька on 12.12.2021.
//  Copyright © 2021 Юлька. All rights reserved.
//

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

void WriteLineInfo(Vec4i lineInfo){
    cout << lineInfo << endl;
    cout << "Length: " << sqrt(
              (lineInfo[0] - lineInfo[2]) * (lineInfo[0] - lineInfo[2]) +
              (lineInfo[1] - lineInfo[3]) * (lineInfo[1] - lineInfo[3])) << endl << endl;
    
}

void DrawPoint(Mat* image, int xcoord, int ycoord) {
    
    for (int i = xcoord-2; i < xcoord+2; i++) {
        for (int j = ycoord-2; j < ycoord+2; j++) {
            //(*image).at<Vec3b>(j, i)[0] = 255;
            (*image).at<Vec3b>(j, i)[0] = 255;
            (*image).at<Vec3b>(j, i)[1] = 100;
            (*image).at<Vec3b>(j, i)[2] = 255;
        }
    }
}

void DrawLine(Mat* image, int x1, int y1, int x2, int y2){
    
    for (int i = 0; i < 51; i++){
        DrawPoint(image, x1 * i / 50 + x2 * (50 - i) / 50, y1 * i / 50 + y2 * (50 - i) / 50);
    }
}

int main(){
    
    Mat paper = imread("/Users/mythings/Documents/С++/repocpp/OpenCV.0/OpenCV.0/paper.jpg");
    
    //DrawPoint(&paper, 2402, 1530);
    
    imshow("showing something charming(1)", paper); //вывод оригинального изображения
    
    Mat cannyPaper;
    Canny(paper, cannyPaper, 60, 300, 3 );
    //imshow("showing something charming(2)", cannyPaper); //вывод изображения краёв
    
    vector<Vec4i> lines;
    HoughLinesP(cannyPaper, lines, 0.1, 2*CV_PI/180, 4, 26, 6);
    cout << endl << lines.size() << endl;
    
    for (int i = 0; i < lines.size(); i++){
        WriteLineInfo(lines[i]);
        //DrawPoint(&paper, lines[i][0], lines[i][1]);
        //DrawPoint(&paper, lines[i][2], lines[i][3]);
        DrawLine(&paper, lines[i][0], lines[i][1], lines[i][2], lines[i][3]);
    }
    
    
    //
    
    imshow("Draw pixels", paper); //вывод изображения краёв
    cout << cannyPaper.size() ;
    
   
    
    waitKey(0);
    return 0;
}
