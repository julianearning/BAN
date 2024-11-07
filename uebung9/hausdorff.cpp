#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>



using namespace std;
using namespace cv;


int main(int argc, char *argv[]) {

    bool v1 = false;

    Mat im1 = imread("B1.png", IMREAD_GRAYSCALE);
    Mat im2 = imread("B2.png", IMREAD_GRAYSCALE);
    Mat output(im1.cols, im1.rows, CV_8UC3, cv::Scalar(0,0,0));
    vector<vector<Point> > contours1;
    vector<Vec4i> hierarchy1;
    vector<vector<Point> > contours2;
    vector<Vec4i> hierarchy2;
    findContours( im1, contours1, hierarchy1,
        RETR_CCOMP, CHAIN_APPROX_SIMPLE );
    findContours( im2, contours2, hierarchy2,
        RETR_CCOMP, CHAIN_APPROX_SIMPLE );
    int idx = 0;
    for( ; idx >= 0; idx = hierarchy1[idx][0] ){
        Scalar color( 255, 255, 255 );
        drawContours( output, contours1, idx, color, 1,LINE_4, hierarchy1 );
    }
    idx = 0;
    for( ; idx >= 0; idx = hierarchy2[idx][0] ){
        Scalar color( 255, 255, 255 );
        drawContours( output, contours2, idx, color, 1,LINE_4, hierarchy2 );
    }

    if(v1) {

        float max_dist=0;
        float max_dist1, max_dist2;
        float curr_dist;
        float min_dist;
        Point pointa_min;
        Point pointb_min;
        Point pointa_max;
        Point pointb_max;
        Point pointa_max2;
        Point pointb_max2;

        for(Point p1 : contours1[0]) {
            min_dist = 100000000;
            pointa_min.x = p1.x;
            pointa_min.y = p1.y;
            for(Point p2: contours2[0]) {
                curr_dist = sqrt(pow(p1.x-p2.x,2)+pow(p1.y-p2.y,2));
                if(curr_dist < min_dist) {
                    min_dist=curr_dist;
                    pointb_min.x = p2.x;
                    pointb_min.y = p2.y;
                }
            }
            if(min_dist > max_dist) {
                max_dist = min_dist;
                pointa_max = pointa_min;
                pointa_max = pointb_min;
            }
        }


        max_dist1 = max_dist;


        max_dist=0;

        for(Point p1 : contours2[0]) {
            min_dist = 100000000;
            pointa_min.x = p1.x;
            pointa_min.y = p1.y;
            for(Point p2: contours1[0]) {
                curr_dist = sqrt(pow(p1.x-p2.x,2)+pow(p1.y-p2.y,2));
                if(curr_dist < min_dist) {
                    min_dist=curr_dist;
                    pointb_min.x = p2.x;
                    pointb_min.y = p2.y;
                }
            }
            if(min_dist > max_dist) {
                max_dist = min_dist;
                pointa_max2 = pointa_min;
                pointb_max2 = pointb_min;
            }
        }

        max_dist2=max_dist;


        if(max_dist1 > max_dist2) {
            line(output, pointa_max, pointb_max, Scalar(255,0,0));
            max_dist = max_dist1;
        } else {
            cout<<pointb_max2.x<<"   "<<pointb_max2.y<<"\n";
            line(output, pointa_max2, pointb_max2, Scalar(255,0,0));
            max_dist = max_dist2;
        }

        cout<<max_dist<<endl;

    } else {
        Mat dist1;
        Mat dist2;
        Mat in1(im1.cols, im1.rows, CV_8UC1);
        Mat in2(im1.cols, im1.rows, CV_8UC1);
        int idx = 0;
        for( ; idx >= 0; idx = hierarchy1[idx][0] ){
            drawContours( in1, contours1, idx, -1, 1,LINE_4, hierarchy1 );
        }
        idx = 0;
        for( ; idx >= 0; idx = hierarchy2[idx][0] ){
            drawContours( in2, contours2, idx, -1, 1,LINE_4, hierarchy2 );
        }
        distanceTransform(in1, dist1, DIST_L2, 3);
        distanceTransform(in2, dist2, DIST_L2, 3);
        imshow( "dst", dist1 );
        waitKey(0);
    }


    imshow( "image", output );
    waitKey(0);

    return 0;
}