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


/*

Trajce Nikolov wrote:
Hi,

does anyone knows what this matrix should look like ?

In row major form, the general orientation matrix is:

Xx  Xy  Xz  0
Yx  Yy  Yz  0
Zx  Zy  Zz  0
0   0   0   1

...where X is the new x axis, Y the new y axis, and Z the new z axis.

Matrices don't really know the meaning of the word "up"; 
that's a subjective term we use. So when you ask for a matrix that converts y up to z up, 
you're really asking for a matrix that replaces the y axis with the z axis. Here's one matrix that does that:

1  0  0  0
0  0  1  0
0  1  0  0
0  0  0  1

There are other ways to make a "y up to z up" matrix, 
that's just one way. The way you actually choose to compose this matrix will depend on 
what you want for the x and z axes in the new coordinate system. In the matrix above, 
I just swapped y and z, and left x unchanged.

An alternative would be to negate the new z axis:

1  0  0  0
0  0  1  0
0 -1  0  0
0  0  0  1

Also: http://forums.cgsociety.org/archive/index.php/t-608589.html


Useful quats:
w	 x	 y	 z	 Description
1	 0	 0	 0	 Identity quaternion, no rotation
0	 1	 0	 0	 180' turn around X axis
0	 0	 1	 0	 180' turn around Y axis
0	 0	 0	 1	 180' turn around Z axis

w            x         y         z          Description
sqrt(0.5)	 sqrt(0.5) 0	     0			90' rotation around X axis
sqrt(0.5)	 0	       sqrt(0.5) 0			90' rotation around Y axis
sqrt(0.5)	 0	       0	     sqrt(0.5)	90' rotation around Z axis
sqrt(0.5)	 -sqrt(0.5)0	     0			-90' rotation around X axis
sqrt(0.5)	 0	       -sqrt(0.5)0			-90' rotation around Y axis
sqrt(0.5)	 0	       0	    -sqrt(0.5)	-90' rotation around Z axis


http://zd.0x209.org/docs/ogre3d/q/u/a/Quaternion_and_Rotation_Primer_bc1e.html

*/


#include "StdAfx.h"
#include "BVHQuaterniser.h"
#include "BVHFile.h"

//
// ctor etc
//
CBVHQuaterniser::CBVHQuaterniser(CBVHFile* pBVHFile, CBVHTweaker* pBVHTweaker)
{
	m_pBVHFile = pBVHFile;
	m_pBVHTweaker = pBVHTweaker;

	// probably not the best way to set up constants, but works
	// probably an un-intuitive way, but it works ;-)
	DEGTORAD = (float)N_PI/180.f;
	RADTODEG = 180.f/(float)N_PI;
	// X
	// Y
	// Z
	UNIT_X_XYZ.x = 1.f;UNIT_X_XYZ.y = 0.f;UNIT_X_XYZ.z = 0.f;//UNIT_X_XYZ.normalize();
	UNIT_Y_XYZ.x = 0.f;UNIT_Y_XYZ.y = 1.f;UNIT_Y_XYZ.z = 0.f;//UNIT_Y_XYZ.normalize();
	UNIT_Z_XYZ.x = 0.f;UNIT_Z_XYZ.y = 0.f;UNIT_Z_XYZ.z = 1.f;//UNIT_Z_ZXY.normalize();
	// X -> Z
	// Y -> X
	// Z -> Y
	UNIT_X_ZXY.x = 0.f;UNIT_X_ZXY.y = 0.f;UNIT_X_ZXY.z = 1.f;//UNIT_X_ZXY.normalize();
	UNIT_Y_ZXY.x = 1.f;UNIT_Y_ZXY.y = 0.f;UNIT_Y_ZXY.z = 0.f;//UNIT_Y_ZXY.normalize();
	UNIT_Z_ZXY.x = 0.f;UNIT_Z_ZXY.y = 1.f;UNIT_Z_ZXY.z = 0.f;//UNIT_Z_ZXY.normalize();
	// X -> Y
	// Y -> Z
	// Z -> X
	UNIT_X_YZX.x = 0.f;UNIT_X_YZX.y = 1.f;UNIT_X_YZX.z = 0.f;//UNIT_X_YZX.normalize();
	UNIT_Y_YZX.x = 0.f;UNIT_Y_YZX.y = 0.f;UNIT_Y_YZX.z = 1.f;//UNIT_Y_YZX.normalize();
	UNIT_Z_YZX.x = 1.f;UNIT_Z_YZX.y = 0.f;UNIT_Z_YZX.z = 0.f;//UNIT_Z_ZXY.normalize();

	UNIT_Z_XZY.x = 1.f;UNIT_X_XZY.y = 0.f;UNIT_X_XZY.z = 0.f;//UNIT_X_XZY.normalize();
	UNIT_Y_XZY.x = 0.f;UNIT_Y_XZY.y = 0.f;UNIT_Y_XZY.z = 1.f;//UNIT_Y_XZY.normalize();
	UNIT_Z_XZY.x = 0.f;UNIT_Z_XZY.y = 1.f;UNIT_Z_XZY.z = 0.f;//UNIT_Z_XZY.normalize();

}

CBVHQuaterniser::~CBVHQuaterniser(void)
{
	m_pBVHFile = NULL;
	m_pBVHTweaker = NULL;
}

