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


int const errorParameter = 88;

void DrawPoint(Mat* image, int xcoord, int ycoord, Vec3i bgr) {
    
    for (int i = xcoord-2; i < xcoord+2; i++) {
        for (int j = ycoord-2; j < ycoord+2; j++) {
            (*image).at<Vec3b>(j, i)[0] = bgr[0];
            (*image).at<Vec3b>(j, i)[1] = bgr[1];
            (*image).at<Vec3b>(j, i)[2] = bgr[2];
        }
    }
}

void DrawLine(Mat* image, int x1, int y1, int x2, int y2){
    
    Vec3i color;
    
    int a = rand() % 156;
    
    color[0] = 100 + a;
    a = rand() % (156 - a);
    color[1] = 100 + a;
    a = rand() % (156 - a);
    color[2] = 100 + a;
    
    for (int i = 0; i < 51; i++){
        DrawPoint(image,
                  x1 * i / 50 + x2 * (50 - i) / 50,
                  y1 * i / 50 + y2 * (50 - i) / 50,
                  color);
    }
}

Vec3i GetRandomColor() {
    Vec3i result;
    int a = rand() % 156;
    
    result[0] = 100 + a;
    a = rand() % (156 - a);
    result[1] = 100 + a;
    a = rand() % (156 - a);
    result[2] = 100 + a;
    
    return result;
}

double Length(int x1, int y1, int x2, int y2){
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

class Line{
public:
    int xStart, yStart, xFinish, yFinish;
    double length;
    Vec3i lineColor;
    
    Line(int x1, int y1, int x2, int y2, Vec3i color){
        xStart = x1;
        yStart = y1;
        xFinish = x2;
        yFinish = y2;
        lineColor = color;
        
        length = Length(x1,y1,x2,y2);
    }
    
    string GetInfo(){
        string result ="StartPoint: [ ";
        result += xStart;
        result +=", ";
        result +=yStart;
        result +="]";
        
        result += " FinishPoint: [ ";
        result += xFinish;
        result +=", ";
        result +=yFinish;
        result +="]";
        
        
        return result;
    }
    
    void DrawLineInImage(Mat* image){
        
        for (int i = 0; i < 51; i++) {
            DrawPoint(image,
                      xStart * i / 50 + xFinish * (50 - i) / 50,
                      yStart * i / 50 + yFinish * (50 - i) / 50,
                      lineColor);
        }
        
    }
    
    ~Line() {};
    
};

// 0 - not found
// 1 - 1.1 ± 2.1
// 2 - 1.1 ± 2.2
// 3 - 1.2 ± 2.1
// 4 - 1.2 ± 2.2
int Comparison_Result(Vec4i first, Vec4i second) {
    
    if (Length(first[0], first[1], second[0], second[1]) < errorParameter)
        return 1;
    
    if (Length(first[0], first[1], second[2], second[3]) < errorParameter)
        return 2;
    
    if (Length(first[2], first[3], second[0], second[1]) < errorParameter)
        return 3;
    
    if (Length(first[2], first[3], second[2], second[3]) < errorParameter)
        return 4;
    
    return 0;
}

void WriteLineInfo(Line lineInfo){
    cout << lineInfo.GetInfo() << endl;
    cout << "Length: " << lineInfo.length;
    
}


int main()
{
    
    Mat paper = imread("/Users/mythings/Documents/С++/repocpp/OpenCV.0/OpenCV.0/paper.jpg");
    
    //DrawPoint(&paper, 2402, 1530);
    
    imshow("showing something charming(1)", paper); //вывод оригинального изображения
    
    Mat cannyPaper;
    Canny(paper, cannyPaper, 60, 300, 3 );
    //imshow("showing something charming(2)", cannyPaper); //вывод изображения краёв
    
    vector<Vec4i> lines;
    HoughLinesP(cannyPaper, lines, 1, 2*CV_PI/180, 10, 55, 20);
    cout << endl << lines.size() << endl;
    
    for (int i = 0; i < lines.size(); i++){
        DrawLine(&paper, lines[i][0], lines[i][1], lines[i][2], lines[i][3]);
    }
    
    imshow("Draw find conturs", paper); //вывод изображения краёв
    cout << cannyPaper.size();
    
    
    // Достроение недостающих линий контуров
    vector<Line> allLines;
    
    for (int i = 0; i < lines.size(); i++){
        for (int j = i + 1; j < lines.size(); j++) {
            int comparationRes = Comparison_Result(lines[i], lines[j]);
            if (comparationRes != 0) {
                switch (comparationRes){
                    case 1:
                        allLines.push_back(Line(lines[i][0], lines[i][1], lines[j][0], lines[j][1], GetRandomColor()));
                        break;
                    case 2:
                        allLines.push_back(Line(lines[i][0], lines[i][1], lines[j][2], lines[j][3], GetRandomColor()));
                        break;
                    case 3:
                        allLines.push_back(Line(lines[i][2], lines[i][3], lines[j][0], lines[j][1], GetRandomColor()));
                        break;
                    case 4:
                        allLines.push_back(Line(lines[i][2], lines[i][3], lines[j][2], lines[j][3], GetRandomColor()));
                        break;
                }
            }
        }
        allLines.push_back(Line(lines[i][0], lines[i][1], lines[i][2], lines[i][3], GetRandomColor()));
    }
    
    cout << endl << allLines.size() << endl;
    
    for (int i = 0; i < allLines.size(); i++){
        WriteLineInfo(allLines[i]);
        allLines[i].DrawLineInImage(&paper);
    }
    
    
    //
    
    imshow("Draw all conturs", paper); //вывод всех найденных краев изображения
    cout << cannyPaper.size();
    
   
    
    waitKey(0);
    return 0;
}
