#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>

void invariant(cv::Mat& b, int anz)
{
	double g1 = sqrt(b.at<cv::Vec2d>(1, 0)[0] * b.at<cv::Vec2d>(1, 0)[0] + b.at<cv::Vec2d>(1, 0)[1] * b.at<cv::Vec2d>(1, 0)[1]);
	for (int j = 2; j < anz; j++)
	{
		//Koeffizienten 2 bis anz-1 (0-ter ist Schwerpunkt, 1-ter wird zum Skalieren verwendet)
		b.at<cv::Vec2d>(j, 0)[0] = b.at<cv::Vec2d>(j, 0)[0] / g1;
		b.at<cv::Vec2d>(j, 0)[1] = b.at<cv::Vec2d>(j, 0)[1] / g1;
		std::cout << sqrt(b.at<cv::Vec2d>(j, 0)[0] * b.at<cv::Vec2d>(j, 0)[0] + b.at<cv::Vec2d>(j, 0)[1] * b.at<cv::Vec2d>(j, 0)[1]) << std::endl;
	}
	
	
}

int main(int argc, char *argv[])
{	
	cv::Mat bild=cv::imread("../bilder/Fische.png",0); //fische.bmp
	cv::namedWindow("Fische", 0);
	cv::resizeWindow("Fische", bild.cols / 2, bild.rows / 2);
	cv::imshow("Fische", bild );
	cv::Mat colorimg;
	cv::cvtColor(bild, colorimg, cv::COLOR_GRAY2BGR);	
	std::vector<std::vector<cv::Point>> cont;
	cv::findContours( bild, cont, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);
		
	for(int i=0; i<cont.size();i++)
    {   
		cv::Mat g(cont[i].size(), 1, CV_64FC2, cv::Scalar(0) );
		cv::Mat a=g.clone();
	    for (int j=0; j<cont[i].size();j++)
		{
			g.at<cv::Vec2d>(j,0)[0] = cont[i][j].x;
			g.at<cv::Vec2d>(j,0)[1] = cont[i][j].y;
		}	
		cv::dft(g, g, cv::DFT_SCALE , 0);

		//Visualisierung der Form
		int anz = 5;	
		for (int j=0; j < anz; j++)
		{
			a.at<cv::Vec2d>(j,0)=g.at<cv::Vec2d>(j,0);
		}
		for (int j=cont[i].size()-1; j > cont[i].size()-anz-1; j--)
		{
			a.at<cv::Vec2d>(j,0)=g.at<cv::Vec2d>(j,0);
		}
		cv::dft(a, a, cv::DFT_INVERSE, 0);
		for (int j = 0; j < cont[i].size() - 1; j++)
		{
			cv::line(colorimg, cv::Point((int)a.at<cv::Vec2d>(j, 0)[0], (int)a.at<cv::Vec2d>(j, 0)[1]),
				cv::Point((int)a.at<cv::Vec2d>(j + 1, 0)[0], (int)a.at<cv::Vec2d>(j + 1, 0)[1]), cv::Scalar(0, 0, 255), 2);
		}
		cv::imshow("Fische", colorimg);
		//Berechnung der Deskriptoren
		cv::Mat b = g.clone();
		std::cout << "Objekt " << i << std::endl;
		invariant(b, anz);
		cv::waitKey(0);
	}
	cv::waitKey(0);
}
