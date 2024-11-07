#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>



using namespace std;
using namespace cv;


bool isDiff(Mat1f & a, Mat1f & b) {
    for(int u = 0; u<a.rows; u++) {
        for(int v = 0; v<a.cols; v++) {
            if(a.at<float>(v,u) != b.at<float>(v,u)) return true;
        }
    }
    return false;
}


int main(int argc, char *argv[]) {
    //Mat im = imread("sieb.png", IMREAD_GRAYSCALE);
    Mat im = imread("test_u.bmp", IMREAD_GRAYSCALE);
    //Mat im;
    //resize(im_orig, im, Size(600, 400), INTER_LINEAR);
    Mat im2;
    cvtColor(im, im2, COLOR_GRAY2BGR);
    Mat1f im_float(im.cols, im.rows);

    for(int u = 0; u<im_float.rows; u++) {
        for(int v = 0; v<im_float.cols; v++) {
            im_float.at<float>(v,u) = (float)im.at<uchar>(v,u) / 255.0;
        }
    }

    Mat1f dx(im.cols, im.rows), dy(im.cols, im.rows);
    
    Sobel(im_float, dx, CV_32F, 1, 0);
    Sobel(im_float, dy, CV_32F, 0, 1);
    
    for(int u = 0; u<dx.rows; u++) {
        for(int v = 0; v<dx.cols; v++) {
            dx.at<float>(v,u) = dx.at<float>(v,u) / 8.0;
        }
    }

    for(int u = 0; u<dx.rows; u++) {
        for(int v = 0; v<dx.cols; v++) {
            dy.at<float>(v,u) = dy.at<float>(v,u) / 8.0;
        }
    }

    Mat1f u = dx.clone();
    Mat1f v = dy.clone();

    Mat1f b=dx.clone(), c1= dx.clone(), c2= dx.clone();
    Mat1f ut=u.clone(), vt=v.clone();


    for(int u = 0; u<im.rows; u++) {
        for(int v = 0; v<im.cols; v++) {
            b.at<float>(v,u) = dx.at<float>(v,u) + dy.at<float>(v,u);
            c1.at<float>(v,u) = b.at<float>(v,u) * dx.at<float>(v,u);
            c2.at<float>(v,u) = b.at<float>(v,u) * dy.at<float>(v,u);
        }
    }
    int n = 0;
    float r = 0.25;
    while((!isDiff(v,vt)) && (!isDiff(u,ut))) {
    //while(n<65) {
        for(int j = 1; j<(im.rows-1); j++) {
            for(int i = 1; i<(im.cols-1); i++) {
                ut.at<float>(i,j) = 
                  (1-b.at<float>(i,j))
                  *u.at<float>(i,j)
                  +r
                  * (( u.at<float>(i+1,j) 
                  + u.at<float>(i,j+1)) 
                  + u.at<float>(i-1,j) 
                  + u.at<float>(i,j-1) 
                  - 4*u.at<float>(i,j)  )
                  + c1.at<float>(i,j);
                vt.at<float>(i,j) = 
                  (1-b.at<float>(i,j))
                  *v.at<float>(i,j)
                  +r
                  * ( (v.at<float>(i+1,j)  
                  + v.at<float>(i,j+1)) 
                  + v.at<float>(i-1,j) 
                  + v.at<float>(i,j-1) 
                  - 4*v.at<float>(i,j)  )
                  + c2.at<float>(i,j);
                //if(ut.at<float>(i,j) != ut.at<float>(i,j)) {
                //    cout<<"sfefsef"<<endl;
                //}
            }
        }
        //u = ut.clone();
        //v = vt.clone(); 
        for(int j = 0; j<(im.rows-1); j++) {
            for(int i = 0; i<(im.cols-1); i++) {
                v.at<float>(i,j) = vt.at<float>(i,j);
                u.at<float>(i,j) = ut.at<float>(i,j);
            }
        }
        n++;
    } 

    int x, y;
    float magnitude;
    for(int j = 1; j<(im.rows-1); j+=8) {
        for(int i = 1; i<(im.cols-1); i+=8) {
            if((u.at<float>(i,j) != 0) || (v.at<float>(i,j) != 0)) {
                if(u.at<float>(i,j)>0) {
                    x=3;
                } else {
                    x=-3;
                }
                if(v.at<float>(i,j)>0) {
                    y=3;
                } else {
                    y=-3;
                }
                magnitude = sqrt(pow(u.at<float>(i,j),2)+pow(v.at<float>(i,j),2));
                arrowedLine( im2, Point(i,j), Point(i+y, j+x), Scalar(255,255,0), (1/(exp(-2*magnitude)+1))+1);
            }
        }
    }

    //namedWindow("image", WND_PROP_FULLSCREEN);
    //setWindowProperty("image",WND_PROP_FULLSCREEN,WINDOW_FULLSCREEN);
    imshow( "image", im2 );
    waitKey(0);

    return 0;
}