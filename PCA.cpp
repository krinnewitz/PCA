/* Copyright (C) 2011 Uni Osnabrück
 * This file is part of the LAS VEGAS Reconstruction Toolkit,
 *
 * LAS VEGAS is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * LAS VEGAS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA
 */


/*
 * PCA.cpp
 *
 *  @date 15.07.2012
 *  @author Kim Rinnewitz (krinnewitz@uos.de)
 */

#include "PCA.hpp"

namespace lssr {
PCA::PCA(Texture *t)
{
	//convert texture to cv::Mat
	cv::Mat img1(cv::Size(t->m_width, t->m_height), CV_MAKETYPE(t->m_numBytesPerChan * 8, t->m_numChannels), t->m_data);

	//make input gray scale 
	cv::Mat img;	
	cv::cvtColor(img1, img, CV_RGB2GRAY);

	calcPCA(img, this->m_x, this->m_y);
	std::cout<<"("<<this->m_x<<","<<this->m_y<<")"<<std::endl;	
	cv::PCA pca(img, cv::Mat(), CV_PCA_DATA_AS_ROW, 2);
	std::cout<<pca.eigenvectors.rows<<"x"<<pca.eigenvectors.cols<<std::endl;
	std::cout<<pca.eigenvectors.at<float>(0,0)<<"   "<<pca.eigenvectors.at<float>(1,0)<<std::endl;
}

PCA::PCA(const cv::Mat &t)
{
	//make input gray scale 
	cv::Mat img;	
	cv::cvtColor(t, img, CV_RGB2GRAY);

//-----------------------------------------------
	cv::Mat test(2, 10, CV_32FC1);
	//x
	test.at<float>(0,0) = 2.5;
	test.at<float>(0,1) = 0.5;
	test.at<float>(0,2) = 2.2;
	test.at<float>(0,3) = 1.9;
	test.at<float>(0,4) = 3.1;
	test.at<float>(0,5) = 2.3;
	test.at<float>(0,6) = 2;
	test.at<float>(0,7) = 1;
	test.at<float>(0,8) = 1.5;
	test.at<float>(0,9) = 1.1;

	//y
	test.at<float>(1,0) = 2.4;
	test.at<float>(1,1) = 0.7;
	test.at<float>(1,2) = 2.9;
	test.at<float>(1,3) = 2.2;
	test.at<float>(1,4) = 3.0;
	test.at<float>(1,5) = 2.7;
	test.at<float>(1,6) = 1.6;
	test.at<float>(1,7) = 1.1;
	test.at<float>(1,8) = 1.6;
	test.at<float>(1,9) = 0.9;
//----------------------------------------------
	cv::Mat test2(2, img.reshape(0,1).cols,CV_32FC1);
	for (int i = 0; i < img.reshape(0,1).cols; i++)
	{
		test2.at<float>(0, i) = img.reshape(0,1).at<uchar>(0, i);
		test2.at<float>(1, i) = i;
	}
//----------------------------------------------
	cv::Mat test3(3, img.cols * img.rows ,CV_32FC1);
	for (int i = 0; i < img.cols * img.rows; i++)
	{
		test3.at<float>(0, i) = i / img.cols;
		test3.at<float>(1, i) = i % img.rows;
		test3.at<float>(2, i) = img.at<uchar>(i / img.cols, i % img.cols);
	}
//-------------------------------------------------


//	calcPCA(img, this->m_x, this->m_y);

//	cv::PCA pca(img.reshape(0,1), cv::Mat(), CV_PCA_DATA_AS_COL, 2);
	cv::PCA pca(test3, cv::Mat(), CV_PCA_DATA_AS_COL, 2);

	//return the first eigenvector
	m_x = pca.eigenvectors.at<float>(0,0);
	m_y = pca.eigenvectors.at<float>(0,1);

//-----------------------Debug Output from here--------------------------------------------

	std::cout<<pca.eigenvectors.rows<<"x"<<pca.eigenvectors.cols<<std::endl;
	for (int j = 0; j<pca.eigenvectors.rows; j++)
	{
		for (int i = 0; i<pca.eigenvectors.cols; i++)
		{
			std::cout<<std::setw(14);
			std::cout<<pca.eigenvectors.at<float>(j,i);
		}
		std::cout<<std::endl;
	}
	std::cout<<pca.eigenvalues.rows<<"x"<<pca.eigenvalues.cols<<std::endl;
	for (int j = 0; j<pca.eigenvalues.rows; j++)
	{
		for (int i = 0; i<pca.eigenvalues.cols; i++)
		{
			std::cout<<std::setw(14);
			std::cout<<pca.eigenvalues.at<float>(j,i);
		}
		std::cout<<std::endl;
	}
	
/*		float a = 1000000;	
		cv::line(img, cv::Point(img.cols/2,img.rows/2 ), cv::Point(a*m_x+img.cols/2, a*m_y+img.rows/2), cv::Scalar(0,0,0), 2);
		cv::startWindowThread();
		
		//show the single pattern
		cv::namedWindow("Pattern", CV_WINDOW_AUTOSIZE);
		cv::imshow("Pattern", img);
		cv::waitKey();
*/
//	std::cout<<"("<<this->m_x<<","<<this->m_y<<")"<<std::endl;	
}

float PCA::compareTo(PCA & other)
{
	float dotProduct = this->m_x * other.m_x + this->m_y * other.m_y;
	float normThis 	= sqrt(this->m_x * this->m_x + this->m_y * this->m_y);
	float normOther = sqrt(other.m_x * other.m_x + other.m_y * other.m_y);
	return acos(dotProduct / (normThis * normOther));
}

void PCA::calcPCA(cv::Mat isrc, float &x, float &y)
{
	//make one row vector from Matrix
	cv::Mat img = isrc.reshape(0, 1);

	float k11 = 0, k12 = 0, k21 = 0, k22 = 0;
	
	int N =  img.cols; 
	float x_mean = (1.0f / N) * (N * (N - 1) / 2);
	float y_mean = cv::mean(img)[0];
	for (int i = 0; i < N; i++)
	{
		k11 += (i - x_mean) * (i - x_mean) / N;
		k22 += (img.at<uchar>(0, i) - y_mean) * (img.at<uchar>(0, i) - y_mean) / N;
		k12 += (i - x_mean) * (img.at<uchar>(0, i) - y_mean) / N;
	}
	k21 = k12;
	std::cout<<k11<<" "<<k21<<" "<<k12<<" "<<k22<<std::endl;
	
	//set covariance matrix
	cv::Mat C(2, 2, CV_32FC1);
	C.at<float>(0,0) = k11;
	C.at<float>(1,0) = k21;
	C.at<float>(0,1) = k12;
	C.at<float>(1,1) = k22;
	
	//get the eigenvectors of C
	cv::Mat E, V;
	cv::eigen(C, E, V);

	//return the first eigenvector
	x = V.at<float>(0,0);
	y = V.at<float>(1,0);
	std::cout<<V.at<float>(0,0)<<" "<<V.at<float>(0,1)<<" "<<V.at<float>(1,0)<<" "<<V.at<float>(1,1)<<std::endl;
/*	
		float t = 10000;	
		cv::line(isrc, cv::Point(0,0), cv::Point(t*x, t*y), cv::Scalar(0,0,0), 2);
		cv::startWindowThread();
		
		//show the single pattern
		cv::namedWindow("Pattern", CV_WINDOW_AUTOSIZE);
		cv::imshow("Pattern", isrc);
		cv::waitKey();
*/
}

PCA::~PCA()
{
	//TODO?!
}
}
