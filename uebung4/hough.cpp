#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;
using namespace cv;

int main(int argc, char *argv[]) {
    Mat orig = imread("F207.png",IMREAD_GRAYSCALE);
    Mat edges = orig.clone();

    // get Canny
    Canny(orig, edges, 100, 300);

    int diagonal = (int) sqrt(pow(orig.cols,2) + pow(orig.rows,2));

    int accu [180][954];
    int y_min [180][954];
    int y_max [180][954];
    int x_min [180][954]={-1};
    int x_max [180][954]={-1};
    int ri = 0;
    double theta_rad;

    for(int x = 0; x<edges.cols; x++) {
        for(int y=0;y<edges.rows;y++) {
            if(edges.at<uchar>(x, y) == 255) {
                for(int theta = 0; theta<180; theta++) {
                    theta_rad = (theta*M_PI)/180;
                    ri = (int)((x * cos(theta_rad)) + (y * sin(theta_rad)));
                    accu[theta][ri]++;
                    y_max[theta][ri] = y;
                    x_max[theta][ri] = x;
                    if(x_min[theta][ri] == -1) {
                        x_min[theta][ri] = x;
                    }
                    if(y_min[theta][ri] == -1) {
                        y_min[theta][ri] = y;
                    }
                }
            }
        }
    }

    vector<vector<int>> maxs;
    vector<int> temp;
    int temp_max=0;
    int temp_max_i = 0;
    int temp_max_j = 0;
    // Max Lines
    for(int count = 0; count<10;count++) {
        for(int i = 0; i<180;i++) {
            for(int j = 0; j<diagonal; j++) {
                if(accu[i][j] > temp_max) {
                    temp_max = accu[i][j]; 
                    temp_max_i = i;
                    temp_max_j = j;
                }
            }
        }
        temp.clear();
        temp.push_back(temp_max_i);
        temp.push_back(temp_max_j);
        maxs.push_back(temp);
        accu[temp_max_i][temp_max_j] = 0;
    }


    Mat accu_scaled = cv::Mat(180, 954, CV_8UC1, accu);

    cvtColor(edges, edges, COLOR_GRAY2BGR);

    for(auto gerade: maxs) {
        //cout<<y_max[gerade[0]][gerade[1] ]<<", "<<
        line(edges, Point(  y_max[gerade[0]][gerade[1] ] , x_max[gerade[0]][gerade[1]] ), Point(  y_min[gerade[0]][gerade[1] ] , x_min[gerade[0]][gerade[1]] ), Scalar(0,0,255));
    }

    imshow( "edges", edges );
    waitKey(0);

}