//
// changes the arms alignment of a ZXY rotation order joint to X_AXIS_ALIGNED
// but also changes the rotation order to ZYX...
//
void CBVHQuaterniser::SetArmsAlignment(int nArmsAlignemnt)
{
	if(m_pBVHFile->CalculateArmsAxisAlignment()==X_AXIS_ALIGNED_ARMS)
	{
		//AfxMessageBox(_T("The figure's arms are already aligned along the X axis"),MB_ICONEXCLAMATION);
		return;
	}

	vector<BVHJoint>::iterator jointItr = m_pBVHFile->m_joints.begin();
	CString szJointName = _T("");

	while(jointItr!=m_pBVHFile->m_joints.end()) {

		szJointName = jointItr->JOINTName.GetBuffer();


		// left arm
		if(szJointName== _T("lShldr"))
		{
			AlignShoulder(&(*jointItr), LEFT);
			m_pBVHTweaker->MotionDataInvertChannel(jointItr->nMotionDataIndex+2); // invert (new) X
			jointItr->channelOrder=ZYX;
			jointItr->CHANNELSLine.Replace("Zrotation Xrotation Yrotation", "Zrotation Yrotation Xrotation");
		}

		if(szJointName== _T("lForeArm"))
		{
			// swap x and y offsets, invert Y and set new channel order 
			float temp = jointItr->offsetChannels[1];
			jointItr->offsetChannels[1] = jointItr->offsetChannels[2];
			jointItr->offsetChannels[2] = -temp;
			jointItr->channelOrder=ZYX;
			jointItr->CHANNELSLine.Replace("Zrotation Xrotation Yrotation", "Zrotation Yrotation Xrotation");
			m_pBVHTweaker->MotionDataInvertChannel(jointItr->nMotionDataIndex+2); // invert X
		}

		if(szJointName== _T("lHand"))
		{			
			// swap x and y offsets, invert Y and set new channel order 
			float temp = jointItr->offsetChannels[1];
			jointItr->offsetChannels[1] = jointItr->offsetChannels[2];
			jointItr->offsetChannels[2] = -temp;
			jointItr->channelOrder=ZYX;
			jointItr->CHANNELSLine.Replace("Zrotation Xrotation Yrotation", "Zrotation Yrotation Xrotation");
			m_pBVHTweaker->MotionDataInvertChannel(jointItr->nMotionDataIndex+2); // invert X

			// End Site - swap x and y offsets
			jointItr++;
			temp = jointItr->offsetChannels[1];
			jointItr->offsetChannels[1] = jointItr->offsetChannels[2];
			jointItr->offsetChannels[2] = -temp;
			jointItr--;
		}

		// right arm
		if(szJointName == _T("rShldr"))
		{
			AlignShoulder(&(*jointItr), RIGHT);
			m_pBVHTweaker->MotionDataInvertChannel(jointItr->nMotionDataIndex+1); // invert Y
			jointItr->channelOrder=ZYX;
			jointItr->CHANNELSLine.Replace("Zrotation Xrotation Yrotation", "Zrotation Yrotation Xrotation");
		}

		if(szJointName == _T("rForeArm"))
		{
			// swap x and y offsets, invert Y and set new channel order 
			float temp = jointItr->offsetChannels[1];
			jointItr->offsetChannels[1] = jointItr->offsetChannels[2];
			jointItr->offsetChannels[2] = temp;
			jointItr->channelOrder=ZYX;
			jointItr->CHANNELSLine.Replace("Zrotation Xrotation Yrotation", "Zrotation Yrotation Xrotation");
			m_pBVHTweaker->MotionDataInvertChannel(jointItr->nMotionDataIndex+1); // invert Y
		}

		if(szJointName == _T("rHand"))
		{
			// swap x and y offsets, invert Y and set new channel order 
			float temp = jointItr->offsetChannels[1];
			jointItr->offsetChannels[1] = jointItr->offsetChannels[2];
			jointItr->offsetChannels[2] = temp;
			jointItr->channelOrder=ZYX;
			jointItr->CHANNELSLine.Replace("Zrotation Xrotation Yrotation", "Zrotation Yrotation Xrotation");
			m_pBVHTweaker->MotionDataInvertChannel(jointItr->nMotionDataIndex+1); // invert Y


			// End Site - swap x and y offsets
			jointItr++;
			temp = jointItr->offsetChannels[1];
			jointItr->offsetChannels[1] = jointItr->offsetChannels[2];
			jointItr->offsetChannels[2] = temp;
			jointItr--;
		}

		jointItr++;
	}
	// trouble is, all joint rotation orders are now set to ZYX.
	// to fix this, call SetArmsRotationOrder(ZXY);
	m_pBVHFile->CalculateArmsAxisAlignment();
}


//
// mainly just adds 90 degrees to the shoulder bone's z value
//
void CBVHQuaterniser::AlignShoulder(BVHJoint* pJoint, int nWayRound)
{
	float fDataVector[3];// used to store the rotation values for each frame

	vector<BVHMotion>::iterator motionItr = m_pBVHFile->m_motion.begin();
	while(motionItr!=m_pBVHFile->m_motion.end()) 
	{
		// get the parent's rotation values
		switch(pJoint->channelOrder)
		{
			case(ZXY):
			{
				// order checked and OK
				fDataVector[0] = motionItr->data[pJoint->nMotionDataIndex+1];
				fDataVector[1] = motionItr->data[pJoint->nMotionDataIndex+2];
				fDataVector[2] = motionItr->data[pJoint->nMotionDataIndex+0];
				break;
			}
			case(ZYX):
			{
				// order has already been changed - not good!
				fDataVector[0] = motionItr->data[pJoint->nMotionDataIndex+2];
				fDataVector[1] = motionItr->data[pJoint->nMotionDataIndex+1];
				fDataVector[2] = motionItr->data[pJoint->nMotionDataIndex+0];
				//__asm int 3;
				break;
			}
			default:
			{
				//__asm int 3;
			}
		}

		// create a quaternion representing the joint's orientation
		Quaternion<float> qOriginalAxis(1.f,0.f,0.f,0.f);

		float fAdjust = 0.f;
		if(nWayRound==LEFT) fAdjust =  -90.f;
		if(nWayRound==RIGHT) fAdjust = 90.f;

		// create a quaternion rotation for each of the Z, X and Y values
		Quaternion<float> qZ = Quaternion<float>::fromAxisRot(UNIT_Z_ZXY,fDataVector[2]+fAdjust);// add adjustment
		Quaternion<float> qX = Quaternion<float>::fromAxisRot(UNIT_X_ZXY,fDataVector[0]);
		Quaternion<float> qY = Quaternion<float>::fromAxisRot(UNIT_Y_ZXY,fDataVector[1]);

		// apply the motion data rotations (maintain ZXY order)
		qOriginalAxis*=qZ;
		qOriginalAxis*=qX;
		qOriginalAxis*=qY;

		// overwrite the existing rotation data 
		Vector3<float> vFinal = QuatToRot(qOriginalAxis);
		float fXfinal = vFinal.x;
		float fYfinal = vFinal.y;
		float fZfinal = vFinal.z;
		motionItr->data[pJoint->nMotionDataIndex+0]= fZfinal;
		motionItr->data[pJoint->nMotionDataIndex+1]= fXfinal;
		motionItr->data[pJoint->nMotionDataIndex+2]= fYfinal;

		//next!
		motionItr++;
	}
}

