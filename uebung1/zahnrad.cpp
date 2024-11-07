#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

int main() {
    Mat orig = imread("bilder/zahnrad.png",IMREAD_GRAYSCALE);
    //Mat img_bw;
    //threshold(orig, img_bw, 128.0, 255.0, THRESH_BINARY);
    //orig = img_bw;
    Mat eroded;
    Mat diff;
    Mat outer = Mat::zeros(Size(orig.cols,orig.rows),CV_8UC1);
    Mat components;
    Mat dilated;
    erode(orig, eroded, getStructuringElement(MORPH_RECT, Size(12,12)));
    diff = orig-eroded;
    vector<vector<Point> > _contoursQuery;
    vector<Point> largest_contour;
    int largest_contour_size=0;
    findContours(diff, _contoursQuery, RETR_TREE , CHAIN_APPROX_NONE);
    for(auto contour : _contoursQuery) {
        if(contour.size() > largest_contour_size) {
            largest_contour = contour;
            largest_contour_size = contour.size();
        }
    }
    for(Point p: largest_contour) {
        outer.at<char>(p) = 255;
    }
    dilate(outer, dilated, Mat(), Point(-1,-1), 7);
    erode(dilated, eroded, Mat(), Point(-1,-1),12);
    imshow("Outer", eroded); 
    waitKey(0);

}

