#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;
using namespace cv;

//int getMin(int val1, int val2, int val3, int val4, int val5){
//    
//}



int main(int argc, char *argv[]) {
    Mat im = imread("strasse1.bmp",IMREAD_GRAYSCALE);
    Mat edges = im.clone();
    Mat im_col = im.clone();

    // get Canny
    Canny(im, edges, 210, 300);

    Mat shape = imread("zeichen1.bmp", IMREAD_GRAYSCALE);

    // Step 1: INITIALIZE


    int D [480][640];

    for(int i = 0; i<480; i++) {
        for(int j = 0; j<640; j++) {
            if(edges.at<uchar>(i,j) > 0) {
                D[i][j] = 0;
            } else {
                //D[i][j] = std::numeric_limits<int>::max();
                D[i][j] = 10000;
            }
        }
    }

    int norm1_1 = 1;
    int norm1_2 = 2; 


    int d1=0;
    int d2=0;
    int d3=0;
    int d4=0;

    std::vector<int> temp;

    // L-R Pass

    for(int v = 0; v<im.rows; v++) {
        for(int u = 0; u<im.cols; u++) {
            if(D[u][v] > 0) {
                //d1 = std::numeric_limits<int>::max();
                //d2 = std::numeric_limits<int>::max();
                //d3 = std::numeric_limits<int>::max();
                //d4 = std::numeric_limits<int>::max();
                d1 = 10000;
                d2 = 10000;
                d3 = 10000;
                d4 = 10000;

                if(u > 0) {
                    d1 = norm1_1 + D[u-1][v];
                    if(v>0){
                        d2 = norm1_2 + D[u-1][v-1];
                    }
                }
                if(v>0) {
                    d3 = norm1_1 + D[u][v-1];
                    if(u < (im.cols-1)) {
                        d4 = norm1_2 + D[u+1][v-1];
                    }
                }

                temp.clear();
                temp.push_back(D[u][v]);
                temp.push_back(d1);
                temp.push_back(d2);
                temp.push_back(d3);
                temp.push_back(d4);

                D[u][v] = *max_element(std::begin(temp), std::end(temp));

            }
        }
    }


    // R-L Pass

    for(int v = im.rows-1; v>=0; v--) {
        for(int u = im.cols-1; u>=0; u--) {
            if(D[u][v] > 0) {
                //d1 = std::numeric_limits<int>::max();
                //d2 = std::numeric_limits<int>::max();
                //d3 = std::numeric_limits<int>::max();
                //d4 = std::numeric_limits<int>::max();
                d1 = 10000;
                d2 = 10000;
                d3 = 10000;
                d4 = 10000;
                if(u<(im.cols-1)) {
                    d1 = norm1_1 + D[u+1][v];
                    if(v < im.rows-1) {
                        d2 = norm1_2 + D[u+1][v+1];
                    }
                }
                if(v < (im.rows-1)) {
                    d3 = norm1_1 + D[u][v+1];
                    if(u > 0) {
                        d4 = norm1_2 + D[u-1][v+1];
                    }
                } 
                temp.clear();
                temp.push_back(D[u][v]);
                temp.push_back(d1);
                temp.push_back(d2);
                temp.push_back(d3);
                temp.push_back(d4);

                D[u][v] = *max_element(std::begin(temp), std::end(temp));               
            }

        }
    }



    // Chamfer match


    int MI = im.cols;
    int NI = im.rows;
    int MR = shape.cols;
    int NR = shape.rows;
    int q=0;
    int n=0;

    //std::cout<<MI-MR+1<<"  "<<NI-NR+1<<endl;
    double Q [338][501];

    for(int r = 0; r <= (MI-MR); r++) {
        for(int s = 0; s <= (NI-NR); s++) {
            q = 0;
            n=0;
            for(int i = 0; i<=(MR-1); i++) {
                for(int j = 0; j<=(NR-1); j++) {
                    if((shape.at<uchar>(i,j))>0) {
                        q = q + D[r+i][s+j];
                        n = n + 1;
                    }
                }
            }

            Q[r][s] = (double)q/(double)n;
        }
    }

    double min= std::numeric_limits<double>::max();
    double mini=0;
    double minj=0;
    for(int i = 0; i<338; i++) {
        for(int j = 0; j <501; j++) {
            //cout<<Q[i][j]<<endl;
            if(Q[i][j] < min) {
                min = Q[i][j];
                mini = i;
                minj = j;
            }
        }
    }

    cvtColor(im, im_col, COLOR_GRAY2BGR);


    circle(im, Point(minj+((int)(shape.rows/2)),mini+((int)(shape.cols/2))), 50, Scalar(0, 0, 255), FILLED, LINE_AA);

    imshow( "edges", im );
    waitKey(0);


}