//
// set the rotation order for all arm joints (after setting arms axis alignment)
//
void CBVHQuaterniser::SetArmsRotationOrder(int nRotationOrder)
{
	vector<BVHJoint>::iterator jointItr = m_pBVHFile->m_joints.begin();
	CString szJointName = _T("");

	while(jointItr!=m_pBVHFile->m_joints.end()) {

		szJointName = jointItr->JOINTName.GetBuffer();

		// left arm
		if(szJointName== _T("lShldr")&&jointItr->channelOrder==ZYX) SetRotationOrderToZXY(&(*jointItr), LEFT);
		if(szJointName== _T("lForeArm")&&jointItr->channelOrder==ZYX) SetRotationOrderToZXY(&(*jointItr), LEFT);
		if(szJointName== _T("lHand")&&jointItr->channelOrder==ZYX) SetRotationOrderToZXY(&(*jointItr), LEFT);

		// right arm
		if(szJointName == _T("rShldr")&&jointItr->channelOrder==ZYX) SetRotationOrderToZXY(&(*jointItr), RIGHT);
		if(szJointName == _T("rForeArm")&&jointItr->channelOrder==ZYX) SetRotationOrderToZXY(&(*jointItr), RIGHT);
		if(szJointName == _T("rHand")&&jointItr->channelOrder==ZYX) SetRotationOrderToZXY(&(*jointItr), RIGHT);

		jointItr++;
	}
}
//
// interpolates new xyz rotations for a specified rot order
//
void CBVHQuaterniser::SetRotationOrderToZXY(BVHJoint* pJoint, int nWayRound)
{
	// used to store the rotation values for each frame
	Vector3<float> vDataVector(0.f,0.f,0.f);

	// local unit vectors (these ones are correct!)
	Vector3<float> unit_X(1.f,0.f,0.f);//
	Vector3<float> unit_Y(0.f,1.f,0.f);//
	Vector3<float> unit_Z(0.f,0.f,1.f);//

	// normalisers for moving calculations into the correct quadrant / hemisphere
	float fXnormaliser = 0.f;
	float fYnormaliser = 0.f;
	float fZnormaliser = 0.f;

	// debug stuff
	int nFrameNumber = 1.f;

	// now go through the motion data for each and every joint 
	vector<BVHMotion>::iterator motionItr = m_pBVHFile->m_motion.begin();
	while(motionItr!=m_pBVHFile->m_motion.end()) 
	{
		// get the parent's rotation values
		switch(pJoint->channelOrder)
		{
			case(ZXY):
			{
				return;
			}
			case(ZYX):
			{
				// get rotations for current joint on the current line of motion data
				vDataVector.x = motionItr->data[pJoint->nMotionDataIndex+2];
				vDataVector.y = motionItr->data[pJoint->nMotionDataIndex+1];
				vDataVector.z = motionItr->data[pJoint->nMotionDataIndex+0]; 

				// hack hack - can't deal with rotations in the 2nd hemisphere
				float fRotMax = 0.f;
				int nOverRotateCount = 0;

				if(abs(vDataVector.x)>fRotMax)nOverRotateCount++;
				if(abs(vDataVector.y)>fRotMax)nOverRotateCount++;
				if(abs(vDataVector.z)>fRotMax)nOverRotateCount++;

				if(nOverRotateCount==1) fRotMax=110.f;
				if(nOverRotateCount>1)  fRotMax=89.9f;

				if(vDataVector.x>fRotMax)vDataVector.x=fRotMax;
				if(vDataVector.y>fRotMax)vDataVector.y=fRotMax;
				if(vDataVector.z>fRotMax)vDataVector.z=fRotMax;
				if(vDataVector.x<-fRotMax)vDataVector.x=-fRotMax;
				if(vDataVector.y<-fRotMax)vDataVector.y=-fRotMax;
				if(vDataVector.z<-fRotMax)vDataVector.z=-fRotMax;


				// normalise to the hemisphere in which the algorithm works (+-90 degrees)
				fXnormaliser=NormaliseToQuadrant(vDataVector.x);
				fYnormaliser=NormaliseToQuadrant(vDataVector.y);
				fZnormaliser=NormaliseToQuadrant(vDataVector.z);

				// angles to add to the ZYX verion to offset it into the 'safe' hemisphere
				Quaternion<float> qZNormaliser = Quaternion<float>::fromAxisRot(unit_Z,fZnormaliser);
				Quaternion<float> qXNormaliser = Quaternion<float>::fromAxisRot(unit_X,fXnormaliser);
				Quaternion<float> qYNormaliser = Quaternion<float>::fromAxisRot(unit_Y,fYnormaliser);

				// angles to subtract from the ZXY verson 
				// (need weird UNIT vectors for un-documented XYZ to ZXY conversion using QuatToRot(qFinal);
				Quaternion<float> qDeNormaliser(1.f,0.f,0.f,0.f);

				// create a 'empty' quaternion for holding the joint's orientation
				Quaternion<float> qOriginalAxis(1.f,0.f,0.f,0.f);

				// this is tested and works (except -ve 60 degree problem)
				//Quaternion<float> qOriginalAxis(0.723317f,0.360423f,0.439679f,0.391904f);// = QG should give 30,60,45

				// create a quaternion rotation for each of the Z, X and Y values
				Quaternion<float> qZ = Quaternion<float>::fromAxisRot(unit_Z,vDataVector.z);//+fZnormaliser);
				Quaternion<float> qY = Quaternion<float>::fromAxisRot(unit_Y,vDataVector.y);//+fYnormaliser);	
				Quaternion<float> qX = Quaternion<float>::fromAxisRot(unit_X,vDataVector.x);//+fXnormaliser);
				

				// apply the rotations and the normaliser
				qOriginalAxis*=qZNormaliser;
				qOriginalAxis*=qYNormaliser;
				qOriginalAxis*=qXNormaliser;
				qOriginalAxis*=qZ;
				qOriginalAxis*=qY;
				qOriginalAxis*=qX;
				qOriginalAxis.normalize();

				//
				// calculate rotations for new rotation order using N.Huges algorithm
				//
				Vector3<float> vRotated = QuatToRot(qOriginalAxis, ZXY);// ZXY

				
				// now convert back from 'safe' quadrant 
				Quaternion<float> qFinal(1.f,0.f,0.f,0.f);
				
				// create an 'about face' quaternion rotation for each of the new Z, X and Y values
				Quaternion<float> qZa = Quaternion<float>::fromAxisRot(UNIT_Z_ZXY,vRotated.z);//-fZnormaliser);
				Quaternion<float> qXa = Quaternion<float>::fromAxisRot(UNIT_X_ZXY,vRotated.x);//-fXnormaliser);
				Quaternion<float> qYa = Quaternion<float>::fromAxisRot(UNIT_Y_ZXY,vRotated.y);//-fYnormaliser);


				Quaternion<float> qZDeNormaliser = Quaternion<float>::fromAxisRot(UNIT_Z_ZXY,-fZnormaliser);
				Quaternion<float> qXDeNormaliser = Quaternion<float>::fromAxisRot(UNIT_X_ZXY,-fXnormaliser);
				Quaternion<float> qYDeNormaliser = Quaternion<float>::fromAxisRot(UNIT_Y_ZXY,-fYnormaliser);
				
				// apply de-normalisers in XYZ order (reverse order to initial application)
				qFinal*=qXDeNormaliser;
				qFinal*=qYDeNormaliser;
				qFinal*=qZDeNormaliser;
				
				// apply new rotation values
				qFinal*=qZa;
				qFinal*=qXa;
				qFinal*=qYa;
		
				// just in case
				qFinal.normalize(); 

				// Convert to ZXY using my novel method
				Vector3<float> vFinal=QuatToRot(qFinal);
				
				// overwrite the existing rotation data using new rotation order
				motionItr->data[pJoint->nMotionDataIndex+0]=vFinal.z;
				motionItr->data[pJoint->nMotionDataIndex+1]=vFinal.x;
				motionItr->data[pJoint->nMotionDataIndex+2]=vFinal.y;

				break;
			}
			default:
			{
				//__asm int 3;
				break;
			}
		}// end switch

		//next!
		motionItr++;
		nFrameNumber++;//debug counter
	}
	// update the model's information
	pJoint->channelOrder=ZXY;// assuming ZXY here
	pJoint->CHANNELSLine.Replace("Zrotation Yrotation Xrotation", "Zrotation Xrotation Yrotation");
}
//
// Not working - which means my understanding of the Tait/Bryan conversion tweak used above is not correct...
//
void CBVHQuaterniser::SetRotationOrderToYZX(BVHJoint* pJoint, int nWayRound)
{
	// used to store the rotation values for each frame
	Vector3<float> vDataVector(0.f,0.f,0.f);

	switch(pJoint->channelOrder)
	{
	case ZXY:
		{
			//vector<BVHMotion>::iterator motionItr = m_pBVHFile->m_motion.begin();
			//while(motionItr!=m_pBVHFile->m_motion.end()) 
			//{
			//	// get rotations for current joint on the current line of motion data
			//	vDataVector.x = motionItr->data[pJoint->nMotionDataIndex+1];
			//	vDataVector.y = motionItr->data[pJoint->nMotionDataIndex+2];
			//	vDataVector.z = motionItr->data[pJoint->nMotionDataIndex+0]; 

			//	// create a 'empty' quaternion for holding the joint's orientation
			//	Quaternion<float> qOriginalAxis(1.f,0.f,0.f,0.f);

			//	// create a quaternion rotation for each of the Z, X and Y values
			//	Quaternion<float> qZ = Quaternion<float>::fromAxisRot(UNIT_Z_ZXY,vDataVector.z);
			//	Quaternion<float> qY = Quaternion<float>::fromAxisRot(UNIT_Y_ZXY,vDataVector.y);
			//	Quaternion<float> qX = Quaternion<float>::fromAxisRot(UNIT_X_ZXY,vDataVector.x);

			//	// apply the rotations in order (which order???)
			//	
			//	qOriginalAxis*=qZ;
			//	qOriginalAxis*=qX;
			//	qOriginalAxis*=qY;
			//	
			//	// voila - a quaternion with our rotations applied. 
			//	// now convert to YZX			
			//	Vector3<float> vFinal=QuatToRot(qOriginalAxis);
			//	
			//	// overwrite the existing rotation data using new rotation order
			//	motionItr->data[pJoint->nMotionDataIndex+0]=vFinal.z;
			//	motionItr->data[pJoint->nMotionDataIndex+1]=vFinal.x;
			//	motionItr->data[pJoint->nMotionDataIndex+2]=vFinal.y;

			//	motionItr++;
			//}


			// swap 1 and 2 (Z and X)   ZXY->XZY
			m_pBVHTweaker->MotionDataSwapChannels(pJoint->nMotionDataIndex+1,pJoint->nMotionDataIndex+2);
			m_pBVHTweaker->MotionDataInvertChannel(pJoint->nMotionDataIndex+2); // invert (new) X // invert 

			// swap 1 and 3 (Y and X)   XZY->YZX
			//m_pBVHTweaker->MotionDataSwapChannels(pJoint->nMotionDataIndex+0,pJoint->nMotionDataIndex+2);
			//m_pBVHTweaker->MotionDataInvertChannel(pJoint->nMotionDataIndex+0); // invert Y

			// update the joint's internal info
			pJoint->channelOrder=ZYX;//XZY;
			pJoint->CHANNELSLine.Replace("Zrotation Xrotation Yrotation", "Yrotation Zrotation Xrotation");
			return;
		}
	default:
		{
			AfxMessageBox(_T("I've not implemented a conversion for this joint rotation order yet. Send me some money, and I'll do it!"),MB_ICONINFORMATION);
			return;
		}
	}
}
//
// display a matrix in a messagebox - debug only
//
void CBVHQuaterniser::ShowMatrix(CString title, Matrix4<float>* m4)
{
	TCHAR wsMatrix[2048];
	sprintf(wsMatrix,"%s\n\n %f %f %f %f \n %f %f %f %f \n %f %f %f %f \n %f %f %f %f \n",title,
		m4->data[0],  m4->data[1],  m4->data[2],  m4->data[3],
		m4->data[4],  m4->data[5],  m4->data[6],  m4->data[7],
		m4->data[8],  m4->data[9],  m4->data[10], m4->data[11],
		m4->data[12], m4->data[13], m4->data[14], m4->data[15]);
	AfxMessageBox(wsMatrix);
	sprintf(wsMatrix,"");
}

