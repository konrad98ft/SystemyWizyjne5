#include<opencv2/opencv.hpp>

#define IMAGES_PATH ""

void wait(int time = 0)
{
	if (cv::waitKey(time) == 'q') exit(0);
}

int ileDziur(int index, const std::vector<cv::Vec4i>& hierarchy)
{
	int n_of_holes = 0;
	for (int i = index; i < hierarchy.size(); i++)
	{
		
		if (hierarchy[i][3] == index)n_of_holes++;
	}
	return n_of_holes;
}

double wspKolowosci(const std::vector<cv::Point>& kontur) 
{
	double L = cv::arcLength(kontur, true);
	double S = cv::contourArea(kontur);
	float K =  3.14 * 4 * S /( L * L);

	return K;
}

char zidentyfikujLitere(int index, const std::vector<cv::Vec4i>& hierarchy, const std::vector<cv::Point>& kontur)
{
	double K = wspKolowosci(kontur);
	int n = ileDziur(index, hierarchy);

	if (hierarchy[index][3] == -1) //czy to wgl litera
	{
		if (n == 2)	//na pewno B
		{
			return 'B';
		}
		else if(n == 1) // O lub D
		{
			if (K > 0.85)
				return 'O';
			else
				return 'D';
		}
		else // F lub H
		{
			if (K > 0.21)
				return 'F';
			else
				return 'H';
		}
	}
	else //to nie jest kontur litery
	{
		return '-';
	}

	
}

int main()
{
	for (int i = 1; i <= 3; ++i)
	{
		std::stringstream ss;
		ss << IMAGES_PATH << "" << i << ".png";
		cv::Mat img = cv::imread(ss.str());
		cv::imshow("Test", img);
		wait();
		cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);
		cv::imshow("Test", img);
		wait();
		cv::threshold(img, img, 250, 255, cv::THRESH_BINARY_INV);
		cv::imshow("Test", img);
		std::vector<std::vector<cv::Point>> contours;
		std::vector<cv::Vec4i> hierarchy;
		//cv::findContours(img, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
		//cv::findContours(img, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);
		cv::findContours(img, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_NONE);
		//cv::findContours(img, contours, hierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_NONE);
		//cv::findContours(img, contours, hierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE);
		//cv::findContours(img, contours, hierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_TC89_L1 );
		wait();
		std::cout << "Ilosc konturow: " << contours.size() << std::endl;
		cv::Mat tmp(img.size(), CV_8UC3);
		tmp = 0;
		cv::drawContours(tmp, contours, -1, cv::Scalar(0, 0, 255), -1, 8, hierarchy);
		cv::imshow("Test", tmp);
		wait();
		tmp = 0;
		
		for (int j = 0; j < contours.size(); ++j)
		{
			std::cout << "Kontur: " << j << (hierarchy[j][3] < 0 ? "; Rodzic" : "; Dziecko") << "; Punktow: " << contours[j].size();
			std::cout << "; Hierarchia: " << hierarchy[j] << std::endl;
			std::cout << "Obwod: " << cv::arcLength(contours[j], true) << "; Pole pow.: " << cv::contourArea(contours[j]) << std::endl;
			std::cout  << "Ilosc dziur: " << ileDziur(j, hierarchy) << std::endl;
			std::cout << "Wspolczynnik kolowosci: " << wspKolowosci(contours[j]) << std::endl;
			std::cout << "Rozpoznano kontur zewnetrzny litery: " << zidentyfikujLitere(j, hierarchy, contours[j]) << std::endl<< std::endl;
			cv::drawContours(tmp, contours, j, cv::Scalar(255, 0, 0), 1);
			cv::imshow("Test", tmp);
			wait();
		}
		wait();
	}
}