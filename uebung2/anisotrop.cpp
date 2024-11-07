#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;
using namespace cv;


//Mat current_image;

double g(double I, double K) {
    return(   1   /
         (    1   +   ( sqrt(pow(I,2)) / K  ))); 
}


vector<double> gradient(Mat * current_image, int x, int y) {
    double north = current_image->at<double>(x,y-1) - current_image->at<double>(x,y);
    double south = current_image->at<double>(x,y+1) - current_image->at<double>(x,y);
    double east = current_image->at<double>(x+1,y) - current_image->at<double>(x,y);
    double west = current_image->at<double>(x-1,y) - current_image->at<double>(x,y);

    vector<double> result;
    result.push_back(north);
    result.push_back(south);
    result.push_back(east);
    result.push_back(west);
    return(result);
}


void anisotrope_diffusion(Mat* image, double lambda, double K) {
    Mat temp_image = image->clone();
    vector<double> grad;
    for(int y = 1; y < (image->cols-1); y++) {
        for(int x = 1; x < (image->rows-1); x++) {
            grad = gradient(image, x,y);
            //cout<<( lambda * (grad.at(0) * g(grad.at(0), K) + grad.at(1) * g(grad.at(1), K) 
            // + grad.at(2)  * g(grad.at(2), K) + grad.at(3) * g(grad.at(3), K))*255 )<<endl;
            temp_image.at<double>(x,y) = image->at<double>(x,y) + ( lambda * (grad.at(0) * g(grad.at(0), K) + grad.at(1) * g(grad.at(1), K) 
             + grad.at(2)  * g(grad.at(2), K) + grad.at(3) * g(grad.at(3), K)));
        }
    }
    //current_image = temp_image;
    temp_image.copyTo(*image);
    
}


int main() {
    Mat orig = imread("bilder/wirbelsaeule.png",IMREAD_GRAYSCALE);
    //current_image = orig;
    imshow("orig", orig);
    orig.convertTo(orig, CV_64FC1);
    imshow("conversion therapy", orig);
    //current_image = orig;
    double lambda = 0.25;
    double K = 2;

    int n_epochs = 50;
    for(int i = 0; i<n_epochs; i++) {
        anisotrope_diffusion(&orig, lambda, K);
    }
    orig.convertTo(orig, CV_8UC1);
    imshow("result", orig);
    waitKey(0);
}