// 
// I think this assumes XYZ rotation order 
// unless I use the UNIT_n unit vectors, then it works for ZXY!
// go figure!
//
Vector3<float> CBVHQuaterniser::QuatToRot(Quaternion<float> &qOriginal)
{
	// poo! this needs RAD not DEG...
	float w = qOriginal.w/DEGTORAD;
	float x = qOriginal.v.x/DEGTORAD;
	float y = qOriginal.v.y/DEGTORAD;
	float z = qOriginal.v.z/DEGTORAD;

	float sqw = w * w;
    float sqx = x * x;
    float sqy = y * y;
    float sqz = z * z;
	float unit = sqx + sqy + sqz + sqw; // if normalised is one, otherwise is correction factor
	float test = x * y + z * w;

	Vector3<float> v1;

	if (test > 0.499*unit) { // singularity at north pole
		v1.z = 2 * atan2(x,w);
		v1.x = (float)N_PI/2.f;
		v1.y = 0;
	}
	else if (test < -0.499*unit) { // singularity at south pole
		v1.z = -2 * atan2(x,w);
		v1.x = (float)-N_PI/2.f;
		v1.y = 0;
	}
    else 
	{
		v1.z = atan2(2*y * w-2 * x*z , sqx - sqy - sqz + sqw);
		v1.x = asin(2*test/unit);
		v1.y = atan2(2*x *w-2 * y*z , -sqx + sqy - sqz + sqw);
	}

	v1/=DEGTORAD;

	return v1;
}

