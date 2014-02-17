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
 
#include "StdAfx.h"
#include "BVHLPFilter.h"

CBVHLPFilter::CBVHLPFilter(void)
{
	PI = (float)3.1415926536;
}

CBVHLPFilter::~CBVHLPFilter(void)
{
}

//Calculate FIR coefficeints.
void CBVHLPFilter::CalCoeff()
{
	// TODO work these out using file properties
	m_tap = 8;
	m_fc = (int)(2 * PI * 29);
	m_fs = (int)(2 * PI * 60);


	//Allocate memory for coefficients
	p_mcoeff = new double[m_tap];

	//Calculate nyquist frequency (in RAD/s, but don't look right to me!)
	m_nqFreq = (2 * PI * m_fc)/m_fs;

	int i=1,count;

	//To keep our filter causal, shift the centre of it to the middle of taps.
	if(m_tap%2 == 0)
	{
		count = (m_tap - 1)>>1;
	}
	else
	{
		count = m_tap >> 1;
	}

	//Calculate centre value of coefficient.
	p_mcoeff[count] = m_nqFreq/PI;

	//Calculate remaining values of coefficient.
	while(i<=count)
	{
		p_mcoeff[count-i]=sin(i*m_nqFreq)/PI/i;
		p_mcoeff[count+i]=p_mcoeff[count-i];
		i++;
	}
}

//Apply hanning window function to coefficients.
void CBVHLPFilter::Windowing()
{
	for(int i = 0 ; i < m_tap ; i++)
	{
		double win_val = 0.5 - 0.5 * cos( 2*PI*(i+ 1)/(m_tap+1));
		p_mcoeff[i] *= win_val;
	}
}

//Convolution of two arrays
void CBVHLPFilter::convolve()
{
	//Calculate the length of output samples.
	int outLen = (int)m_data->size() + m_coeffLen -1;
	int offset = (m_coeffLen -1)/2;

	vector<float> output;

	for(int i=0;i<=outLen;i++)
	{
		output.push_back(0);
		for(int j=0;j<m_coeffLen;j++)
		{
			float coefficient = (float)p_mcoeff[j];

			int stop = 0;
			
			if(((i-j)>=0) && ((i-j)<(int)m_data->size()))
			{
				float inval = m_data->at(i-j);
				float outval = inval * coefficient;
				output.at(i)+=outval;

				float dbgval = output.at(i);

				//output.at(i)+=m_data->at(i-j)*(float)p_mcoeff[j];
			}
		}
	}

	// copy output over m_data
	for(int i=0;i<(int)m_data->size();i++)
	{
		m_data->at(i) = output.at(i+offset);
	}

	int stophere = 0;
}



//Do filtering
void CBVHLPFilter::Filtering(vector<float>* data)
{
	//Set the data
	m_data = data;

	//Calculate coefficients
	CalCoeff();

	// apply windowing
	//Windowing();

	// convolve arrays to apply filter
	convolve();
}
