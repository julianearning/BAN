#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <cmath>


using namespace std;
using namespace cv;


struct entry {
    int smallest;
    int biggest;
    double mean;
};

double P(double (&hist)[256], int Tk, int Tkm1, int N) {
    double res=0;
    for(int i = Tkm1; i <= Tk; i++) {
        res+=(double)hist[i];
    }
    return res;
}


double mCk(double (&hist)[256], int Tk, int Tkm1, int N) {
    double div = P(hist, Tk, Tkm1, N);
    double s = 0;
    for(int i = Tkm1; i <= Tk; i++) {
        s+=(i*hist[i]);
    }
    return s/div;
}

double interclass_variance(double (&hist)[256], entry entry1, entry entry2, int N) {
    double PCk1 = P(hist, entry1.biggest, entry1.smallest, N);
    double PCk2 = P(hist, entry2.biggest, entry2.smallest, N);
    return pow(((PCk1*PCk2)/pow(PCk1+PCk2,2)) * (entry1.mean  - entry2.mean),2);
}


double intraclass_variance(double (&hist)[256], entry entry1, entry entry2, int N) {
    double PCk1 = P(hist, entry1.biggest, entry1.smallest, N);
    double PCk2 = P(hist, entry2.biggest, entry2.smallest, N);
    double constant = 1/(PCk1+PCk2);
    double s=0.0;
    double MCk1Ck2=(PCk1*entry1.mean + PCk2*entry2.mean)/(PCk1+PCk2);
    for(int i = entry1.smallest; i <= entry2.biggest; i++) {
        s += pow((double)i - MCk1Ck2, 2)*hist[i];
    }
   return constant * s; 
}

int main(int argc, char *argv[]) {

    Mat im = imread("roentgen.jpg",IMREAD_GRAYSCALE);
    Mat thresholded = im.clone();

    // Step 1: 

    double hist [256] = {0.0};
    int N=im.rows*im.cols;


    for(int v = 0; v<im.rows; v++) {
        for(int u = 0; u<im.cols; u++) {
            hist[im.at<uchar>(u,v)] += 1;
        }
    }

    for(int i = 0; i<256; i++) {
        hist[i] /= (double)(im.rows*im.cols);
    }


    vector<entry> vec;
    entry e;

    for(int i = 0; i<256; i++) {
        if(hist[i] > 0) {
            e.biggest = i;
            e.smallest = i;
            e.mean = hist[i];
            vec.push_back(e);
        }
    }

    double dist=std::numeric_limits<double>::max();
    double min_dist=std::numeric_limits<double>::max();
    vector<entry>::iterator mindist_it;
    while(vec.size()>2) {
        min_dist=std::numeric_limits<double>::max();
        for (vector<entry>::iterator it1 = vec.begin(); it1<(vec.end()-1); ++it1)     {
            dist = interclass_variance(hist, *it1, *(it1+1), N)*intraclass_variance(hist, *it1, *(it1+1), N); 
            if(dist < min_dist) {
                min_dist = dist;
                mindist_it = it1;
            }
        }

        cout<<mindist_it->smallest<<":"<<mindist_it->biggest<<" - "<<(mindist_it+1)->smallest<<":"<<(mindist_it+1)->biggest<<endl;
        // Verschmelzen
        mindist_it->biggest = (mindist_it+1)->biggest;
        
        
        mindist_it->mean = mCk(hist, mindist_it->biggest, mindist_it->smallest, N);
        vec.erase(mindist_it+1);
    }

    int thres = vec[0].biggest;

    cout<<"\n\n"<<vec[0].smallest<<":"<<vec[0].biggest<<" und "<<vec[1].smallest<<":"<<vec[1].biggest<<endl;
    cout<<"FINAL THRESHOLD="<<thres<<endl;
    
    threshold(im,thresholded,thres,255,THRESH_BINARY);

    imshow( "threshold", thresholded );
    waitKey(0);

    return 0;
}