//
// implementation of Noel Huges algorithm
//
Vector3<float> CBVHQuaterniser::QuatToRot(Quaternion<float> &qOriginal, int nRotationOrder)
{
	// preparation: 
	// convert quat to radians (note qOrignial == QG in algorithm)
	Quaternion<float>qRotations(qOriginal);
	qRotations.w*=DEGTORAD;
	qRotations.v.x*=DEGTORAD;
	qRotations.v.y*=DEGTORAD;
	qRotations.v.z*=DEGTORAD;// = QG
	qRotations.normalize();

	//Quaternion<float> qRotations(0.723317f,0.360423f,0.439679f,0.391904f);// = QG should give 30,60,45


	//// check for possible gimble lock situations - didn't work too well...
	//float sqw = qRotations.w * qRotations.w;
 //   float sqx = qRotations.v.x * qRotations.v.x;
 //   float sqy = qRotations.v.y * qRotations.v.y;
 //   float sqz = qRotations.v.z * qRotations.v.z;
	//float unit = sqx + sqy + sqz + sqw; // if normalised is one, otherwise is correction factor
	//float test = qRotations.v.x * qRotations.v.y + qRotations.v.z * qRotations.w;
	//if (test > 0.4*unit) { // singularity at north pole
	//	qRotations.v.z = 2 * atan2(qRotations.v.x,qRotations.w);
	//	qRotations.v.x = (float)N_PI/2.f;
	//	qRotations.v.y = 0;
	//}
	//else if (test < -0.4*unit) { // singularity at south pole
	//	qRotations.v.z = -2 * atan2(qRotations.v.x,qRotations.w);
	//	qRotations.v.x = (float)-N_PI/2.f;
	//	qRotations.v.y = 0;
	//}


	// create a vector for the result
	Vector3<float> result(0.f,0.f,0.f);

	// desired rotation sequence, in terms of N.Huges paper, is 3-1-2 (ZXY)
	// so i1 = 3 (Z) i3n = 1(X) i3nm = 2(Y)
	//    i2 = 1 (X) i1n = 2(Y) i1nm = 3(Z)
	//    i3 = 2 (Y) i2n = 3(Z) i2nm = 1(X)

	// create unit vector in 3rd (i3) Euler rotation axis
	Vector3<float> unit_Y(0.f,1.f,0.f);//( =v3)
	
	// create unit vector
	Vector3<float> v3Rot = qRotations*unit_Y;   // v3rot
	v3Rot.normalize();

	// Calculate Z and X (fTheta1 and fTheta2)
	// i1 = 3 (Z) i3n = 1(X) i3nm = 2(Y)
	float fTheta1;
	if(v3Rot.x==0&&v3Rot.y==0) fTheta1 = 0.f;
	else fTheta1 = atanf(-1*v3Rot.x/v3Rot.y);
	float fTheta2 = asinf(v3Rot.z);// figures wrong with a -ve here (as per Noel's paper)

	if(abs(fTheta1)>(N_PI/2))
	{
		int stop = 0;
	}

	if(abs(fTheta2)>(N_PI/2))
	{
		int stop = 0;
	}

	// now calculate the third angle (Y)
	// put sin(theta1) in z(qi1)
	Quaternion<float> qQ1(cosf(fTheta1/2.f),0.f,0.f,sinf(fTheta1/2.f));

	// put sin(theta2) in x(qi2)
	Quaternion<float> qQ2(cosf(fTheta2/2.f),sinf(fTheta2/2.f),0.f,0.f);

	// calculate Q12 
	Quaternion<float> qQ12 = qQ1*qQ2;

	// create unit vector in i3n axis
	Vector3<float> unit_i3n(0.f,0.f,1.f);//( =v3n)

	// perform quat rotation op on v3n twice, using both Q12 and QG
	Vector3<float> vV3n12 = qQ12*unit_i3n;
	Vector3<float> vV3nG = qRotations*unit_i3n;

	// magnitude of the 3rd Euler angle is the angle between vV3n12 and vV3nG
	float fCosTheta = vV3n12.dotProduct(vV3nG);
	fCosTheta = fround(fCosTheta, 5);
	float fTheta3 = acos(fCosTheta);

	if(abs(fTheta3)>(N_PI/2))
	{
		int stop = 0;
	}

	// calculate the sign of the final angle
	Vector3<float> vParallelTest = vV3n12.crossProduct(vV3nG); //  (Vc)
	float fM = vParallelTest.dotProduct(v3Rot);

	// finally, combine the sign with angle 3
	if(fM>0.f)fTheta3 = fTheta3;// no change
	else if(fM<0.f)fTheta3*=-1;
	else{//oops
		//AfxMessageBox(_T("Problem here - fM in BVHQuaterniser::QuatToRot returned zero."));
		fTheta3=fTheta3;
	}

	// QED

	// put results in Vector3 and return
	result.z=fTheta1/DEGTORAD;
	result.x=fTheta2/DEGTORAD;
	result.y=fTheta3/DEGTORAD;

	return result;
}

