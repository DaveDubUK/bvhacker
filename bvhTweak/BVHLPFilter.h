/*
	bvhacker - a program for animation previewing and editing

	bvhacker Copyright © 2006 to 2014 David Wooldridge  

		dave@davedub.co.uk
		http://davedub.co.uk/

	This file is part of bvhacker.

    bvhacker is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    bvhacker is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with bvhacker.  If not, see <http://www.gnu.org/licenses/>.

*/
 
//#include <math.h>
#include "math.h"
#include "common.h"

#pragma once

class CBVHLPFilter
{
public:
	CBVHLPFilter();
	~CBVHLPFilter(void);

	//main function to do filtering
	void Filtering(vector<float>* data);

private:

	float PI;

	//number of taps
	int m_tap;
	//Cut-of frequency
	int m_fc;
	//Sampling frequency
	int m_fs;
	//Nyquist Frequency
	double m_nqFreq;
	//Array of coefficients
	double *p_mcoeff;
	//Final size of coeff array
	int m_coeffLen;
	//Vector of passed data
	vector<float>* m_data;

	//Calculate coefficients
	void CalCoeff();
	//Apply windowing function to coefficients
	void Windowing();
	// Convolve arrays to apply filter
	void convolve();
};
