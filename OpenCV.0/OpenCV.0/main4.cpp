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

void DrawPoint(Mat* image, int xcoord, int ycoord, Vec3i bgr) // нарисовать 1 точку на изображении
{

    for (int i = xcoord-2; i < xcoord+2; i++) {
        for (int j = ycoord-2; j < ycoord+2; j++) {
            (*image).at<Vec3b>(j, i)[0] = bgr[0];
            (*image).at<Vec3b>(j, i)[1] = bgr[1];
            (*image).at<Vec3b>(j, i)[2] = bgr[2];
        }
    }
}

Mat GluePictures(Mat first, Mat second, int borderPosition, int secondBorderPos) //склеиваем 2 изображения
{
    Mat result(second.rows, second.cols + borderPosition - secondBorderPos, CV_8UC3, Scalar(255, 255, 0));
    
    first(Rect(0,0,borderPosition, first.rows))
        .copyTo(result(Rect(0,0,borderPosition, first.rows)));
    
    second(Rect(secondBorderPos, 0, second.cols - secondBorderPos, second.rows))
        .copyTo(result(Rect(borderPosition, 0, second.cols - secondBorderPos, second.rows)));
    return result;
}

double GetDistance(Point2i first, Point2i second) //метод нахождения расстояния между 2мя точками
{
    return sqrt(
        (first.x - second.x) * (first.x - second.x) +
        (first.y - second.y) * (first.y - second.y)
                );
}

vector<Point2i> FindMainPoints(Mat cannyImage)  //ищем ключевые точки
{
    vector<vector<Point2i>> contours;
    findContours(cannyImage, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    vector<Point2i> approx;
    for (int i = 0; i < contours.size(); i++) {
        vector<Point2i> contourPoints;
        approxPolyDP(contours[i], contourPoints, 30, true);
        for (int j = 0; j < contourPoints.size(); j++)
            approx.push_back(contourPoints[j]);
}
    return approx;
}


vector<double> GetAllDistance(vector<Point2i> points, int pointIndex)  //массив элементов полученных getdistance
{
    vector<double> result;
    
    for (int i = 0; i < points.size(); i++){
        if (i != pointIndex)
            result.push_back(GetDistance(points[pointIndex], points[i]));
    }
    
    // По возрастанию
    sort(result.begin(), result.end());
    return result;
}

int GetCorelation (vector<double> firstDistances, vector<double> secondDistances) //сравниваем
{
    int resultCount = 0;
    
    int fIter = 0;
    int sIter = 0;
    
    while (fIter < firstDistances.size() && sIter < secondDistances.size()){
        // Если различие значений меньше чем
        if (fabs(firstDistances[fIter] - secondDistances[sIter]) < 0.001) {
            resultCount++;
            fIter++;
            sIter++;
            continue;
        }
        
        if (firstDistances[fIter] > secondDistances[sIter]) {
            sIter++;
        }
        
        if (firstDistances[fIter] < secondDistances[sIter]) {
            fIter++;
        }
    }
    
    return resultCount;
}

vector<Point2i> FindBestPoint(vector<Point2i> firstArray, vector<Point2i> secondArray) //находим из этих точек лучшую
{
    Point2i bestPointF = firstArray[0];
    Point2i bestPointS = secondArray[0];

    int bestCorelation = 0;
    
    auto comp = [](const Point2i& a, const Point2i& b) { return a.x < b.x; }; //метод comp - сравнивает между собой 2 точки
    map<Point2i, vector<double>, decltype(comp)> firstPointsDistances(comp); //map - набор связей point+расстояние для других точек
    map<Point2i, vector<double>, decltype(comp)> secondPointsDistances(comp);
    
    for (int i = 0; i < firstArray.size(); i++)
    {
        firstPointsDistances.insert(pair<Point2i, vector<double>>( firstArray[i], GetAllDistance(firstArray, i))); //высчитываем массив дистанций для каждого элемента массива
    }
    
    for (int i = 0; i < secondArray.size(); i++)
    {
        secondPointsDistances.insert(pair<Point2i, vector<double>>( secondArray[i], GetAllDistance(secondArray, i)));
    }
    
    for (auto fIter = firstPointsDistances.begin(); fIter != firstPointsDistances.end(); fIter++ ) //так работает map, это цикл по firstPointsDistances(1я map)
    {
        for (auto sIter = secondPointsDistances.begin(); sIter != secondPointsDistances.end(); sIter++)
        {
            int correlation = GetCorelation(fIter->second, sIter->second); //находим точек у которых наибольшее количество одинаковых дистанций
            if (correlation > bestCorelation)
            {
                bestPointF = fIter->first;
                bestPointS = sIter->first;
                bestCorelation = correlation;
            }
        }
    }
    
    vector<Point2i> result;
    result.push_back(bestPointF);
    result.push_back(bestPointS);
    
    return result;
}

void ShowPoints(Mat image, vector<Point2i> points, string windName)
{
    for (int i = 0 ; i < points.size(); i++){
        DrawPoint(&image, points[i].x, points[i].y, Vec3i(0,0,255));
    }
    
    imshow(windName, image);
    
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
    
    // ShowPoints(first, firstPoints, "Точки первой картинки");
    // ShowPoints(second, secondPoints, "Точки второй картинки");
    
    // Поиск общей точки
    vector<Point2i> jointPoint = FindBestPoint(firstPoints, secondPoints);
    
    DrawPoint(&first, jointPoint[0].x, jointPoint[0].y, Vec3i(0,255,0)); //рисуем общую (зелёную) точку
    DrawPoint(&second, jointPoint[1].x, jointPoint[1].y, Vec3i(0,255,0));
    
    imshow("First Point", first);
    imshow("Second Point", second);
    
    return GluePictures(first, second, jointPoint[0].x, jointPoint[1].x);
    
}

int main()
{
    
    Mat first = imread("/Users/mythings/Documents/С++/repocpp/OpenCV.0/OpenCV.0/1.jpg");
    imshow("first part", first); //вывод оригинального изображения 1
    
    Mat second = imread("/Users/mythings/Documents/С++/repocpp/OpenCV.0/OpenCV.0/2.jpg");
    imshow("second part", second); //вывод оригинального изображения 2
    
    
    imshow("Result", MainPointConcatenation(first, second));
    
    waitKey(0);
    return 0;
}