// 
// returns the offset necessary to make the angle to be adjusted +- 45 degrees
//
float CBVHQuaterniser::NormaliseToQuadrant(float fValue)
{
	float fAdjustment = 0.f;
	float fTargetAngle = 0.f;
	//if(fValue>360.f) return -315.f;
	//if(fValue<-360.f) return 315.f;
	//if(fValue>180.f) return -135.f;
	//if(fValue<-180.f) return 135.f;
	//if(fValue>90.f) return -45.f;
	//if(fValue<-90.f) return 45.f;
	//if(fValue>45.f) return -22.5f;
	//if(fValue<-45.f) return 22.5f;


	if(abs(fValue)>360.f) fTargetAngle = 89.f;
	else if(abs(fValue)>180.f) fTargetAngle = 75.f;
	else if(abs(fValue)>135.f) fTargetAngle = 75.f;
	else if(abs(fValue)>90.f) fTargetAngle = 75.f;// important ones
	else if(abs(fValue)>80.f) fTargetAngle = 50.f;//
	else if(abs(fValue)>60.f) fTargetAngle = 40.f;//
	else if(abs(fValue)>45.f) fTargetAngle = 35.0f;// important ones
	else return 0.f;

	if(fValue>=0.f) fAdjustment=fValue-fTargetAngle;
	if(fValue<0.f) fAdjustment=fValue+fTargetAngle;
	
	return -fAdjustment;
}
//
// calculates the max, min and average rotations for each rotation axis for a given joint
// returns the average for X,Y and Z in a Vector3
//
Vector3<float> CBVHQuaterniser::GetAverageRotation(BVHJoint* pJoint)
{
	Vector3<float> vDataVector(0.f,0.f,0.f);
	float fXmax = -90.f;
	float fYmax = -90.f;
	float fZmax = -90.f;
	float fXmin = 90.f;
	float fYmin = 90.f;
	float fZmin = 90.f;

	// first we need to move our rotations to within the quadrant/s
	// that the algorithm works within - easier said than done!
	vector<BVHMotion>::iterator motionItr = m_pBVHFile->m_motion.begin();
	while(motionItr!=m_pBVHFile->m_motion.end()) 
	{
		// replace values over |180|
		//if(motionItr->data[pJoint->nMotionDataIndex+2]>180.f) 
		//	motionItr->data[pJoint->nMotionDataIndex+2]=
		//	  -180.f+(motionItr->data[pJoint->nMotionDataIndex+2]-180.f);
		//if(motionItr->data[pJoint->nMotionDataIndex+1]>180.f) 
		//	motionItr->data[pJoint->nMotionDataIndex+1]=
		//	  -180.f+(motionItr->data[pJoint->nMotionDataIndex+1]-180.f);
		//if(motionItr->data[pJoint->nMotionDataIndex+0]>180.f) 
		//	motionItr->data[pJoint->nMotionDataIndex+0]=
		//	  -180.f+(motionItr->data[pJoint->nMotionDataIndex+0]-180.f);

		//if(motionItr->data[pJoint->nMotionDataIndex+2]<180.f) 
		//	motionItr->data[pJoint->nMotionDataIndex+2]=
		//	  180.f-(motionItr->data[pJoint->nMotionDataIndex+2]+180.f);
		//if(motionItr->data[pJoint->nMotionDataIndex+1]<180.f) 
		//	motionItr->data[pJoint->nMotionDataIndex+1]=
		//	  180.f-(motionItr->data[pJoint->nMotionDataIndex+1]+180.f);
		//if(motionItr->data[pJoint->nMotionDataIndex+0]<180.f) 
		//	motionItr->data[pJoint->nMotionDataIndex+0]=
		//	  180.f-(motionItr->data[pJoint->nMotionDataIndex+0]+180.f);


		vDataVector.x = motionItr->data[pJoint->nMotionDataIndex+2];
		vDataVector.y = motionItr->data[pJoint->nMotionDataIndex+1];
		vDataVector.z = motionItr->data[pJoint->nMotionDataIndex+0];		

		if(vDataVector.x>fXmax)fXmax=vDataVector.x;
		if(vDataVector.y>fYmax)fYmax=vDataVector.y;
		if(vDataVector.z>fZmax)fZmax=vDataVector.z;

		if(vDataVector.x<fXmin)fXmin=vDataVector.x;
		if(vDataVector.y<fYmin)fYmin=vDataVector.y;
		if(vDataVector.z<fZmin)fZmin=vDataVector.z;

		motionItr++;
	}

	float fXnormaliser = 0.f;
	float fYnormaliser = 0.f;
	float fZnormaliser = 0.f;

	if(fXmax==fXmin)fXnormaliser=-fXmax/2.F;
	else fXnormaliser = -(fXmax+fXmin)/2.f;

	if(fYmax==fYmin) fYnormaliser=-fYmax/2.F;
	else fYnormaliser = -(fYmax+fYmin)/2.f;

	if(fZmax==fZmin) fZnormaliser=-fZmax/2.F;
	else fZnormaliser = -(fZmax+fZmin)/2.f;

	Vector3<float> vNormaliser(fXnormaliser,fYnormaliser,fZnormaliser);
	return vNormaliser;
}

