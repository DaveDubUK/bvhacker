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
 
#pragma once

#include <gl/gl.h>
#include <gl/glu.h>
#include <vector>

using namespace std;

#define N_PI 3.1415926535897932384626433832795
#define N_PI_BY_TWO 1.5707963267948966192313216916398
#define N_TWO_PI 6.283185307179586476925286766559

//            x    y   z      x  y   z    x  y  z
//([x,y,z]=[(256||128||64), (32||16||8), (4||2||1)]] 
enum ChannelOrder{
	XYZ = 273,
	XZY = 266,
	YXZ = 161,
	YZX = 140,
	ZXY = 98,
	ZYX = 84,
	NONE = 0
};

enum Coords{
	N_X = 0,
	N_Y = 1,
	N_Z = 2,
	N_XY = 3,
	N_XZ = 4,
	N_YZ = 5,
	N_XYZ = 6
};

enum LeftOrRight{
	NEITHER = 1,
	LEFT = 1,
	RIGHT = -1
};

// arm alignment
#define UNKNOWN_ALIGNMENT 0
#define X_AXIS_ALIGNED_ARMS 1
#define Y_AXIS_ALIGNED_ARMS 2

// status
#define FILE_LOADED 1
#define FILE_PLAYING 2
#define FILE_JUST_LOADED 4
#define MODE_TRANSLATING 8
#define MODE_ROTATING 16
#define EDITING_PAUSE 32

// 
#define TRANSLATION 1
#define ROTATION 2
#define SCALE 3

typedef struct t_BVHJoint {
	CString JOINTName;
	CString internalName;
	int heirarchy;
	int parentIndex;
	int index;
	bool isROOT;
	bool isEndSite;
	int nChannels;
	int channelOrder;				//(Rotation channel order, ZXY || XZY whatever)
	int offsetChannelOrder;			// 
	int scaleChannelOrder;			//
	vector<float> offsetChannels;	// offsets from parent
	vector<float> rotationChannels; // last ones used
	vector<float> scaleChannels;	// for changing figure height (TBI)
	GLfloat fTransformMatrix[16]; 
	int nMotionDataIndex;			// Rotation data index
	int nOffsetMotionDataIndex;		// bone offset data index
	int nScaleMotionDataIndex;		// scale data index
	CString CHANNELSLine;
}BVHJoint;

typedef struct t_BVHMotion {
	vector<GLfloat> data;
}BVHMotion;
