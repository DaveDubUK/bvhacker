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

#include "common.h"
//#include "math\quaternion.h"
#include <math.h>
#include "vmath.h"
#include "BVHTweaker.h"

class CBVHQuaterniser
{
	friend class CBVHFile;


public:
	CBVHQuaterniser(CBVHFile* pBVHFile, CBVHTweaker* pBVHTweaker);
	~CBVHQuaterniser(void);

	// rotation order and arms alignment stuff
	void SetArmsAlignment(int nArmsAlignemnt);// works, but sets joint rotation order to ZYX from ZXY
	void SetArmsRotationOrder(int nRotationOrder);// sets joint rotation order to ZXY on all arms joints
	void SetRotationOrder(int nRotationOrder);// set all joints to specified order

	// Global rotation stuff 
	bool RotateGlobal(BVHJoint* pJoint, Vector3<float> vRotation);
	Vector3<float> GetCurrentGlobalRotation(BVHJoint* pJoint);

private:
	CBVHFile* m_pBVHFile;
	CBVHTweaker* m_pBVHTweaker;
	
	Vector3<float> QuatToRot(Quaternion<float> &qOriginal);// uses degrees?
	Vector3<float> QuatToRot(Quaternion<float> &qOriginal, int nRotationOrder);// interpolates new rot order (Noel Huges algorithm)
	float NormaliseToQuadrant(float fValue);
	Vector3<float> GetAverageRotation(BVHJoint* npJoint);

	void AlignShoulder(BVHJoint* pJoint, int nWayRound);
	void SetRotationOrderToZXY(BVHJoint* pJoint, int nWayRound);
	void SetRotationOrderToYZX(BVHJoint* pJoint, int nWayRound);

	// constants	
	float DEGTORAD;
	float RADTODEG;
	Vector3<float> UNIT_X_ZXY;
	Vector3<float> UNIT_Y_ZXY;
	Vector3<float> UNIT_Z_ZXY;

	Vector3<float> UNIT_X_YZX;
	Vector3<float> UNIT_Y_YZX;
	Vector3<float> UNIT_Z_YZX;
	//XYZ
	Vector3<float> UNIT_X_XYZ;
	Vector3<float> UNIT_Y_XYZ;
	Vector3<float> UNIT_Z_XYZ;
	// XZY
	Vector3<float> UNIT_X_XZY;
	Vector3<float> UNIT_Y_XZY;
	Vector3<float> UNIT_Z_XZY;


	// debug fns
	void ShowMatrix(CString title, Matrix4<float>* m4);

	/* round number n to d decimal points */
	inline float fround(float n, int d)
	{
		return floor(n * pow(10.f, d) + .5f) / pow(10.f, d);
	}
};