//
// Sets all joints to new rotation order
//
void CBVHQuaterniser::SetRotationOrder(int nRotationOrder)
{
	switch(nRotationOrder)
	{
		case YZX:
		{
			vector<BVHJoint>::iterator jointItr = m_pBVHFile->m_joints.begin();
			CString szJointName = _T("");
			while(jointItr!=m_pBVHFile->m_joints.end()) {

				szJointName = jointItr->internalName.GetBuffer();//

				// middle
				//if(szJointName== _T("hip")) SetRotationOrderToYZX(&(*jointItr), NEITHER);
				//if(szJointName== _T("abdomen")) SetRotationOrderToYZX(&(*jointItr), NEITHER);
				//if(szJointName== _T("chest")) SetRotationOrderToYZX(&(*jointItr), NEITHER);
				//if(szJointName== _T("neck")) SetRotationOrderToYZX(&(*jointItr), NEITHER);
				//if(szJointName== _T("head")) SetRotationOrderToYZX(&(*jointItr), NEITHER);

				// left side
				//if(szJointName== _T("lCollar")) SetRotationOrderToYZX(&(*jointItr), LEFT);
				//if(szJointName== _T("lShldr")) SetRotationOrderToYZX(&(*jointItr), LEFT);
				if(szJointName== _T("lForeArm")) SetRotationOrderToYZX(&(*jointItr), LEFT);
				//if(szJointName== _T("lHand")) SetRotationOrderToYZX(&(*jointItr), LEFT);
				//if(szJointName== _T("lThigh")) SetRotationOrderToYZX(&(*jointItr), LEFT);
				//if(szJointName== _T("lShin")) SetRotationOrderToYZX(&(*jointItr), LEFT);
				//if(szJointName== _T("lFoot")) SetRotationOrderToYZX(&(*jointItr), LEFT);

				// right side
				//if(szJointName== _T("rCollar")) SetRotationOrderToYZX(&(*jointItr), LEFT);
				//if(szJointName== _T("rShldr")) SetRotationOrderToYZX(&(*jointItr), RIGHT);
				//if(szJointName== _T("rForeArm")) SetRotationOrderToYZX(&(*jointItr), RIGHT);
				//if(szJointName== _T("rHand")) SetRotationOrderToYZX(&(*jointItr), RIGHT);
				//if(szJointName== _T("rThigh")) SetRotationOrderToYZX(&(*jointItr), LEFT);
				//if(szJointName== _T("rShin")) SetRotationOrderToYZX(&(*jointItr), LEFT);
				//if(szJointName== _T("rFoot")) SetRotationOrderToYZX(&(*jointItr), LEFT);

				jointItr++;
			}
		}
	}
}

