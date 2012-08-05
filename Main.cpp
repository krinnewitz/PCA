#include <stdlib.h>
#include "PCA.hpp"
#include <stdio.h>
#include <math.h>
#include <iostream>

/**
 * \file	Main.cpp
 * \brief 	This is an implementation of principal component 
 *		analysis  for images. 
 *
 * \author	Kim Oliver Rinnewitz, krinnewitz@uos.de
 */

using namespace std;


int main (int argc, char** argv)
{

	if (argc != 2 && argc != 3)
	{
		cout<<"Usage: "<<argv[0]<<" <filename1> [<filename2>]"<<endl;
		return EXIT_FAILURE;	
	}
	else
	{
		cv::Mat src1 = cv::imread(argv[1]);
		lssr::PCA* p1 = new lssr::PCA(src1);
		cv::Mat src2 = cv::imread(argv[2]);
		lssr::PCA* p2 = new lssr::PCA(src2);
		cout<<p1->compareTo(*p2)<<endl;

		return EXIT_SUCCESS;
	}

}
