#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

int main(int argc, char *argv[]) {
    Mat orig_fische = imread("../bilder/Fische.png",0);
    vector<vector<Point> > contoursQuery;
    vector<Point> newcontour;
    vector<vector<Point>> contours;
    Mat col_fische;
    findContours(orig_fische, contoursQuery, RETR_LIST, CHAIN_APPROX_NONE);
    cvtColor(orig_fische, col_fische, COLOR_GRAY2BGR);


    for(int i = 0; i<contoursQuery.size(); i++) {
        Mat g(contoursQuery[i].size(), 1, CV_64FC2, cv::Scalar(0) );
		Mat a=g.clone();
	    for (int j=0; j<contoursQuery[i].size();j++)
		{
			g.at<Vec2d>(j,0)[0] = contoursQuery[i][j].x;
			g.at<Vec2d>(j,0)[1] = contoursQuery[i][j].y;
		}
        // transform to f	
		dft(g, g, DFT_SCALE , 0);

        for(int j = (contoursQuery[i].size()/2); j < contoursQuery[i].size()-5; j++) {
            g.at<Vec2d>(j,0)[0] = 0;
            g.at<Vec2d>(j,0)[1] = 0;
        }

        dft(g, g, DFT_INVERSE, 0);


        //for(int j=0; j<contoursQuery[i].size();j++) {
        //    newcontour.push_back(Point(g.at<Vec2d>(j,0)[0], g.at<Vec2d>(j,0)[1]));
        //}
        for (int j = 0; j < contoursQuery[i].size() - 1; j++) {
			cv::line(col_fische, cv::Point((int)g.at<Vec2d>(j,0)[0], (int)g.at<Vec2d>(j,0)[1]),
				cv::Point((int)g.at<Vec2d>(j+1,0)[0], (int)g.at<Vec2d>(j+1,0)[1]), cv::Scalar(0, 0, 255), 2);
		}

        //contours.push_back(newcontour);

    }
    //for(auto i : contours) {
    //    cout<<i.size()<<endl;
    //}
    //drawContours(col_fische, contours, -1, Scalar((255,0,0)));
    imshow( "image", col_fische );
    waitKey(0);

}