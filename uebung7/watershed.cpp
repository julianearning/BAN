#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

#define MASK -2
#define WSHED 0
#define INIT -1

using namespace std;
using namespace cv;


struct entry {
    int x;
    int y;
    float gradient;
};


bool compareByGradient(const entry &a, const entry &b)
{
    return a.gradient < b.gradient;
}

int main(int argc, char *argv[]) {
    //Mat im = imread("sieb.png", IMREAD_GRAYSCALE);
    Mat im = imread("sieb.png", IMREAD_GRAYSCALE);

    Mat1f im_i;
    Mat im_i_gray = im.clone();
    Mat1f dx, dy;
    //Mat dy = dx.clone();
    Mat1f im_o(im.rows, im.cols);
    Mat1f im_d(im.rows, im.cols);

    Sobel(im, dx, CV_32F, 1, 0);
    Sobel(im, dy, CV_32F, 0, 1);
    
    cv::magnitude(dx, dy, im_i);

    //normalize(im_i, im_i, 0, 255);

    GaussianBlur(im_i, im_i, Size( 3, 3 ), 0, 0 );

    float min=1000000;
    float max=0;

    for(int u = 0; u<im.rows; u++) {
        for(int v = 0; v<im.rows; v++) {
            if(im_i.at<float>(v,u)< min) {
                min=im_i.at<float>(v,u);
            } 
            if(im_i.at<float>(v,u)> max) {
                max=im_i.at<float>(v,u);
            }

        }
    }

    for(int u = 0; u<im.rows; u++) {
        for(int v = 0; v<im.rows; v++) {
            im_i.at<float>(v,u) = floor(((im_i.at<float>(v,u) - min) * 255) / (max - min));
        }
    }

    int hist [256] = {0};
    vector<entry> vec;
    entry e;

    for(int u = 0; u<im.rows; u++) {
        for(int v = 0; v<im.rows; v++) {
            hist[(int)im_i.at<float>(v,u)]++;
            e.x = v;
            e.y = u;
            e.gradient =im_i.at<float>(v,u);
            vec.push_back(e);
        }
    }


    sort(vec.begin(), vec.end(), compareByGradient);
    
    int idx_current=0;
    for(int i=0; i<256; i++) {
        while((vec.at(idx_current) == i) && (idx_current < vec.size())) {
            // TODOOOOOo
            idx_current++;
        }
    }


    imshow( "gradient", im_i );
    waitKey(0);

    return 0;
}