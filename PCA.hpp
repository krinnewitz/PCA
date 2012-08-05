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
 * PCA.hpp
 *
 *  @date 15.07.2012
 *  @author Kim Rinnewitz (krinnewitz@uos.de)
 */

#ifndef PCA_HPP_
#define PCA_HPP_

#include <cstring>
#include <math.h>
#include <opencv/highgui.h>
#include <opencv/cv.h>
#include <cstdio>
#include "Texture.hpp"

namespace lssr {


/**
 * @brief	This class provides principal component analysis for textures.
 */
class PCA {
public:


	/**
	* \brief Constructor. Calculates the main axis for the given Texture.
	*
	* \param	t		The texture
	*
	*/
	PCA(Texture* t);

	/**
	* \brief Constructor. Calculates the main axis for the given Texture.
	*
	* \param	t		The texture
	*
	*/
	PCA(const cv::Mat &t);

	/**
	 * \brief Compares the PCA to the given one and returns the angle between them.
	 *
 	 * \param	other	The other PCA
	 *
	 * \return	The angle between the axis of the two images
	 */
	float compareTo(PCA &other);

	/**
	 * Destructor.
	 */
	virtual ~PCA();

	/// 2 dimensional vector for fist direction
	float m_x, m_y;
private:

	void calcPCA(cv::Mat img, float &x, float &y);

};

}

#endif /* PCA_HPP_ */