bool CBVHQuaterniser::RotateGlobal(BVHJoint* pJoint, Vector3<float> vRotation)
{
	// create a quaternion rotation for each of the new (global) Z, X and Y values
	// note: there should only ever be a change in one at a time, so rotation order not a worry
	Quaternion<float> qX = Quaternion<float>::fromAxisRot(UNIT_X_ZXY,vRotation.x);
	Quaternion<float> qY = Quaternion<float>::fromAxisRot(UNIT_Y_ZXY,vRotation.y);
	Quaternion<float> qZ = Quaternion<float>::fromAxisRot(UNIT_Z_ZXY,vRotation.z);

	// now go through the motion data for each and every joint 
	vector<BVHMotion>::iterator motionItr = m_pBVHFile->m_motion.begin();
	while(motionItr!=m_pBVHFile->m_motion.end()) 
	{
		// then apply the normal rotation values
		switch(pJoint->channelOrder)
		{
			case(ZXY):
			{
				// get (local) rotation data values for this frame
				Vector3<float> vDataValues(0.f,0.f,0.f);
				vDataValues.x = motionItr->data[pJoint->nMotionDataIndex+1];
				vDataValues.y = motionItr->data[pJoint->nMotionDataIndex+2];
				vDataValues.z = motionItr->data[pJoint->nMotionDataIndex+0]; 

				// create unit vectors for the (local) rotation data values
				Quaternion<float> qDataZ = Quaternion<float>::fromAxisRot(UNIT_Z_ZXY,vDataValues.z);
				Quaternion<float> qDataX = Quaternion<float>::fromAxisRot(UNIT_X_ZXY,vDataValues.x);
				Quaternion<float> qDataY = Quaternion<float>::fromAxisRot(UNIT_Y_ZXY,vDataValues.y);
				
				// create vector with offsets and add any translation motion data
				Vector3<float> vTranslation(pJoint->offsetChannels[2],
											pJoint->offsetChannels[1],
											pJoint->offsetChannels[0]);	
				if(pJoint->offsetChannelOrder!=NONE)
				{
					vTranslation.x += motionItr->data[pJoint->nOffsetMotionDataIndex+0];
					vTranslation.y += motionItr->data[pJoint->nOffsetMotionDataIndex+1];
					vTranslation.z += motionItr->data[pJoint->nOffsetMotionDataIndex+2]; 
				}

				// rotate current translation values by new global rotation
				vTranslation.rotate(vRotation.x,vRotation.y,vRotation.z);

				// need to subtract any bone offset's here, 
				// as don't want to add them back to the motion data
				vTranslation.x -= pJoint->offsetChannels[2];
				vTranslation.y -= pJoint->offsetChannels[1];
				vTranslation.z -= pJoint->offsetChannels[0]; 


				// overwrite the existing translation data 
				motionItr->data[pJoint->nOffsetMotionDataIndex+0]=vTranslation.x;
				motionItr->data[pJoint->nOffsetMotionDataIndex+1]=vTranslation.y;
				motionItr->data[pJoint->nOffsetMotionDataIndex+2]=vTranslation.z;

				// create quat with our total translation for this frame
				Quaternion<float> qFinal(1.f,0.f,0.f,0.f);

				// apply the global transformation
				qFinal*=qZ;
				qFinal*=qX;
				qFinal*=qY;				
				
				// apply the (local) rotation data values for this frame
				qFinal*=qDataZ;
				qFinal*=qDataX;
				qFinal*=qDataY;
				qFinal.normalize(); // just in case

				// Convert to ZXY using my novel method
				Vector3<float> vFinal=QuatToRot(qFinal);
				
				// overwrite the existing rotation data 
				motionItr->data[pJoint->nMotionDataIndex+0]=vFinal.z;
				motionItr->data[pJoint->nMotionDataIndex+1]=vFinal.x;
				motionItr->data[pJoint->nMotionDataIndex+2]=vFinal.y;

				break;
			}
			case(XZY):
			{
				return false;


				// get (local) rotation data values for this frame
				Vector3<float> vDataValues(0.f,0.f,0.f);
				vDataValues.x = motionItr->data[pJoint->nMotionDataIndex+0];
				vDataValues.y = motionItr->data[pJoint->nMotionDataIndex+2];
				vDataValues.z = motionItr->data[pJoint->nMotionDataIndex+1]; 

				// create unit vectors for the (local) rotation data values
				Quaternion<float> qDataZ = Quaternion<float>::fromAxisRot(UNIT_Z_ZXY,vDataValues.z);
				Quaternion<float> qDataX = Quaternion<float>::fromAxisRot(UNIT_X_ZXY,vDataValues.x);
				Quaternion<float> qDataY = Quaternion<float>::fromAxisRot(UNIT_Y_ZXY,vDataValues.y);
				
				// create vector with offsets and add any translation motion data
				Vector3<float> vTranslation(pJoint->offsetChannels[2],
											pJoint->offsetChannels[1],
											pJoint->offsetChannels[0]);				
				if(pJoint->offsetChannelOrder!=NONE)
				{
					vTranslation.x += motionItr->data[pJoint->nOffsetMotionDataIndex+0];
					vTranslation.y += motionItr->data[pJoint->nOffsetMotionDataIndex+1];
					vTranslation.z += motionItr->data[pJoint->nOffsetMotionDataIndex+2]; 
				}

				// rotate current translation values by new global rotation
				vTranslation.rotate(vRotation.x,vRotation.y,vRotation.z);

				// need to subtract any bone offset's here 
				// not necessary on zero offset ROOT
				// ...
				// overwrite the existing translation data 
				motionItr->data[pJoint->nOffsetMotionDataIndex+0]=vTranslation.x;
				motionItr->data[pJoint->nOffsetMotionDataIndex+1]=vTranslation.y;
				motionItr->data[pJoint->nOffsetMotionDataIndex+2]=vTranslation.z;

				// create quat with our total translation for this frame
				Quaternion<float> qFinal(1.f,0.f,0.f,0.f);

				// apply the global transformation
				qFinal*=qZ;
				qFinal*=qX;
				qFinal*=qY;				
				
				// apply the (local) rotation data values for this frame
				qFinal*=qDataZ;
				qFinal*=qDataX;
				qFinal*=qDataY;
				qFinal.normalize(); // just in case

				// Convert to ZXY using my novel method
				Vector3<float> vFinal=QuatToRot(qFinal);
				
				// overwrite the existing rotation data 
				motionItr->data[pJoint->nMotionDataIndex+0]=vFinal.z;
				motionItr->data[pJoint->nMotionDataIndex+1]=vFinal.x;
				motionItr->data[pJoint->nMotionDataIndex+2]=vFinal.y;
			}
			default:
			{
				return false;
			}
		}
		motionItr++;
	}
	return true;
}

//
// not working yet - just returns same value
//
Vector3<float> CBVHQuaterniser::GetCurrentGlobalRotation(BVHJoint* pJoint)
{
	// note doesn't work, just returns local values
	BVHMotion* pMotion = m_pBVHFile->GetCurrentMotionFrame();

	Vector3<float> vLocalRot(pMotion->data[pJoint->nMotionDataIndex+1],
							 pMotion->data[pJoint->nMotionDataIndex+2],
							 pMotion->data[pJoint->nMotionDataIndex+0]);

	Quaternion<float>qFinal(1.f,0.f,0.f,0.f);
	Vector3<float> vFinal(0.f,0.f,0.f);

	// get the rotation values 
	switch(pJoint->channelOrder)
	{
		case(ZXY):
		{
			Quaternion<float> qDataZ = Quaternion<float>::fromAxisRot(UNIT_Z_ZXY,vLocalRot.z);
			Quaternion<float> qDataX = Quaternion<float>::fromAxisRot(UNIT_X_ZXY,vLocalRot.x);
			Quaternion<float> qDataY = Quaternion<float>::fromAxisRot(UNIT_Y_ZXY,vLocalRot.y);
			qFinal*=qDataZ;
			qFinal*=qDataX;
			qFinal*=qDataY;
			qFinal.normalize(); //just in case

			// Convert back to ZXY using my method (UNIT_Z_ZXY etc)
			vFinal=QuatToRot(qFinal);

			return vFinal;
		}
	}
	return vLocalRot;
}
