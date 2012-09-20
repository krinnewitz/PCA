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

	//calculate the main axis
	calcPCA(img, this->m_x, this->m_y, this->m_z);
}

PCA::PCA(const cv::Mat &t)
{
	//make input gray scale 
	cv::Mat img;	
	cv::cvtColor(t, img, CV_RGB2GRAY);

	//calculate the main axis
	calcPCA(img, this->m_x, this->m_y, this->m_z);
}

float PCA::compareTo(PCA & other)
{
	float dotProduct = this->m_x * other.m_x + this->m_y * other.m_y + this->m_z * other.m_z;
	float normThis 	= sqrt(this->m_x * this->m_x + this->m_y * this->m_y + this->m_z * this->m_z);
	float normOther = sqrt(other.m_x * other.m_x + other.m_y * other.m_y + other.m_z * other.m_z);
	return acos(dotProduct / (normThis * normOther));
}

void PCA::calcPCA(cv::Mat isrc, float &x, float &y, float &z)
{
	//build input vectors: First line = x values, second line = y values, third line = intensity
	cv::Mat img(3, isrc.cols * isrc.rows ,CV_32FC1);
	for (int i = 0; i < isrc.cols * isrc.rows; i++)
	{
		img.at<float>(0, i) = i % isrc.rows;
		img.at<float>(1, i) = i / isrc.cols;
		img.at<float>(2, i) = isrc.at<uchar>(i / isrc.cols, i % isrc.cols);
	}

	//calculate PCA	
	cv::PCA pca(img, cv::Mat(), CV_PCA_DATA_AS_COL, 2);
	//return the first eigenvector
	x = pca.eigenvectors.at<float>(0,0);
	y = pca.eigenvectors.at<float>(0,1);
	z = pca.eigenvectors.at<float>(0,2);
}

PCA::~PCA()
{
}
}
